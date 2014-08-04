#include "Mesh.h"

FMesh::FMesh(WCHAR* MeshName)
:	m_szMeshName(MeshName)
{
}

FSDKMesh::FSDKMesh(WCHAR* MeshName, bool bCreateAdjIndices /* = false */, SDKMeshLoadCallbackType pLoadedCallback /* = NULL */)
: FMesh(MeshName)
, m_pLoadedCallBack(NULL)
, m_bCreateAdjIndices(bCreateAdjIndices)
{
}

void FSDKMesh::Init()
{
	HRESULT hr;
	ID3D11Device* pd3dDevice = DXUTGetD3D11Device();

	V(m_SDKMesh.Create(pd3dDevice, (LPCTSTR)m_szMeshName.c_str(), m_bCreateAdjIndices, NULL));
}

void FSDKMesh::Render()
{
	//for (int MeshIndex = 0; MeshIndex < m_SDKMesh.GetNumMeshes(); ++MeshIndex)
	//{
	//	SDKMESH_MESH* Mesh = m_SDKMesh.GetMesh(MeshIndex);
	//	if (Mesh != NULL)
	//	{
	//		for (int VBIndex = 0; VBIndex < Mesh->NumVertexBuffers; ++VBIndex)
	//		{
	//			ID3D11Buffer* VB = m_SDKMesh.GetVB11(MeshIndex, VBIndex);
	//			RHI->SetVertexBuffer()
	//		}
	//	}
	//}

	ID3D11DeviceContext* pDeviceContext = DXUTGetD3D11DeviceContext();

	for (int VBIndex = 0; VBIndex < m_SDKMesh.GetNumVBs(); ++VBIndex)
	{
		ID3D11Buffer* pVB = m_SDKMesh.GetVB11At(VBIndex);
		//UINT Stride = m_SDKMesh.GetVertexStride()
		//pDeviceContext->IASetVertexBuffers(VBIndex, 1, &pVB, 
		//m_SDKMesh.GetAdjIB11()


	}

	/*for (int i = 0; i < NumVBs; ++i)
	{
	UINT Stride = m_SDKMesh.GetVertexStride()
	}*/
}