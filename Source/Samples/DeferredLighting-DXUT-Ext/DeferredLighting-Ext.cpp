#include "DeferredLighting-Ext.h"
#include "D3DDriver.h"

//--------------------------------------------------------------------------------------
// Win Main
//--------------------------------------------------------------------------------------

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	DeferredLightingApp App(L"Deferred Lighting Ext", L"..\\Source\\Samples\\DeferredLighting-DXUT-Ext");
	
	App.Run();

	return 0;
}


/**
 * 初始化APP，创建D3D资源
 */
void DeferredLightingApp::OnInit()
{
	// 创建各种FrmaeBuffer
	uint Width = DXUTGetDXGIBackBufferSurfaceDesc()->Width;
	uint Height = DXUTGetDXGIBackBufferSurfaceDesc()->Height;
	m_pfbGPass	= D3D->CreateFrameBuffer(Width, Height, PF_A8R8G8B8);
	m_pfbDL		= D3D->CreateFrameBuffer(Width, Height, PF_A8R8G8B8);

	// 顶点声明工厂
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

	// 创建Mesh
	m_pMesh = TSharedPtr<FSDKMesh>(new FSDKMesh(L"Crypt\\Crypt.sdkmesh", false));
	m_pMesh->Init();

	// 创建SamplerState
	m_pSamplerState = D3D->CreateSamplerState(SF_Trilinear);

	// 创建BlendState
	m_pColorWriteOn = D3D->CreateBlendState();

	// 创建RasterizeState
	m_pCullBack = D3D->CreateRasterizerState(FM_Solid, CM_CCW);

	// 创建CB
	m_pcbGPass	= MakeSharedPtr<FConstantBuffer>();
	m_pcbDLPass = MakeSharedPtr<FConstantBuffer>();
	m_pcbScenePass = MakeSharedPtr<FConstantBuffer>();

	m_pcbGPass->CreateBuffer(sizeof(CB_GPass));
	m_pcbDLPass->CreateBuffer(sizeof(CB_DLPass));
	m_pcbScenePass->CreateBuffer(sizeof(CB_ScenePass));
}

/**
 * 销毁这个APP前的操作
 */
void DeferredLightingApp::OnDestroy()
{
}

/** 
 * 渲染更新
 */
void DeferredLightingApp::OnRender(float fDeltaSeconds)
{
	D3D->BeginRender();

	// 先清空BackBuffer
	D3D->SetDefaultBackBuffer();
	D3D->Clear(true, FLinearColor(sqrt(0.25f), sqrt(0.25f), sqrt(0.5f)), true, 1, false, 0);

	// 分3个Pass进行渲染
	RenderGPass();
	RenderDeferredLight();
	RenderScene();

	D3D->EndRender();
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

	// 设置CB
	m_GPassVS.SetVariables(mWVP, mWorld);
	D3D->CommitConstantBuffer();

	// Render State
	D3D->SetBlendState(m_pColorWriteOn);
	D3D->SetRasterizerState(m_pCullBack);
	D3D->SetBoundShaderState(m_pGPassBST);

	D3D->SetSamplerState(ST_PixelShader, 0, m_pSamplerState);

	m_pMesh->Render();
}

void DeferredLightingApp::RenderDeferredLight()
{
	D3D->SetRenderTarget(m_pfbDL->RenderTarget, NULL);
	D3D->ClearColor();

	// 设置IA、VS、PS
	D3D->SetBoundShaderState(m_pDLPassBST);

	// 设置CB的值
	m_DLPassPS.SetVariables(D3DXVECTOR3(1.0, 1.0, -1.0));
	D3D->CommitConstantBuffer();

	// 设置Texture、Sampler
	m_DLPassPS.SetTextureVariables(m_pfbGPass->RenderTarget, m_pSamplerState);

	// 要用TriangleStrip
	D3D->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	D3D->GetDeviceContext()->Draw(4, 0);
}

void DeferredLightingApp::RenderScene()
{
	ID3D11DeviceContext* pd3dImmediateContext = D3D->GetDeviceContext();

	// 把RT设为BackBuffer
	D3D->SetDefaultBackBuffer();

	D3DXMATRIXA16 mWVP;
	mWVP = *m_Camera.GetWorldMatrix() * *m_Camera.GetViewMatrix() * *m_Camera.GetProjMatrix();
	D3DXMatrixTranspose(&mWVP, &mWVP);

	// Render State`
	D3D->SetBlendState(m_pColorWriteOn);
	D3D->SetRasterizerState(m_pCullBack);
	D3D->SetBoundShaderState(m_pScenePassBST);

	// 设置CB的值
	m_ScenePassVS.SetVariables(mWVP);
	m_ScenePassPS.SetVariables(float4(0.15f, 0.15f, 0.15f, 1.0), float4(1.0f, 1.0f, 1.0f, 1.f));
	D3D->CommitConstantBuffer();

	// 设置贴图和采样器
	m_ScenePassPS.SetTextureVariables(m_pfbDL->RenderTarget, m_pSamplerState);

	m_pMesh->Render();
}