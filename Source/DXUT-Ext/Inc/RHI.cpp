#include "RHI.h"
#include "Core.h"


FDynamicRHI* RHI;

void FDynamicRHI::InitRHI()
{
	Check(!RHI);
	RHI = new FDynamicRHI();
}

void FDynamicRHI::ReleaseRHI()
{
	Check(RHI);
	delete RHI;
}

FDynamicRHI::FDynamicRHI()
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

FDynamicRHI::~FDynamicRHI()
{
	m_pd3dDevice = NULL;
	m_pd3dImmediateContext = NULL;
}

ID3D11DeviceContext* FDynamicRHI::GetDeviceContext()
{
	return m_pd3dImmediateContext;
}

ID3D11Device* FDynamicRHI::GetDevice()
{
	return m_pd3dDevice;
}


void FDynamicRHI::InitConstantBuffers()
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