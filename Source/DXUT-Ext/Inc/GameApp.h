#ifndef _GAME_APP_H_
#define _GAME_APP_H_

#include "Core.h"
#include "D3DDriver.h"
#include "SystemSetting.h"

// 引用一下这个库
#if INCLUDE_LIB
#pragma comment(lib, "DXUT-Ext.lib")
#endif

class FGameApp
{
public:
	FGameApp(const WString& AppName = L"GameApp", const WString& ResPath = L"");
	virtual ~FGameApp();

	int Run();

	//////////////////////////////////////////////////////////////////////////
	// 需要子类实现的 - Begin 

	// 初始化APP，在这个时候创建D3D资源
	virtual void OnInit() {}
	// 销毁App
	virtual void OnDestroy() {}
	// 窗口大小改变回调
	virtual void OnResize(uint Width, uint Height) {}
	// 更新信息
	virtual void OnTick(float fDeltaSeconds) {}
	// 更新渲染资源、发生在Tick之后
	virtual void OnRender(float fDeltaSeconds) {}
	// 相应键盘信息
	virtual void OnKeyboard(uint nChar, bool bKeyDown, bool bAltDown, void* pUserContext) {}
	// 响应GUI信息
	virtual void OnGUIEvent(uint nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext) {}

	// 需要子类实现的 - End
	//////////////////////////////////////////////////////////////////////////

protected:

	void PreInit();
	void DestroyApp();
	void RenderText();

	/** 
	 * 初始化摄像机
	 * 子类也可实现 
	 */
	virtual void InitCamera();

	//--------------------------------------------------------------------------------------
	// 各种CallBack
	//--------------------------------------------------------------------------------------
	static LRESULT CALLBACK __MsgProc(HWND hWnd, uint uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void* pUserContext);
	static void CALLBACK __OnKeyboard(uint nChar, bool bKeyDown, bool bAltDown, void* pUserContext);
	static void CALLBACK __OnGUIEvent(uint nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext);
	static HRESULT CALLBACK __OnD3D11CreateDevice(ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext);
	static HRESULT CALLBACK __OnD3D11ResizedSwapChain(ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext);
	static void CALLBACK __OnD3D11ReleasingSwapChain(void* pUserContext);
	static void CALLBACK __OnD3D11DestroyDevice(void* pUserContext);
	static void CALLBACK __OnFrameMove(double fTime, float fElapsedTime, void* pUserContext);
	static bool CALLBACK __ModifyDeviceSettings(DXUTDeviceSettings* pDeviceSettings, void* pUserContext) { return true; }
	static void CALLBACK __OnD3D11FrameRender(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext, double fTime, float fElapsedTime, void* pUserContext);

	//--------------------------------------------------------------------------------------
	// 静态回调函数对应的各种成员函数
	//--------------------------------------------------------------------------------------
	LRESULT MsgProc(HWND hWnd, uint uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void* pUserContext);
	HRESULT OnD3D11CreateDevice(ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext);
	HRESULT OnResizedSwapChain(ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc);
	void OnD3D11ReleasingSwapChain(void* pUserContext);
	void OnD3D11DestroyDevice(void* pUserContext);
	void OnFrameMove(double fTime, float fElapsedTime, void* pUserContext);
	void OnD3D11FrameRender(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext, double fTime, float fElapsedTime, void* pUserContext);

protected:
	bool						m_bShouldRenderText;
	CModelViewerCamera          m_Camera;                // A model viewing camera
	CModelViewerCamera          m_LightCamera;           // Light camera
	WString						m_szAppName;			 // Application name
	WString						m_szResPath;			 // 资源路径
};

// 当前游戏Instance
extern FGameApp* g_pCurrGame;

// 各种全局变量
extern CDXUTDialogResourceManager  g_DialogResourceManager; // manager for shared resources of dialogs
extern CD3DSettingsDlg             g_SettingsDlg;           // Device settings dialog
extern CDXUTTextHelper*            g_pTxtHelper;			// Text Helper
extern CDXUTDialog                 g_HUD;                   // dialog for standard controls
extern CDXUTDialog                 g_SampleUI;              // dialog for sample specific controls

#endif