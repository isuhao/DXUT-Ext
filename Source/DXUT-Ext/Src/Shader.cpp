#include "Shader.h"
#include "DXUT-Ext.h"

FShader::FShader(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, EShaderType ShaderType)
:	m_szFileName	(szFileName)
,	m_szEntryPoint	(szEntryPoint)
,	m_szShaderModel	(szShaderModel)
,	m_ShaderType	(ShaderType)
{
	m_pVertexShader.reset();
	m_pPixelShader.reset();
	m_pDomainShader.reset();
	m_pHullShader.reset();
	m_pGemetryShader.reset();
	m_pComputeShader.reset();
}

void FShader::InitShader()
{
	// 编译与创建
	CompileAndCreateShader();

	// 绑定各种参数
	BindParameters();
}

void FShader::CompileAndCreateShader()
{
	HRESULT				hr;
	ID3DBlob*			pBlob = NULL;
	ID3D11Device*		pd3dDevice = DXUTGetD3D11Device();

	V(CompileShaderFromFile((WCHAR*)m_szFileName.c_str(), (LPCSTR)m_szEntryPoint.c_str(), (LPCSTR)m_szShaderModel.c_str(), &pBlob));

	if (m_ShaderType == EST_VertexShader)
	{
		ID3D11VertexShader* pVertexShader = NULL;
		V(pd3dDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &pVertexShader));
		m_pVertexShader = MakeCOMPtr<ID3D11VertexShader>(pVertexShader);
	}
	else if (m_ShaderType == EST_PixelShader)
	{
		ID3D11PixelShader* pPixelShader = NULL;
		V(pd3dDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &pPixelShader));
		m_pPixelShader = MakeCOMPtr<ID3D11PixelShader>(pPixelShader);
	}
	else if (m_ShaderType == EST_DomainShader)
	{
		ID3D11DomainShader* pDomainShader = NULL;
		V(pd3dDevice->CreateDomainShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &pDomainShader));
		m_pDomainShader = MakeCOMPtr<ID3D11DomainShader>(pDomainShader);
	}
	else if (m_ShaderType == EST_HullShader)
	{
		ID3D11HullShader* pHullShader = NULL;
		V(pd3dDevice->CreateHullShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &pHullShader));
		m_pHullShader = MakeCOMPtr<ID3D11HullShader>(pHullShader);
	}
	else if (m_ShaderType == EST_GeometryShader)
	{
		ID3D11GeometryShader* pGeometryShader = NULL;
		V(pd3dDevice->CreateGeometryShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &pGeometryShader));
		m_pGemetryShader = MakeCOMPtr<ID3D11GeometryShader>(pGeometryShader);
	}
	else if (m_ShaderType == EST_ComputeShader)
	{
		ID3D11ComputeShader* pComputeShader = NULL;
		V(pd3dDevice->CreateComputeShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &pComputeShader));
		m_pComputeShader = MakeCOMPtr<ID3D11ComputeShader>(pComputeShader);
	}
	else
	{
		Check(0);
	}
	
	m_pCode.reset();
	m_pCode = MakeCOMPtr<ID3DBlob>(pBlob);
}

