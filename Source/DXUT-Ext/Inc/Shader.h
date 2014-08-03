#ifndef _SHADER_H_
#define _SHADER_H_

#include "Core.h"
#include "RHI.h"

#define DECLARE_SHADER(ShaderClass, FileName, EntryPoint, ShaderModel, ShaderType) \
	ShaderClass() : Shader(FileName, EntryPoint, ShaderModel, ShaderType) {} \
	public:

#define SET_SHADER_VERT_DECL(ShaderClass, VerDecl) \
	void ShaderClass::InitVertexDeclaration() \
	{ m_pVertexDeclaration = VerDecl; }

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

class VertexDeclaration 
{
public:
	VertexDeclaration()
	{
		Clear();
	}

	VertexDeclaration(const VertexDeclaration& Other)
	{
		m_nCurrOffset	= Other.m_nCurrOffset;
		m_Layouts		= Other.m_Layouts;
	}

	const TArray<D3D11_INPUT_ELEMENT_DESC>& GetLayouts() 
	{ 
		return m_Layouts;
	}

	void AppendElement(UINT StreamIndex, UINT Offset, EVertexElementType ElemType, EVertexElementUsage Usage, UINT UsageIndex, 
		bool bUseInstanceIndex = false,	UINT NumVerticesPerInstance = 0);

	void AppendElementFast(UINT StreamIndex, EVertexElementType ElemType, EVertexElementUsage Usage, UINT UsageIndex, bool bResetOffset = false, 
		bool bUseInstanceIndex = false,	UINT NumVerticesPerInstance = 0);

	static UINT GetFormatByteSize(EVertexElementType UType);

	void Clear();

private:
	UINT										m_nCurrOffset; // 这个值只有AppendElementFast才有用
	TArray<D3D11_INPUT_ELEMENT_DESC>			m_Layouts;

	friend class VertexDeclaration;
};

class Shader 
{
public:
	Shader(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, EShaderType ShaderType);
	~Shader() {}

	/**
	 * 初始化顶点声明
	 */
	virtual void InitVertexDeclaration() {}

	/** 
	 * 绑定各种参数
	 * @TODO: 实现ShaderParameterMap，然后根据Map绑定参数
	 */
	virtual void Bind() {}

	/** 
	 * 设置参数
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
	TSharedPtr<VertexDeclaration>		m_pVertexDeclaration;

	TSharedPtr<ID3D11InputLayout>		m_pInputLayout;
	TSharedPtr<ID3D11VertexShader>		m_pVertexShader;
	TSharedPtr<ID3D11PixelShader>		m_pPixelShader;
	TSharedPtr<ID3D11DomainShader>		m_pDomainShader;
	TSharedPtr<ID3D11HullShader>		m_pHullShader;
	TSharedPtr<ID3D11GeometryShader>	m_pGemetryShader;
	TSharedPtr<ID3D11ComputeShader>		m_pComputeShader;
};

#endif