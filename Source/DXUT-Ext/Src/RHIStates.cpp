#include "RHI.h"
#include "Shader.h"

void FDynamicRHI::SetBoundShaderState(TSharedPtr<FBoundShaderState> BoundShaderState)
{
	m_pd3dImmediateContext->IASetInputLayout(BoundShaderState->InputLayouts.get());
	m_pd3dImmediateContext->VSSetShader(BoundShaderState->VertexShader.get(), NULL, 0);
	m_pd3dImmediateContext->PSSetShader(BoundShaderState->PixelShader.get(), NULL, 0);
	m_pd3dImmediateContext->DSSetShader(BoundShaderState->DomainShader.get(), NULL, 0);
	m_pd3dImmediateContext->HSSetShader(BoundShaderState->HullShader.get(), NULL, 0);
	m_pd3dImmediateContext->GSSetShader(BoundShaderState->GeometryShader.get(), NULL, 0);
	m_pd3dImmediateContext->CSSetShader(BoundShaderState->ComputeShader.get(), NULL, 0);
}

TSharedPtr<FBoundShaderState> FDynamicRHI::CreateBoundShaderState(TSharedPtr<FVertexDeclaration> VertexDeclaration, TSharedPtr<ID3DBlob> BindCode, TSharedPtr<ID3D11VertexShader> VS,
	TSharedPtr<ID3D11PixelShader> PS, TSharedPtr<ID3D11DomainShader> DS, TSharedPtr<ID3D11HullShader> HS, TSharedPtr<ID3D11GeometryShader> GS, TSharedPtr<ID3D11ComputeShader> CS)
{
	HRESULT hr;

	TSharedPtr<FBoundShaderState> BoundState = TSharedPtr<FBoundShaderState>(new FBoundShaderState);
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