#ifndef _GAME_APP_H_
#define _GAME_APP_H_

#include "DXUT-Ext.h"

class GameApp
{
public:
	GameApp();
	virtual ~GameApp();

	int Run();

	// 需要子类实现的 - Begin 

	virtual void OnInit() {}
	virtual void OnDestroy() {}
	virtual void OnResize(uint32_t width, uint32_t height) {}
	virtual void OnTick(float fDeltaSeconds) {}
	virtual void OnRender(float fDeltaSeconds) {}

	/**
	 * @brief 初始化D3D资源
	 */
	virtual HRESULT OnCreateDevice(ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc) { return S_FALSE; }

	virtual void OnKeyboard(UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext) {}
	virtual void OnGUIEvent(UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext) {}

	// 需要子类实现的 - End

protected:

	void InitApp();
	void DestroyApp();
	void RenderText();
	virtual HRESULT OnResizedSwapChain(ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc);

	//--------------------------------------------------------------------------------------
	// 各种CallBack
	//--------------------------------------------------------------------------------------
	static LRESULT CALLBACK __MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void* pUserContext);
	static void CALLBACK __OnKeyboard(UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext);
	static void CALLBACK __OnGUIEvent(UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext);
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
	LRESULT MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void* pUserContext);
	//void OnKeyboard(UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext);
	//void OnGUIEvent(UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext);
	HRESULT OnD3D11CreateDevice(ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext);
	//HRESULT OnD3D11ResizedSwapChain(ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext);
	void OnD3D11ReleasingSwapChain(void* pUserContext);
	void OnD3D11DestroyDevice(void* pUserContext);
	void OnFrameMove(double fTime, float fElapsedTime, void* pUserContext);
	void OnD3D11FrameRender(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext, double fTime, float fElapsedTime, void* pUserContext);

protected:
	bool						m_bShouldRenderText;
	CModelViewerCamera          m_Camera;                // A model viewing camera
	CModelViewerCamera          m_LightCamera;           // Light camera
};

// 当前游戏Instance
extern GameApp* g_pCurrGame;

// 各种全局变量
extern CDXUTDialogResourceManager  g_DialogResourceManager; // manager for shared resources of dialogs
extern CD3DSettingsDlg             g_SettingsDlg;           // Device settings dialog
extern CDXUTTextHelper*            g_pTxtHelper;			// Text Helper
extern CDXUTDialog                 g_HUD;                   // dialog for standard controls
extern CDXUTDialog                 g_SampleUI;              // dialog for sample specific controls

#endif