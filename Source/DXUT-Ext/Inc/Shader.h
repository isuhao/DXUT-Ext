#ifndef _SHADER_H_
#define _SHADER_H_

#include "Core.h"
#include "RHI.h"
#include "VertexDelcaration.h"

#define DECLARE_SHADER(ShaderClass, FileName, EntryPoint, ShaderModel, ShaderType) \
public: \
	ShaderClass() : FShader(FileName, EntryPoint, ShaderModel, ShaderType) {}


#define CREATE_SHADER(ShaderClass, ShaderName) \
	ShaderClass ShaderName; \
	ShaderName.InitShader();

/**
 * ��Code�л�ȡ��ShaderReflection��������
 * �Դ�����Shader����Դ������CB��Texture��Sampler��
 */
struct FShaderVariableDesc
{
	uint	BufferIndex;	// CB�����������ֻ��CB���õ���Tex��Sampler�����õ�
	uint	BindIndex;		// �൱��BindPoint
	uint	NumBytes;		// �����Ĵ�С

	FShaderVariableDesc(
		uint InBufferIndex,
		uint InBindIndex,
		uint InNumBytes
		)
		: BufferIndex(InBufferIndex)
		, BindIndex(InBindIndex)
		, NumBytes(InNumBytes)
	{
	}
};

/**
 * Shader�еı��������������ӳ���
 */
class FShaderVarialbleMap
{
public:
	void AddVariable(const String& VarName, uint BufferIndex, uint BindIndex, uint NumBytes);
	bool GetVariable(const String& VarName, uint& OutBufferIndex, uint& OutBindIndex, uint& OutNumBytes) const;
	void Clear();

private:
	TMap<String, FShaderVariableDesc> m_VarDescMap;
};

class FShaderVarialbleMap;

class FShader 
{
public:
	FShader(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, EShaderType ShaderType);
	~FShader() {}

	/**
	 * ��ʼ��������ʲô��
	 */
	void InitShader();

	/**
	 * ��ȡ�����Ĵ���
	 */
	const TSharedPtr<ID3DBlob>& GetCode() { return m_pCode;  }

	/** 
	 * �󶨸��ֱ���
	 * @TODO: ʵ��ShaderParameterMap��Ȼ�����Map�󶨲���
	 */
	virtual void BindVariables() {}

	/** 
	 * ����Shader����
	 */
	virtual void SetVariables(void* VarContext) {}

	/**
	 * ��ȡVarialbleMap
	 */
	FShaderVarialbleMap* GetVariableMap()
	{
		return &m_VariableMap;
	}

	const TSharedPtr<ID3D11VertexShader>& GetVertexShader() { return m_pVertexShader;  }
	const TSharedPtr<ID3D11PixelShader>& GetPixelShader() { return m_pPixelShader;  }
	const TSharedPtr<ID3D11DomainShader>& GetDomainShader() { return m_pDomainShader;  }
	const TSharedPtr<ID3D11HullShader>& GetHullShader() { return m_pHullShader; }
	const TSharedPtr<ID3D11GeometryShader>& GetGeometryShader() { return m_pGemetryShader;  }
	const TSharedPtr<ID3D11ComputeShader>& GetComputeShader() { return m_pComputeShader; }

protected:
	void CompileAndCreateShader();

protected:
	WString								m_szFileName;
	String								m_szEntryPoint;
	String								m_szShaderModel;
	EShaderType							m_ShaderType;

	TSharedPtr<ID3D11VertexShader>		m_pVertexShader;
	TSharedPtr<ID3D11PixelShader>		m_pPixelShader;
	TSharedPtr<ID3D11DomainShader>		m_pDomainShader;
	TSharedPtr<ID3D11HullShader>		m_pHullShader;
	TSharedPtr<ID3D11GeometryShader>	m_pGemetryShader;
	TSharedPtr<ID3D11ComputeShader>		m_pComputeShader;
	TSharedPtr<ID3DBlob>				m_pCode;

	FShaderVarialbleMap					m_VariableMap;
};

/** 
 * ConstantBuffer����ĳ���
 */
class FShaderConstantVarialble
{
public:
	uint GetBufferIndex() const { return m_iBufferIndex; }
	uint GetBindIndex() const { return m_iBindIndex; }
	uint GetNumBytes() const { return m_iNumBytes; }

	void Bind(const FShaderVarialbleMap& VariableMap, const String& VariableName);

protected:
	uint m_iBufferIndex;	// CB����
	uint m_iBindIndex;		// CB�б���������
	uint m_iNumBytes;		// CB�б����Ĵ�С
};

/** 
 * Shader��������Դ������Sampler��Texture��
 */
class FShaderResourceVariable
{
public:
	uint GetBindIndex() const { return m_iBindIndex; }

	void Bind(const FShaderVarialbleMap& VariableMap, const String& VariableName);

protected:
	uint m_iBindIndex;
};


#endif