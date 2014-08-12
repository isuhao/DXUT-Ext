#include "SimplePrimitive.h"


TSharedPtr<FQuad> FQuad::Create()
{
	return TSharedPtr<FQuad>(new FQuad);
}


FQuad::FQuad()
{
	// 必须在D3DDriver创建了之后才能New
	Check(D3D);


}