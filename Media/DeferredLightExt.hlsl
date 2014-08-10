struct VSToPS
{
	float4 Pos:			SV_POSITION;
	float2 TexUV:		TEXCOORD0;
};

struct PSOutput
{
	float4 Diffuse:		SV_TARGET0;
};

cbuffer CB_DL
{
	float3	g_vLightDir;
};

// Texture
Texture2D   g_txNormal			: register(t0);
// Sampler
SamplerState g_samPointClamp    : register(s0);

VSToPS VS_Main(uint id : SV_VertexID)
{
	VSToPS Out;

	if (id == 0)
	{
		Out.Pos = float4(-1, 1, 0, 1);
		Out.TexUV = float2(0, 0);
	}
	else if (id == 1)
	{
		Out.Pos = float4(1, 1, 0, 1);
		Out.TexUV = float2(1, 0);
	}
	else if (id == 2)
	{
		Out.Pos = float4(-1, -1, 0, 1);
		Out.TexUV = float2(0, 1);
	}
	else
	{
		Out.Pos = float4(1, -1, 0, 1);
		Out.TexUV = float2(1, 1);
	}

	return Out;
}

PSOutput PS_Main(VSToPS In)
{
	PSOutput Out;
	float4 tNormal;

	// Get the normal
	tNormal = g_txNormal.Sample(g_samPointClamp, In.TexUV);

	// Calc diffuse albedo : N * L
	float3 LightDir = normalize(g_vLightDir);
	float Albedo = saturate(dot(tNormal.rgb, LightDir));
	Out.Diffuse = float4(Albedo, 0.0, 0.0, 1.0);

	return Out;
}