#ifndef _RHI_H_
#define _RHI_H_

#include "Core.h"
#include "RHIResources.h"
#include "RHITranslate.h"

#include "RHIBuffer.h"
#include "RHITexture.h"

// 最大的RT数量
#define MAX_RT_COUNT	D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT

/** 
 * Render hardware interface
 */
class FDynamicRHI
{
public:
	FDynamicRHI();
	~FDynamicRHI();

	static void InitRHI();
	static void ReleaseRHI();

	ID3D11DeviceContext* GetDeviceContext();
	ID3D11Device* GetDevice();

#include "RHIMethods.h"

private:
	ID3D11DeviceContext*					m_pd3dImmediateContext;
	ID3D11Device*							m_pd3dDevice;

	FRHIRenderTargetView*					m_pCurrRTVs[MAX_RT_COUNT];
	FRHIDepthStencilView*					m_pCurrDSV;
};

extern FDynamicRHI* RHI;

#endif