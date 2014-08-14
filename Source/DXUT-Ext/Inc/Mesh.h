#ifndef _MESH_H_
#define _MESH_H_

#include "D3DDriver.h"

#pragma comment(lib, "DXUTOptd.lib")

class FMesh 
{
public:
	FMesh(const WString& MeshName = L"Mesh");
	void Init();

	virtual void OnInit() {}
	virtual void Render() {}

	bool IsInitialized() const { return m_bInitialized; }

protected:
	WString					m_szMeshName;
	bool					m_bInitialized;
};

class FSDKMesh : public FMesh, public CDXUTSDKMesh
{
public:

	FSDKMesh(const WString& MeshName, bool bCreateAdjIndices = false);

	~FSDKMesh() { Destroy(); }

	virtual void OnInit();

	virtual void Render(/*UINT iDiffuseSlot = 0, UINT iNormalSlot = INVALID_SAMPLER_SLOT, UINT iSpecularSlot = INVALID_SAMPLER_SLOT*/);

	static TSharedPtr<FSDKMesh> CreateNew(const WString& MeshName);

protected:
	bool						m_bCreateAdjIndices;

	UINT						m_iDiffuseSlot;
	UINT						m_iNormalSlot;
	UINT						m_iSpecularSlot;
};


#endif