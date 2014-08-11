#ifndef _DL_SHADERS_H_
#define _DL_SHADERS_H_

#include "Shader.h"

class FGPassVertexShader : public FShader
{
	DECLARE_SHADER(FGPassVertexShader, L"GPassExt.hlsl", "VS_Main", "vs_5_0", ST_VertexShader)

	void BindVariables()
	{
		m_mWVP.Bind(m_VariableMap, "g_mWVP");
		m_mWorld.Bind(m_VariableMap, "g_mWorld");
	}

	void SetVariables(float4x4& InWVP, float4x4& InWolrd)
	{
		SetShaderVariable(ST_VertexShader, m_mWVP, &InWVP);
		SetShaderVariable(ST_VertexShader, m_mWorld, &InWolrd);
	}

private:
	FShaderConstantVarialble	m_mWVP;
	FShaderConstantVarialble	m_mWorld;
};

class FGPassPixelShader : public FShader
{
	DECLARE_SHADER(FGPassPixelShader, L"GPassExt.hlsl", "PS_Main", "ps_5_0", ST_PixelShader)
};

class FDLVertexShader : public FShader
{
	DECLARE_SHADER(FDLVertexShader, L"DeferredLightExt.hlsl", "VS_Main", "vs_5_0", ST_VertexShader)
};

class FDLPixelShader : public FShader
{
	DECLARE_SHADER(FDLPixelShader, L"DeferredLightExt.hlsl", "PS_Main", "ps_5_0", ST_PixelShader)

	void BindVariables()
	{
		m_vLightVec.Bind(m_VariableMap, "g_vLightDir");
		m_txNormal.Bind(m_VariableMap, "g_txNormal");
		m_Sampler.Bind(m_VariableMap, "g_samPointClamp");
	}

	void SetVariables(float3& LightVec)
	{
		SetShaderVariable(ST_PixelShader, m_vLightVec, &LightVec);
	}

	void SetTextureVariables(const TSharedPtr<FRenderSurface>& NormalTexSurf, const TSharedPtr<FD3D11SamplerState>& Sampler)
	{
		SetShaderVariable(ST_PixelShader, m_txNormal, NormalTexSurf->ShaderResourceView);
		SetShaderVariable(ST_PixelShader, m_Sampler, Sampler);
	}

private:
	FShaderConstantVarialble	m_vLightVec;
	FShaderResourceVariable		m_txNormal;
	FShaderResourceVariable		m_Sampler;
};

class FSceneVertexShader : public FShader
{
	DECLARE_SHADER(FSceneVertexShader, L"ScenePassExt.hlsl", "RenderSceneVS", "vs_5_0", ST_VertexShader)

	void BindVariables()
	{
		m_mWVP.Bind(m_VariableMap, "g_mWVP");
	}

	void SetVariables(float4x4& WVP)
	{
		SetShaderVariable(ST_PixelShader, m_mWVP, &WVP);
	}

private:
	FShaderConstantVarialble	m_mWVP;
};

class FScenePixelShader : public FShader
{
	DECLARE_SHADER(FScenePixelShader, L"ScenePassExt.hlsl", "RenderScenePS", "ps_5_0", ST_PixelShader)

	void BindVariables()
	{
		m_AmbientColor.Bind(m_VariableMap, "g_AmbientColor");
		m_LightDiffuse.Bind(m_VariableMap, "g_LightDiffuse");
		m_txDiffuseAlbedo.Bind(m_VariableMap, "g_txDiffuseAlbedo");
		m_samPointClamp.Bind(m_VariableMap, "g_samPointClamp");
	}

	void SetVariables(float4& AmbientColor, float4& Diffuse)
	{
		SetShaderVariable(ST_PixelShader, m_AmbientColor, &AmbientColor);
		SetShaderVariable(ST_PixelShader, m_LightDiffuse, &Diffuse);
	}

	void SetTextureVariables(const TSharedPtr<FRenderSurface>& InSurf, const TSharedPtr<FD3D11SamplerState>& Sampler)
	{
		SetShaderVariable(ST_PixelShader, m_txDiffuseAlbedo, InSurf->ShaderResourceView);
		SetShaderVariable(ST_PixelShader, m_samPointClamp, Sampler);
	}

private:
	FShaderConstantVarialble	m_AmbientColor;
	FShaderConstantVarialble	m_LightDiffuse;
	FShaderResourceVariable		m_txDiffuseAlbedo;
	FShaderResourceVariable		m_samPointClamp;
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