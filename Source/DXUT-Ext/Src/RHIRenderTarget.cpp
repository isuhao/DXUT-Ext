#include "RHIRenderTarget.h"
#include "RHI.h"


void FDynamicRHI::SetRenderTarget(const TSharedPtr<FRenderSurface>& NewRenderTarget, const TSharedPtr<FRenderSurface>& NewDepthStencil)
{
	FRHIRenderTargetView* pNewRTV = NULL;
	FRHIDepthStencilView* pNewDSV = NULL;

	if (NewRenderTarget)
	{
		pNewRTV = NewRenderTarget->GetRawRTV();
	}
	if (NewDepthStencil)
	{
		pNewDSV = NewDepthStencil->GetRawDSV();
	}

	bool bMRT = false;
	for (int i = 1; i < MAX_RT_COUNT; ++i)
	{
		if (m_pCurrRTVs[i] != NULL)
		{
			bMRT = true;
			break;
		}
	}

	if (m_pCurrDSV != pNewDSV || m_pCurrRTVs[0] != pNewRTV || bMRT)
	{
		m_pCurrDSV		= pNewDSV;
		m_pCurrRTVs[0]	= pNewRTV;

		m_pd3dImmediateContext->OMSetRenderTargets(1, &m_pCurrRTVs[0], m_pCurrDSV);
	}
}

void FDynamicRHI::SetMRTRenderTarget(const TSharedPtr<FRenderSurface>& NewRenderTarget, uint RenderTargetIndex)
{
	FRHIRenderTargetView* pNewRTV = NULL;
	if (NewRenderTarget)
	{
		pNewRTV = NewRenderTarget->GetRawRTV();
	}

	if (m_pCurrRTVs[RenderTargetIndex] != pNewRTV)
	{
		m_pCurrRTVs[RenderTargetIndex] = pNewRTV;

		m_pd3dImmediateContext->OMSetRenderTargets(MAX_RT_COUNT, &m_pCurrRTVs[0], m_pCurrDSV);
	}
}

TSharedPtr<FRenderSurface> FDynamicRHI::CreateRenderSurface(uint Width, uint Height, EPixelFormat PixFormat, TSharedPtr<FTexture2D> TextureToAttach)
{
	HRESULT hr;

	// 是不是要创建DepthBuffer
	bool bIsDepthBuffer = (PixFormat == PF_DepthStencil || PixFormat == PF_ShadowDepth);
	// 转成DXGI_FORMAT
	DXGI_FORMAT DxFmt = (DXGI_FORMAT)GPixelFormats[PixFormat].Format;

	D3D11_DSV_DIMENSION DepthStencilViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	D3D11_RTV_DIMENSION RenderTargetViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	D3D11_SRV_DIMENSION ShaderResourceViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;

	TSharedPtr<FRHIRenderTargetView>	RTV;
	TSharedPtr<FRHIDepthStencilView>	DSV;
	TSharedPtr<FRHIShaderResourceView>	SRV;

	// 如果有指定的Texture，就不创建新的了
	TSharedPtr<FRHITexture2D> TextureResource;
	if (TextureToAttach != NULL)
	{
		TextureResource = TextureToAttach->Resource;
	}
	else
	{
		FRHITexture2D* pNewTex = NULL;
		D3D11_TEXTURE2D_DESC Desc;
		::ZeroMemory(&Desc, sizeof (Desc));
		Desc.Format = DxFmt;
		Desc.Height = Height;
		Desc.Width = Width;
		Desc.ArraySize = 1;
		Desc.SampleDesc.Count = GSystemSettings.MSAACount;
		Desc.SampleDesc.Quality = GSystemSettings.MSAAQuality;
		Desc.SampleDesc.Count = 1;
		Desc.SampleDesc.Quality = 0;
		Desc.MipLevels = 1;
		Desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | (bIsDepthBuffer ? D3D11_BIND_DEPTH_STENCIL: D3D11_BIND_RENDER_TARGET);

		m_pd3dDevice->CreateTexture2D(&Desc, 0, &pNewTex);
		TextureResource = MakeCOMPtr<FRHITexture2D>(pNewTex);
	}

	FRHITexture2D* SurfaceRes = TextureResource.get();
	// 一定要非空!
	Check(SurfaceRes);

	// 创建DSV
	if (bIsDepthBuffer)
	{
		DXGI_FORMAT DepthViewFormat = DxFmt;
		if (DxFmt == DXGI_FORMAT_R24G8_TYPELESS)
		{
			// Use the typed depth stencil view format corresponding to DXGI_FORMAT_R24G8_TYPELESS
			DepthViewFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
		}

		FRHIDepthStencilView* pDSV = NULL;
		D3D11_DEPTH_STENCIL_VIEW_DESC DSVDesc;
		ZeroMemory(&DSVDesc, sizeof(DSVDesc));
		DSVDesc.Format = DepthViewFormat;
		DSVDesc.ViewDimension = DepthStencilViewDimension;
		DSVDesc.Texture2D.MipSlice = 0;
		V(m_pd3dDevice->CreateDepthStencilView(SurfaceRes, &DSVDesc, &pDSV));
		DSV = MakeCOMPtr<FRHIDepthStencilView>(pDSV);

		// Create a shader resource view for the depth buffer.
		FRHIShaderResourceView* pSRV = NULL;
		D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
		ZeroMemory(&SRVDesc, sizeof(SRVDesc));
		SRVDesc.Format = DxFmt;
		if (DxFmt == DXGI_FORMAT_R24G8_TYPELESS)
		{
			// Use the typed shader resource view format corresponding to DXGI_FORMAT_R24G8_TYPELESS
			SRVDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		}
		SRVDesc.ViewDimension = ShaderResourceViewDimension;
		SRVDesc.Texture2D.MipLevels = 1;
		V(m_pd3dDevice->CreateShaderResourceView(SurfaceRes, &SRVDesc, &pSRV));
		SRV = MakeCOMPtr<FRHIShaderResourceView>(pSRV);
	}
	// 创建RTV
	else
	{
		D3D11_TEXTURE2D_DESC ResolveTargetDesc;
		SurfaceRes->GetDesc(&ResolveTargetDesc);

		FRHIRenderTargetView* pRTV = NULL;
		D3D11_RENDER_TARGET_VIEW_DESC RTVDesc;
		ZeroMemory(&RTVDesc, sizeof(RTVDesc));
		RTVDesc.Format = ResolveTargetDesc.Format;
		RTVDesc.ViewDimension = RenderTargetViewDimension;
		RTVDesc.Texture2D.MipSlice = 0;
		V(m_pd3dDevice->CreateRenderTargetView(SurfaceRes, &RTVDesc, &pRTV));
		RTV = MakeCOMPtr<FRHIRenderTargetView>(pRTV);

		// Create a shader resource view for the render target.
		FRHIShaderResourceView* pSRV = NULL;
		D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
		ZeroMemory(&SRVDesc, sizeof(SRVDesc));
		SRVDesc.Format = ResolveTargetDesc.Format;
		SRVDesc.ViewDimension = ShaderResourceViewDimension;
		// 是否在这个View中允许生成Mips
		SRVDesc.Texture2D.MipLevels = 1;
		V(m_pd3dDevice->CreateShaderResourceView(SurfaceRes, &SRVDesc, &pSRV));
		SRV = MakeCOMPtr<FRHIShaderResourceView>(pSRV);
	}

	return TSharedPtr<FRenderSurface>(new FRenderSurface(RTV, DSV, SRV, TextureResource));
}

TSharedPtr<FRenderSurface> FDynamicRHI::CreateDepthTargetSurface(uint Width, uint Height, TSharedPtr<FTexture2D> TextureToAttach)
{
	return CreateRenderSurface(Width, Height, PF_DepthStencil, TextureToAttach);
}

TSharedPtr<FRenderSurface> FDynamicRHI::CreateRenderTargetSurface(uint Width, uint Height, EPixelFormat PixFormat, TSharedPtr<FTexture2D> TextureToAttach)
{
	return CreateRenderSurface(Width, Height, PixFormat, TextureToAttach);
}

TSharedPtr<FFrameBuffer> FDynamicRHI::CreateFrameBuffer(uint Width, uint Height, EPixelFormat PixFormat)
{
	return TSharedPtr<FFrameBuffer>(new FFrameBuffer(
		CreateRenderSurface(Width, Height, PixFormat),
		CreateRenderSurface(Width, Height, PF_DepthStencil)
		));
}

void FDynamicRHI::SetFrameBuffer(const TSharedPtr<FFrameBuffer>& FrameBuffer)
{
	Check(FrameBuffer);
	SetRenderTarget(FrameBuffer->RenderTarget, FrameBuffer->DepthTarget);
}

void FDynamicRHI::SetMRTFrameBuffer(const TSharedPtr<FFrameBuffer>& FrameBuffer, uint RTIndex)
{
	Check(FrameBuffer);
	SetMRTRenderTarget(FrameBuffer->RenderTarget, RTIndex);
}

void FDynamicRHI::ClearColor(FLinearColor Color, uint RTIndex)
{
	Check(RTIndex < MAX_RT_COUNT);

	if (m_pCurrRTVs[RTIndex])
	{
		m_pd3dImmediateContext->ClearRenderTargetView(m_pCurrRTVs[RTIndex], &Color.R);
	}
}

void FDynamicRHI::ClearDepth(float fDepth)
{
	if (m_pCurrDSV)
	{
		m_pd3dImmediateContext->ClearDepthStencilView(m_pCurrDSV, D3D11_CLEAR_DEPTH, fDepth, 0);
	}
}

void FDynamicRHI::ClearStencil(uint iStencil)
{
	if (m_pCurrDSV)
	{
		m_pd3dImmediateContext->ClearDepthStencilView(m_pCurrDSV, D3D11_CLEAR_STENCIL, 0, iStencil);
	}
}

void FDynamicRHI::Clear(bool bClearColor, FLinearColor Color, bool bClearDepth, float fDepth, bool bClearStencil, uint iStencil, bool bClearAllTargets)
{
	if (bClearColor)
	{
		uint MaxRTIndex = bClearAllTargets ? MAX_RT_COUNT : 1;
		for (uint RTIndex = 0; RTIndex < MaxRTIndex; ++RTIndex)
		{
			if (m_pCurrRTVs[RTIndex])
			{
				m_pd3dImmediateContext->ClearRenderTargetView(m_pCurrRTVs[RTIndex], &Color.R);
			}
		}
	}

	if (bClearDepth)
	{
		ClearDepth(fDepth);
	}

	if (bClearStencil)
	{
		ClearStencil(iStencil);
	}
}

void FDynamicRHI::SetDefaultBackBuffer()
{
	m_pCurrRTVs[0] = DXUTGetD3D11RenderTargetView();
	m_pCurrDSV = DXUTGetD3D11DepthStencilView();

	m_pd3dImmediateContext->OMSetRenderTargets(1, &m_pCurrRTVs[0], m_pCurrDSV);
}