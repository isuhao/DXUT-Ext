#ifndef _RHI_RENDER_TARGET_H_
#define _RHI_RENDER_TARGET_H_

#include "Core.h"
#include "RHIResources.h"

/** 
 * ¶ÔRTV¡¢DSV¡¢SRVµÄ·â×°
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

#endif