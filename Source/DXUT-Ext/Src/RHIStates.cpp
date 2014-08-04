#include "RHI.h"
#include "Shader.h"

void FDynamicRHI::SetBoundShaderState(TSharedPtr<FRHIBoundShaderState> BoundShaderState)
{
	m_pd3dImmediateContext->IASetInputLayout(BoundShaderState->InputLayouts.get());
	m_pd3dImmediateContext->VSSetShader(BoundShaderState->VertexShader.get(), NULL, 0);
	m_pd3dImmediateContext->PSSetShader(BoundShaderState->PixelShader.get(), NULL, 0);
	m_pd3dImmediateContext->DSSetShader(BoundShaderState->DomainShader.get(), NULL, 0);
	m_pd3dImmediateContext->HSSetShader(BoundShaderState->HullShader.get(), NULL, 0);
	m_pd3dImmediateContext->GSSetShader(BoundShaderState->GeometryShader.get(), NULL, 0);
	m_pd3dImmediateContext->CSSetShader(BoundShaderState->ComputeShader.get(), NULL, 0);
}

TSharedPtr<FRHIBoundShaderState> FDynamicRHI::CreateBoundShaderState(TSharedPtr<FVertexDeclaration> VertexDeclaration, TSharedPtr<ID3DBlob> BindCode, TSharedPtr<ID3D11VertexShader> VS,
	TSharedPtr<ID3D11PixelShader> PS, TSharedPtr<ID3D11DomainShader> DS, TSharedPtr<ID3D11HullShader> HS, TSharedPtr<ID3D11GeometryShader> GS, TSharedPtr<ID3D11ComputeShader> CS)
{
	HRESULT hr;

	TSharedPtr<FRHIBoundShaderState> BoundState = TSharedPtr<FRHIBoundShaderState>(new FRHIBoundShaderState);
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

static D3D11_TEXTURE_ADDRESS_MODE GetD3DSamplerAddressMode(ESamplerAddressMode AddressMode)
{
	switch (AddressMode)
	{
	case ESAM_Clamp: 
		return D3D11_TEXTURE_ADDRESS_CLAMP;

	case ESAM_Mirror: 
		return D3D11_TEXTURE_ADDRESS_MIRROR;

	case ESAM_Border: 
		return D3D11_TEXTURE_ADDRESS_BORDER;

	default: 
		return D3D11_TEXTURE_ADDRESS_WRAP;
	};
}

static D3D11_COMPARISON_FUNC GetD3DSamplerCompareFunc(ESamplerCompareFunction CompareFunc)
{
	switch (CompareFunc)
	{
	case ESCF_Never:
		return D3D11_COMPARISON_NEVER;

	case ESCF_Less:
		return D3D11_COMPARISON_LESS;

	case ESCF_Equal:
		return D3D11_COMPARISON_EQUAL;

	case ESCF_LessEqual:
		return D3D11_COMPARISON_LESS_EQUAL;

	case ESCF_Greater:
		return D3D11_COMPARISON_GREATER;

	case ESCF_NotEqual:
		return D3D11_COMPARISON_NOT_EQUAL;

	case ESCF_GreaterEqual:
		return D3D11_COMPARISON_GREATER_EQUAL;

	case ESCF_Always:
		return D3D11_COMPARISON_ALWAYS;

	default:
		return D3D11_COMPARISON_NEVER;
	}
}

TSharedPtr<FRHISamplerState> FDynamicRHI::CreateSamplerState(ESamplerFilter Filter, ESamplerAddressMode AddressU, ESamplerAddressMode AddressV, ESamplerAddressMode AddressW,
	float MipBias, UINT MaxAnisotropy, FLinearColor BorderColor, ESamplerCompareFunction CompareFunc)
{
	ID3D11SamplerState*		pSamplerState = NULL;
	D3D11_SAMPLER_DESC		SamplerDesc;
	HRESULT					hr;

	ZeroMemory(&SamplerDesc, sizeof(D3D11_SAMPLER_DESC));

	const bool bComparisonEnabled = (CompareFunc != ESCF_Never);
	switch (Filter)
	{
	case ESF_AnisotropicLinear:
	case ESF_AnisotropicPoint:
		// D3D11 doesn't allow using point filtering for mip filter when using anisotropic filtering
		SamplerDesc.Filter = bComparisonEnabled ? D3D11_FILTER_COMPARISON_ANISOTROPIC : D3D11_FILTER_ANISOTROPIC;
		break;
	case ESF_Trilinear:
		SamplerDesc.Filter = bComparisonEnabled ? D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR : D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		break;
	case ESF_Bilinear:
		SamplerDesc.Filter = bComparisonEnabled ? D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT : D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
		break;
	case ESF_Point:
		SamplerDesc.Filter = bComparisonEnabled ? D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT : D3D11_FILTER_MIN_MAG_MIP_POINT;
		break;
	}

	SamplerDesc.AddressU = GetD3DSamplerAddressMode(AddressU);
	SamplerDesc.AddressV = GetD3DSamplerAddressMode(AddressV);
	SamplerDesc.AddressW = GetD3DSamplerAddressMode(AddressW);

	SamplerDesc.MipLODBias = MipBias;
	SamplerDesc.MaxAnisotropy = MaxAnisotropy;
	SamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	SamplerDesc.BorderColor[0] = BorderColor.R;
	SamplerDesc.BorderColor[1] = BorderColor.G;
	SamplerDesc.BorderColor[2] = BorderColor.B;
	SamplerDesc.BorderColor[3] = BorderColor.A;

	SamplerDesc.ComparisonFunc = GetD3DSamplerCompareFunc(CompareFunc);

	V(m_pd3dDevice->CreateSamplerState(&SamplerDesc, &pSamplerState));

	TSharedPtr<FRHISamplerState> NewSamplerState = MakeCOMPtr<ID3D11SamplerState>(pSamplerState);

	return NewSamplerState;
}

// PS set sampler
void FDynamicRHI::PSSetSamplerState(int SamplerIndex, const TSharedPtr<FRHISamplerState>& SamplerState)
{
	FRHISamplerState* pSamp = SamplerState.get();
	m_pd3dImmediateContext->PSSetSamplers(SamplerIndex, 1, &pSamp);
}

// VS set sampler
void FDynamicRHI::VSSetSamplerState(int SamplerIndex, const TSharedPtr<FRHISamplerState>& SamplerState)
{
	FRHISamplerState* pSamp = SamplerState.get();
	m_pd3dImmediateContext->VSSetSamplers(SamplerIndex, 1, &pSamp);
}