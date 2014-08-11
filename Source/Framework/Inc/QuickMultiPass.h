#ifndef _QUICK_MULTI_PASS_H_
#define _QUICK_MULTI_PASS_H_

#include "AppFramework.h"


enum EPassDrawType
{
	PDT_Geometry,	// ��������
	PDT_Buffer,		// ��Buffer
};


/**
 * ����Pass������
 */
struct FSinglePassDesc
{
	// ��������
	EPassDrawType	DrawType;
	// ����Ǽ����壬ʹ����Щ���㼯��
	TArray<uint>	VertexSetSlots;
	// �����Buffer��ʹ����ЩRenderTexture����ȡRT��Texture��
	TArray<uint>	RenderTextureSlots;
	// ָ��������ЩRT
	TArray<uint>	RenderTargetSlots;
	// ָ��ʹ����ЩTexture
	TArray<uint>	TextureSlots;
	// ָ��ʹ����Щ������
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