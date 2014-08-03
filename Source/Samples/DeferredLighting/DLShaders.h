#ifndef _DL_SHADERS_H_
#define _DL_SHADERS_H_

#include "Shader.h"

class GPassVertexShader : public Shader
{
	DECLARE_SHADER(GPassVertexShader, L"GPass.hlsl", "VS_Main", "vs_5_0", EST_VertexShader)

	void InitVertexDeclaration();
};

class GPassPixelShader : public Shader
{
	DECLARE_SHADER(GPassPixelShader, L"GPass.hlsl", "PS_Main", "ps_5_0", EST_PixelShader)
};

class DLVertexShader : public Shader
{
	DECLARE_SHADER(DLVertexShader, L"DeferredLight.hlsl", "VS_Main", "vs_5_0", EST_VertexShader)

	void InitVertexDeclaration();
};

class DLPixelShader : public Shader
{
	DECLARE_SHADER(DLPixelShader, L"DeferredLight.hlsl", "PS_Main", "ps_5_0", EST_PixelShader)
};

class SceneVertexShader : public Shader
{
	DECLARE_SHADER(SceneVertexShader, L"ScenePass.hlsl", "RenderSceneVS", "vs_5_0", EST_VertexShader)

	void InitVertexDeclaration();
};

class ScenePixelShader : public Shader
{
	DECLARE_SHADER(ScenePixelShader, L"ScenePass.hlsl", "RenderScenePS", "ps_5_0", EST_PixelShader)
};

#endif