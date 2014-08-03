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
}

FDynamicRHI::~FDynamicRHI()
{
	m_pd3dDevice = NULL;
	m_pd3dImmediateContext = NULL;
}
