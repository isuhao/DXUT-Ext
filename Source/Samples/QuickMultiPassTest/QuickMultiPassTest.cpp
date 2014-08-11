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

void FQuickMultiPassTest::OnInitObjs()
{

}