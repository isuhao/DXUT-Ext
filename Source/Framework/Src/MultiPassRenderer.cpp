#include "MultiPassRenderer.h"

void FMultiPassRenderer::OnInit()
{
	// 先注册资源
	RegisterResources();

	// 绑定顶点声明
	InitInputLayouts();
}

void FMultiPassRenderer::OnRender(float fDeltaSeconds)
{

}