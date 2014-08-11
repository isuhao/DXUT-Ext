#ifndef _GAME_APP_H_
#define _GAME_APP_H_

#include "Core.h"
#include "D3DDriver.h"
#include "SystemSetting.h"

// ����һ�������
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
	// ��Ҫ����ʵ�ֵ� - Begin 

	// ��ʼ��APP�������ʱ�򴴽�D3D��Դ
	virtual void OnInit() {}
	// ����App
	virtual void OnDestroy() {}
	// ���ڴ�С�ı�ص�
	virtual void OnResize(uint Width, uint Height) {}
	// ������Ϣ
	virtual void OnTick(float fDeltaSeconds) {}
	// ������Ⱦ��Դ��������Tick֮��
	virtual void OnRender(float fDeltaSeconds) {}
	// ��Ӧ������Ϣ
	virtual void OnKeyboard(uint nChar, bool bKeyDown, bool bAltDown, void* pUserContext) {}
	// ��ӦGUI��Ϣ
	virtual void OnGUIEvent(uint nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext) {}

	// ��Ҫ����ʵ�ֵ� - End
	//////////////////////////////////////////////////////////////////////////

protected:

	void PreInit();
	void DestroyApp();
	void RenderText();

	/** 
	 * ��ʼ�������
	 * ����Ҳ��ʵ�� 
	 */
	virtual void InitCamera();

	//--------------------------------------------------------------------------------------
	// ����CallBack
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
	// ��̬�ص�������Ӧ�ĸ��ֳ�Ա����
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
	WString						m_szResPath;			 // ��Դ·��
};

// ��ǰ��ϷInstance
extern FGameApp* g_pCurrGame;

// ����ȫ�ֱ���
extern CDXUTDialogResourceManager  g_DialogResourceManager; // manager for shared resources of dialogs
extern CD3DSettingsDlg             g_SettingsDlg;           // Device settings dialog
extern CDXUTTextHelper*            g_pTxtHelper;			// Text Helper
extern CDXUTDialog                 g_HUD;                   // dialog for standard controls
extern CDXUTDialog                 g_SampleUI;              // dialog for sample specific controls

#endif