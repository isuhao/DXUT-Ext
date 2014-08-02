#include "DeferredLighting-Ext.h"

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

//--------------------------------------------------------------------------------------
// Create any D3D11 resources that aren't dependant on the back buffer
// ��ʼ��D3D��Դ
//--------------------------------------------------------------------------------------

HRESULT DeferredLightingApp::OnCreateDevice(ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc)
{
	HRESULT hr;

	ID3D11DeviceContext* pDeviceContext = DXUTGetD3D11DeviceContext();
	V_RETURN(g_DialogResourceManager.OnD3D11CreateDevice(pd3dDevice, pDeviceContext));
	V_RETURN(g_SettingsDlg.OnD3D11CreateDevice(pd3dDevice));

	//////////////////////////////////////////////////////////////////////////

	// DL Init
	DLInitResource(pd3dDevice, pBackBufferSurfaceDesc);

	// ������������Normal-Buffer��Depth
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

	// InputLayout
	const D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	// ����Shader
	ID3DBlob* pBlob = NULL;
	V_RETURN(CompileShaderFromFile(L"GPass.hlsl", "VS_Main", "vs_5_0", &pBlob));
	V_RETURN(pd3dDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &g_pGPVertexShader));
	V_RETURN(pd3dDevice->CreateInputLayout(layout, ARRAYSIZE(layout), pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &g_pGPLayout));
	SAFE_RELEASE(pBlob);

	V_RETURN(CompileShaderFromFile(L"GPass.hlsl", "PS_Main", "ps_5_0", &pBlob));
	V_RETURN(pd3dDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &g_pGPPixelShader));
	SAFE_RELEASE(pBlob);

	// DL Shader
	V_RETURN(CompileShaderFromFile(L"DeferredLight.hlsl", "VS_Main", "vs_5_0", &pBlob));
	V_RETURN(pd3dDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &g_pDLVertexShader));
	V_RETURN(pd3dDevice->CreateInputLayout(layout, ARRAYSIZE(layout), pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &g_pDLLayout));
	SAFE_RELEASE(pBlob);

	V_RETURN(CompileShaderFromFile(L"DeferredLight.hlsl", "PS_Main", "ps_5_0", &pBlob));
	V_RETURN(pd3dDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &g_pDLPixelShader));
	SAFE_RELEASE(pBlob);

	// RenderScene Shader
	V_RETURN(CompileShaderFromFile(L"ScenePass.hlsl", "RenderSceneVS", "vs_5_0", &pBlob));
	V_RETURN(pd3dDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &g_pSPVertexShader));
	V_RETURN(pd3dDevice->CreateInputLayout(layout, ARRAYSIZE(layout), pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &g_pSPLayout));
	SAFE_RELEASE(pBlob);

	V_RETURN(CompileShaderFromFile(L"ScenePass.hlsl", "RenderScenePS", "ps_5_0", &pBlob));
	V_RETURN(pd3dDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &g_pSPPixelShader));
	SAFE_RELEASE(pBlob);

	// ����Mesh
	V_RETURN(g_Mesh.Create(pd3dDevice, L"Crypt\\Crypt.sdkmesh", true));

	// ����SamplerState
	D3D11_SAMPLER_DESC samDesc;

	ZeroMemory(&samDesc, sizeof(samDesc));
	//samDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	samDesc.MipLODBias = 0.0f;
	samDesc.MaxAnisotropy = 1;
	samDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samDesc.BorderColor[0] = samDesc.BorderColor[1] = samDesc.BorderColor[2] = samDesc.BorderColor[3] = 0;
	samDesc.MinLOD = 0;
	samDesc.MaxLOD = D3D11_FLOAT32_MAX;
	samDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	V_RETURN(pd3dDevice->CreateSamplerState(&samDesc, &g_pSamLinearWrap));

	// ����BlendState
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));
	blendDesc.IndependentBlendEnable = FALSE;
	blendDesc.RenderTarget[0].BlendEnable = FALSE;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	V_RETURN(pd3dDevice->CreateBlendState(&blendDesc, &g_pColorWritesOn));

	// ����RasterizeState
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

	// ����CB
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
// Handle updates to the scene.  This is called regardless of which D3D API is used
// �൱��Tick
//--------------------------------------------------------------------------------------
void DeferredLightingApp::OnTick(float DeltaSeconds)
{
	//// Update the camera's position based on user input 
	//g_Camera.FrameMove(fElapsedTime);
	//g_LightCamera.FrameMove(fElapsedTime);
}

//--------------------------------------------------------------------------------------
// Release D3D11 resources created in OnD3D11CreateDevice 
// ��������COM����
//--------------------------------------------------------------------------------------
void DeferredLightingApp::OnDestroy()
{
	g_Mesh.Destroy();

	SAFE_RELEASE(g_pDLLayout);
	SAFE_RELEASE(g_pDLVertexShader);
	SAFE_RELEASE(g_pDLPixelShader);
	SAFE_RELEASE(g_pSPLayout);
	SAFE_RELEASE(g_pSPVertexShader);
	SAFE_RELEASE(g_pSPPixelShader);
	SAFE_RELEASE(g_pcbGPass);
	SAFE_RELEASE(g_pcbDLPass);
	SAFE_RELEASE(g_pcbScenePass);
	SAFE_RELEASE(g_pGPLayout);
	SAFE_RELEASE(g_pGPVertexShader);
	SAFE_RELEASE(g_pGPPixelShader);
	SAFE_RELEASE(g_pSamLinearWrap);
	SAFE_RELEASE(g_pColorWritesOn);
	SAFE_RELEASE(g_pCullBack);
	SAFE_RELEASE(g_pDepthTexture);
	SAFE_RELEASE(g_pDepthTextureSRV);
	SAFE_RELEASE(g_pDepthTextureDSV);

	// ��Release
	for (INT RTType = EGBT_Normal; RTType < EGBT_MaxSize; ++RTType)
	{
		SAFE_RELEASE(g_pGBufferTexture[RTType]);
		SAFE_RELEASE(g_pGBufferTextureRT[RTType]);
		SAFE_RELEASE(g_pGBufferTextureSRV[RTType]);
	}
}

//--------------------------------------------------------------------------------------
// ��������RT
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
// ��Ⱦ����
//--------------------------------------------------------------------------------------
void DeferredLightingApp::OnRender(float fDeltaSeconds)
{
	// If the settings dialog is being shown, then render it instead of rendering the app's scene
	//if (g_SettingsDlg.IsActive())
	//{
	//	g_SettingsDlg.OnRender(fDeltaSeconds);
	//	return;
	//}

	ID3D11Device* pd3dDevice = DXUTGetD3D11Device();
	ID3D11DeviceContext* pd3dImmediateContext = DXUTGetD3D11DeviceContext();

	ID3D11RenderTargetView* pRTV = DXUTGetD3D11RenderTargetView();
	ID3D11DepthStencilView* pDSV = DXUTGetD3D11DepthStencilView();

	float ClearColor[4] = { sqrt(0.25f), sqrt(0.25f), sqrt(0.5f), 0.0f };
	pd3dImmediateContext->ClearRenderTargetView(pRTV, ClearColor);
	pd3dImmediateContext->ClearDepthStencilView(pDSV, D3D11_CLEAR_DEPTH, 1.0, 0);

	RenderGPass(pd3dDevice, pd3dImmediateContext);

	RenderDeferredLight(pd3dDevice, pd3dImmediateContext);

	RenderScene(pd3dDevice, pd3dImmediateContext);
}

//--------------------------------------------------------------------------------------
// ��ʼ��APP 
//--------------------------------------------------------------------------------------
void DeferredLightingApp::OnInit()
{
	//g_Camera.SetRotateButtons( true, false, false );
	g_Camera.SetButtonMasks(MOUSE_LEFT_BUTTON, MOUSE_WHEEL, MOUSE_MIDDLE_BUTTON);
	g_LightCamera.SetButtonMasks(MOUSE_RIGHT_BUTTON, 0, 0);

	// Setup the camera's view parameters
	D3DXVECTOR3 vecEye(7.0f, 7.0f, -7.0f);
	D3DXVECTOR3 vecAt(0.0f, 0.0f, 0.0f);
	g_Camera.SetViewParams(&vecEye, &vecAt);

	// Setup light camera's view parameters
	D3DXVECTOR3 vecEyeLight(9.0f, 15.0f, 9.0f);
	D3DXVECTOR3 vecAtLight(0.0f, 0.0f, 0.0f);
	g_LightCamera.SetViewParams(&vecEyeLight, &vecAtLight);
}


void DeferredLightingApp::RenderGPass(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext)
{
	HRESULT hr;
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	D3DXMATRIXA16 mWVP, mWorld;

	mWVP = *g_Camera.GetWorldMatrix() * *g_Camera.GetViewMatrix() * *g_Camera.GetProjMatrix();
	D3DXMatrixTranspose(&mWVP, &mWVP);
	mWorld = *g_Camera.GetWorldMatrix();
	D3DXMatrixTranspose(&mWorld, &mWorld);

	// ����CB��ֵ
	V(pd3dImmediateContext->Map(g_pcbGPass, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource));
	CB_GPass* pcbGPass = (CB_GPass*)(MappedResource.pData);
	pcbGPass->m_mWorldViewProj = mWVP;
	pcbGPass->m_mWorld = mWorld;
	pd3dImmediateContext->Unmap(g_pcbGPass, 0);

	pd3dImmediateContext->VSSetConstantBuffers(0, 1, &g_pcbGPass);
	pd3dImmediateContext->PSSetConstantBuffers(0, 1, &g_pcbGPass);

	pd3dImmediateContext->IASetInputLayout(g_pGPLayout);

	UINT Strides[1];
	UINT Offsets[1];
	ID3D11Buffer* pVB[1];
	pVB[0] = g_Mesh.GetVB11(0, 0);
	Strides[0] = (UINT)g_Mesh.GetVertexStride(0, 0);
	Offsets[0] = 0;
	pd3dImmediateContext->IASetVertexBuffers(0, 1, pVB, Strides, Offsets);
	pd3dImmediateContext->IASetIndexBuffer(g_Mesh.GetIB11(0), g_Mesh.GetIBFormat11(0), 0);

	ID3D11RenderTargetView* pRTV = g_pGBufferTextureRT[EGBT_Normal];
	ID3D11DepthStencilView* pDSV = g_pDepthTextureDSV;

	//ID3D11RenderTargetView* pRTV = DXUTGetD3D11RenderTargetView();
	//ID3D11DepthStencilView* pDSV = DXUTGetD3D11DepthStencilView();

	pd3dImmediateContext->OMSetRenderTargets(1, &pRTV, pDSV);
	float ClearColor[4] = { 0.f, 0.f, 0.f, 1.0f };
	pd3dImmediateContext->ClearRenderTargetView(pRTV, ClearColor);
	pd3dImmediateContext->ClearDepthStencilView(pDSV, D3D11_CLEAR_DEPTH, 1.0, 0);

	// Render State
	pd3dImmediateContext->OMSetBlendState(g_pColorWritesOn, 0, 0xffffffff);
	pd3dImmediateContext->RSSetState(g_pCullBack);

	pd3dImmediateContext->VSSetShader(g_pGPVertexShader, NULL, 0);
	pd3dImmediateContext->PSSetShader(g_pGPPixelShader, NULL, 0);
	pd3dImmediateContext->PSSetSamplers(0, 1, &g_pSamLinearWrap);

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
	
	//D3DXVECTOR3 LightVec = *g_LightCamera.GetEyePt() - *g_LightCamera.GetLookAtPt();
	D3DXVECTOR3 LightVec = D3DXVECTOR3(1.0, 1.0, -1.0);

	// ����CB��ֵ
	V(pd3dImmediateContext->Map(g_pcbDLPass, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource));
	CB_DLPass* pcbGPass = (CB_DLPass*)(MappedResource.pData);
	pcbGPass->g_vLightDir = LightVec;
	pd3dImmediateContext->Unmap(g_pcbDLPass, 0);

	pd3dImmediateContext->PSSetShaderResources(0, 1, &g_pGBufferTextureSRV[EGBT_Normal]);

	pd3dImmediateContext->VSSetConstantBuffers(0, 1, &g_pcbDLPass);
	pd3dImmediateContext->PSSetConstantBuffers(0, 1, &g_pcbDLPass);
	pd3dImmediateContext->IASetInputLayout(g_pDLLayout);

	pd3dImmediateContext->VSSetShader(g_pDLVertexShader, NULL, 0);
	pd3dImmediateContext->PSSetShader(g_pDLPixelShader, NULL, 0);
	pd3dImmediateContext->PSSetSamplers(0, 1, &g_pSamLinearWrap);

	// Ҫ��TriangleStrip
	pd3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	pd3dImmediateContext->Draw(4, 0);
}

void DeferredLightingApp::RenderScene(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext)
{
	ID3D11RenderTargetView* pRTV = DXUTGetD3D11RenderTargetView();
	ID3D11DepthStencilView* pDSV = DXUTGetD3D11DepthStencilView();

	// ��RT��ΪBackBuffer
	pd3dImmediateContext->OMSetRenderTargets(1, &pRTV, pDSV);

	HRESULT hr;
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	D3DXMATRIXA16 mWVP;

	mWVP = *g_Camera.GetWorldMatrix() * *g_Camera.GetViewMatrix() * *g_Camera.GetProjMatrix();
	D3DXMatrixTranspose(&mWVP, &mWVP);

	// ����CB��ֵ
	V(pd3dImmediateContext->Map(g_pcbScenePass, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource));
	CB_ScenePass* pcbScenePass = (CB_ScenePass*)(MappedResource.pData);
	pcbScenePass->m_mWVP = mWVP;
	pcbScenePass->m_AmbientColor = D3DXVECTOR3(0.15f, 0.15f, 0.15f);
	pcbScenePass->m_LightDiffuse = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	pcbScenePass->Padding = D3DXVECTOR2(1.0, 1.0);
	pd3dImmediateContext->Unmap(g_pcbScenePass, 0);

	// ��һ���ӱ�Mesh����ͼռ��
	pd3dImmediateContext->PSSetShaderResources(1, 1, &g_pGBufferTextureSRV[EGBT_Diffuse]);

	pd3dImmediateContext->VSSetConstantBuffers(0, 1, &g_pcbScenePass);
	pd3dImmediateContext->PSSetConstantBuffers(0, 1, &g_pcbScenePass);
	pd3dImmediateContext->IASetInputLayout(g_pGPLayout);

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

	pd3dImmediateContext->VSSetShader(g_pSPVertexShader, NULL, 0);
	pd3dImmediateContext->PSSetShader(g_pSPPixelShader, NULL, 0);
	pd3dImmediateContext->PSSetSamplers(0, 1, &g_pSamLinearWrap);

	g_Mesh.Render(pd3dImmediateContext, 0);
}