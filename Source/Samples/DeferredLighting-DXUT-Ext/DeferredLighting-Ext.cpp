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
	m_pGPassBST.reset();
	m_pDLPassBST.reset();
	m_pScenePassBST.reset();
	m_pCullBack.reset();
	m_pColorWriteOn.reset();
	m_pcbGPass		= MakeSharedPtr<FConstantBuffer>();
	m_pcbDLPass		= MakeSharedPtr<FConstantBuffer>();
	m_pcbScenePass	= MakeSharedPtr<FConstantBuffer>();
	m_pMesh			= TSharedPtr<FSDKMesh>(new FSDKMesh(L"Crypt\\Crypt.sdkmesh", false));
}

//--------------------------------------------------------------------------------------
// Create any D3D11 resources that aren't dependant on the back buffer
// 初始化D3D资源
//--------------------------------------------------------------------------------------

HRESULT DeferredLightingApp::OnCreateDevice(ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc)
{
	HRESULT hr;

	// DL Init
	DLInitResource(pd3dDevice, pBackBufferSurfaceDesc);

	// 创建用来绘制Normal-Buffer的Depth
	// Create shadowmap surface and views
	D3D11_TEXTURE2D_DESC surfDesc;
	surfDesc.Width = pBackBufferSurfaceDesc->Width;
	surfDesc.Height = pBackBufferSurfaceDesc->Height;
	surfDesc.MipLevels = 1;
	surfDesc.ArraySize = 1;
	surfDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	surfDesc.SampleDesc.Count = 1;
	surfDesc.SampleDesc.Quality = 0;
	surfDesc.Usage = D3D11_USAGE_DEFAULT;
	surfDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	surfDesc.CPUAccessFlags = 0;
	surfDesc.MiscFlags = 0;
	V(pd3dDevice->CreateTexture2D(&surfDesc, 0, &g_pDepthTexture));

	// Create depth stencil view for shadowmap rendering
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Flags = 0;
	dsvDesc.Texture2D.MipSlice = 0;
	V(pd3dDevice->CreateDepthStencilView(g_pDepthTexture, &dsvDesc, &g_pDepthTextureDSV));

	// Test Begin
	D3D11_DEPTH_STENCIL_DESC dsDesc;
	ZeroMemory(&dsDesc, sizeof(dsDesc));
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	dsDesc.DepthEnable = false;
	g_depthStencilStateDisableDepth = NULL;
	V(pd3dDevice->CreateDepthStencilState(&dsDesc, &g_depthStencilStateDisableDepth));
	// Test End

	// Create shader resource view for shadowmap
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Texture2D.MostDetailedMip = 0;
	V(pd3dDevice->CreateShaderResourceView(g_pDepthTexture, &srvDesc, &g_pDepthTextureSRV));

	// 顶点声明工厂
	FVertexDeclarationFactory_DL VDF;

	// GPass Shader
	CREATE_SHADER(GPassVertexShader, GPassVS);
	CREATE_SHADER(GPassPixelShader, GPassPS);

	m_pGPassBST = RHI->CreateBoundShaderState(
		VDF.GetVertexDeclaration(),
		GPassVS.GetCode(),
		GPassVS.GetVertexShader(),
		GPassPS.GetPixelShader()
		);

	// DL Shader
	CREATE_SHADER(DLVertexShader, DLPassVS);
	CREATE_SHADER(DLPixelShader, DLPassPS);
	m_pDLPassBST = RHI->CreateBoundShaderState(
		VDF.GetVertexDeclaration(),
		DLPassVS.GetCode(),
		DLPassVS.GetVertexShader(),
		DLPassPS.GetPixelShader()
		);

	// Scene Pass Shader
	CREATE_SHADER(SceneVertexShader, ScenePassVS);
	CREATE_SHADER(ScenePixelShader, ScenePassPS);
	m_pScenePassBST = RHI->CreateBoundShaderState(
		VDF.GetVertexDeclaration(),
		ScenePassVS.GetCode(),
		ScenePassVS.GetVertexShader(),
		ScenePassPS.GetPixelShader()
		);

	// 创建Mesh
	m_pMesh->Init();

	// 创建SamplerState
	m_pSamplerState = RHI->CreateSamplerState(SF_Trilinear);

	// 创建BlendState
	m_pColorWriteOn = RHI->CreateBlendState();

	// 创建RasterizeState
	m_pCullBack = RHI->CreateRasterizerState(FM_Solid, CM_CCW);

	// 创建CB
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
	SAFE_RELEASE(g_pDepthTexture);
	SAFE_RELEASE(g_pDepthTextureSRV);
	SAFE_RELEASE(g_pDepthTextureDSV);
	SAFE_RELEASE(g_depthStencilStateDisableDepth);

	// 先Release
	for (INT RTType = EGBT_Normal; RTType < EGBT_MaxSize; ++RTType)
	{
		SAFE_RELEASE(g_pGBufferTexture[RTType]);
		SAFE_RELEASE(g_pGBufferTextureRT[RTType]);
		SAFE_RELEASE(g_pGBufferTextureSRV[RTType]);
	}
}

//--------------------------------------------------------------------------------------
// 创建各种RT
//--------------------------------------------------------------------------------------
void DeferredLightingApp::DLInitResource(ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc)
{
	D3D11_TEXTURE2D_DESC desc;
	::ZeroMemory(&desc, sizeof (desc));
	desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.Height = pBackBufferSurfaceDesc->Height;
	desc.Width = pBackBufferSurfaceDesc->Width;
	desc.ArraySize = 1;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.SampleDesc.Count = pBackBufferSurfaceDesc->SampleDesc.Count;
	desc.SampleDesc.Quality = pBackBufferSurfaceDesc->SampleDesc.Quality;
	desc.MipLevels = 1;

	for (INT RTType = EGBT_Normal; RTType < EGBT_MaxSize; ++RTType)
	{
		pd3dDevice->CreateTexture2D(&desc, 0, &g_pGBufferTexture[RTType]);
		pd3dDevice->CreateRenderTargetView(g_pGBufferTexture[RTType], 0, &g_pGBufferTextureRT[RTType]);
		pd3dDevice->CreateShaderResourceView(g_pGBufferTexture[RTType], 0, &g_pGBufferTextureSRV[RTType]);
	}
}

//--------------------------------------------------------------------------------------
// Render the scene using the D3D11 device
// 渲染更新
//--------------------------------------------------------------------------------------
void DeferredLightingApp::OnRender(float fDeltaSeconds)
{
	ID3D11Device* pd3dDevice = DXUTGetD3D11Device();
	ID3D11DeviceContext* pd3dImmediateContext = DXUTGetD3D11DeviceContext();

	ID3D11RenderTargetView* pRTV = DXUTGetD3D11RenderTargetView();
	ID3D11DepthStencilView* pDSV = DXUTGetD3D11DepthStencilView();

	float ClearColor[4] = { sqrt(0.25f), sqrt(0.25f), sqrt(0.5f), 0.0f };
	pd3dImmediateContext->ClearRenderTargetView(pRTV, ClearColor);
	pd3dImmediateContext->ClearDepthStencilView(pDSV, D3D11_CLEAR_DEPTH, 1.0, 0);

	// 分3个Pass进行渲染
	RenderGPass(pd3dDevice, pd3dImmediateContext);
	RenderDeferredLight(pd3dDevice, pd3dImmediateContext);
	RenderScene(pd3dDevice, pd3dImmediateContext);
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


void DeferredLightingApp::RenderGPass(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext)
{
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

	ID3D11RenderTargetView* pRTV = g_pGBufferTextureRT[EGBT_Normal];
	ID3D11DepthStencilView* pDSV = g_pDepthTextureDSV;

	pd3dImmediateContext->OMSetRenderTargets(1, &pRTV, pDSV);
	float ClearColor[4] = { 0.f, 0.f, 0.f, 1.0f };
	pd3dImmediateContext->ClearRenderTargetView(pRTV, ClearColor);
	pd3dImmediateContext->ClearDepthStencilView(pDSV, D3D11_CLEAR_DEPTH, 1.0, 0);
	pd3dImmediateContext->OMSetDepthStencilState(g_depthStencilStateDisableDepth, 0);

	// Render State
	RHI->SetBlendState(m_pColorWriteOn);
	RHI->SetRasterizerState(m_pCullBack);
	RHI->SetBoundShaderState(m_pGPassBST);
	RHI->PSSetSamplerState(0, m_pSamplerState);

	m_pMesh->Render(0);
}

void DeferredLightingApp::RenderDeferredLight(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext)
{
	ID3D11RenderTargetView* pRTV = g_pGBufferTextureRT[EGBT_Diffuse];
	ID3D11DepthStencilView* pDSV = 0;

	pd3dImmediateContext->OMSetRenderTargets(1, &pRTV, pDSV);
	float ClearColor[4] = { 0.f, 0.f, 0.f, 1.0f };
	pd3dImmediateContext->ClearRenderTargetView(pRTV, ClearColor);

	D3DXVECTOR3 LightVec = D3DXVECTOR3(1.0, 1.0, -1.0);

	// 设置CB的值
	CB_DLPass TempVal = { LightVec, 0 };
	m_pcbDLPass->UpdateData((byte*)&TempVal, 0, sizeof(CB_DLPass));
	m_pcbDLPass->CommitData();
	FRHIBuffer* TempBuffer = m_pcbDLPass->GetBuffer().get();

	pd3dImmediateContext->PSSetShaderResources(0, 1, &g_pGBufferTextureSRV[EGBT_Normal]);
	pd3dImmediateContext->VSSetConstantBuffers(0, 1, &TempBuffer);
	pd3dImmediateContext->PSSetConstantBuffers(0, 1, &TempBuffer);

	RHI->SetBoundShaderState(m_pDLPassBST);
	RHI->PSSetSamplerState(0, m_pSamplerState);

	// 要用TriangleStrip
	pd3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	pd3dImmediateContext->Draw(4, 0);
}

void DeferredLightingApp::RenderScene(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext)
{
	ID3D11RenderTargetView* pRTV = DXUTGetD3D11RenderTargetView();
	ID3D11DepthStencilView* pDSV = DXUTGetD3D11DepthStencilView();

	// 把RT设为BackBuffer
	pd3dImmediateContext->OMSetRenderTargets(1, &pRTV, pDSV);

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
	pd3dImmediateContext->PSSetShaderResources(1, 1, &g_pGBufferTextureSRV[EGBT_Diffuse]);
	pd3dImmediateContext->VSSetConstantBuffers(0, 1, &TempBuffer);
	pd3dImmediateContext->PSSetConstantBuffers(0, 1, &TempBuffer);

	// Render State
	RHI->SetBlendState(m_pColorWriteOn);
	RHI->SetRasterizerState(m_pCullBack);
	RHI->SetBoundShaderState(m_pScenePassBST);
	RHI->PSSetSamplerState(0, m_pSamplerState);

	m_pMesh->Render(0);
}