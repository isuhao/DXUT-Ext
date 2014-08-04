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
	void,
	SetBoundShaderState,
	(TSharedPtr<FRHIBoundShaderState> BoundShaderState)
	);

DEFINE_RHIMETHOD(
	TSharedPtr<FRHISamplerState>,
	CreateSamplerState,
	(ESamplerFilter Filter, ESamplerAddressMode	AddressU = ESAM_Warp, ESamplerAddressMode AddressV = ESAM_Warp, ESamplerAddressMode AddressW = ESAM_Warp, float MipBias = 0.f, UINT MaxAnisotropy = 0, FLinearColor BorderColor = FLinearColor(), ESamplerCompareFunction CompareFunc = ESCF_Never)
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