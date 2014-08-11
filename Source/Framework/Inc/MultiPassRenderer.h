#ifndef _QUICK_MULTI_PASS_H_
#define _QUICK_MULTI_PASS_H_

#include "AppFramework.h"


enum EPassDrawType
{
	PDT_Geometry,	// ��������
	PDT_Buffer,		// ��Buffer
};


///**
// * ����Pass������
// */
//struct FSinglePassDesc
//{
//	// ��������
//	EPassDrawType	DrawType;
//	// ����Ǽ����壬ʹ����Щ���㼯��
//	TArray<uint>	VertexSetSlots;
//	// �����Buffer��ʹ����ЩRenderTexture����ȡRT��Texture��
//	TArray<uint>	RenderTextureSlots;
//	// ָ��������ЩRT
//	TArray<uint>	RenderTargetSlots;
//	// ָ��ʹ����ЩTexture
//	TArray<uint>	TextureSlots;
//	// ָ��ʹ����Щ������
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
 * ����Model��������
 */
struct FPassModelContext
{
	String	Name;
	FMesh	Mesh;
};

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

	/**
	 * ע����Դ
	 */
	virtual void RegisterResources() {}

	void BeginRenderPass() {}
	void EndRenderPass() {}


	/** 
	 * ��Ⱦһ������Model��Pass
	 * @param ActualPassFunc ������Ⱦ�Ļص�����
	 */
	void RenderPassWithModel(const String& RegisterModel, RenderModelPassType ActualPassFunc);

	// SinglePass Begin
	// 1.����
	/**
	 * ��ģ�ͣ�VB��IB��TexCoord�ȣ�
	 */
	void PassSetModel(const String& RegisterModel);

	// 2. ��ͼ
	/**
	 * ������ͼ
	 * @param ShaderVarName		Shader�ж�Ӧ�ı�����
	 * @param RegisterTexture	�Ѿ�ע���Texture��
	 */
	void PassSetTexture(const String& ShaderVarName, const String& RegisterTexture);

	/**
	 * ����RT(Surface)����ͼ
	 * @param ShaderVarName		Shader�ж�Ӧ�ı�����
	 * @param RegisterTexture	�Ѿ�ע���Texture��
	 */
	void PassSetSurfaceTexture(const String& ShaderVarName, const String& RegisterTexture);

	// 3. Sampler
	/**
	 * ����Sampler
	 * @param ShaderVarName		Shader�ж�Ӧ�ı�����
	 * @param RegisterSampler	�Ѿ�ע���Sampler��
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