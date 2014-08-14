#include "Mesh.h"

#define MAX_D3D11_VERTEX_STREAMS D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT

FMesh::FMesh(const WString& MeshName)
:	m_szMeshName(MeshName)
,	m_bInitialized(false)
{
}

void FMesh::Init()
{
	OnInit();
	m_bInitialized = true;
}

FSDKMesh::FSDKMesh(const WString& MeshName, bool bCreateAdjIndices /* = false */)
: FMesh(MeshName) 
, m_bCreateAdjIndices(bCreateAdjIndices)
, m_iDiffuseSlot(0)
, m_iNormalSlot(0)
, m_iSpecularSlot(0)
{
}

void FSDKMesh::OnInit()
{
	HRESULT hr;
	ID3D11Device* pd3dDevice = DXUTGetD3D11Device();

	V(Create(pd3dDevice, (LPCTSTR)m_szMeshName.c_str(), m_bCreateAdjIndices, NULL));
}

void FSDKMesh::Render()
{
	ID3D11DeviceContext* pDeviceContext = DXUTGetD3D11DeviceContext();
	Check(pDeviceContext);
	CDXUTSDKMesh::Render(pDeviceContext, 0, 0, 0);
}

TSharedPtr<FSDKMesh> FSDKMesh::CreateNew(const WString& MeshName)
{
	CHECK_D3D_VALID;
	TSharedPtr<FSDKMesh> NewMesh = TSharedPtr<FSDKMesh>(new FSDKMesh(MeshName, false));
	NewMesh->Init();
	return NewMesh;
}