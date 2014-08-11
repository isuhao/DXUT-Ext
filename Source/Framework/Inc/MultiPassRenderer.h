#ifndef _QUICK_MULTI_PASS_H_
#define _QUICK_MULTI_PASS_H_

#include "AppFramework.h"


enum EPassDrawType
{
	PDT_Geometry,	// 画几何体
	PDT_Buffer,		// 画Buffer
};


///**
// * 单个Pass的描述
// */
//struct FSinglePassDesc
//{
//	// 绘制类型
//	EPassDrawType	DrawType;
//	// 如果是几何体，使用哪些顶点集合
//	TArray<uint>	VertexSetSlots;
//	// 如果是Buffer，使用哪些RenderTexture（读取RT的Texture）
//	TArray<uint>	RenderTextureSlots;
//	// 指定绘制哪些RT
//	TArray<uint>	RenderTargetSlots;
//	// 指定使用哪些Texture
//	TArray<uint>	TextureSlots;
//	// 指定使用哪些采样器
//	TArray<uint>	SamplerSlots;
//
//	FSinglePassDesc(
//		EPassDrawType InDrawType,
//		const TArray<uint>& InVertexSetSlots,
//		const TArray<uint>& InRenderTextureSlots,
//		const TArray<uint>& InRenderTargetSlots,
//		const TArray<uint>& InTextureSlots,
//		const TArray<uint>& InSamplerSlots
//		)
//		: DrawType(InDrawType)
//		, VertexSetSlots(InVertexSetSlots)
//		, RenderTextureSlots(InRenderTextureSlots)
//		, RenderTargetSlots(InRenderTargetSlots)
//		, TextureSlots(InTextureSlots)
//		, SamplerSlots(InSamplerSlots)
//	{
//	}
//};

/**
 * 关于Model的上下文
 */
struct FPassModelContext
{
	String	Name;
	FMesh	Mesh;
};

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

	/**
	 * 注册资源
	 */
	virtual void RegisterResources() {}

	void BeginRenderPass() {}
	void EndRenderPass() {}


	/** 
	 * 渲染一个带有Model的Pass
	 * @param ActualPassFunc 真正渲染的回调函数
	 */
	void RenderPassWithModel(const String& RegisterModel, RenderModelPassType ActualPassFunc);

	// SinglePass Begin
	// 1.顶点
	/**
	 * 绑定模型（VB、IB、TexCoord等）
	 */
	void PassSetModel(const String& RegisterModel);

	// 2. 贴图
	/**
	 * 设置贴图
	 * @param ShaderVarName		Shader中对应的变量名
	 * @param RegisterTexture	已经注册的Texture名
	 */
	void PassSetTexture(const String& ShaderVarName, const String& RegisterTexture);

	/**
	 * 设置RT(Surface)的贴图
	 * @param ShaderVarName		Shader中对应的变量名
	 * @param RegisterTexture	已经注册的Texture名
	 */
	void PassSetSurfaceTexture(const String& ShaderVarName, const String& RegisterTexture);

	// 3. Sampler
	/**
	 * 设置Sampler
	 * @param ShaderVarName		Shader中对应的变量名
	 * @param RegisterSampler	已经注册的Sampler名
	 */
	void PassSetSampler(const String& ShaderVarName, const String& RegisterSampler);

	// 4. ConstantValue
	template <typename ValueType>
	void PassSetConstantValue(const String& ShaderVarName, ValueType& Value);
	template <typename ValueType>
	void PassSetConstantValue(const String& ShaderVarName, ValueType* Value);

	// SinglePass End

	// 0.InputLayout
	void InitInputLayouts();

protected:
};


template <typename ValueType>
void FMultiPassRenderer::PassSetConstantValue(const String& ShaderVarName, ValueType& Value)
{

}

template <typename ValueType>
void FMultiPassRenderer::PassSetConstantValue(const String& ShaderVarName, ValueType* Value)
{

}


#endif