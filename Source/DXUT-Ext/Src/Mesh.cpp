#include "Mesh.h"

#define MAX_D3D11_VERTEX_STREAMS D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT

FMesh::FMesh(WCHAR* MeshName)
:	m_szMeshName(MeshName)
{
}

FSDKMesh::FSDKMesh(WCHAR* MeshName, bool bCreateAdjIndices /* = false */)
: FMesh(MeshName) 
, m_bCreateAdjIndices(bCreateAdjIndices)
, m_iDiffuseSlot(0)
, m_iNormalSlot(0)
, m_iSpecularSlot(0)
{
}

void FSDKMesh::Init()
{
	HRESULT hr;
	ID3D11Device* pd3dDevice = DXUTGetD3D11Device();

	V(Create(pd3dDevice, (LPCTSTR)m_szMeshName.c_str(), m_bCreateAdjIndices, NULL));
}

void FSDKMesh::Render(UINT iDiffuseSlot /* = 0 */, UINT iNormalSlot /* = INVALID_SAMPLER_SLOT */, UINT iSpecularSlot /* = INVALID_SAMPLER_SLOT */)
{
	CDXUTSDKMesh::Render(DXUTGetD3D11DeviceContext(), iDiffuseSlot, iNormalSlot, iSpecularSlot);
}

