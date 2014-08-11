#include "D3DDriver.h"
#include "Shader.h"

void FD3D11Driver::SetBoundShaderState(TSharedPtr<FD3D11BoundShaderState> BoundShaderState)
{
	m_pd3dImmediateContext->IASetInputLayout(BoundShaderState->InputLayouts.get());
	m_pd3dImmediateContext->VSSetShader(BoundShaderState->VertexShader.get(), NULL, 0);
	m_pd3dImmediateContext->PSSetShader(BoundShaderState->PixelShader.get(), NULL, 0);
	m_pd3dImmediateContext->DSSetShader(BoundShaderState->DomainShader.get(), NULL, 0);
	m_pd3dImmediateContext->HSSetShader(BoundShaderState->HullShader.get(), NULL, 0);
	m_pd3dImmediateContext->GSSetShader(BoundShaderState->GeometryShader.get(), NULL, 0);
	m_pd3dImmediateContext->CSSetShader(BoundShaderState->ComputeShader.get(), NULL, 0);
}

TSharedPtr<FD3D11BoundShaderState> FD3D11Driver::CreateBoundShaderState(TSharedPtr<FVertexDeclaration> VertexDeclaration, TSharedPtr<ID3DBlob> BindCode, TSharedPtr<ID3D11VertexShader> VS,
	TSharedPtr<ID3D11PixelShader> PS, TSharedPtr<ID3D11DomainShader> DS, TSharedPtr<ID3D11HullShader> HS, TSharedPtr<ID3D11GeometryShader> GS, TSharedPtr<ID3D11ComputeShader> CS)
{
	HRESULT hr;

	TSharedPtr<FD3D11BoundShaderState> BoundState = TSharedPtr<FD3D11BoundShaderState>(new FD3D11BoundShaderState);
	BoundState->VertexShader = VS;
	BoundState->PixelShader = PS;
	BoundState->DomainShader = DS;
	BoundState->HullShader = HS;
	BoundState->GeometryShader = GS;
	BoundState->ComputeShader = CS;

	if (VertexDeclaration) 
	{
		ID3D11InputLayout* pInputLayout = NULL;
		D3D11_INPUT_ELEMENT_DESC* Elements = VertexDeclaration->GetElements();
		UINT NumElements = VertexDeclaration->GetElementNum();
		ID3DBlob* pBlob = BindCode.get();
		V(m_pd3dDevice->CreateInputLayout(Elements, NumElements, pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &pInputLayout));
		BoundState->InputLayouts = MakeCOMPtr<ID3D11InputLayout>(pInputLayout);
	}

	return BoundState;
}

// VS & PS only
TSharedPtr<FD3D11BoundShaderState> FD3D11Driver::CreateBoundShaderState(TSharedPtr<FVertexDeclaration> VertexDeclaration, TSharedPtr<ID3DBlob> BindCode, TSharedPtr<ID3D11VertexShader> VS,
	TSharedPtr<ID3D11PixelShader> PS)
{
	HRESULT hr;

	TSharedPtr<FD3D11BoundShaderState> BoundState = TSharedPtr<FD3D11BoundShaderState>(new FD3D11BoundShaderState);
	BoundState->VertexShader = VS;
	BoundState->PixelShader = PS;
	BoundState->DomainShader = NULL;
	BoundState->HullShader = NULL;
	BoundState->GeometryShader = NULL;
	BoundState->ComputeShader = NULL;

	if (VertexDeclaration)
	{
		ID3D11InputLayout* pInputLayout = NULL;
		D3D11_INPUT_ELEMENT_DESC* Elements = VertexDeclaration->GetElements();
		UINT NumElements = VertexDeclaration->GetElementNum();
		ID3DBlob* pBlob = BindCode.get();
		V(m_pd3dDevice->CreateInputLayout(Elements, NumElements, pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &pInputLayout));
		BoundState->InputLayouts = MakeCOMPtr<ID3D11InputLayout>(pInputLayout);
	}

	return BoundState;
}

TSharedPtr<FD3D11SamplerState> FD3D11Driver::CreateSamplerState(ESamplerFilter Filter, ESamplerAddressMode AddressU, ESamplerAddressMode AddressV, ESamplerAddressMode AddressW,
	float MipBias, UINT MaxAnisotropy, FLinearColor BorderColor, ECompareFunction CompareFunc)
{
	ID3D11SamplerState*		pSamplerState = NULL;
	D3D11_SAMPLER_DESC		SamplerDesc;
	HRESULT					hr;

	ZeroMemory(&SamplerDesc, sizeof(D3D11_SAMPLER_DESC));

	const bool bComparisonEnabled = (CompareFunc != CF_Never);
	switch (Filter)
	{
	case SF_AnisotropicLinear:
	case SF_AnisotropicPoint:
		// D3D11 doesn't allow using point filtering for mip filter when using anisotropic filtering
		SamplerDesc.Filter = bComparisonEnabled ? D3D11_FILTER_COMPARISON_ANISOTROPIC : D3D11_FILTER_ANISOTROPIC;
		break;
	case SF_Trilinear:
		SamplerDesc.Filter = bComparisonEnabled ? D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR : D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		break;
	case SF_Bilinear:
		SamplerDesc.Filter = bComparisonEnabled ? D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT : D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
		break;
	case SF_Point:
		SamplerDesc.Filter = bComparisonEnabled ? D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT : D3D11_FILTER_MIN_MAG_MIP_POINT;
		break;
	}

	SamplerDesc.AddressU = TranslateSamplerAddressMode(AddressU);
	SamplerDesc.AddressV = TranslateSamplerAddressMode(AddressV);
	SamplerDesc.AddressW = TranslateSamplerAddressMode(AddressW);

	SamplerDesc.MipLODBias = MipBias;
	SamplerDesc.MaxAnisotropy = MaxAnisotropy;
	SamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	SamplerDesc.BorderColor[0] = BorderColor.R;
	SamplerDesc.BorderColor[1] = BorderColor.G;
	SamplerDesc.BorderColor[2] = BorderColor.B;
	SamplerDesc.BorderColor[3] = BorderColor.A;

	SamplerDesc.ComparisonFunc = TranslateCompareFunc(CompareFunc);

	V(m_pd3dDevice->CreateSamplerState(&SamplerDesc, &pSamplerState));

	TSharedPtr<FD3D11SamplerState> NewSamplerState = MakeCOMPtr<ID3D11SamplerState>(pSamplerState);

	return NewSamplerState;
}

TSharedPtr<FD3D11RasterState> FD3D11Driver::CreateRasterizerState(ERasterizerFillMode FillMode, ERasterizerCullMode CullMode, bool bAllowMSAA, bool bFrontCounterClockwise, INT DepthBias, float SlopeScaledDepthBias)
{
	D3D11_RASTERIZER_DESC	RasterizerDesc;
	ID3D11RasterizerState*	pRasterizer;
	HRESULT					hr;

	ZeroMemory(&RasterizerDesc, sizeof(RasterizerDesc));

	RasterizerDesc.FillMode = TranslateRasterizeFillMode(FillMode);
	RasterizerDesc.CullMode = TranslateRasterizeCullMode(CullMode);
	RasterizerDesc.SlopeScaledDepthBias = SlopeScaledDepthBias;
	RasterizerDesc.FrontCounterClockwise = bFrontCounterClockwise;
	RasterizerDesc.DepthBias = DepthBias;
	RasterizerDesc.DepthClipEnable = TRUE;
	RasterizerDesc.MultisampleEnable = bAllowMSAA;

	V(m_pd3dDevice->CreateRasterizerState(&RasterizerDesc, &pRasterizer));

	return MakeCOMPtr<FD3D11RasterState>(pRasterizer);
}

void FD3D11Driver::SetRasterizerState(const TSharedPtr<FD3D11RasterState>& RasterizerState)
{
	m_pd3dImmediateContext->RSSetState(RasterizerState.get());
}

bool g_bIndependentBlendEnable = true;

TSharedPtr<FD3D11BlendState> FD3D11Driver::CreateBlendState(EBlendOperation ColorBlendOp, EBlendFactor ColorSrcBlend, EBlendFactor ColorDstBlend, EBlendOperation AlphaBlendOp,
	EBlendFactor AlphaSrcBlend, EBlendFactor AlphaDstBlend, EBlendColorWriteEnable ColorWriteEnable)
{
	D3D11_BLEND_DESC	BlendDesc;
	FD3D11BlendState*		pBlendState;
	HRESULT				hr;

	ZeroMemory(&BlendDesc, sizeof(D3D11_BLEND_DESC));

	BlendDesc.AlphaToCoverageEnable = FALSE;
	BlendDesc.IndependentBlendEnable = g_bIndependentBlendEnable;
	BlendDesc.RenderTarget[0].BlendEnable =
		ColorBlendOp != BO_Add || ColorDstBlend != BF_Zero || ColorSrcBlend != BF_One ||
		AlphaBlendOp != BO_Add || AlphaDstBlend != BF_Zero || AlphaSrcBlend != BF_One;
	BlendDesc.RenderTarget[0].BlendOp = TranslateBlendOp(ColorBlendOp);
	BlendDesc.RenderTarget[0].SrcBlend = TranslateBlendFactor(ColorSrcBlend);
	BlendDesc.RenderTarget[0].DestBlend = TranslateBlendFactor(ColorDstBlend);
	BlendDesc.RenderTarget[0].BlendOpAlpha = TranslateBlendOp(AlphaBlendOp);
	BlendDesc.RenderTarget[0].SrcBlendAlpha = TranslateBlendFactor(AlphaSrcBlend);
	BlendDesc.RenderTarget[0].DestBlendAlpha = TranslateBlendFactor(AlphaDstBlend);
	BlendDesc.RenderTarget[0].RenderTargetWriteMask = TranslateBlendColorWriteEnable(ColorWriteEnable);

	// For MRT
	if (g_bIndependentBlendEnable)
	{
		for (UINT RenderTargetIndex = 1; RenderTargetIndex < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; ++RenderTargetIndex)
		{
			CopyMemory(&BlendDesc.RenderTarget[RenderTargetIndex], &BlendDesc.RenderTarget[0], sizeof(BlendDesc.RenderTarget[0]));
		}
	}

	V(m_pd3dDevice->CreateBlendState(&BlendDesc, &pBlendState));

	return MakeCOMPtr<FD3D11BlendState>(pBlendState);
}


void FD3D11Driver::SetBlendState(const TSharedPtr<FD3D11BlendState>& BlendState, FLinearColor Color, UINT SimpleMask)
{
	m_pd3dImmediateContext->OMSetBlendState(BlendState.get(), &Color.R, SimpleMask);
}

//TSharedPtr<FD3D11DepthStencilView> FD3D11Driver::CreateDepthStencilView()
//{
//	return TSharedPtr<FD3D11DepthStencilView>();
//}

