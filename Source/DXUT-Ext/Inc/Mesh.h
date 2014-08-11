#ifndef _MESH_H_
#define _MESH_H_

#include "D3DDriver.h"

#pragma comment(lib, "DXUTOptd.lib")

class FMesh 
{
public:
	FMesh(WCHAR* MeshName);

	virtual void Init() {}

	virtual void Render() {}

protected:
	WString					m_szMeshName;
};

class FSDKMesh : public FMesh, public CDXUTSDKMesh
{
public:

	FSDKMesh(WCHAR* MeshName, bool bCreateAdjIndices = false);

	~FSDKMesh() { Destroy(); }

	virtual void Init();

	virtual void Render(UINT iDiffuseSlot = 0, UINT iNormalSlot = INVALID_SAMPLER_SLOT, UINT iSpecularSlot = INVALID_SAMPLER_SLOT);

protected:
	bool						m_bCreateAdjIndices;

	UINT						m_iDiffuseSlot;
	UINT						m_iNormalSlot;
	UINT						m_iSpecularSlot;
};


#endif