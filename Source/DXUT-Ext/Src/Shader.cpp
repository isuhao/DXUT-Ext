#include "Shader.h"
#include "DXUT-Ext.h"

Shader::Shader(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, EShaderType ShaderType)
:	m_szFileName	(szFileName)
,	m_szEntryPoint	(szEntryPoint)
,	m_szShaderModel	(szShaderModel)
,	m_ShaderType	(ShaderType)
{
	InitShader();
}

void Shader::InitShader()
{
	// 初始化顶点声明
	InitVertexDeclaration();

	// 编译与创建
	CompileAndCreateShader();

	// 绑定各种参数
	Bind();
}

void Shader::CompileAndCreateShader()
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
	
	UINT NumElements = m_VertexDeclaration.Layouts.GetSize();
	if (NumElements > 0)
	{
		const D3D11_INPUT_ELEMENT_DESC* Layouts = m_VertexDeclaration.Layouts.GetData();
		ID3D11InputLayout*				pInputLayout = NULL;
		V(pd3dDevice->CreateInputLayout(Layouts, NumElements, pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &pInputLayout));
		m_pInputLayout = MakeCOMPtr<ID3D11InputLayout>(pInputLayout);
	}

	SAFE_RELEASE(pBlob);
}

//////////////////////////////////////////////////////////////////////////

void VertexDeclaration::Clear()
{
	Layouts.RemoveAll();
	CurrOffset = 0;
}

void VertexDeclaration::AppendElement(UINT StreamIndex, UINT Offset, EVertexElementType ElemType, EVertexElementUsage Usage, UINT UsageIndex, 
	bool bUseInstanceIndex /* = false */, UINT NumVerticesPerInstance /* = 0 */)
{
	D3D11_INPUT_ELEMENT_DESC D3DElement;

	D3DElement.InputSlot = StreamIndex;
	D3DElement.AlignedByteOffset = Offset;
	switch (ElemType)
	{
	case VET_Float1:		D3DElement.Format = DXGI_FORMAT_R32_FLOAT; break;
	case VET_Float2:		D3DElement.Format = DXGI_FORMAT_R32G32_FLOAT; break;
	case VET_Float3:		D3DElement.Format = DXGI_FORMAT_R32G32B32_FLOAT; break;
	case VET_Float4:		D3DElement.Format = DXGI_FORMAT_R32G32B32A32_FLOAT; break;
	case VET_PackedNormal:	D3DElement.Format = DXGI_FORMAT_R8G8B8A8_UNORM; break; 
	case VET_UByte4:		D3DElement.Format = DXGI_FORMAT_R8G8B8A8_UINT; break; 
	case VET_UByte4N:		D3DElement.Format = DXGI_FORMAT_R8G8B8A8_UNORM; break;
	case VET_Color:			D3DElement.Format = DXGI_FORMAT_R8G8B8A8_UNORM; break;
	case VET_Short2:		D3DElement.Format = DXGI_FORMAT_R16G16_SINT; break;
	case VET_Short2N:		D3DElement.Format = DXGI_FORMAT_R16G16_SNORM; break;
	case VET_Half2:			D3DElement.Format = DXGI_FORMAT_R16G16_FLOAT; break;
	default: 
		// @TODO: Aeron,  printf error 
		break;
	};

	switch (Usage)
	{
	case VEU_Position:			D3DElement.SemanticName = "POSITION"; break;
	case VEU_TextureCoordinate:	D3DElement.SemanticName = "TEXCOORD"; break;
	case VEU_BlendWeight:		D3DElement.SemanticName = "BLENDWEIGHT"; break;
	case VEU_BlendIndices:		D3DElement.SemanticName = "BLENDINDICES"; break;
	case VEU_Normal:			D3DElement.SemanticName = "NORMAL"; break;
	case VEU_Tangent:			D3DElement.SemanticName = "TANGENT"; break;
	case VEU_Binormal:			D3DElement.SemanticName = "BINORMAL"; break;
	case VEU_Color:				D3DElement.SemanticName = "COLOR"; break;
	};

	D3DElement.SemanticIndex = UsageIndex;
	D3DElement.InputSlotClass = bUseInstanceIndex ? D3D11_INPUT_PER_INSTANCE_DATA : D3D11_INPUT_PER_VERTEX_DATA;
	D3DElement.InstanceDataStepRate = bUseInstanceIndex ? 1 : 0;

	Layouts.Add(D3DElement);
}

/** 
 * 获取一个顶点说明的数据宽度
 */
UINT VertexDeclaration::GetFormatByteSize(EVertexElementType UType)
{
	UINT FormatSize = 0;
	switch (UType)
	{
	case VET_Float1:       return sizeof(FLOAT)* 1; break;
	case VET_Float2:       return sizeof(FLOAT)* 2; break;
	case VET_Float3:       return sizeof(FLOAT)* 3; break;
	case VET_Float4:       return sizeof(FLOAT)* 4; break;
	case VET_PackedNormal: return sizeof(BYTE)* 4;  break;
	case VET_UByte4:       return sizeof(BYTE)* 4;  break;
	case VET_UByte4N:      return sizeof(BYTE)* 4;  break;
	case VET_Color:        return sizeof(BYTE)* 4;  break;
	case VET_Short2:       return sizeof(WORD)* 2;  break;
	case VET_Short2N:      return sizeof(WORD)* 2;  break;
	case VET_Half2:        return sizeof(WORD)* 2;  break;
	}
	Check(FormatSize > 0);
	return FormatSize;
}

/** 
 * 在同一个Stream中快速加入一个顶点说明，自动计算偏移量
 * @param bResetOffset 是否重置Offset
 */
void VertexDeclaration::AppendElementFast(UINT StreamIndex, EVertexElementType ElemType, EVertexElementUsage Usage, UINT UsageIndex, bool bResetOffset /* = false */, 
	bool bUseInstanceIndex /* = false */, UINT NumVerticesPerInstance /* = 0 */)
{
	if (bResetOffset)
	{
		CurrOffset = 0;
	}

	AppendElement(StreamIndex, CurrOffset, ElemType, Usage, UsageIndex, bUseInstanceIndex, NumVerticesPerInstance);

	CurrOffset += GetFormatByteSize(ElemType);
}