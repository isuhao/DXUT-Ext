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
 * 从Code中获取的ShaderReflection的描述，
 * 以此来绑定Shader中资源（包括CB、Texture、Sampler）
 */
struct FShaderVariableDesc
{
	uint	BufferIndex;	// CB的索引，这个只有CB会用到，Tex和Sampler不会用到
	uint	BindIndex;		// 相当于BindPoint
	uint	NumBytes;		// 变量的大小

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
 * Shader中的变量与相关描述的映射表
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
	 * 初始化，编译什么的
	 */
	void InitShader();

	/**
	 * 获取编译后的代码
	 */
	const TSharedPtr<ID3DBlob>& GetCode() { return m_pCode;  }

	/** 
	 * 绑定各种变量
	 * @TODO: 实现ShaderParameterMap，然后根据Map绑定参数
	 */
	virtual void BindVariables() {}

	/** 
	 * 设置Shader变量
	 */
	virtual void SetVariables(void* VarContext) {}

	/**
	 * 获取VarialbleMap
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
 * ConstantBuffer保存的常量
 */
class FShaderConstantVarialble
{
public:
	uint GetBufferIndex() const { return m_iBufferIndex; }
	uint GetBindIndex() const { return m_iBindIndex; }
	uint GetNumBytes() const { return m_iNumBytes; }

	void Bind(const FShaderVarialbleMap& VariableMap, const String& VariableName);

protected:
	uint m_iBufferIndex;	// CB索引
	uint m_iBindIndex;		// CB中变量的索引
	uint m_iNumBytes;		// CB中变量的大小
};

/** 
 * Shader中其他资源变量（Sampler和Texture）
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