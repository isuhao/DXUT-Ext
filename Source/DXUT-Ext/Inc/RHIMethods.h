#include "Core.h"

#ifndef DEFINE_RHIMETHOD
#define DEFINE_RHIMETHOD(Type,Name,Parameters) Type Name Parameters
#endif

DEFINE_RHIMETHOD(
	void,
	SetBoundShaderState,
	(TSharedPtr<ID3D11InputLayout> InputLayout, TSharedPtr<ID3D11VertexShader> VS, TSharedPtr<ID3D11PixelShader> PS, TSharedPtr<ID3D11DomainShader> DS, TSharedPtr<ID3D11HullShader> HS, TSharedPtr<ID3D11GeometryShader> GS, TSharedPtr<ID3D11ComputeShader> CS)
	);