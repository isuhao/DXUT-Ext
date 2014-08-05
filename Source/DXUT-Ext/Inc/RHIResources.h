#ifndef _RHI_RESOURCES_H_
#define _RHI_RESOURCES_H_

#include "Core.h"

// 前向声明
PRE_DECLARE_CLASS(FVertexDeclaration)


// D3D资源
typedef ID3D11SamplerState		FRHISamplerState;
typedef ID3D11RasterizerState	FRHIRasterState;
typedef ID3D11BlendState		FRHIBlendState;
typedef ID3D11InputLayout		FRHIInputLayout;
typedef ID3D11Buffer			FRHIBuffer;
typedef ID3D11VertexShader		FRHIVertexShader;
typedef ID3D11PixelShader		FRHIPixelShader;
typedef ID3D11DomainShader		FRHIDomainShader;
typedef ID3D11HullShader		FRHIHullShader;
typedef ID3D11GeometryShader	FRHIGeometryShader;
typedef ID3D11ComputeShader		FRHIComputeShader;


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
	SF_Point,
	SF_Bilinear,
	SF_Trilinear,
	SF_AnisotropicPoint,
	SF_AnisotropicLinear,
};

enum ESamplerAddressMode
{
	AM_Warp,
	AM_Clamp,
	AM_Mirror,
	AM_Border,
};

enum ECompareFunction
{
	CF_Never,
	CF_Less,
	CF_Equal,
	CF_LessEqual,
	CF_Greater,
	CF_NotEqual,
	CF_GreaterEqual,
	CF_Always,
};

enum EPrimitiveType
{
	PT_TriangleList,		
	PT_TriangleStrip,
	PT_LineList,
	PT_QuadList,
};

enum ERasterizerFillMode
{
	FM_Wireframe,
	FM_Solid
};

enum ERasterizerCullMode
{
	CM_None,
	CM_CW,
	CM_CCW
};

enum EBlendOperation
{
	BO_Add,
	BO_Subtract,
	BO_Min,
	BO_Max,
	BO_ReverseSubtract,
};

enum EBlendFactor
{
	BF_Zero,
	BF_One,
	BF_SourceColor,
	BF_InverseSourceColor,
	BF_SourceAlpha,
	BF_InverseSourceAlpha,
	BF_DestAlpha,
	BF_InverseDestAlpha,
	BF_DestColor,
	BF_InverseDestColor,
};

enum EBlendColorWriteEnable
{
	CWE_None,
	CWE_Red,
	CWE_Green,
	CWE_Blue,
	CWE_Alpha,
	CWE_All
};


#include "RHIBuffer.h"

#endif