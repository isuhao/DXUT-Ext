#include "SimplePrimitive.h"


TSharedPtr<FQuad> FQuad::Create()
{
	return TSharedPtr<FQuad>(new FQuad);
}


FQuad::FQuad()
{
	// ������D3DDriver������֮�����New
	Check(D3D);


}