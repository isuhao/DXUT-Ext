#ifndef _SHADER_H_
#define _SHADER_H_

#include "Core.h"
#include "RHI.h"

#define DECLARE_SHADER(ShaderClass, FileName, EntryPoint, ShaderModel, ShaderType) \
	ShaderClass() : Shader(FileName, EntryPoint, ShaderModel, ShaderType) {} \
	public:

enum EShaderType
{
	EST_VertexShader,
	EST_PixelShader,
	EST_DomainShader,
	EST_HullShader,
	EST_GeometryShader,
	EST_ComputeShader,

	EST_Max
};


enum EVertexElementType
{
	VET_None,
	VET_Float1,
	VET_Float2,
	VET_Float3,
	VET_Float4,
	VET_PackedNormal,	
	VET_UByte4,
	VET_UByte4N,
	VET_Color,
	VET_Short2,
	VET_Short2N,		
	VET_Half2,			
	VET_MAX
};

enum EVertexElementUsage
{
	VEU_Position,
	VEU_TextureCoordinate,
	VEU_BlendWeight,
	VEU_BlendIndices,
	VEU_Normal,
	VEU_Tangent,
	VEU_Binormal,
	VEU_Color
};

struct VertexDeclaration 
{
	TArray<D3D11_INPUT_ELEMENT_DESC>			Layouts;

	// ���ֵֻ��AppendElementFast������
	UINT										CurrOffset;

	void Clear();

	void AppendElement(UINT StreamIndex, UINT Offset, EVertexElementType ElemType, EVertexElementUsage Usage, UINT UsageIndex, 
		bool bUseInstanceIndex = false,	UINT NumVerticesPerInstance = 0);

	void AppendElementFast(UINT StreamIndex, EVertexElementType ElemType, EVertexElementUsage Usage, UINT UsageIndex, bool bResetOffset = false, 
		bool bUseInstanceIndex = false,	UINT NumVerticesPerInstance = 0);

	static UINT GetFormatByteSize(EVertexElementType UType);
};

class Shader 
{
public:
	Shader(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, EShaderType ShaderType);
	~Shader() {}

	/**
	 * ��ʼ����������
	 */
	virtual void InitVertexDeclaration() {}

	/** 
	 * �󶨸��ֲ���
	 * @TODO: ʵ��ShaderParameterMap��Ȼ�����Map�󶨲���
	 */
	virtual void Bind() {}

	/** 
	 * ���ò���
	 */
	virtual void SetShaderParameters(void* Data) {}

	const TSharedPtr<ID3D11InputLayout>& GetInputLayout() { return m_pInputLayout; }
	const TSharedPtr<ID3D11VertexShader>& GetVertexShader() { return m_pVertexShader;  }
	const TSharedPtr<ID3D11PixelShader>& GetPixelShader() { return m_pPixelShader;  }
	const TSharedPtr<ID3D11DomainShader>& GetDomainShader() { return m_pDomainShader;  }
	const TSharedPtr<ID3D11HullShader>& GetHullShader() { return m_pHullShader; }
	const TSharedPtr<ID3D11GeometryShader>& GetGeometryShader() { return m_pGemetryShader;  }
	const TSharedPtr<ID3D11ComputeShader>& GetComputeShader() { return m_pComputeShader; }

protected:
	void InitShader();
	void CompileAndCreateShader();

protected:
	WString								m_szFileName;
	String								m_szEntryPoint;
	String								m_szShaderModel;
	EShaderType							m_ShaderType;
	VertexDeclaration					m_VertexDeclaration;

	TSharedPtr<ID3D11InputLayout>		m_pInputLayout;
	TSharedPtr<ID3D11VertexShader>		m_pVertexShader;
	TSharedPtr<ID3D11PixelShader>		m_pPixelShader;
	TSharedPtr<ID3D11DomainShader>		m_pDomainShader;
	TSharedPtr<ID3D11HullShader>		m_pHullShader;
	TSharedPtr<ID3D11GeometryShader>	m_pGemetryShader;
	TSharedPtr<ID3D11ComputeShader>		m_pComputeShader;
};

#endif