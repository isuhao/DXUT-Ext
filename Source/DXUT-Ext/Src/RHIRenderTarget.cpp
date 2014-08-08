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

	if (m_pCurrDSV != pNewDSV || m_pCurrRTVs[0] != pNewRTV)
	{
		m_pCurrDSV		= pNewDSV;
		m_pCurrRTVs[0]	= pNewRTV;

		m_pd3dImmediateContext->OMSetRenderTargets(MAX_RT_COUNT, &m_pCurrRTVs[0], m_pCurrDSV);
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

TSharedPtr<FRenderSurface> FDynamicRHI::CreateRenderSurface(uint Width, uint Height, EPixelFormat PixFormat, uint CreateFlag, TSharedPtr<FTexture2D> TextureToAttach)
{
	TSharedPtr<FRHITexture2D> Texture;
	if (TextureToAttach != NULL)
	{
		Texture = TextureToAttach->Texture2D;
	}

	bool bDepthFormat = (PixFormat == PF_DepthStencil || PixFormat == PF_ShadowDepth);

	D3D11_TEXTURE2D_DESC Desc;
	::ZeroMemory(&Desc, sizeof (Desc));
	Desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	Desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	Desc.Height = Height;
	Desc.Width = Width;
	Desc.ArraySize = 1;
	Desc.SampleDesc.Count = 1;
	Desc.SampleDesc.Quality = 0;
	Desc.SampleDesc.Count = 1;
	Desc.SampleDesc.Quality = 0;
	Desc.MipLevels = 1;
	Desc.BindFlags = bDepthFormat ? D3D11_BIND_DEPTH_STENCIL : (D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);

	return TSharedPtr<FRenderSurface>();
}