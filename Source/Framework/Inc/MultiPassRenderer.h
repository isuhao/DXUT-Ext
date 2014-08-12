#ifndef _QUICK_MULTI_PASS_H_
#define _QUICK_MULTI_PASS_H_

#include "MultiPassBase.h"


#define RENDER_PASS_BEGIN	{ RenderPassBegin(); }
#define RENDER_PASS_END		{ RenderPassEnd(); }

/**
 * ���Pass����Ⱦ��
 */
class FMultiPassRenderer : public FAppFramework
{
public:
	typedef void(*RenderModelPassType)(FPassModelContext* pModelContext);


	FMultiPassRenderer(const WString& AppName = L"GameApp", const WString& ResPath = L"")
		: FAppFramework(AppName, ResPath)
	{
	}

	~FMultiPassRenderer()
	{
	}

	virtual void OnInit();
	virtual void OnRender(float fDeltaSeconds);
	virtual void OnGUIEvent(uint nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext);

	/**
	 * ע����Դ
	 */
	virtual void RegisterResources() {}
	virtual void RenderPasses() {}

	void PostRegiester();
	void RenderPassBegin();
	void RenderPassEnd();

	/** 
	 * ��Ⱦһ������Model��Pass
	 * @param ActualPassFunc ������Ⱦ�Ļص�����
	 */
	void RenderPassWithModel(const String& RegisterModel, RenderModelPassType ActualPassFunc);

	void InitInputLayouts();

	// SinglePass Begin

	// 1.����
	/**
	 * ��ģ�ͣ�VB��IB��TexCoord�ȣ�
	 */
	void PassSetModel(const String& RegisterModel);

	// 2. ��ͼ
	/**
	 * ������ͨ2D��ͼ
	 * @param ShaderVarName		Shader�ж�Ӧ�ı�����
	 * @param RegisterTexture	�Ѿ�ע���Texture��
	 */
	void PassSetTexture2D(const String& ShaderVarName, const String& RegisterTexture);

	/**
	 * ����RenderTarget��ͼ
	 * @param ShaderVarName		Shader�ж�Ӧ�ı�����
	 * @param RegisterTexture	�Ѿ�ע���FrameBuffer��
	 */
	void PassSetRenderTexture(const String& ShaderVarName, const String& RegisterName);
	/**
	 * ����RenderTarget��ͼ
	 * @param ShaderVarName		Shader�ж�Ӧ�ı�����
	 * @param RegisterTexture	�Ѿ�ע���FrameBuffer��
	 */
	void PassSetDepthTexture(const String& ShaderVarName, const String& RegisterName);

	// 3. Sampler
	/**
	 * ����Sampler
	 * @param ShaderVarName		Shader�ж�Ӧ�ı�����
	 * @param RegisterSampler	�Ѿ�ע���Sampler��
	 */
	void PassSetSampler(const String& ShaderVarName, const String& RegisterSampler);

	// 4. ConstantValue
	template <typename ValueType>
	void PassSetConstantValue(EShaderType ShaderType, const String& ShaderVarName, ValueType& Value);
	template <typename ValueType>
	void PassSetConstantValue(EShaderType ShaderType, const String& ShaderVarName, uint NumBytes, ValueType* Value);
	// ʹ���Ѿ�ע��õĳ���������Constant
	void PassSetConstantValue(EShaderType ShaderType, const String& ShaderVarName, const String& RegisterName);

	// 5. Shader
	void PassSetShader(const String& RegisterName);

	// 6. ����MRT
	void PassSetTarget(const String& RegisterName, uint RTIndex = 0);

	// SinglePass End

protected:

	// Register Begin

	// ע��Shader
	void RegisterShader(const String& RegisterName, const WString& FileName, const String& EntryPoint, const String& ShaderModel, EShaderType ShaderType);
	// ע��һ��PassҪ�õ���Shader
	void RegisterPassShader(const String& RegisterName, EShaderType ShaderType, const WString& FileName, const String& EntryPoint, const String& ShaderModel);

	// ע�᳣��
	template <typename ValueType>
	void RegisterConstant(const String& RegisterName, ValueType& Value);
	template <typename ValueType>
	void RegisterConstant(const String& RegisterName, ValueType* Value, uint NumBytes);

	// ע��Model, Ŀǰֻ��Mesh
	void RegisterModel(const String& RegisterName, const TSharedPtr<FMesh>& InMesh);
	void RegisterModel(const String& RegisterName, ESimplePrimitiveType PrimType);

	// ע��FrameBuffer
	// ���ûָ�����͸ߣ���ʹ����BackBufferһ���Ĵ�С
	void RegisterFrameBuffer(const String& RegisterName, uint Width, uint Height, EPixelFormat PixFormat);
	void RegisterFrameBuffer(const String& RegisterName);

	// Register End

protected:

	TMap<String, TSharedPtr<FD3D11BoundShaderState> >	m_RegisterBoundShaders;
	TMap<String, TSharedPtr<FMultiPassShader> >			m_RegisterShaders;							// �Ѿ�ע���Shader
	String												m_CurrRunningShaders[ST_NumShaderTypes];	// Ŀǰ��Ч��Shader

	TMap<String, TSharedPtr<FPassConstantContext> >		m_RegisterConstants;
	TMap<String, FShaderResourceVariable>				m_RegisterSamplers;
	TMap<String, FShaderResourceVariable>				m_RegisterTextures;
	TMap<String, TSharedPtr<FFrameBuffer> >				m_RegisterFrameBuffers;
	TMap<String, TSharedPtr<FMesh> >					m_RegisterModels;

	TSharedPtr<FVertexDeclaration>						m_pVertexDeclaration;
};


template <typename ValueType>
void FMultiPassRenderer::PassSetConstantValue(EShaderType ShaderType, const String& ShaderVarName, ValueType& Value)
{
	PassSetConstantValue(ShaderType, ShaderVarName, sizeof(ValueType), &Value);
}

template <typename ValueType>
void FMultiPassRenderer::PassSetConstantValue(EShaderType ShaderType, const String& ShaderVarName, uint NumBytes, ValueType* Value)
{
	const String& ShaderName = m_CurrRunningShaders[ShaderType];
	if (ShaderName != "")
	{
		TMap<String, TSharedPtr<FMultiPassShader> >::iterator Itr = m_RegisterShaders.find(ShaderName);
		if (Itr != m_RegisterShaders.end())
		{
			TSharedPtr<FMultiPassShader>& FoundShader = Itr->second;
			if (FoundShader)
			{
				FoundShader->SetConstantVariables(ShaderVarName, NumBytes, Value);
			}
		}
	}

	// @TODO: ��ӡ������Ϣ
	Check(0);
}

// ����������ע�ᰡ��
template <typename ValueType>
void FMultiPassRenderer::RegisterConstant(const String& RegisterName, ValueType& Value)
{
	RegisterConstant(RegisterName, &Value, sizeof(ValueType));
}

template <typename ValueType>
void FMultiPassRenderer::RegisterConstant(const String& RegisterName, ValueType* Value, uint NumBytes)
{
	TSharedPtr<FPassConstantContext> NewConst = TSharedPtr<FPassConstantContext>(new FPassConstantContext(NumBytes, Value));
	m_RegisterConstants[RegisterName] = NewConst;
}


#endif