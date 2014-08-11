#ifndef _D3D_TRANSLATE_H_
#define _D3D_TRANSLATE_H_

static D3D11_COMPARISON_FUNC TranslateCompareFunc(ECompareFunction CompareFunc)
{
	switch (CompareFunc)
	{
	case CF_Never: 
		return D3D11_COMPARISON_NEVER;
	case CF_Less: 
		return D3D11_COMPARISON_LESS;
	case CF_Equal: 
		return D3D11_COMPARISON_EQUAL;
	case CF_LessEqual: 
		return D3D11_COMPARISON_LESS_EQUAL;
	case CF_Greater: 
		return D3D11_COMPARISON_GREATER;
	case CF_NotEqual: 
		return D3D11_COMPARISON_NOT_EQUAL;
	case CF_GreaterEqual: 
		return D3D11_COMPARISON_GREATER_EQUAL;
	case CF_Always: 
		return D3D11_COMPARISON_ALWAYS;
	default: 
		return D3D11_COMPARISON_NEVER;
	}
}

static D3D11_TEXTURE_ADDRESS_MODE TranslateSamplerAddressMode(ESamplerAddressMode AddressMode)
{
	switch (AddressMode)
	{
	case AM_Clamp:
		return D3D11_TEXTURE_ADDRESS_CLAMP;
	case AM_Mirror:
		return D3D11_TEXTURE_ADDRESS_MIRROR;
	case AM_Border:
		return D3D11_TEXTURE_ADDRESS_BORDER;
	default:
		return D3D11_TEXTURE_ADDRESS_WRAP;
	};
}

static D3D11_FILL_MODE TranslateRasterizeFillMode(ERasterizerFillMode FillMode)
{
	switch (FillMode)
	{
	case FM_Solid: 
		return D3D11_FILL_SOLID;
	default: 
		return D3D11_FILL_WIREFRAME;
	};
}

static D3D11_CULL_MODE TranslateRasterizeCullMode(ERasterizerCullMode CullMode)
{
	switch (CullMode)
	{
	case CM_CCW: 
		return D3D11_CULL_BACK;
	case CM_CW: 
		return D3D11_CULL_FRONT;
	default: 
		return D3D11_CULL_NONE;
	};
}

static D3D11_BLEND_OP TranslateBlendOp(EBlendOperation BlendOp)
{
	switch (BlendOp)
	{
	case BO_Subtract: 
		return D3D11_BLEND_OP_SUBTRACT;
	case BO_Min: 
		return D3D11_BLEND_OP_MIN;
	case BO_Max: 
		return D3D11_BLEND_OP_MAX;
	case BO_ReverseSubtract: 
		return D3D11_BLEND_OP_REV_SUBTRACT;
	default: 
		return D3D11_BLEND_OP_ADD;
	};
}

static D3D11_BLEND TranslateBlendFactor(EBlendFactor BlendFactor)
{
	switch (BlendFactor)
	{
	case BF_One: 
		return D3D11_BLEND_ONE;
	case BF_SourceColor: 
		return D3D11_BLEND_SRC_COLOR;
	case BF_InverseSourceColor: 
		return D3D11_BLEND_INV_SRC_COLOR;
	case BF_SourceAlpha: 
		return D3D11_BLEND_SRC_ALPHA;
	case BF_InverseSourceAlpha: 
		return D3D11_BLEND_INV_SRC_ALPHA;
	case BF_DestAlpha: 
		return D3D11_BLEND_DEST_ALPHA;
	case BF_InverseDestAlpha:
		return D3D11_BLEND_INV_DEST_ALPHA;
	case BF_DestColor: 
		return D3D11_BLEND_DEST_COLOR;
	case BF_InverseDestColor:
		return D3D11_BLEND_INV_DEST_COLOR;
	default: 
		return D3D11_BLEND_ZERO;
	};
}

static D3D11_COLOR_WRITE_ENABLE TranslateBlendColorWriteEnable(EBlendColorWriteEnable ColorWriteEnable)
{
	switch (ColorWriteEnable)
	{
	case CWE_None: 
		return D3D11_COLOR_WRITE_ENABLE(0);
	case CWE_Red: 
		return D3D11_COLOR_WRITE_ENABLE_RED;
	case CWE_Green:
		return D3D11_COLOR_WRITE_ENABLE_GREEN;
	case CWE_Blue:
		return D3D11_COLOR_WRITE_ENABLE_BLUE;
	case CWE_Alpha: 
		return D3D11_COLOR_WRITE_ENABLE_ALPHA;
	default: 
		return D3D11_COLOR_WRITE_ENABLE_ALL;
	}
}

#endif