#include "RHIRenderTarget.h"
#include "RHI.h"


void FDynamicRHI::SetRenderTarget(const TSharedPtr<FRenderSurface>& NewRenderTarget, const TSharedPtr<FRenderSurface>& NewDepthStencil)
{
	FRHIRenderTargetView* pNewRTV = NULL;
	FRHIDepthStencilView* pNewDSV = NULL;

	if (NewRenderTarget)
	{
		pNewRTV = NewRenderTarget->GetRawRTV();
	}
	if (NewDepthStencil)
	{
		pNewDSV = NewDepthStencil->GetRawDSV();
	}

	bool bMRT = false;
	for (int i = 1; i < MAX_RT_COUNT; ++i)
	{
		if (m_pCurrRTVs[i] != NULL)
		{
			bMRT = true;
			break;
		}
	}

	if (m_pCurrDSV != pNewDSV || m_pCurrRTVs[0] != pNewRTV)
	{
		m_pCurrDSV		= pNewDSV;
		m_pCurrRTVs[0]	= pNewRTV;

		m_pd3dImmediateContext->OMSetRenderTargets(MAX_RT_COUNT, &m_pCurrRTVs[0], m_pCurrDSV);
	}
}

void FDynamicRHI::SetMRTRenderTarget(const TSharedPtr<FRenderSurface>& NewRenderTarget, uint RenderTargetIndex)
{
	FRHIRenderTargetView* pNewRTV = NULL;
	if (NewRenderTarget)
	{
		pNewRTV = NewRenderTarget->GetRawRTV();
	}

	if (m_pCurrRTVs[RenderTargetIndex] != pNewRTV)
	{
		m_pCurrRTVs[RenderTargetIndex] = pNewRTV;

		m_pd3dImmediateContext->OMSetRenderTargets(MAX_RT_COUNT, &m_pCurrRTVs[0], m_pCurrDSV);
	}
}

TSharedPtr<FRenderSurface> FDynamicRHI::CreateRenderSurface(uint Width, uint Height, uint Format, TSharedPtr<FTexture2D> TextureToAttach)
{
	return TSharedPtr<FRenderSurface>();
}