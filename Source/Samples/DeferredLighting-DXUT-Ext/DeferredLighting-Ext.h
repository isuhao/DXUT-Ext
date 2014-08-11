#ifndef _DEFERRED_LIGHTING_EXT_H_
#define _DEFERRED_LIGHTING_EXT_H_


#include "DXUT-Ext.h"
#include "DLShaders.h"


// Constant Buffer
struct CB_GPass
{
	D3DXMATRIX  m_mWorldViewProj;
	D3DXMATRIX	m_mWorld;
};

struct CB_DLPass
{
	D3DXVECTOR3 g_vLightDir;
	FLOAT		Padding;
};

struct CB_ScenePass
{
	D3DXMATRIX	m_mWVP;
	D3DXVECTOR3	m_AmbientColor;
	D3DXVECTOR3	m_LightDiffuse;
	D3DXVECTOR2	Padding;
};


enum EGBufferType
{
	EGBT_Normal,
	EGBT_Diffuse,
	EGBT_Specular,

	EGBT_MaxSize
};

class DeferredLightingApp : public FGameApp
{
public:
	DeferredLightingApp(const WString& AppName = L"GameApp", const WString& ResPath = L"")
		: FGameApp(AppName, ResPath)
	{
	}

	~DeferredLightingApp() 
	{
	}

	virtual void OnInit();
	virtual void OnDestroy();
	void OnRender(float fDeltaSeconds);

protected:
	void RenderGPass();
	void RenderDeferredLight();
	void RenderScene();

protected:
	// D3D×ÊÔ´
	TSharedPtr<FD3D11BoundShaderState>		m_pGPassBST;
	TSharedPtr<FD3D11BoundShaderState>		m_pDLPassBST;
	TSharedPtr<FD3D11BoundShaderState>		m_pScenePassBST;
	TSharedPtr<FD3D11SamplerState>			m_pSamplerState;
	TSharedPtr<FD3D11RasterState>				m_pCullBack;
	TSharedPtr<FD3D11BlendState>				m_pColorWriteOn;
	TSharedPtr<FSDKMesh>					m_pMesh;

	TSharedPtr<FConstantBuffer>				m_pcbGPass;
	TSharedPtr<FConstantBuffer>				m_pcbDLPass;
	TSharedPtr<FConstantBuffer>				m_pcbScenePass;

	TSharedPtr<FFrameBuffer>				m_pfbGPass;
	TSharedPtr<FFrameBuffer>				m_pfbDL;

	FGPassVertexShader						m_GPassVS;
	FGPassPixelShader						m_GPassPS;
	FDLVertexShader							m_DLPassVS;
	FDLPixelShader							m_DLPassPS;
	FSceneVertexShader						m_ScenePassVS;
	FScenePixelShader						m_ScenePassPS;
};



#endif