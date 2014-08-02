#ifndef _DL_SHADERS_H_
#define _DL_SHADERS_H_

#include "Shader.h"

class DLGPassVertexShader : public Shader
{
	DECLARE_SHADER(DLGPassVertexShader, L"GPass.hlsl", "VS_Main", "vs_5_0", EST_VertexShader)

public:
	void InitVertexDeclaration();
};

class DLGPassPixelShader : public Shader
{
	DECLARE_SHADER(DLGPassPixelShader, L"GPass.hlsl", "PS_Main", "vs_5_0", EST_PixelShader)
};

#endif