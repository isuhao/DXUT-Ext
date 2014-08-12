struct VS_Input
{
	float4 Pos		: POSITION;
	float3 Normal	: NORMAL;
	float2 TexUV	: TEXCOORD;
};


float4 VS_Main(VS_Input In) : SV_POSITION
{
	return In.Pos;
}