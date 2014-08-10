cbuffer C_B : register(b0)
{
	matrix	g_mWVP;
	matrix	g_mWorld;
};

cbuffer C_B2 : register(b1)
{
	matrix	g2_mWVP;
	matrix	g2_mWorld;
};

// Texture
Texture2D   g_txDiffuse                     : register(t0);
// Sampler
SamplerState g_samLinearWrap                : register(s0);

float4 DummyColor[4];
float4 DummyColor2[4];

struct VSInput
{
	float4 Pos:			POSITION;
	float2 TexUV:		TEXCOORD0;
	float3 Normal:		NORMAL;
};

struct VSToPS
{
	float4 Pos:			SV_POSITION;
	float2 TexUV:		TEXCOORD0;
	float3 Normal:		NORMAL;
};

struct PSOutput
{
	float4 Normal:		SV_TARGET0;
};

VSToPS VS_Main(VSInput In)
{
	VSToPS Out;

	Out.Pos	= mul(In.Pos, g_mWVP);
	Out.Normal = mul(In.Normal, (float3x3)g_mWorld);
	Out.Normal = normalize(In.Normal);

	return Out;
}

PSOutput PS_Main(VSToPS In)
{
	PSOutput Out;

	//DummyColor[0] = float4(1.0, 1.0, 1.0, 1.0);
	Out.Normal = float4(In.Normal, 1.0);
	Out.Normal.r -= DummyColor[0].r;
	Out.Normal.g -= DummyColor2[1].r;

	return Out;
}