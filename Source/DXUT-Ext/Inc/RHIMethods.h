#include "Core.h"
#include "RHIResources.h"

#ifndef DEFINE_RHIMETHOD
#define DEFINE_RHIMETHOD(Type,Name,Parameters) Type Name Parameters
#endif

DEFINE_RHIMETHOD(
	TSharedPtr<FRHIBoundShaderState>,
	CreateBoundShaderState,
	(TSharedPtr<FVertexDeclaration> VertexDeclaration, TSharedPtr<ID3DBlob> BindCode, TSharedPtr<ID3D11VertexShader> VS, TSharedPtr<ID3D11PixelShader> PS, TSharedPtr<ID3D11DomainShader> DS, TSharedPtr<ID3D11HullShader> HS, TSharedPtr<ID3D11GeometryShader> GS, TSharedPtr<ID3D11ComputeShader> CS)
	);

DEFINE_RHIMETHOD(
	TSharedPtr<FRHIBoundShaderState>,
	CreateBoundShaderState,
	(TSharedPtr<FVertexDeclaration> VertexDeclaration, TSharedPtr<ID3DBlob> BindCode, TSharedPtr<ID3D11VertexShader> VS, TSharedPtr<ID3D11PixelShader> PS)
	);

DEFINE_RHIMETHOD(
	void,
	SetBoundShaderState,
	(TSharedPtr<FRHIBoundShaderState> BoundShaderState)
	);

DEFINE_RHIMETHOD(
	TSharedPtr<FRHISamplerState>,
	CreateSamplerState,
	(ESamplerFilter Filter, ESamplerAddressMode	AddressU = AM_Warp, ESamplerAddressMode AddressV = AM_Warp, ESamplerAddressMode AddressW = AM_Warp, float MipBias = 0.f, UINT MaxAnisotropy = 0, FLinearColor BorderColor = FLinearColor(), ECompareFunction CompareFunc = CF_Never)
	);

DEFINE_RHIMETHOD(
	void,
	PSSetSamplerState,
	(int SamplerIndex, const TSharedPtr<FRHISamplerState>& SamplerState)
	);

DEFINE_RHIMETHOD(
	void,
	VSSetSamplerState,
	(int SamplerIndex, const TSharedPtr<FRHISamplerState>& SamplerState)
	);

DEFINE_RHIMETHOD(
	void,
	SetVertexBuffer,
	(int BufferIndex, const TSharedPtr<FRHIBuffer>& VertexBuffer, UINT Stride)
	);

DEFINE_RHIMETHOD(
	void,
	SetIndexBuffer,
	(int BufferIndex, const TSharedPtr<FRHIBuffer>& IndexBuffer, DXGI_FORMAT Format)
	);

DEFINE_RHIMETHOD(
	void,
	DrawIndexedPrimitive,
	(const TSharedPtr<FRHIBuffer>& IndexBuffer, EPrimitiveType PrimitiveType)
	);

DEFINE_RHIMETHOD(
	TSharedPtr<FRHIRasterState>,
	CreateRasterizerState,
	(ERasterizerFillMode FillMode = FM_Solid, ERasterizerCullMode CullMode = CM_CCW, bool bAllowMSAA = true, bool bFrontCounterClockwise = false, INT DepthBias = 0, float SlopeScaledDepthBias = 0)
	);

DEFINE_RHIMETHOD(
	void,
	SetRasterizerState,
	(const TSharedPtr<FRHIRasterState>& RasterizerState)
	);

DEFINE_RHIMETHOD(
	TSharedPtr<FRHIBlendState>,
	CreateBlendState,
	(EBlendOperation ColorBlendOp = BO_Add, EBlendFactor ColorSrcBlend = BF_One, EBlendFactor ColorDstBlend = BF_Zero, EBlendOperation AlphaBlendOp = BO_Add, EBlendFactor AlphaSrcBlend = BF_One, EBlendFactor AlphaDstBlend = BF_Zero, EBlendColorWriteEnable ColorWriteEnable = CWE_All)
	);

DEFINE_RHIMETHOD(
	void,
	SetBlendState,
	(const TSharedPtr<FRHIBlendState>& BlendState, FLinearColor Color = FLinearColor(), UINT SimpleMask = 0xffffffff)
	);