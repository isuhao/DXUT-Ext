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

DECLARE_VAR_NAME(R_HOUSE, "House");
DECLARE_VAR_NAME(R_PASS0, "Pass0");


void FQuickMultiPassTest::RegisterResources()
{
	RegisterShader(R_PASS0, ST_VertexShader, L"Pass0.hlsl", "VS_Main", "vs_5_0");
	RegisterShader(R_PASS0, ST_PixelShader, L"Pass0.hlsl", "PS_Main", "ps_5_0");

	RegisterModel(R_HOUSE, FSDKMesh::CreateNew(L"Crypt\\Crypt.sdkmesh"));
}

void FQuickMultiPassTest::RenderPasses()
{
	// Pass0
	RENDER_PASS_BEGIN
	{
		PassSetShader(R_PASS0);
		PassSetModel(R_HOUSE);
	}
	//RENDER_PASS_END
	RenderPassEnd();
}

