#ifndef _RHI_RESOURCES_H_
#define _RHI_RESOURCES_H_

#include "Core.h"

PRE_DECLARE_CLASS(FVertexDeclaration)


struct FBoundShaderState
{
	TSharedPtr<ID3D11InputLayout>		InputLayouts;
	TSharedPtr<ID3D11VertexShader>		VertexShader;
	TSharedPtr<ID3D11PixelShader>		PixelShader;
	TSharedPtr<ID3D11DomainShader>		DomainShader;
	TSharedPtr<ID3D11HullShader>		HullShader;
	TSharedPtr<ID3D11GeometryShader>	GeometryShader;
	TSharedPtr<ID3D11ComputeShader>		ComputeShader;
};


#endif