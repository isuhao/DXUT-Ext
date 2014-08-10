#ifndef _RHI_H_
#define _RHI_H_

#include "Core.h"
#include "RHIResources.h"
#include "RHITranslate.h"
#include "RHIBuffer.h"
#include "RHITexture.h"
#include "RHIRenderTarget.h"

#include "SystemSetting.h"

// 最大的RT数量
#define MAX_RT_COUNT	D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT
// 最大的ShaderResource数量
#define MAX_SR_COUNT	D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT 
// 最大的CB数量
#define MAX_CB_COUNT	D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT

class FNullRHI;

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
	void InitPixelFormat();
	void InitConstantBuffers();

private:
	ID3D11DeviceContext*					m_pd3dImmediateContext;
	ID3D11Device*							m_pd3dDevice;

	FRHIRenderTargetView*					m_pCurrRTVs[MAX_RT_COUNT];
	FRHIDepthStencilView*					m_pCurrDSV;

	TArray<TSharedPtr<FConstantBuffer> >	m_ConstantBuffers[ST_NumShaderTypes];
};

extern FDynamicRHI* RHI;


#endif