#ifndef _SIMPLE_PRIMITIVE_H_
#define _SIMPLE_PRIMITIVE_H_

#include "Mesh.h"


enum ESimplePrimitiveType
{
	SPT_Quad,
};



class FSimplePrimitive : public FMesh
{
public:
	FSimplePrimitive() : FMesh() {}
};


class FQuad : public FSimplePrimitive
{
public:
	FQuad();

	virtual void Render();

	static TSharedPtr<FQuad> Create();
};

#endif