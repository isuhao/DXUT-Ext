#include "DeferredLighting-Ext.h"
#include "D3DDriver.h"

//--------------------------------------------------------------------------------------
// Win Main
//--------------------------------------------------------------------------------------

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	DeferredLightingApp App;
	
	App.Run();

	return 0;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

DeferredLightingApp::DeferredLightingApp()
{
}

//--------------------------------------------------------------------------------------
// Create any D3D11 resources that aren't dependant on the back buffer
// ��ʼ��D3D��Դ
//--------------------------------------------------------------------------------------

HRESULT DeferredLightingApp::OnCreateDevice(ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc)
{
	// ��������FrmaeBuffer
	uint Width	= pBackBufferSurfaceDesc->Width;
	uint Height = pBackBufferSurfaceDesc->Height;
	m_pfbGPass	= D3D->CreateFrameBuffer(Width, Height, PF_A8R8G8B8);
	m_pfbDL		= D3D->CreateFrameBuffer(Width, Height, PF_A8R8G8B8);

	// ������������
	FVertexDeclarationFactory_DL VDF;

	// GPass Shader
	m_GPassVS.InitShader();
	m_GPassPS.InitShader();
	m_pGPassBST = D3D->CreateBoundShaderState
	(
		VDF.GetVertexDeclaration(),
		m_GPassVS.GetCode(),
		m_GPassVS.GetVertexShader(),
		m_GPassPS.GetPixelShader()
	);

	// DL Shader
	m_DLPassVS.InitShader();
	m_DLPassPS.InitShader();
	m_pDLPassBST = D3D->CreateBoundShaderState
	(
		VDF.GetVertexDeclaration(),
		m_DLPassVS.GetCode(),
		m_DLPassVS.GetVertexShader(),
		m_DLPassPS.GetPixelShader()
	);

	// Scene Pass Shader
	m_ScenePassVS.InitShader();
	m_ScenePassPS.InitShader();
	m_pScenePassBST = D3D->CreateBoundShaderState
	(
		VDF.GetVertexDeclaration(),
		m_ScenePassVS.GetCode(),
		m_ScenePassVS.GetVertexShader(),
		m_ScenePassPS.GetPixelShader()
	);

	// ����Mesh
	m_pMesh = TSharedPtr<FSDKMesh>(new FSDKMesh(L"Crypt\\Crypt.sdkmesh", false));
	m_pMesh->Init();

	// ����SamplerState
	m_pSamplerState = D3D->CreateSamplerState(SF_Trilinear);

	// ����BlendState
	m_pColorWriteOn = D3D->CreateBlendState();

	// ����RasterizeState
	m_pCullBack = D3D->CreateRasterizerState(FM_Solid, CM_CCW);

	// ����CB
	m_pcbGPass	= MakeSharedPtr<FConstantBuffer>();
	m_pcbDLPass = MakeSharedPtr<FConstantBuffer>();
	m_pcbScenePass = MakeSharedPtr<FConstantBuffer>();

	m_pcbGPass->CreateBuffer(sizeof(CB_GPass));
	m_pcbDLPass->CreateBuffer(sizeof(CB_DLPass));
	m_pcbScenePass->CreateBuffer(sizeof(CB_ScenePass));

	return S_OK;
}

//--------------------------------------------------------------------------------------
// Tick
//--------------------------------------------------------------------------------------
void DeferredLightingApp::OnTick(float DeltaSeconds)
{
}

//--------------------------------------------------------------------------------------
// Release D3D11 resources created in OnD3D11CreateDevice 
// ��������COM����
//--------------------------------------------------------------------------------------
void DeferredLightingApp::OnDestroy()
{
}

//--------------------------------------------------------------------------------------
// Render the scene using the D3D11 device
// ��Ⱦ����
//--------------------------------------------------------------------------------------
void DeferredLightingApp::OnRender(float fDeltaSeconds)
{
	ID3D11Device* pd3dDevice = DXUTGetD3D11Device();
	ID3D11DeviceContext* pd3dImmediateContext = DXUTGetD3D11DeviceContext();

	D3D->BeginRender();

	// �����BackBuffer
	D3D->SetDefaultBackBuffer();
	D3D->Clear(true, FLinearColor(sqrt(0.25f), sqrt(0.25f), sqrt(0.5f)), true, 1, false, 0);

	// ��3��Pass������Ⱦ
	RenderGPass();
	RenderDeferredLight();
	RenderScene();

	D3D->EndRender();
}

//--------------------------------------------------------------------------------------
// ��ʼ��APP 
//--------------------------------------------------------------------------------------
void DeferredLightingApp::OnInit()
{
	//m_Camera.SetRotateButtons( true, false, false );
	m_Camera.SetButtonMasks(MOUSE_LEFT_BUTTON, MOUSE_WHEEL, MOUSE_MIDDLE_BUTTON);
	m_LightCamera.SetButtonMasks(MOUSE_RIGHT_BUTTON, 0, 0);

	// Setup the camera's view parameters
	D3DXVECTOR3 vecEye(7.0f, 7.0f, -7.0f);
	D3DXVECTOR3 vecAt(0.0f, 0.0f, 0.0f);
	m_Camera.SetViewParams(&vecEye, &vecAt);

	// Setup light camera's view parameters
	D3DXVECTOR3 vecEyeLight(9.0f, 15.0f, 9.0f);
	D3DXVECTOR3 vecAtLight(0.0f, 0.0f, 0.0f);
	m_LightCamera.SetViewParams(&vecEyeLight, &vecAtLight);
}


void DeferredLightingApp::RenderGPass()
{
	D3DXMATRIXA16 mWVP, mWorld;

	mWVP = *m_Camera.GetWorldMatrix() * *m_Camera.GetViewMatrix() * *m_Camera.GetProjMatrix();
	D3DXMatrixTranspose(&mWVP, &mWVP);
	mWorld = *m_Camera.GetWorldMatrix();
	D3DXMatrixTranspose(&mWorld, &mWorld);

	D3D->SetFrameBuffer(m_pfbGPass);
	D3D->Clear(true, FLinearColor::Black, true, 1, false, 0);

	// ����CB
	m_GPassVS.SetVariables(mWVP, mWorld);
	D3D->CommitConstantBuffer();

	// Render State
	D3D->SetBlendState(m_pColorWriteOn);
	D3D->SetRasterizerState(m_pCullBack);
	D3D->SetBoundShaderState(m_pGPassBST);

	D3D->SetSamplerState(ST_PixelShader, 0, m_pSamplerState);

	m_pMesh->Render(0);
}

void DeferredLightingApp::RenderDeferredLight()
{
	D3D->SetRenderTarget(m_pfbDL->RenderTarget, NULL);
	D3D->ClearColor();

	// ����IA��VS��PS
	D3D->SetBoundShaderState(m_pDLPassBST);

	// ����CB��ֵ
	m_DLPassPS.SetVariables(D3DXVECTOR3(1.0, 1.0, -1.0));
	D3D->CommitConstantBuffer();

	// ����Texture��Sampler
	m_DLPassPS.SetTextureVariables(m_pfbGPass->RenderTarget, m_pSamplerState);

	// Ҫ��TriangleStrip
	D3D->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	D3D->GetDeviceContext()->Draw(4, 0);
}

void DeferredLightingApp::RenderScene()
{
	ID3D11DeviceContext* pd3dImmediateContext = D3D->GetDeviceContext();

	// ��RT��ΪBackBuffer
	D3D->SetDefaultBackBuffer();

	D3DXMATRIXA16 mWVP;
	mWVP = *m_Camera.GetWorldMatrix() * *m_Camera.GetViewMatrix() * *m_Camera.GetProjMatrix();
	D3DXMatrixTranspose(&mWVP, &mWVP);

	// Render State`
	D3D->SetBlendState(m_pColorWriteOn);
	D3D->SetRasterizerState(m_pCullBack);
	D3D->SetBoundShaderState(m_pScenePassBST);

	// ����CB��ֵ
	m_ScenePassVS.SetVariables(mWVP);
	m_ScenePassPS.SetVariables(float4(0.15f, 0.15f, 0.15f, 1.0), float4(1.0f, 1.0f, 1.0f, 1.f));
	D3D->CommitConstantBuffer();

	// ������ͼ�Ͳ�����
	m_ScenePassPS.SetTextureVariables(m_pfbDL->RenderTarget, m_pSamplerState);

	m_pMesh->Render(0);
}