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
	DeferredLightingApp();
	~DeferredLightingApp() {}

	virtual void OnInit();
	virtual void OnDestroy();
	HRESULT OnCreateDevice(ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc);
	HRESULT OnResizedSwapChain(ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc) { return S_OK;  }
	void OnTick(float DeltaSeconds);
	void OnRender(float fDeltaSeconds);
	void OnKeyboard(UINT nChar, bool bKeyDown, bool bAltDown) {}
	void OnGUIEvent(UINT nEvent, int nControlID, CDXUTControl* pControl) {}

protected:
	void RenderGPass();
	void RenderDeferredLight();
	void RenderScene();
	void DLInitResource(ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc);

protected:
	// D3D��Դ

	// G-Buffer
	//ID3D11DepthStencilState*	g_depthStencilStateDisableDepth = NULL;
	//ID3D11Texture2D*			g_pGBufferTexture[EGBT_MaxSize];
	//ID3D11RenderTargetView*		g_pGBufferTextureRT[EGBT_MaxSize];
	//ID3D11ShaderResourceView*	g_pGBufferTextureSRV[EGBT_MaxSize];

	TSharedPtr<FRHIBoundShaderState>		m_pGPassBST;
	TSharedPtr<FRHIBoundShaderState>		m_pDLPassBST;
	TSharedPtr<FRHIBoundShaderState>		m_pScenePassBST;
	TSharedPtr<FRHISamplerState>			m_pSamplerState;
	TSharedPtr<FRHIRasterState>				m_pCullBack;
	TSharedPtr<FRHIBlendState>				m_pColorWriteOn;
	TSharedPtr<FSDKMesh>					m_pMesh;

	TSharedPtr<FConstantBuffer>				m_pcbGPass;
	TSharedPtr<FConstantBuffer>				m_pcbDLPass;
	TSharedPtr<FConstantBuffer>				m_pcbScenePass;

	TSharedPtr<FTexture2D>					m_pDepthTex;

	TSharedPtr<FFrameBuffer>				m_pfbGPass;
	TSharedPtr<FFrameBuffer>				m_pfbDL;
	TSharedPtr<FFrameBuffer>				m_pfbScene;
};



#endif