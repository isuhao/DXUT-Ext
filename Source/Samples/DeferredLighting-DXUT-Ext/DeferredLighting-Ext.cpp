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
// 初始化D3D资源
//--------------------------------------------------------------------------------------

HRESULT DeferredLightingApp::OnCreateDevice(ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc)
{
	// 创建各种FrmaeBuffer
	uint Width	= pBackBufferSurfaceDesc->Width;
	uint Height = pBackBufferSurfaceDesc->Height;
	m_pfbGPass	= RHI->CreateFrameBuffer(Width, Height, PF_A8R8G8B8);
	m_pfbDL		= RHI->CreateFrameBuffer(Width, Height, PF_A8R8G8B8);

	// 顶点声明工厂
	FVertexDeclarationFactory_DL VDF;

	// GPass Shader
	CREATE_SHADER(GPassVertexShader, GPassVS);
	CREATE_SHADER(GPassPixelShader,  GPassPS);

	m_pGPassBST = RHI->CreateBoundShaderState
	(
		VDF.GetVertexDeclaration(),
		GPassVS.GetCode(),
		GPassVS.GetVertexShader(),
		GPassPS.GetPixelShader()
	);

	// DL Shader
	CREATE_SHADER(DLVertexShader, DLPassVS);
	CREATE_SHADER(DLPixelShader,  DLPassPS);
	m_pDLPassBST = RHI->CreateBoundShaderState
	(
		VDF.GetVertexDeclaration(),
		DLPassVS.GetCode(),
		DLPassVS.GetVertexShader(),
		DLPassPS.GetPixelShader()
	);

	// Scene Pass Shader
	CREATE_SHADER(SceneVertexShader, ScenePassVS);
	CREATE_SHADER(ScenePixelShader,  ScenePassPS);
	m_pScenePassBST = RHI->CreateBoundShaderState
	(
		VDF.GetVertexDeclaration(),
		ScenePassVS.GetCode(),
		ScenePassVS.GetVertexShader(),
		ScenePassPS.GetPixelShader()
	);

	// 创建Mesh
	m_pMesh = TSharedPtr<FSDKMesh>(new FSDKMesh(L"Crypt\\Crypt.sdkmesh", false));
	m_pMesh->Init();

	// 创建SamplerState
	m_pSamplerState = RHI->CreateSamplerState(SF_Trilinear);

	// 创建BlendState
	m_pColorWriteOn = RHI->CreateBlendState();

	// 创建RasterizeState
	m_pCullBack = RHI->CreateRasterizerState(FM_Solid, CM_CCW);

	// 创建CB
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
// 销毁所有COM对象
//--------------------------------------------------------------------------------------
void DeferredLightingApp::OnDestroy()
{
}

//--------------------------------------------------------------------------------------
// Render the scene using the D3D11 device
// 渲染更新
//--------------------------------------------------------------------------------------
void DeferredLightingApp::OnRender(float fDeltaSeconds)
{
	ID3D11Device* pd3dDevice = DXUTGetD3D11Device();
	ID3D11DeviceContext* pd3dImmediateContext = DXUTGetD3D11DeviceContext();

	RHI->BeginRender();

	// 先清空BackBuffer
	RHI->SetDefaultBackBuffer();
	RHI->Clear(true, FLinearColor(sqrt(0.25f), sqrt(0.25f), sqrt(0.5f)), true, 1, false, 0);

	// 分3个Pass进行渲染
	RenderGPass();
	RenderDeferredLight();
	RenderScene();

	RHI->EndRender();
}

//--------------------------------------------------------------------------------------
// 初始化APP 
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
	ID3D11DeviceContext* pd3dImmediateContext = RHI->GetDeviceContext();
	D3DXMATRIXA16 mWVP, mWorld;

	mWVP = *m_Camera.GetWorldMatrix() * *m_Camera.GetViewMatrix() * *m_Camera.GetProjMatrix();
	D3DXMatrixTranspose(&mWVP, &mWVP);
	mWorld = *m_Camera.GetWorldMatrix();
	D3DXMatrixTranspose(&mWorld, &mWorld);

	// 设置CB的值
	CB_GPass TempValue = { mWVP, mWorld };
	m_pcbGPass->UpdateData((byte*)&TempValue, 0, sizeof(CB_GPass));
	m_pcbGPass->CommitData();

	FRHIBuffer* pGP = m_pcbGPass->GetBuffer().get();
	pd3dImmediateContext->VSSetConstantBuffers(0, 1, &pGP);
	pd3dImmediateContext->PSSetConstantBuffers(0, 1, &pGP);

	RHI->SetFrameBuffer(m_pfbGPass);
	RHI->Clear(true, FLinearColor::Black, true, 1, false, 0);

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

	D3DXVECTOR3 LightVec = D3DXVECTOR3(1.0, 1.0, -1.0);
	// 设置CB的值
	CB_DLPass TempVal = { LightVec, 0 };
	m_pcbDLPass->UpdateData((byte*)&TempVal, 0, sizeof(CB_DLPass));
	m_pcbDLPass->CommitData();
	FRHIBuffer* TempBuffer = m_pcbDLPass->GetBuffer().get();

	FRHIShaderResourceView* pSRV = m_pfbGPass->RenderTarget->GetRawSRV();
	RHI->GetDeviceContext()->PSSetShaderResources(0, 1, &pSRV);
	RHI->GetDeviceContext()->VSSetConstantBuffers(0, 1, &TempBuffer);
	RHI->GetDeviceContext()->PSSetConstantBuffers(0, 1, &TempBuffer);

	RHI->SetBoundShaderState(m_pDLPassBST);
	RHI->SetSamplerState(ST_PixelShader, 0, m_pSamplerState);

	// 要用TriangleStrip
	RHI->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	RHI->GetDeviceContext()->Draw(4, 0);
}

void DeferredLightingApp::RenderScene()
{
	ID3D11DeviceContext* pd3dImmediateContext = RHI->GetDeviceContext();

	// 把RT设为BackBuffer
	RHI->SetDefaultBackBuffer();

	D3DXMATRIXA16 mWVP;
	mWVP = *m_Camera.GetWorldMatrix() * *m_Camera.GetViewMatrix() * *m_Camera.GetProjMatrix();
	D3DXMatrixTranspose(&mWVP, &mWVP);

	// 设置CB的值
	CB_ScenePass TempVal;
	TempVal.m_mWVP = mWVP;
	TempVal.m_AmbientColor = D3DXVECTOR3(0.15f, 0.15f, 0.15f);
	TempVal.m_LightDiffuse = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	TempVal.Padding = float2(1.0, 1.0);
	m_pcbScenePass->UpdateData((byte*)&TempVal, 0, sizeof(CB_ScenePass));
	m_pcbScenePass->CommitData();
	FRHIBuffer* TempBuffer = m_pcbScenePass->GetBuffer().get();

	// 第一个坑被Mesh的贴图占了
	//pd3dImmediateContext->PSSetShaderResources(1, 1, &g_pGBufferTextureSRV[EGBT_Diffuse]);
	ID3D11ShaderResourceView* pSRV = m_pfbDL->RenderTarget->GetRawSRV();
	pd3dImmediateContext->PSSetShaderResources(1, 1, &pSRV);
	pd3dImmediateContext->VSSetConstantBuffers(0, 1, &TempBuffer);
	pd3dImmediateContext->PSSetConstantBuffers(0, 1, &TempBuffer);

	// Render State
	RHI->SetBlendState(m_pColorWriteOn);
	RHI->SetRasterizerState(m_pCullBack);
	RHI->SetBoundShaderState(m_pScenePassBST);
	RHI->SetSamplerState(ST_PixelShader, 0, m_pSamplerState);

	m_pMesh->Render(0);
}