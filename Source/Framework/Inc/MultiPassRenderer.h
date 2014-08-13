#ifndef _QUICK_MULTI_PASS_H_
#define _QUICK_MULTI_PASS_H_

#include "MultiPassBase.h"


#define RENDER_PASS_BEGIN	{ RenderPassBegin(); }
#define RENDER_PASS_END		{ RenderPassEnd(); }

/**
 * 多个Pass的渲染器
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
	 * 注册资源
	 */
	virtual void RegisterResources() {}
	virtual void RenderPasses() {}

	/** 
	 * 渲染一个带有Model的Pass
	 * @param ActualPassFunc 真正渲染的回调函数
	 */
	void RenderPassWithModel(const String& RegisterModel, RenderModelPassType ActualPassFunc);

	void InitInputLayouts();

	// SinglePass Begin

	// 1.顶点
	/**
	 * 绑定模型（VB、IB、TexCoord等）
	 */
	void PassSetModel(const String& RegisterName);

	// 2. 贴图
	/**
	 * 设置普通2D贴图
	 * @param ShaderVarName		Shader中对应的变量名
	 * @param RegisterTexture	已经注册的Texture名
	 */
	void PassSetTexture2D(const String& ShaderVarName, const String& RegisterTexture);

	/**
	 * 设置RenderTarget贴图
	 * @param ShaderVarName		Shader中对应的变量名
	 * @param RegisterTexture	已经注册的FrameBuffer名
	 */
	void PassSetRenderTexture(const String& ShaderVarName, const String& RegisterName);
	/**
	 * 设置RenderTarget贴图
	 * @param ShaderVarName		Shader中对应的变量名
	 * @param RegisterTexture	已经注册的FrameBuffer名
	 */
	void PassSetDepthTexture(const String& ShaderVarName, const String& RegisterName);

	// 3. Sampler
	/**
	 * 设置Sampler
	 * @param ShaderVarName		Shader中对应的变量名
	 * @param RegisterSampler	已经注册的Sampler名
	 */
	void PassSetSampler(const String& ShaderVarName, const String& RegisterSampler);

	// 4. ConstantValue
	template <typename ValueType>
	void PassSetConstantValue(EShaderType ShaderType, const String& ShaderVarName, ValueType& Value);
	template <typename ValueType>
	void PassSetConstantValue(EShaderType ShaderType, const String& ShaderVarName, uint NumBytes, ValueType* Value);
	// 使用已经注册好的常量来设置Constant
	void PassSetConstantValue(EShaderType ShaderType, const String& ShaderVarName, const String& RegisterName);

	// 5. Shader
	void PassSetShader(const String& RegisterName);

	// 6. 设置MRT
	void PassSetTarget(const String& RegisterName, uint RTIndex = 0);

	// 7. 设置RasterizeState
	void PassSetRasterizeState(const String& RegisterName);

	// 8. 设置BlendState
	void PassSetBlendState(const String& RegisterName);

	// SinglePass End

protected:

	// 默认的状态
	void RegisterDefaultStates();
	void PassSetDefaultStates();
	// 默认的常量
	void RegisterDefaultConstants();
	void PassSetDefaultConstants();

	void PostRegiester();
	void RenderPassBegin();
	void RenderPassEnd();

	// Register Begin

	// 注册一个Pass要用到的Shader
	void RegisterShader(const String& RegisterName, EShaderType ShaderType, const WString& FileName, const String& EntryPoint, const String& ShaderModel);

	// 注册常量
	template <typename ValueType>
	void RegisterConstant(const String& RegisterName, ValueType& Value);
	template <typename ValueType>
	void RegisterConstant(const String& RegisterName, ValueType* Value, uint NumBytes);

	// 注册Model, 目前只有Mesh
	void RegisterModel(const String& RegisterName, const TSharedPtr<FMesh>& InMesh);
	void RegisterModel(const String& RegisterName, ESimplePrimitiveType PrimType);

	// 注册FrameBuffer
	// 如果没指定宽和高，则使用与BackBuffer一样的大小
	void RegisterFrameBuffer(const String& RegisterName, uint Width, uint Height, EPixelFormat PixFormat);
	void RegisterFrameBuffer(const String& RegisterName);

	void RegisterRasterizeState(const String& RegisterName, ERasterizerFillMode FillMode = FM_Solid, ERasterizerCullMode CullMode = CM_CCW, bool bAllowMSAA = true,
		bool bFrontCounterClockwise = false, INT DepthBias = 0, float SlopeScaledDepthBias = 0);
	void RegisterBlendState(const String& RegisterName, EBlendOperation ColorBlendOp = BO_Add, EBlendFactor ColorSrcBlend = BF_One, EBlendFactor ColorDstBlend = BF_Zero,
		EBlendOperation AlphaBlendOp = BO_Add, EBlendFactor AlphaSrcBlend = BF_One, EBlendFactor AlphaDstBlend = BF_Zero, EBlendColorWriteEnable ColorWriteEnable = CWE_All);

	// Register End

protected:

	TMap<String, TSharedPtr<FD3D11RasterState> >		m_RegisterRasterizeStates;
	TMap<String, TSharedPtr<FD3D11BlendState> >			m_RegisterBlendStates;

	TMap<String, TSharedPtr<FD3D11BoundShaderState> >	m_RegisterBoundShaders;
	TMap<String, TSharedPtr<FMultiPassShader> >			m_RegisterShaders;		// 已经注册的Shader
	String												m_CurrBoundShader;		// 目前生效的Shader

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
	// @TODO: 这个要重新写了！！！
	// !!!!!!!!!!!!!!!!!!<<<<<<<<<<<<<<<<<<<<<<<<<

	//const String& ShaderName = m_CurrRunningShaders[ShaderType];
	//if (ShaderName != "")
	//{
	//	TMap<String, TSharedPtr<FMultiPassShader> >::iterator Itr = m_RegisterShaders.find(ShaderName);
	//	if (Itr != m_RegisterShaders.end())
	//	{
	//		TSharedPtr<FMultiPassShader>& FoundShader = Itr->second;
	//		if (FoundShader)
	//		{
	//			FoundShader->SetConstantVariables(ShaderVarName, NumBytes, Value);
	//		}
	//	}
	//}

	// @TODO: 打印错误信息
	Check(0);
}

// 常量好像不用注册啊！
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