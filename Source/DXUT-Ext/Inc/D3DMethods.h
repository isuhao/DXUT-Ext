#include "Core.h"
#include "D3DResources.h"

#ifndef DEFINE_D3DMETHOD
#define DEFINE_D3DMETHOD(Type,Name,Parameters) Type Name Parameters
#endif

DEFINE_D3DMETHOD(
	TSharedPtr<FD3D11BoundShaderState>,
	CreateBoundShaderState,
	(TSharedPtr<FVertexDeclaration> VertexDeclaration, TSharedPtr<ID3DBlob> BindCode, TSharedPtr<ID3D11VertexShader> VS, TSharedPtr<ID3D11PixelShader> PS, TSharedPtr<ID3D11DomainShader> DS, TSharedPtr<ID3D11HullShader> HS, TSharedPtr<ID3D11GeometryShader> GS, TSharedPtr<ID3D11ComputeShader> CS)
	);

DEFINE_D3DMETHOD(
	TSharedPtr<FD3D11BoundShaderState>,
	CreateBoundShaderState,
	(TSharedPtr<FVertexDeclaration> VertexDeclaration, TSharedPtr<ID3DBlob> BindCode, TSharedPtr<ID3D11VertexShader> VS, TSharedPtr<ID3D11PixelShader> PS)
	);

DEFINE_D3DMETHOD(
	void,
	SetBoundShaderState,
	(TSharedPtr<FD3D11BoundShaderState> BoundShaderState)
	);

DEFINE_D3DMETHOD(
	TSharedPtr<FD3D11SamplerState>,
	CreateSamplerState,
	(ESamplerFilter Filter, ESamplerAddressMode	AddressU = AM_Warp, ESamplerAddressMode AddressV = AM_Warp, ESamplerAddressMode AddressW = AM_Warp, float MipBias = 0.f, UINT MaxAnisotropy = 0, FLinearColor BorderColor = FLinearColor(), ECompareFunction CompareFunc = CF_Never)
	);

DEFINE_D3DMETHOD(
	void,
	SetSamplerState,
	(EShaderType ShaderType, int SamplerIndex, const TSharedPtr<FD3D11SamplerState>& SamplerState)
	);

DEFINE_D3DMETHOD(
	void,
	SetVertexBuffer,
	(int BufferIndex, const TSharedPtr<FD3D11Buffer>& VertexBuffer, UINT Stride)
	);

DEFINE_D3DMETHOD(
	void,
	SetIndexBuffer,
	(int BufferIndex, const TSharedPtr<FD3D11Buffer>& IndexBuffer, DXGI_FORMAT Format)
	);

DEFINE_D3DMETHOD(
	void,
	DrawIndexedPrimitive,
	(const TSharedPtr<FD3D11Buffer>& IndexBuffer, EPrimitiveType PrimitiveType)
	);

DEFINE_D3DMETHOD(
	TSharedPtr<FD3D11RasterState>,
	CreateRasterizerState,
	(ERasterizerFillMode FillMode = FM_Solid, ERasterizerCullMode CullMode = CM_CCW, bool bAllowMSAA = true, bool bFrontCounterClockwise = false, INT DepthBias = 0, float SlopeScaledDepthBias = 0)
	);

DEFINE_D3DMETHOD(
	void,
	SetRasterizerState,
	(const TSharedPtr<FD3D11RasterState>& RasterizerState)
	);

DEFINE_D3DMETHOD(
	TSharedPtr<FD3D11BlendState>,
	CreateBlendState,
	(EBlendOperation ColorBlendOp = BO_Add, EBlendFactor ColorSrcBlend = BF_One, EBlendFactor ColorDstBlend = BF_Zero, EBlendOperation AlphaBlendOp = BO_Add, EBlendFactor AlphaSrcBlend = BF_One, EBlendFactor AlphaDstBlend = BF_Zero, EBlendColorWriteEnable ColorWriteEnable = CWE_All)
	);

DEFINE_D3DMETHOD(
	void,
	SetBlendState,
	(const TSharedPtr<FD3D11BlendState>& BlendState, FLinearColor Color = FLinearColor(), UINT SimpleMask = 0xffffffff)
	);

//DEFINE_D3DMETHOD(
//	TSharedPtr<FD3D11Buffer>,
//	CreateBuffer,
//	()
//	);
//
//DEFINE_D3DMETHOD(
//	TSharedPtr<FD3D11DepthStencilView>,
//	CreateDepthStencilView,
//	()
//	);

DEFINE_D3DMETHOD(
	TSharedPtr<FTexture2D>,
	CreateTexture2D,
	(uint Width, uint Height, EPixelFormat PixFormat, uint CreateFlag = 0, uint NumMips = 1)
	);

DEFINE_D3DMETHOD(
	void,
	SetRenderTarget,
	(const TSharedPtr<FRenderSurface>& RTSurface, const TSharedPtr<FRenderSurface>& DepthSurface)
	);

DEFINE_D3DMETHOD(
	void,
	SetMRTRenderTarget,
	(const TSharedPtr<FRenderSurface>& RenderTarget, uint RTIndex)
	);

DEFINE_D3DMETHOD(
	TSharedPtr<FRenderSurface>,
	CreateRenderSurface,
	(uint Width, uint Height, EPixelFormat PixFormat, TSharedPtr<FTexture2D> TextureToAttach = NULL)
	);

DEFINE_D3DMETHOD(
	TSharedPtr<FRenderSurface>,
	CreateDepthTargetSurface,
	(uint Width, uint Height, TSharedPtr<FTexture2D> TextureToAttach = NULL)
	);

DEFINE_D3DMETHOD(
	TSharedPtr<FRenderSurface>,
	CreateRenderTargetSurface,
	(uint Width, uint Height, EPixelFormat PixFormat, TSharedPtr<FTexture2D> TextureToAttach = NULL)
	);

DEFINE_D3DMETHOD(
	TSharedPtr<FFrameBuffer>,
	CreateFrameBuffer,
	(uint Width, uint Height, EPixelFormat PixFormat)
	);

DEFINE_D3DMETHOD(
	void,
	SetFrameBuffer,
	(const TSharedPtr<FFrameBuffer>& FrameBuffer)
	);

DEFINE_D3DMETHOD(
	void,
	SetMRTFrameBuffer,
	(const TSharedPtr<FFrameBuffer>& FrameBuffer, uint RTIndex)
	);


DEFINE_D3DMETHOD(
	void,
	ClearColor,
	(FLinearColor Color = FLinearColor::Black, uint RTIndex = 0)
	);

DEFINE_D3DMETHOD(
	void,
	ClearDepth,
	(float fDepth)
	);

DEFINE_D3DMETHOD(
	void,
	ClearStencil,
	(uint iStencil)
	);

DEFINE_D3DMETHOD(
	void,
	Clear,
	(bool bClearColor, FLinearColor Color, bool bClearDepth, float fDepth, bool bClearStencil, uint iStencil, bool bClearAllTargets = false)
	);

DEFINE_D3DMETHOD(
	void,
	SetDefaultBackBuffer,
	()
	);

DEFINE_D3DMETHOD(
	void,
	SetShaderConstantVariable,
	(EShaderType ShaderType, uint BufferIndex, uint BindIndex, uint NumBytes, byte* Data)
	);

DEFINE_D3DMETHOD(
	void,
	SetShaderResourceVariable,
	(EShaderType ShaderType, uint BindIndex, const TSharedPtr<FD3D11SamplerState>& Sampler)
	);

DEFINE_D3DMETHOD(
	void,
	SetShaderResourceVariable,
	(EShaderType ShaderType, uint BindIndex, const TSharedPtr<FD3D11ShaderResourceView>& SRV)
	);

DEFINE_D3DMETHOD(
	void,
	CommitConstantBuffer,
	(void)
	);

DEFINE_D3DMETHOD(
	void,
	BeginRender,
	()
	);

DEFINE_D3DMETHOD(
	void,
	EndRender,
	()
	);

DEFINE_D3DMETHOD(
	void,
	GetBackBufferDesc,
	(uint& OutWidth, uint OutHeight)
	);