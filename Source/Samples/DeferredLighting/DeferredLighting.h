#ifndef _DEFERRED_LIGHTING_H_
#define _DEFERRED_LIGHTING_H_


#include "DXUT-Ext.h"


//--------------------------------------------------------------------------------------
// Global variables
//--------------------------------------------------------------------------------------
CModelViewerCamera          g_Camera;                // A model viewing camera
CModelViewerCamera          g_LightCamera;           // Light camera
CDXUTDialogResourceManager  g_DialogResourceManager; // manager for shared resources of dialogs
CD3DSettingsDlg             g_SettingsDlg;           // Device settings dialog
CDXUTTextHelper*            g_pTxtHelper = NULL;
CDXUTDialog                 g_HUD;                   // dialog for standard controls
CDXUTDialog                 g_SampleUI;              // dialog for sample specific controls

//////////////////////////////////////////////////////////////////////////

// Mesh
CDXUTSDKMesh                g_Mesh;

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

// RHI资源
ID3D11Buffer*				g_pcbGPass = NULL;
ID3D11Buffer*				g_pcbDLPass = NULL;
ID3D11Buffer*				g_pcbScenePass = NULL;
ID3D11InputLayout*			g_pGPLayout = NULL;
ID3D11VertexShader*			g_pGPVertexShader = NULL;
ID3D11PixelShader*			g_pGPPixelShader = NULL;
ID3D11InputLayout*			g_pDLLayout = NULL;
ID3D11VertexShader*			g_pDLVertexShader = NULL;
ID3D11PixelShader*			g_pDLPixelShader = NULL;
ID3D11InputLayout*			g_pSPLayout = NULL;
ID3D11VertexShader*			g_pSPVertexShader = NULL;
ID3D11PixelShader*			g_pSPPixelShader = NULL;
ID3D11SamplerState*			g_pSamLinearWrap = NULL;
ID3D11BlendState*			g_pColorWritesOn = NULL;
ID3D11RasterizerState*		g_pCullBack = NULL;

enum EGBufferType
{
	EGBT_Normal,
	EGBT_Diffuse,
	EGBT_Specular,

	EGBT_MaxSize
};

// G-Buffer
ID3D11Texture2D*			g_pGBufferTexture[EGBT_MaxSize];
ID3D11RenderTargetView*		g_pGBufferTextureRT[EGBT_MaxSize];
ID3D11ShaderResourceView*	g_pGBufferTextureSRV[EGBT_MaxSize];

// SM depth stencil texture data
ID3D11Texture2D*            g_pDepthTexture = NULL;
ID3D11ShaderResourceView*   g_pDepthTextureSRV = NULL; // SRV of the SM
ID3D11DepthStencilView*     g_pDepthTextureDSV = NULL; // depth stencil view of the SM

//--------------------------------------------------------------------------------------
// 函数的前向声明
//--------------------------------------------------------------------------------------
LRESULT CALLBACK MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing,
	void* pUserContext);
void CALLBACK OnKeyboard(UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext);
void CALLBACK OnGUIEvent(UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext);
void CALLBACK OnFrameMove(double fTime, float fElapsedTime, void* pUserContext);
bool CALLBACK ModifyDeviceSettings(DXUTDeviceSettings* pDeviceSettings, void* pUserContext);

bool CALLBACK IsD3D11DeviceAcceptable(const CD3D11EnumAdapterInfo *AdapterInfo, UINT Output, const CD3D11EnumDeviceInfo *DeviceInfo,
	DXGI_FORMAT BackBufferFormat, bool bWindowed, void* pUserContext);
HRESULT CALLBACK OnD3D11CreateDevice(ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc,
	void* pUserContext);
HRESULT CALLBACK OnD3D11ResizedSwapChain(ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain,
	const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext);
void CALLBACK OnD3D11ReleasingSwapChain(void* pUserContext);
void CALLBACK OnD3D11DestroyDevice(void* pUserContext);
void CALLBACK OnD3D11FrameRender(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext, double fTime,
	float fElapsedTime, void* pUserContext);

void InitApp();
void RenderText();

void RenderGPass(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext);
void RenderDeferredLight(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext);
void RenderScene(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext);

void DLInitResource(ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc);


#endif