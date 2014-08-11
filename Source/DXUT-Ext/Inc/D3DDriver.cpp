#include "D3DDriver.h"
#include "Core.h"


FD3D11Driver* D3D;

void FD3D11Driver::InitD3DDriver()
{
	Check(!D3D);
	D3D = new FD3D11Driver();
}

void FD3D11Driver::ReleaseD3DDriver()
{
	Check(D3D);
	delete D3D;
}

FD3D11Driver::FD3D11Driver()
{
	m_pd3dDevice = DXUTGetD3D11Device();
	m_pd3dImmediateContext  = DXUTGetD3D11DeviceContext();

	for (int i = 0; i < MAX_RT_COUNT; ++i)
	{
		m_pCurrRTVs[i] = NULL;
	}

	InitPixelFormat();

	InitConstantBuffers();
}

FD3D11Driver::~FD3D11Driver()
{
	m_pd3dDevice = NULL;
	m_pd3dImmediateContext = NULL;
}

ID3D11DeviceContext* FD3D11Driver::GetDeviceContext()
{
	return m_pd3dImmediateContext;
}

ID3D11Device* FD3D11Driver::GetDevice()
{
	return m_pd3dDevice;
}


void FD3D11Driver::InitConstantBuffers()
{
	for (byte ShaderType = ST_VertexShader; ShaderType < ST_NumShaderTypes; ++ShaderType)
	{
		m_ConstantBuffers[ShaderType].Reset();
		for (uint i = 0; i < MAX_CB_COUNT; ++i)
		{
			m_ConstantBuffers[ShaderType].Add(TSharedPtr<FConstantBuffer>(new FConstantBuffer()));
			m_ConstantBuffers[ShaderType][i]->CreateBuffer(GSystemSettings.MaxConstantBufferSize);
		}
	}
}