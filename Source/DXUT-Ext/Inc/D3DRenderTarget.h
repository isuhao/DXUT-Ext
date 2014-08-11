#ifndef _D3D_RENDER_TARGET_H_
#define _D3D_RENDER_TARGET_H_

#include "Core.h"
#include "D3DResources.h"

/** 
 * 对RTV、DSV、SRV的封装
 */
class FRenderSurface
{
public:
	TSharedPtr<FD3D11RenderTargetView>		RenderTargetView;
	TSharedPtr<FD3D11DepthStencilView>		DepthStencilView;
	TSharedPtr<FD3D11ShaderResourceView>	ShaderResourceView;
	TSharedPtr<FD3D11Texture2D>				SurfaceResource;

	FRenderSurface(
		const TSharedPtr<FD3D11RenderTargetView>& InRenderTargetView,
		const TSharedPtr<FD3D11DepthStencilView>& InDepthStencilView,
		const TSharedPtr<FD3D11ShaderResourceView>& InShaderResourceView,
		const TSharedPtr<FD3D11Texture2D>& InSurfaceResource
		)
		: RenderTargetView(InRenderTargetView)
		, DepthStencilView(InDepthStencilView)
		, ShaderResourceView(InShaderResourceView)
		, SurfaceResource(InSurfaceResource)
	{
	}

	FD3D11RenderTargetView*		GetRawRTV() { return RenderTargetView.get(); }
	FD3D11DepthStencilView*		GetRawDSV() { return DepthStencilView.get(); }
	FD3D11ShaderResourceView*	GetRawSRV() { return ShaderResourceView.get(); }
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