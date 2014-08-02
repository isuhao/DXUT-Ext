cbuffer CB_ScenePass
{
	matrix	g_mWVP;
	float3	g_AmbientColor;
	float3	g_LightDiffuse;
	float2	padding;
};

// Texture
Texture2D   g_txColorDiffuse                     : register(t0);
Texture2D	g_txDiffuseAlbedo					 : register(t1);

// Sampler
SamplerState g_samPointClamp					 : register(s0);


struct VSInput
{
	float4 Pos:			POSITION;
	float2 TexUV:		TEXCOORD0;
	float3 Normal:		NORMAL;
};

struct VSToPS
{
	float2 TexUV:		TEXCOORD0;
	float4 Pos:			SV_POSITION;
	float4 RealPos:		TEXCOORD1;
};


VSToPS RenderSceneVS(VSInput In) 
{
	VSToPS Out;

	Out.TexUV = In.TexUV;
	Out.Pos = mul(In.Pos, g_mWVP);
	Out.RealPos = mul(In.Pos, g_mWVP);

	return Out;
}

float4 RenderScenePS(VSToPS In) : SV_TARGET
{
	float4 TexColor = g_txColorDiffuse.Sample(g_samPointClamp, In.TexUV);

	// Get diffuse
	float2 projectTexCoord;
	projectTexCoord.x = In.RealPos.x / In.RealPos.w / 2.0f + 0.5f;
	projectTexCoord.y = -In.RealPos.y / In.RealPos.w / 2.0f + 0.5f;
	
	// N * L * LightDiffuse
	float4 DiffuseColor = float4(g_txDiffuseAlbedo.Sample(g_samPointClamp, projectTexCoord).r * g_LightDiffuse, 1.0);

	// Get Ambient
	float4 Color = float4(g_AmbientColor, 1.0) + DiffuseColor;

	Color *= TexColor;

	return Color;
}