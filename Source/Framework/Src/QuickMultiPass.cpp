#include "QuickMultiPass.h"

void FQuickMultiPass::OnInit()
{
	// 先初始化资源
	InitResources();

	// 增加Pass
	AddPasses();
}