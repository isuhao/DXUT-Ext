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
	 * �󶨸��ֲ���
	 * @TODO: ʵ��ShaderParameterMap��Ȼ�����Map�󶨲���
	 */
	virtual void BindParameters() {}

	/** 
	 * ���ò���
	 */
	virtual void SetShaderParameters(void* Data) {}

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
};

#endif