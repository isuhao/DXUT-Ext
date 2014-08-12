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

void FQuickMultiPassTest::RegisterResources()
{
	RegisterShader("Pass0", L"Pass0.hlsl", "VS_Main", "vs_5_0", ST_VertexShader);
	RegisterShader("Pass0", L"Pass0.hlsl", "PS_Main", "ps_5_0", ST_PixelShader);
}

void FQuickMultiPassTest::RenderPasses()
{
	// Pass0
	RENDER_PASS_BEGIN
	{
		PassSetShader("Pass0");
	}
	RENDER_PASS_END
}
