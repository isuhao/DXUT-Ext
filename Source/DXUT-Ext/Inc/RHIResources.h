#ifndef _RHI_RESOURCES_H_
#define _RHI_RESOURCES_H_

#include "Core.h"

// D3D×ÊÔ´
typedef ID3D11SamplerState	FRHISamplerState;
typedef ID3D11InputLayout	FRHIInputLayout;
typedef ID3D11Buffer		FRHIBuffer;
typedef ID3D11VertexShader	FRHIVertexShader;
typedef ID3D11PixelShader	FRHIPixelShader;
typedef ID3D11DomainShader	FRHIDomainShader;

PRE_DECLARE_CLASS(FVertexDeclaration)

struct FRHIBoundShaderState
{
	TSharedPtr<ID3D11InputLayout>		InputLayouts;
	TSharedPtr<ID3D11VertexShader>		VertexShader;
	TSharedPtr<ID3D11PixelShader>		PixelShader;
	TSharedPtr<ID3D11DomainShader>		DomainShader;
	TSharedPtr<ID3D11HullShader>		HullShader;
	TSharedPtr<ID3D11GeometryShader>	GeometryShader;
	TSharedPtr<ID3D11ComputeShader>		ComputeShader;
};

enum ESamplerFilter
{
	ESF_Point,
	ESF_Bilinear,
	ESF_Trilinear,
	ESF_AnisotropicPoint,
	ESF_AnisotropicLinear,
};

enum ESamplerAddressMode
{
	ESAM_Warp,
	ESAM_Clamp,
	ESAM_Mirror,
	ESAM_Border,
};

enum ESamplerCompareFunction
{
	ESCF_Never,
	ESCF_Less,
	ESCF_Equal,
	ESCF_LessEqual,
	ESCF_Greater,
	ESCF_NotEqual,
	ESCF_GreaterEqual,
	ESCF_Always,
};

enum EPrimitiveType
{
	EPT_TriangleList,		
	EPT_TriangleStrip,
	EPT_LineList,
	EPT_QuadList,
};

#endif