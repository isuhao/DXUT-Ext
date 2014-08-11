#ifndef _D3D_H_
#define _D3D_H_

#include "Core.h"
#include "D3DResources.h"
#include "D3DTranslate.h"
#include "D3DBuffer.h"
#include "D3DTexture.h"
#include "D3DRenderTarget.h"

#include "SystemSetting.h"

// 最大的RT数量
#define MAX_RT_COUNT	D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT
// 最大的ShaderResource数量
#define MAX_SR_COUNT	D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT 
// 最大的CB数量
#define MAX_CB_COUNT	D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT
// 最大的MRT数量
#define MAX_MRT_SLOT_COUNT	16


/** 
 * Render hardware interface
 */
class FD3D11Driver
{
public:
	FD3D11Driver();
	~FD3D11Driver();

	static void InitD3DDriver();
	static void ReleaseD3DDriver();

	ID3D11DeviceContext* GetDeviceContext();
	ID3D11Device* GetDevice();

#include "D3DMethods.h"

private:
	void InitPixelFormat();
	void InitConstantBuffers();

private:
	ID3D11DeviceContext*					m_pd3dImmediateContext;
	ID3D11Device*							m_pd3dDevice;

	FD3D11RenderTargetView*					m_pCurrRTVs[MAX_RT_COUNT];
	FD3D11DepthStencilView*					m_pCurrDSV;

	TArray<TSharedPtr<FConstantBuffer> >	m_ConstantBuffers[ST_NumShaderTypes];
};

extern FD3D11Driver* D3D;


#endif