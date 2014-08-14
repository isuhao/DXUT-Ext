#include "SimplePrimitive.h"

FQuad::FQuad()
{
	// 必须在D3DDriver创建了之后才能New
	CHECK_D3D_VALID
}

void FQuad::Render()
{
}

TSharedPtr<FQuad> FQuad::Create()
{
	return TSharedPtr<FQuad>(new FQuad);
} 