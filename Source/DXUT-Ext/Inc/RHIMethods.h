#include "Core.h"
#include "RHIResources.h"

#ifndef DEFINE_RHIMETHOD
#define DEFINE_RHIMETHOD(Type,Name,Parameters) Type Name Parameters
#endif

DEFINE_RHIMETHOD(
	TSharedPtr<FBoundShaderState>,
	CreateBoundShaderState,
	(TSharedPtr<FVertexDeclaration> VertexDeclaration, TSharedPtr<ID3DBlob> BindCode, TSharedPtr<ID3D11VertexShader> VS, TSharedPtr<ID3D11PixelShader> PS, TSharedPtr<ID3D11DomainShader> DS, TSharedPtr<ID3D11HullShader> HS, TSharedPtr<ID3D11GeometryShader> GS, TSharedPtr<ID3D11ComputeShader> CS)
	);

DEFINE_RHIMETHOD(
	void,
	SetBoundShaderState,
	(TSharedPtr<FBoundShaderState> BoundShaderState)
	);