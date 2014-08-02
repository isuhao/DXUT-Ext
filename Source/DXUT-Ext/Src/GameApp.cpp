#include "GameApp.h"

// 当前游戏GameApp的全局指针
GameApp* g_pCurrGame = NULL;

// 构造
GameApp::GameApp()
: m_bShouldRenderText(false)
{	
	g_pCurrGame = this;
}

/// 各种CallBack
//////////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------------------
// Handle messages to the application
//--------------------------------------------------------------------------------------
LRESULT CALLBACK GameApp::__MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing,
	void* pUserContext)
{
	return g_pCurrGame->MsgProc(hWnd, uMsg, wParam, lParam, pbNoFurtherProcessing, pUserContext);
}

LRESULT GameApp::MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing,
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
	g_Camera.HandleMessages(hWnd, uMsg, wParam, lParam);
	g_LightCamera.HandleMessages(hWnd, uMsg, wParam, lParam);

	return 0;
}

//--------------------------------------------------------------------------------------
// Handle key presses
//--------------------------------------------------------------------------------------
void CALLBACK GameApp::__OnKeyboard(UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext)
{
	g_pCurrGame->OnKeyboard(nChar, bKeyDown, bAltDown, pUserContext);
}

//--------------------------------------------------------------------------------------
// Handles the GUI events
//--------------------------------------------------------------------------------------
void CALLBACK GameApp::__OnGUIEvent(UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext)
{
	g_pCurrGame->OnGUIEvent(nEvent, nControlID, pControl, pUserContext);
}

//--------------------------------------------------------------------------------------
// Handle updates to the scene.  This is called regardless of which D3D API is used
// 相当于Tick
//--------------------------------------------------------------------------------------
void CALLBACK GameApp::__OnFrameMove(double fTime, float fElapsedTime, void* pUserContext)
{
	g_pCurrGame->OnFrameMove(fTime, fElapsedTime, pUserContext);
}

void GameApp::OnFrameMove(double fTime, float fElapsedTime, void* pUserContext)
{
	// Update the camera's position based on user input 
	g_Camera.FrameMove(fElapsedTime);
	g_LightCamera.FrameMove(fElapsedTime);

	OnTick(fElapsedTime);
}

HRESULT CALLBACK GameApp::__OnD3D11CreateDevice(ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc,
	void* pUserContext)
{
	return g_pCurrGame->OnCreateDevice(pd3dDevice, pBackBufferSurfaceDesc);
}

//--------------------------------------------------------------------------------------
// Release D3D11 resources created in OnD3D11ResizedSwapChain 
//--------------------------------------------------------------------------------------
void CALLBACK GameApp::__OnD3D11ReleasingSwapChain(void* pUserContext)
{
	g_pCurrGame->OnD3D11ReleasingSwapChain(pUserContext);
}

void GameApp::OnD3D11ReleasingSwapChain(void* pUserContext)
{
	g_DialogResourceManager.OnD3D11ReleasingSwapChain();
}

//--------------------------------------------------------------------------------------
// Create any D3D11 resources that depend on the back buffer
// 根据BackBuffer来创建D3D资源
//--------------------------------------------------------------------------------------
HRESULT CALLBACK GameApp::__OnD3D11ResizedSwapChain(ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain,
	const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext)
{
	return g_pCurrGame->OnResizedSwapChain(pd3dDevice, pSwapChain, pBackBufferSurfaceDesc);
}

HRESULT GameApp::OnResizedSwapChain(ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc)
{
	HRESULT hr;

	V_RETURN(g_DialogResourceManager.OnD3D11ResizedSwapChain(pd3dDevice, pBackBufferSurfaceDesc));
	V_RETURN(g_SettingsDlg.OnD3D11ResizedSwapChain(pd3dDevice, pBackBufferSurfaceDesc));

	// Setup the camera's projection parameters
	float fAspectRatio = pBackBufferSurfaceDesc->Width / (FLOAT)pBackBufferSurfaceDesc->Height;
	g_Camera.SetProjParams(D3DX_PI / 4, fAspectRatio, 0.1f, 1000.0f);
	g_Camera.SetWindow(pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height);

	g_LightCamera.SetProjParams(D3DX_PI / 4, 1.0f, 0.1f, 1000.0f);
	//g_Camera.SetButtonMasks( MOUSE_LEFT_BUTTON, MOUSE_WHEEL, MOUSE_MIDDLE_BUTTON );

	g_HUD.SetLocation(pBackBufferSurfaceDesc->Width - 170, 0);
	g_HUD.SetSize(170, 170);
	g_SampleUI.SetLocation(pBackBufferSurfaceDesc->Width - 170, pBackBufferSurfaceDesc->Height - 300);
	g_SampleUI.SetSize(170, 300);

	return S_OK;
}

void CALLBACK GameApp::__OnD3D11DestroyDevice(void* pUserContext)
{
	g_pCurrGame->OnD3D11DestroyDevice(pUserContext);
}

void GameApp::OnD3D11DestroyDevice(void* pUserContext)
{
	g_DialogResourceManager.OnD3D11DestroyDevice();
	g_SettingsDlg.OnD3D11DestroyDevice();
	DXUTGetGlobalResourceCache().OnDestroyDevice();
	SAFE_DELETE(g_pTxtHelper);

	OnDestroy();
}

//--------------------------------------------------------------------------------------
// Render the scene using the D3D11 device
// 渲染更新
//--------------------------------------------------------------------------------------
void CALLBACK GameApp::__OnD3D11FrameRender(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext, double fTime,
	float fElapsedTime, void* pUserContext)
{
	g_pCurrGame->OnD3D11FrameRender(pd3dDevice, pd3dImmediateContext, fTime, fElapsedTime, pUserContext);
}

void GameApp::OnD3D11FrameRender(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext, double fTime, float fElapsedTime, void* pUserContext)
{
	// If the settings dialog is being shown, then render it instead of rendering the app's scene
	if (g_SettingsDlg.IsActive())
	{
		g_SettingsDlg.OnRender(fElapsedTime);
		return;
	}

	OnRender(fElapsedTime);
}


void GameApp::RenderText()
{
	g_pTxtHelper->Begin();
	g_pTxtHelper->SetInsertionPos(5, 5);
	g_pTxtHelper->SetForegroundColor(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	g_pTxtHelper->DrawTextLine(DXUTGetFrameStats(DXUTIsVsyncEnabled()));
	g_pTxtHelper->DrawTextLine(DXUTGetDeviceStats());
	g_pTxtHelper->End();
}

int GameApp::Run()
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

	InitApp();

	DXUTInit(true, true, NULL); // Parse the command line, show msgboxes on error, no extra command line params
	DXUTSetCursorSettings(true, true);
	DXUTCreateWindow(L"GameApp");

	DXUTCreateDevice(D3D_FEATURE_LEVEL_11_0, true, 640, 480);

	DXUTMainLoop(); // Enter into the DXUT render loop

	return DXUTGetExitCode();
}

void GameApp::InitApp()
{
	g_SettingsDlg.Init(&g_DialogResourceManager);
	g_HUD.Init(&g_DialogResourceManager);
	g_SampleUI.Init(&g_DialogResourceManager);

	g_HUD.SetCallback(__OnGUIEvent);
	g_SampleUI.SetCallback(__OnGUIEvent);

	OnInit();
}