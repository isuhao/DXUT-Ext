#include "SimplePrimitive.h"

FQuad::FQuad()
{
	// ������D3DDriver������֮�����New
	CHECK_D3D_VALID
}

void FQuad::Render()
{
}

TSharedPtr<FQuad> FQuad::Create()
{
	return TSharedPtr<FQuad>(new FQuad);
} 