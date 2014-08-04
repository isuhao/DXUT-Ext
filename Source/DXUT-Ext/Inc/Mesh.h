#ifndef _MESH_H_
#define _MESH_H_

#include "RHI.h"

class FMesh 
{
public:
	FMesh(WCHAR* MeshName);

	virtual void Init() {}

	virtual void Render() {}

protected:
	WString					m_szMeshName;
};

class FSDKMesh : public FMesh
{
public:
	typedef SDKMESH_CALLBACKS11*		SDKMeshLoadCallbackType;

	FSDKMesh(WCHAR* MeshName, bool bCreateAdjIndices = false, SDKMeshLoadCallbackType pLoadedCallback = NULL);

	~FSDKMesh() { Destroy(); }

	virtual void Init();

	virtual void Render();
	
	void Destroy() { m_SDKMesh.Destroy(); }

protected:
	bool						m_bCreateAdjIndices;
	CDXUTSDKMesh				m_SDKMesh;
	SDKMeshLoadCallbackType		m_pLoadedCallBack;
};



#endif