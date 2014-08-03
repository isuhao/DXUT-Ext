#include "DLShaders.h"

struct DLVertexDeclarationFactory
{
	DLVertexDeclarationFactory()
	{
		VertexDecls = TSharedPtr<VertexDeclaration>(new VertexDeclaration());
		VertexDecls->AppendElementFast(0, VET_Float4, VEU_Position,			 0);
		VertexDecls->AppendElementFast(0, VET_Float3, VEU_Normal,			 0);
		VertexDecls->AppendElementFast(0, VET_Float2, VEU_TextureCoordinate, 0);
	}

	TSharedPtr<VertexDeclaration> VertexDecls;
}; 

DLVertexDeclarationFactory DLVDFactory;


SET_SHADER_VERT_DECL(GPassVertexShader, DLVDFactory.VertexDecls);
SET_SHADER_VERT_DECL(DLVertexShader,	DLVDFactory.VertexDecls);
SET_SHADER_VERT_DECL(SceneVertexShader, DLVDFactory.VertexDecls);
