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
	BindVariables();
}

void FShader::CompileAndCreateShader()
{
	HRESULT				hr;
	ID3DBlob*			pBlob = NULL;
	ID3D11Device*		pd3dDevice = DXUTGetD3D11Device();

	V(CompileShaderFromFile((WCHAR*)m_szFileName.c_str(), (LPCSTR)m_szEntryPoint.c_str(), (LPCSTR)m_szShaderModel.c_str(), &pBlob));

	if (m_ShaderType == ST_VertexShader)
	{
		ID3D11VertexShader* pVertexShader = NULL;
		V(pd3dDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &pVertexShader));
		m_pVertexShader = MakeCOMPtr<ID3D11VertexShader>(pVertexShader);
	}
	else if (m_ShaderType == ST_PixelShader)
	{
		ID3D11PixelShader* pPixelShader = NULL;
		V(pd3dDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &pPixelShader));
		m_pPixelShader = MakeCOMPtr<ID3D11PixelShader>(pPixelShader);
	}
	else if (m_ShaderType == ST_DomainShader)
	{
		ID3D11DomainShader* pDomainShader = NULL;
		V(pd3dDevice->CreateDomainShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &pDomainShader));
		m_pDomainShader = MakeCOMPtr<ID3D11DomainShader>(pDomainShader);
	}
	else if (m_ShaderType == ST_HullShader)
	{
		ID3D11HullShader* pHullShader = NULL;
		V(pd3dDevice->CreateHullShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &pHullShader));
		m_pHullShader = MakeCOMPtr<ID3D11HullShader>(pHullShader);
	}
	else if (m_ShaderType == ST_GeometryShader)
	{
		ID3D11GeometryShader* pGeometryShader = NULL;
		V(pd3dDevice->CreateGeometryShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &pGeometryShader));
		m_pGemetryShader = MakeCOMPtr<ID3D11GeometryShader>(pGeometryShader);
	}
	else if (m_ShaderType == ST_ComputeShader)
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

	// 根据编译好的代码生成ShaderVarMap
	GenParamMapByD3DReflection(m_pCode, m_VariableMap);
}

void FShaderVarialbleMap::AddVariable(const String& VarName, uint BufferIndex, uint BindIndex, uint NumBytes)
{
	m_VarDescMap.insert(TPair<String, FShaderVariableDesc>(
		VarName, 
		FShaderVariableDesc(
			BufferIndex, 
			BindIndex, 
			NumBytes
			)
		)
	);
}

bool FShaderVarialbleMap::GetVariable(const String& VarName, uint& OutBufferIndex, uint& OutBindIndex, uint& OutNumBytes) const
{
	TMap<String, FShaderVariableDesc>::const_iterator MapItr = m_VarDescMap.find(VarName);

	if (MapItr != m_VarDescMap.end())
	{
		OutBufferIndex	= MapItr->second.BufferIndex;
		OutBindIndex	= MapItr->second.BindIndex;
		OutNumBytes		= MapItr->second.NumBytes;
		return true;
	}
	else
	{
		return false;
	}

	return true;
}

void FShaderVarialbleMap::Clear()
{
	m_VarDescMap.clear();
}

void FShaderConstantVarialble::Bind(const FShaderVarialbleMap& VariableMap, const String& VariableName)
{
	if (!VariableMap.GetVariable(VariableName, m_iBufferIndex, m_iBindIndex, m_iNumBytes))
	{
		// 获取失败，报错！
		Check(0);
	}
}

void FShaderResourceVariable::Bind(const FShaderVarialbleMap& VariableMap, const String& VariableName)
{
	uint DummyBufferIndex;
	uint DummySize;

	if (!VariableMap.GetVariable(VariableName, DummyBufferIndex, m_iBindIndex, DummySize))
	{
		// 获取失败，报错！
		Check(0);
	}
}