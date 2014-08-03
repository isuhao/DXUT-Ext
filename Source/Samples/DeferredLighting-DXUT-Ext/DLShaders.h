#ifndef _DL_SHADERS_H_
#define _DL_SHADERS_H_

#include "Shader.h"

class GPassVertexShader : public FShader
{
	DECLARE_SHADER(GPassVertexShader, L"GPass.hlsl", "VS_Main", "vs_5_0", EST_VertexShader)

	void InitVertexDeclaration();
};

class GPassPixelShader : public FShader
{
	DECLARE_SHADER(GPassPixelShader, L"GPass.hlsl", "PS_Main", "ps_5_0", EST_PixelShader)
};

class DLVertexShader : public FShader
{
	DECLARE_SHADER(DLVertexShader, L"DeferredLight.hlsl", "VS_Main", "vs_5_0", EST_VertexShader)

	void InitVertexDeclaration();
};

class DLPixelShader : public FShader
{
	DECLARE_SHADER(DLPixelShader, L"DeferredLight.hlsl", "PS_Main", "ps_5_0", EST_PixelShader)
};

class SceneVertexShader : public FShader
{
	DECLARE_SHADER(SceneVertexShader, L"ScenePass.hlsl", "RenderSceneVS", "vs_5_0", EST_VertexShader)

	void InitVertexDeclaration();
};

class ScenePixelShader : public FShader
{
	DECLARE_SHADER(ScenePixelShader, L"ScenePass.hlsl", "RenderScenePS", "ps_5_0", EST_PixelShader)
};

struct FVertexDeclarationFactory_DL : public FVertexDeclarationFactory
{
	FVertexDeclarationFactory_DL()
	{
		VertexDecls = TSharedPtr<FVertexDeclaration>(new FVertexDeclaration());
		VertexDecls->AppendElement(0, 0, VET_Float3, VEU_Position, 0);
		VertexDecls->AppendElement(0, 12, VET_Float3, VEU_Normal, 0);
		VertexDecls->AppendElement(0, 24, VET_Float2, VEU_TextureCoordinate, 0);
	}
};


#endif