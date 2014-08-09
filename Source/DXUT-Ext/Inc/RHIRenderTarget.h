#ifndef _RHI_RENDER_TARGET_H_
#define _RHI_RENDER_TARGET_H_

#include "Core.h"
#include "RHIResources.h"

/** 
 * 对RTV、DSV、SRV的封装
 */
class FRenderSurface
{
public:
	TSharedPtr<FRHIRenderTargetView>		RenderTargetView;
	TSharedPtr<FRHIDepthStencilView>		DepthStencilView;
	TSharedPtr<FRHIShaderResourceView>		ShaderResourceView;
	TSharedPtr<FRHITexture2D>				SurfaceResource;

	FRenderSurface(
		const TSharedPtr<FRHIRenderTargetView>& InRenderTargetView,
		const TSharedPtr<FRHIDepthStencilView>& InDepthStencilView,
		const TSharedPtr<FRHIShaderResourceView>& InShaderResourceView,
		const TSharedPtr<FRHITexture2D>& InSurfaceResource
		)
		: RenderTargetView(InRenderTargetView)
		, DepthStencilView(InDepthStencilView)
		, ShaderResourceView(InShaderResourceView)
		, SurfaceResource(InSurfaceResource)
	{
	}

	FRHIRenderTargetView*		GetRawRTV() { return RenderTargetView.get(); }
	FRHIDepthStencilView*		GetRawDSV() { return DepthStencilView.get(); }
	FRHIShaderResourceView*		GetRawSRV() { return ShaderResourceView.get(); }
};

/** 
 * 对RenderTarget、DepthTarget的封装
 */
class FFrameBuffer
{
public:
	TSharedPtr<FRenderSurface>		RenderTarget;
	TSharedPtr<FRenderSurface>		DepthTarget;

	FFrameBuffer(const TSharedPtr<FRenderSurface>& InRT, const TSharedPtr<FRenderSurface>& InDT)
		: RenderTarget(InRT)
		, DepthTarget(InDT)
	{
	}
};

#endif