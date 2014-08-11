#ifndef _QUICK_MULTI_PASS_H_
#define _QUICK_MULTI_PASS_H_

#include "AppFramework.h"


enum EPassDrawType
{
	PDT_Geometry,	// 画几何体
	PDT_Buffer,		// 画Buffer
};


/**
 * 单个Pass的描述
 */
struct FSinglePassDesc
{
	// 绘制类型
	EPassDrawType	DrawType;
	// 如果是几何体，使用哪些顶点集合
	TArray<uint>	VertexSetSlots;
	// 如果是Buffer，使用哪些RenderTexture（读取RT的Texture）
	TArray<uint>	RenderTextureSlots;
	// 指定绘制哪些RT
	TArray<uint>	RenderTargetSlots;
	// 指定使用哪些Texture
	TArray<uint>	TextureSlots;
	// 指定使用哪些采样器
	TArray<uint>	SamplerSlots;

	FSinglePassDesc(
		EPassDrawType InDrawType,
		const TArray<uint>& InVertexSetSlots,
		const TArray<uint>& InRenderTextureSlots,
		const TArray<uint>& InRenderTargetSlots,
		const TArray<uint>& InTextureSlots,
		const TArray<uint>& InSamplerSlots
		)
		: DrawType(InDrawType)
		, VertexSetSlots(InVertexSetSlots)
		, RenderTextureSlots(InRenderTextureSlots)
		, RenderTargetSlots(InRenderTargetSlots)
		, TextureSlots(InTextureSlots)
		, SamplerSlots(InSamplerSlots)
	{
	}
};

class FQuickMultiPass : public FAppFramework
{
public:
	FQuickMultiPass(const WString& AppName = L"GameApp", const WString& ResPath = L"")
		: FAppFramework(AppName, ResPath)
	{
	}

	~FQuickMultiPass()
	{
	}

	virtual void OnInit();

	virtual void InitResources() {}

	virtual void AddPasses() {}

protected:
	TArray<FSinglePassDesc>		m_RenderPasses;
};

#endif