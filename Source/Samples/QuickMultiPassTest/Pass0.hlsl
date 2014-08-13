matrix matWorldViewProjection;
matrix matWorld;

SamplerState smpPointClamp;
Texture2D	 texColorDiffuse;

struct VS_Input
{
	float4 Pos		: POSITION;
	float3 Normal	: NORMAL;
	float2 TexUV	: TEXCOORD;
};

struct VSToPS
{
	float4 Pos		: POSITION;
	float TexUV		: TEXCOORD;
};


VSToPS VS_Main(VS_Input In)
{
	VSToPS Out;

	Out.Pos = mul(In.Pos, matWorldViewProjection);
	Out.TexUV = In.TexUV;

	return Out;
}

float4 PS_Main(VSToPS In) : SV_TARGET
{
	return texColorDiffuse.Sample(smpPointClamp, In.TexUV);
}