#include "MultiPassRenderer.h"

void FMultiPassRenderer::OnInit()
{
	// ��ע����Դ
	RegisterResources();

	// �󶨶�������
	InitInputLayouts();
}

void FMultiPassRenderer::OnRender(float fDeltaSeconds)
{

}