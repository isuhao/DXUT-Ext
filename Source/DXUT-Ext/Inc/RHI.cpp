#include "RHI.h"
#include "Core.h"

DynamicRHI* RHI;

void DynamicRHI::InitRHI()
{
	Check(!RHI);
	RHI = new DynamicRHI();
}

void DynamicRHI::ReleaseRHI()
{
	Check(RHI);
	delete RHI;
}

DynamicRHI::DynamicRHI()
{
	m_pd3dDevice = DXUTGetD3D11Device();
	m_pd3dImmediateContext  = DXUTGetD3D11DeviceContext();
}

DynamicRHI::~DynamicRHI()
{
	m_pd3dDevice = NULL;
	m_pd3dImmediateContext = NULL;
}

void DynamicRHI::SetBoundShaderState(TSharedPtr<ID3D11InputLayout> InputLayout, TSharedPtr<ID3D11VertexShader> VS, TSharedPtr<ID3D11PixelShader> PS, 
	TSharedPtr<ID3D11DomainShader> DS, TSharedPtr<ID3D11HullShader> HS, TSharedPtr<ID3D11GeometryShader> GS, TSharedPtr<ID3D11ComputeShader> CS)
{
	m_pd3dImmediateContext->IASetInputLayout(InputLayout.get());
	m_pd3dImmediateContext->VSSetShader(VS.get(), NULL, 0);
	m_pd3dImmediateContext->PSSetShader(PS.get(), NULL, 0);
	m_pd3dImmediateContext->DSSetShader(DS.get(), NULL, 0);
	m_pd3dImmediateContext->HSSetShader(HS.get(), NULL, 0);
	m_pd3dImmediateContext->GSSetShader(GS.get(), NULL, 0);
	m_pd3dImmediateContext->CSSetShader(CS.get(), NULL, 0);
}