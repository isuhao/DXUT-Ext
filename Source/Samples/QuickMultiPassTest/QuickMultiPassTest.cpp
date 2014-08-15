#include "QuickMultiPassTest.h"

//--------------------------------------------------------------------------------------
// Win Main
//--------------------------------------------------------------------------------------
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	FQuickMultiPassTest().Run();

	return 0;
}

FQuickMultiPassTest::FQuickMultiPassTest()
{
}

DECLARE_VAR_NAME(R_HOUSE);
DECLARE_VAR_NAME(R_PASS0);
DECLARE_VAR_NAME(R_SAMPLER);

void FQuickMultiPassTest::RegisterResources()
{
	RegisterShader(R_PASS0, ST_VertexShader, L"Pass0.hlsl", "VS_Main", "vs_5_0");
	RegisterShader(R_PASS0, ST_PixelShader, L"Pass0.hlsl", "PS_Main", "ps_5_0");

	RegisterModel(R_HOUSE, FSDKMesh::CreateNew(L"Crypt\\Crypt.sdkmesh"));
	RegisterSampler(R_SAMPLER, SF_Trilinear);
}

void FQuickMultiPassTest::RenderPasses()
{
	// Pass0
	//RENDER_PASS_BEGIN
	RenderPassBegin();
	{
		PassSetShader(R_PASS0);
		PassSetModel(R_HOUSE);
		PassSetSampler(ST_PixelShader, "smpPointClamp", R_SAMPLER);
	}
	//RENDER_PASS_END
	RenderPassEnd();
}

