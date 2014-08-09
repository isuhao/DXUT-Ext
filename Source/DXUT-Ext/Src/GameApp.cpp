#include "GameApp.h"

// 当前游戏FGameApp的全局指针
FGameApp* g_pCurrGame = NULL;

// 各种全局变量
CDXUTDialogResourceManager  g_DialogResourceManager;
CD3DSettingsDlg             g_SettingsDlg;
CDXUTTextHelper*            g_pTxtHelper = NULL;
CDXUTDialog                 g_HUD;
CDXUTDialog                 g_SampleUI;

// 构造
FGameApp::FGameApp()
: m_bShouldRenderText(false)
{	
	g_pCurrGame = this;
}

FGameApp::~FGameApp()
{
	g_pCurrGame = NULL;
}

/// 各种CallBack
//////////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------------------
// Handle messages to the application
//--------------------------------------------------------------------------------------
LRESULT CALLBACK FGameApp::__MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing,
	void* pUserContext)
{
	Check(g_pCurrGame);
	return g_pCurrGame->MsgProc(hWnd, uMsg, wParam, lParam, pbNoFurtherProcessing, pUserContext);
}

LRESULT FGameApp::MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing,
	void* pUserContext)
{
	// Pass messages to dialog resource manager calls so GUI state is updated correctly
	*pbNoFurtherProcessing = g_DialogResourceManager.MsgProc(hWnd, uMsg, wParam, lParam);
	if (*pbNoFurtherProcessing)
		return 0;

	// Pass messages to settings dialog if its active
	if (g_SettingsDlg.IsActive())
	{
		g_SettingsDlg.MsgProc(hWnd, uMsg, wParam, lParam);
		return 0;
	}

	// Give the dialogs a chance to handle the message first
	*pbNoFurtherProcessing = g_HUD.MsgProc(hWnd, uMsg, wParam, lParam);
	if (*pbNoFurtherProcessing)
		return 0;
	*pbNoFurtherProcessing = g_SampleUI.MsgProc(hWnd, uMsg, wParam, lParam);
	if (*pbNoFurtherProcessing)
		return 0;

	// Pass all remaining windows messages to camera so it can respond to user input
	m_Camera.HandleMessages(hWnd, uMsg, wParam, lParam);
	m_LightCamera.HandleMessages(hWnd, uMsg, wParam, lParam);

	return 0;
}

//--------------------------------------------------------------------------------------
// Handle key presses
//--------------------------------------------------------------------------------------
void CALLBACK FGameApp::__OnKeyboard(UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext)
{
	if (g_pCurrGame)
	{
		g_pCurrGame->OnKeyboard(nChar, bKeyDown, bAltDown, pUserContext);
	}
}

//--------------------------------------------------------------------------------------
// Handles the GUI events
//--------------------------------------------------------------------------------------
void CALLBACK FGameApp::__OnGUIEvent(UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext)
{
	if (g_pCurrGame)
	{
		g_pCurrGame->OnGUIEvent(nEvent, nControlID, pControl, pUserContext);
	}
}

//--------------------------------------------------------------------------------------
// Handle updates to the scene.  This is called regardless of which D3D API is used
// 相当于Tick
//--------------------------------------------------------------------------------------
void CALLBACK FGameApp::__OnFrameMove(double fTime, float fElapsedTime, void* pUserContext)
{
	if (g_pCurrGame)
	{
		g_pCurrGame->OnFrameMove(fTime, fElapsedTime, pUserContext);
	}
}

void FGameApp::OnFrameMove(double fTime, float fElapsedTime, void* pUserContext)
{
	// Update the camera's position based on user input 
	m_Camera.FrameMove(fElapsedTime);
	m_LightCamera.FrameMove(fElapsedTime);

	OnTick(fElapsedTime);
}

HRESULT CALLBACK FGameApp::__OnD3D11CreateDevice(ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc,
	void* pUserContext)
{
	FDynamicRHI::InitRHI();
	FNullRHI::InitRHI();

	Check(g_pCurrGame);
	return g_pCurrGame->OnD3D11CreateDevice(pd3dDevice, pBackBufferSurfaceDesc, pUserContext);
}

HRESULT FGameApp::OnD3D11CreateDevice(ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc,
	void* pUserContext)
{
	HRESULT hr;

	ID3D11DeviceContext* pDeviceContext = DXUTGetD3D11DeviceContext();
	V_RETURN(g_DialogResourceManager.OnD3D11CreateDevice(pd3dDevice, pDeviceContext));
	V_RETURN(g_SettingsDlg.OnD3D11CreateDevice(pd3dDevice));

	return OnCreateDevice(pd3dDevice, pBackBufferSurfaceDesc);
}

//--------------------------------------------------------------------------------------
// Release D3D11 resources created in OnD3D11ResizedSwapChain 
//--------------------------------------------------------------------------------------
void CALLBACK FGameApp::__OnD3D11ReleasingSwapChain(void* pUserContext)
{
	if (g_pCurrGame)
	{
		g_pCurrGame->OnD3D11ReleasingSwapChain(pUserContext);
	}
}

void FGameApp::OnD3D11ReleasingSwapChain(void* pUserContext)
{
	g_DialogResourceManager.OnD3D11ReleasingSwapChain();
}

//--------------------------------------------------------------------------------------
// Create any D3D11 resources that depend on the back buffer
// 根据BackBuffer来创建D3D资源
//--------------------------------------------------------------------------------------
HRESULT CALLBACK FGameApp::__OnD3D11ResizedSwapChain(ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain,
	const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext)
{
	Check(g_pCurrGame);
	return g_pCurrGame->OnResizedSwapChain(pd3dDevice, pSwapChain, pBackBufferSurfaceDesc);
}

HRESULT FGameApp::OnResizedSwapChain(ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc)
{
	HRESULT hr;

	V_RETURN(g_DialogResourceManager.OnD3D11ResizedSwapChain(pd3dDevice, pBackBufferSurfaceDesc));
	V_RETURN(g_SettingsDlg.OnD3D11ResizedSwapChain(pd3dDevice, pBackBufferSurfaceDesc));

	// Setup the camera's projection parameters
	float fAspectRatio = pBackBufferSurfaceDesc->Width / (FLOAT)pBackBufferSurfaceDesc->Height;
	m_Camera.SetProjParams(D3DX_PI / 4, fAspectRatio, 0.1f, 1000.0f);
	m_Camera.SetWindow(pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height);

	m_LightCamera.SetProjParams(D3DX_PI / 4, 1.0f, 0.1f, 1000.0f);
	//m_Camera.SetButtonMasks( MOUSE_LEFT_BUTTON, MOUSE_WHEEL, MOUSE_MIDDLE_BUTTON );

	g_HUD.SetLocation(pBackBufferSurfaceDesc->Width - 170, 0);
	g_HUD.SetSize(170, 170);
	g_SampleUI.SetLocation(pBackBufferSurfaceDesc->Width - 170, pBackBufferSurfaceDesc->Height - 300);
	g_SampleUI.SetSize(170, 300);
	
	OnResize(pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height);

	return S_OK;
}

void CALLBACK FGameApp::__OnD3D11DestroyDevice(void* pUserContext)
{
	FNullRHI::ReleaseRHI();
	FDynamicRHI::ReleaseRHI();

	if (g_pCurrGame)
	{
		g_pCurrGame->OnDestroy();
	}
}

void FGameApp::OnD3D11DestroyDevice(void* pUserContext)
{	
}

//--------------------------------------------------------------------------------------
// Render the scene using the D3D11 device
// 渲染更新
//--------------------------------------------------------------------------------------
void CALLBACK FGameApp::__OnD3D11FrameRender(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext, double fTime,
	float fElapsedTime, void* pUserContext)
{
	g_pCurrGame->OnD3D11FrameRender(pd3dDevice, pd3dImmediateContext, fTime, fElapsedTime, pUserContext);
}

void FGameApp::OnD3D11FrameRender(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext, double fTime, float fElapsedTime, void* pUserContext)
{
	// If the settings dialog is being shown, then render it instead of rendering the app's scene
	if (g_SettingsDlg.IsActive())
	{
		g_SettingsDlg.OnRender(fElapsedTime);
		return;
	}

	OnRender(fElapsedTime);
}


void FGameApp::RenderText()
{
	g_pTxtHelper->Begin();
	g_pTxtHelper->SetInsertionPos(5, 5);
	g_pTxtHelper->SetForegroundColor(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	g_pTxtHelper->DrawTextLine(DXUTGetFrameStats(DXUTIsVsyncEnabled()));
	g_pTxtHelper->DrawTextLine(DXUTGetDeviceStats());
	g_pTxtHelper->End();
}

int FGameApp::Run()
{
	HRESULT hr;
	// @TODO: Aeron ???!!!! 资源搜索路径，要改改
	V_RETURN(DXUTSetMediaSearchPath(L"..\\Source\\FXAA")); 
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	DXUTSetIsInGammaCorrectMode(FALSE);

	// Set DXUT callbacks
	DXUTSetCallbackMsgProc(__MsgProc);
	DXUTSetCallbackKeyboard(__OnKeyboard);
	DXUTSetCallbackFrameMove(__OnFrameMove);
	DXUTSetCallbackDeviceChanging(__ModifyDeviceSettings);

	//DXUTSetCallbackD3D11DeviceAcceptable(IsD3D11DeviceAcceptable);
	DXUTSetCallbackD3D11DeviceCreated(__OnD3D11CreateDevice);
	DXUTSetCallbackD3D11SwapChainResized(__OnD3D11ResizedSwapChain);
	DXUTSetCallbackD3D11SwapChainReleasing(__OnD3D11ReleasingSwapChain);
	DXUTSetCallbackD3D11DeviceDestroyed(__OnD3D11DestroyDevice);
	DXUTSetCallbackD3D11FrameRender(__OnD3D11FrameRender);

	// 初始化App
	InitApp();

	DXUTInit(true, true, NULL); // Parse the command line, show msgboxes on error, no extra command line params
	DXUTSetCursorSettings(true, true);
	DXUTCreateWindow(L"GameApp");

	DXUTCreateDevice(D3D_FEATURE_LEVEL_11_0, true, 640, 480);

	DXUTMainLoop(); // Enter into the DXUT render loop

	// 销毁App
	DestroyApp();

	return DXUTGetExitCode();
}

void FGameApp::InitApp()
{
	g_SettingsDlg.Init(&g_DialogResourceManager);
	g_HUD.Init(&g_DialogResourceManager);
	g_SampleUI.Init(&g_DialogResourceManager);

	g_HUD.SetCallback(__OnGUIEvent);
	g_SampleUI.SetCallback(__OnGUIEvent);

	OnInit();
}

void FGameApp::DestroyApp()
{
	// 销毁之前创建的对象
	OnDestroy();

	g_DialogResourceManager.OnD3D11ReleasingSwapChain();
	g_DialogResourceManager.OnD3D11DestroyDevice();
	g_SettingsDlg.OnD3D11DestroyDevice();
	DXUTGetGlobalResourceCache().OnDestroyDevice();
	SAFE_DELETE(g_pTxtHelper);
}