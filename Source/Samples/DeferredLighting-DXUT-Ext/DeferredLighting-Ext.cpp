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
	pd3dDevice->CreateTexture2D(&surfDesc, 0, &g_pDepthTexture);

	// Create depth stencil view for shadowmap rendering
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Flags = 0;
	dsvDesc.Texture2D.MipSlice = 0;
	pd3dDevice->CreateDepthStencilView(g_pDepthTexture, &dsvDesc, &g_pDepthTextureDSV);

	// Create shader resource view for shadowmap
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Texture2D.MostDetailedMip = 0;
	pd3dDevice->CreateShaderResourceView(g_pDepthTexture, &srvDesc, &g_pDepthTextureSRV);

	// Vertex Declaration
	FVertexDeclarationFactory_DL VDF;

	// GPass Shader
	CREATE_SHADER(GPassVertexShader, GPassVS);
	CREATE_SHADER(GPassPixelShader, GPassPS);

	m_pGPassBST = RHI->CreateBoundShaderState(
		VDF.GetVertexDeclaration(),
		GPassVS.GetCode(),
		GPassVS.GetVertexShader(),
		GPassPS.GetPixelShader(),
		NULL,
		NULL,
		NULL,
		NULL
		);

	// DL Shader
	CREATE_SHADER(DLVertexShader, DLPassVS);
	CREATE_SHADER(DLPixelShader, DLPassPS);
	m_pDLPassBST = RHI->CreateBoundShaderState(
		VDF.GetVertexDeclaration(),
		DLPassVS.GetCode(),
		DLPassVS.GetVertexShader(),
		DLPassPS.GetPixelShader(),
		NULL,
		NULL,
		NULL,
		NULL
		);

	// Scene Pass Shader
	CREATE_SHADER(SceneVertexShader, ScenePassVS);
	CREATE_SHADER(ScenePixelShader, ScenePassPS);
	m_pScenePassBST = RHI->CreateBoundShaderState(
		VDF.GetVertexDeclaration(),
		ScenePassVS.GetCode(),
		ScenePassVS.GetVertexShader(),
		ScenePassPS.GetPixelShader(),
		NULL,
		NULL,
		NULL,
		NULL
		);

	// 创建Mesh
	V_RETURN(g_Mesh.Create(pd3dDevice, L"Crypt\\Crypt.sdkmesh", true));

	// 创建SamplerState
	D3D11_SAMPLER_DESC samDesc;

	ZeroMemory(&samDesc, sizeof(samDesc));
	//samDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	samDesc.MipLODBias = 0.0f;
	samDesc.MaxAnisotropy = 1;
	samDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samDesc.BorderColor[0] = samDesc.BorderColor[1] = samDesc.BorderColor[2] = samDesc.BorderColor[3] = 0;
	samDesc.MinLOD = 0;
	samDesc.MaxLOD = D3D11_FLOAT32_MAX;
	samDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	V_RETURN(pd3dDevice->CreateSamplerState(&samDesc, &g_pSamLinearWrap)); 

	m_pSamplerState = RHI->CreateSamplerState(ESF_Trilinear);

	// 创建BlendState
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));
	blendDesc.IndependentBlendEnable = FALSE;
	blendDesc.RenderTarget[0].BlendEnable = FALSE;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	V_RETURN(pd3dDevice->CreateBlendState(&blendDesc, &g_pColorWritesOn));

	// 创建RasterizeState
	D3D11_RASTERIZER_DESC rastDesc;
	ZeroMemory(&rastDesc, sizeof(rastDesc));
	rastDesc.CullMode = D3D11_CULL_BACK;
	rastDesc.FillMode = D3D11_FILL_SOLID;
	rastDesc.AntialiasedLineEnable = FALSE;
	rastDesc.DepthBias = 0;
	rastDesc.DepthBiasClamp = 0;
	rastDesc.DepthClipEnable = TRUE;
	rastDesc.FrontCounterClockwise = FALSE;
	rastDesc.MultisampleEnable = TRUE;
	rastDesc.ScissorEnable = FALSE;
	rastDesc.SlopeScaledDepthBias = 0;
	V_RETURN(pd3dDevice->CreateRasterizerState(&rastDesc, &g_pCullBack));

	// 创建CB
	D3D11_BUFFER_DESC cbDesc;
	ZeroMemory(&cbDesc, sizeof(cbDesc));
	cbDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDesc.ByteWidth = sizeof(CB_GPass);
	V_RETURN(pd3dDevice->CreateBuffer(&cbDesc, NULL, &g_pcbGPass));

	cbDesc.ByteWidth = sizeof(CB_DLPass);
	V_RETURN(pd3dDevice->CreateBuffer(&cbDesc, NULL, &g_pcbDLPass));

	cbDesc.ByteWidth = sizeof(CB_ScenePass);
	V_RETURN(pd3dDevice->CreateBuffer(&cbDesc, NULL, &g_pcbScenePass));

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
	g_Mesh.Destroy();

	SAFE_RELEASE(g_pcbGPass);
	SAFE_RELEASE(g_pcbDLPass);
	SAFE_RELEASE(g_pcbScenePass);
	SAFE_RELEASE(g_pSamLinearWrap);
	SAFE_RELEASE(g_pColorWritesOn);
	SAFE_RELEASE(g_pCullBack);
	SAFE_RELEASE(g_pDepthTexture);
	SAFE_RELEASE(g_pDepthTextureSRV);
	SAFE_RELEASE(g_pDepthTextureDSV);

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
	HRESULT hr;
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	D3DXMATRIXA16 mWVP, mWorld;

	mWVP = *m_Camera.GetWorldMatrix() * *m_Camera.GetViewMatrix() * *m_Camera.GetProjMatrix();
	D3DXMatrixTranspose(&mWVP, &mWVP);
	mWorld = *m_Camera.GetWorldMatrix();
	D3DXMatrixTranspose(&mWorld, &mWorld);

	// 设置CB的值
	V(pd3dImmediateContext->Map(g_pcbGPass, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource));
	CB_GPass* pcbGPass = (CB_GPass*)(MappedResource.pData);
	pcbGPass->m_mWorldViewProj = mWVP;
	pcbGPass->m_mWorld = mWorld;
	pd3dImmediateContext->Unmap(g_pcbGPass, 0);

	pd3dImmediateContext->VSSetConstantBuffers(0, 1, &g_pcbGPass);
	pd3dImmediateContext->PSSetConstantBuffers(0, 1, &g_pcbGPass);

	ID3D11RenderTargetView* pRTV = g_pGBufferTextureRT[EGBT_Normal];
	ID3D11DepthStencilView* pDSV = g_pDepthTextureDSV;

	pd3dImmediateContext->OMSetRenderTargets(1, &pRTV, pDSV);
	float ClearColor[4] = { 0.f, 0.f, 0.f, 1.0f };
	pd3dImmediateContext->ClearRenderTargetView(pRTV, ClearColor);
	pd3dImmediateContext->ClearDepthStencilView(pDSV, D3D11_CLEAR_DEPTH, 1.0, 0);

	// Render State
	pd3dImmediateContext->OMSetBlendState(g_pColorWritesOn, 0, 0xffffffff);
	pd3dImmediateContext->RSSetState(g_pCullBack);

	RHI->SetBoundShaderState(m_pGPassBST);
	RHI->PSSetSamplerState(0, m_pSamplerState);

	UINT Strides[1];
	UINT Offsets[1];
	ID3D11Buffer* pVB[1];
	pVB[0] = g_Mesh.GetVB11(0, 0);
	Strides[0] = (UINT)g_Mesh.GetVertexStride(0, 0);
	Offsets[0] = 0;
	pd3dImmediateContext->IASetVertexBuffers(0, 1, pVB, Strides, Offsets);
	pd3dImmediateContext->IASetIndexBuffer(g_Mesh.GetIB11(0), g_Mesh.GetIBFormat11(0), 0);

	g_Mesh.Render(pd3dImmediateContext, 0);
}

void DeferredLightingApp::RenderDeferredLight(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext)
{
	ID3D11RenderTargetView* pRTV = g_pGBufferTextureRT[EGBT_Diffuse];
	ID3D11DepthStencilView* pDSV = 0;

	pd3dImmediateContext->OMSetRenderTargets(1, &pRTV, pDSV);
	float ClearColor[4] = { 0.f, 0.f, 0.f, 1.0f };
	pd3dImmediateContext->ClearRenderTargetView(pRTV, ClearColor);

	HRESULT hr;
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	
	//D3DXVECTOR3 LightVec = *m_LightCamera.GetEyePt() - *m_LightCamera.GetLookAtPt();
	D3DXVECTOR3 LightVec = D3DXVECTOR3(1.0, 1.0, -1.0);

	// 设置CB的值
	V(pd3dImmediateContext->Map(g_pcbDLPass, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource));
	CB_DLPass* pcbGPass = (CB_DLPass*)(MappedResource.pData);
	pcbGPass->g_vLightDir = LightVec;
	pd3dImmediateContext->Unmap(g_pcbDLPass, 0);

	pd3dImmediateContext->PSSetShaderResources(0, 1, &g_pGBufferTextureSRV[EGBT_Normal]);
	pd3dImmediateContext->VSSetConstantBuffers(0, 1, &g_pcbDLPass);
	pd3dImmediateContext->PSSetConstantBuffers(0, 1, &g_pcbDLPass);

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

	HRESULT hr;
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	D3DXMATRIXA16 mWVP;

	mWVP = *m_Camera.GetWorldMatrix() * *m_Camera.GetViewMatrix() * *m_Camera.GetProjMatrix();
	D3DXMatrixTranspose(&mWVP, &mWVP);

	// 设置CB的值
	V(pd3dImmediateContext->Map(g_pcbScenePass, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource));
	CB_ScenePass* pcbScenePass = (CB_ScenePass*)(MappedResource.pData);
	pcbScenePass->m_mWVP = mWVP;
	pcbScenePass->m_AmbientColor = D3DXVECTOR3(0.15f, 0.15f, 0.15f);
	pcbScenePass->m_LightDiffuse = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	pcbScenePass->Padding = D3DXVECTOR2(1.0, 1.0);
	pd3dImmediateContext->Unmap(g_pcbScenePass, 0);

	// 第一个坑被Mesh的贴图占了
	pd3dImmediateContext->PSSetShaderResources(1, 1, &g_pGBufferTextureSRV[EGBT_Diffuse]);

	pd3dImmediateContext->VSSetConstantBuffers(0, 1, &g_pcbScenePass);
	pd3dImmediateContext->PSSetConstantBuffers(0, 1, &g_pcbScenePass);
	//pd3dImmediateContext->IASetInputLayout(g_pSPLayout);

	UINT Strides[1];
	UINT Offsets[1];
	ID3D11Buffer* pVB[1];
	pVB[0] = g_Mesh.GetVB11(0, 0);
	Strides[0] = (UINT)g_Mesh.GetVertexStride(0, 0);
	Offsets[0] = 0;
	pd3dImmediateContext->IASetVertexBuffers(0, 1, pVB, Strides, Offsets);
	pd3dImmediateContext->IASetIndexBuffer(g_Mesh.GetIB11(0), g_Mesh.GetIBFormat11(0), 0);

	// Render State
	pd3dImmediateContext->OMSetBlendState(g_pColorWritesOn, 0, 0xffffffff);
	pd3dImmediateContext->RSSetState(g_pCullBack);

	RHI->SetBoundShaderState(m_pScenePassBST);
	RHI->PSSetSamplerState(0, m_pSamplerState);

	g_Mesh.Render(pd3dImmediateContext, 0);
}