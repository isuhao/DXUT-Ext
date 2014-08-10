#include "DeferredLighting-Ext.h"
#include "RHI.h"

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
	m_pfbGPass	= RHI->CreateFrameBuffer(Width, Height, PF_A8R8G8B8);
	m_pfbDL		= RHI->CreateFrameBuffer(Width, Height, PF_A8R8G8B8);

	// ������������
	FVertexDeclarationFactory_DL VDF;

	// GPass Shader
	m_GPassVS.InitShader();
	m_GPassPS.InitShader();
	m_pGPassBST = RHI->CreateBoundShaderState
	(
		VDF.GetVertexDeclaration(),
		m_GPassVS.GetCode(),
		m_GPassVS.GetVertexShader(),
		m_GPassPS.GetPixelShader()
	);

	// DL Shader
	m_DLPassVS.InitShader();
	m_DLPassPS.InitShader();
	m_pDLPassBST = RHI->CreateBoundShaderState
	(
		VDF.GetVertexDeclaration(),
		m_DLPassVS.GetCode(),
		m_DLPassVS.GetVertexShader(),
		m_DLPassPS.GetPixelShader()
	);

	// Scene Pass Shader
	m_ScenePassVS.InitShader();
	m_ScenePassPS.InitShader();
	m_pScenePassBST = RHI->CreateBoundShaderState
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
	m_pSamplerState = RHI->CreateSamplerState(SF_Trilinear);

	// ����BlendState
	m_pColorWriteOn = RHI->CreateBlendState();

	// ����RasterizeState
	m_pCullBack = RHI->CreateRasterizerState(FM_Solid, CM_CCW);

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

	RHI->BeginRender();

	// �����BackBuffer
	RHI->SetDefaultBackBuffer();
	RHI->Clear(true, FLinearColor(sqrt(0.25f), sqrt(0.25f), sqrt(0.5f)), true, 1, false, 0);

	// ��3��Pass������Ⱦ
	RenderGPass();
	RenderDeferredLight();
	RenderScene();

	RHI->EndRender();
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

	RHI->SetFrameBuffer(m_pfbGPass);
	RHI->Clear(true, FLinearColor::Black, true, 1, false, 0);

	// ����CB
	m_GPassVS.SetVariables(mWVP, mWorld);
	RHI->CommitConstantBuffer();

	// Render State
	RHI->SetBlendState(m_pColorWriteOn);
	RHI->SetRasterizerState(m_pCullBack);
	RHI->SetBoundShaderState(m_pGPassBST);

	RHI->SetSamplerState(ST_PixelShader, 0, m_pSamplerState);

	m_pMesh->Render(0);
}

void DeferredLightingApp::RenderDeferredLight()
{
	RHI->SetRenderTarget(m_pfbDL->RenderTarget, NULL);
	RHI->ClearColor();

	// ����IA��VS��PS
	RHI->SetBoundShaderState(m_pDLPassBST);

	// ����CB��ֵ
	m_DLPassPS.SetVariables(D3DXVECTOR3(1.0, 1.0, -1.0));
	RHI->CommitConstantBuffer();

	// ����Texture��Sampler
	m_DLPassPS.SetTextureVariables(m_pfbGPass->RenderTarget, m_pSamplerState);

	// Ҫ��TriangleStrip
	RHI->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	RHI->GetDeviceContext()->Draw(4, 0);
}

void DeferredLightingApp::RenderScene()
{
	ID3D11DeviceContext* pd3dImmediateContext = RHI->GetDeviceContext();

	// ��RT��ΪBackBuffer
	RHI->SetDefaultBackBuffer();

	D3DXMATRIXA16 mWVP;
	mWVP = *m_Camera.GetWorldMatrix() * *m_Camera.GetViewMatrix() * *m_Camera.GetProjMatrix();
	D3DXMatrixTranspose(&mWVP, &mWVP);

	// Render State
	RHI->SetBlendState(m_pColorWriteOn);
	RHI->SetRasterizerState(m_pCullBack);
	RHI->SetBoundShaderState(m_pScenePassBST);

	// ����CB��ֵ
	m_ScenePassVS.SetVariables(mWVP);
	m_ScenePassPS.SetVariables(float4(0.15f, 0.15f, 0.15f, 1.0), float4(1.0f, 1.0f, 1.0f, 1.f));
	RHI->CommitConstantBuffer();

	// ������ͼ�Ͳ�����
	m_ScenePassPS.SetTextureVariables(m_pfbDL->RenderTarget, m_pSamplerState);

	m_pMesh->Render(0);
}