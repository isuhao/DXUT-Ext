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

void FDynamicRHI::InitPixelFormat()
{
	GPixelFormats[PF_Unknown].Format = DXGI_FORMAT_UNKNOWN;
	GPixelFormats[PF_A32B32G32R32F].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	GPixelFormats[PF_A8R8G8B8].Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	GPixelFormats[PF_R8].Format = DXGI_FORMAT_R8_UNORM;
	GPixelFormats[PF_DXT1].Format = DXGI_FORMAT_BC1_UNORM;
	GPixelFormats[PF_DXT3].Format = DXGI_FORMAT_BC2_UNORM;
	GPixelFormats[PF_DXT5].Format = DXGI_FORMAT_BC3_UNORM;
	GPixelFormats[PF_DepthStencil].Format = DXGI_FORMAT_R24G8_TYPELESS;
	GPixelFormats[PF_ShadowDepth].Format = DXGI_FORMAT_R24G8_TYPELESS;
}