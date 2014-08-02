#ifndef _GAME_APP_H_
#define _GAME_APP_H_

#include "DXUT-Ext.h"

class GameApp
{
public:
	GameApp();
	virtual ~GameApp() {}

	int Run();
	void InitApp();
	void RenderText();

	/** 
	 * @brief 初始化
	 */
	virtual void OnInit() {}

	/** 
	 * @brief 销毁
	 */
	virtual void OnDestroy() {}

	/** 
	 * @brief 初始化D3D资源
	 */
	virtual HRESULT OnCreateDevice(ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc) { return S_FALSE; }

	virtual HRESULT OnResizedSwapChain(ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc);

	/**
	 * @brief 每帧更新
	 */
	virtual void OnTick(float DeltaSeconds) {}

	virtual void OnKeyboard(UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext) {}
	virtual void OnGUIEvent(UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext) {}

	/**
	 * @brief 渲染更新,发生在OnTick之后
	 */
	virtual void OnRender(float fDeltaSeconds) {}

protected:

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
	// 对应的各种成员
	//--------------------------------------------------------------------------------------
	LRESULT MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void* pUserContext);
	//void OnKeyboard(UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext);
	//void OnGUIEvent(UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext);
	//HRESULT OnD3D11CreateDevice(ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext);
	//HRESULT OnD3D11ResizedSwapChain(ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext);
	void OnD3D11ReleasingSwapChain(void* pUserContext);
	void OnD3D11DestroyDevice(void* pUserContext);
	void OnFrameMove(double fTime, float fElapsedTime, void* pUserContext);
	void OnD3D11FrameRender(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext, double fTime, float fElapsedTime, void* pUserContext);

protected:
	bool						m_bShouldRenderText;
	//--------------------------------------------------------------------------------------
	// 各种全局变量
	//--------------------------------------------------------------------------------------
	CModelViewerCamera          g_Camera;                // A model viewing camera
	CModelViewerCamera          g_LightCamera;           // Light camera
	CDXUTDialogResourceManager  g_DialogResourceManager; // manager for shared resources of dialogs
	CD3DSettingsDlg             g_SettingsDlg;           // Device settings dialog
	CDXUTTextHelper*            g_pTxtHelper = NULL;	 // Text Helper
	CDXUTDialog                 g_HUD;                   // dialog for standard controls
	CDXUTDialog                 g_SampleUI;              // dialog for sample specific controls
};

// 当前游戏Instance
extern GameApp* g_pCurrGame;


#endif