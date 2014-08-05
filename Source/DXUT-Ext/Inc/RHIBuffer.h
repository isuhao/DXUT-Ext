#ifndef _RHI_BUFFER_H_
#define _RHI_BUFFER_H_

#include "Core.h"
#include "RHIResources.h"

template<typename ElemType>
class TBuffer
{
public:
	TBuffer()
		: m_iMaxSize(0)
		, m_iCurrUpdateSize(0)
		, m_pRawData(NULL)
	{
		m_pd3dBuffer.reset();
	}

	~TBuffer() { Destroy(); }

	void CreateBuffer(int iSize, ElemType* InitData = NULL)
	{
		Reserve(iSize, InitData);
		DoCreateBuffer(iSize, InitData);
	}

	void CreateBufferInDetail()
	{

	}

	// 新建一个
	void Reset(int iSize, ElemType* InData = NULL, uint BindFlag = 0)
	{
		// 先销毁旧的
		Destroy();
		// 再创建
		CreateBuffer(iSize, InData, BindFlag);
	}

	void Destroy()
	{
		SAFE_DELETE_ARRAY(m_pRawData);
		m_pd3dBuffer.reset();
		m_iCurrUpdateSize = m_iMaxSize = 0;
	}

	const TSharedPtr<FRHIBuffer>& GetBuffer()
	{
		return m_pd3dBuffer;
	}

	void UpdateData(ElemType* InData, uint iOffset, uint iSize)
	{
		Check(iOffset + iSize <= m_iMaxSize);
		CopyMemory((byte*)m_pRawData, (byte*)InData, sizeof(ElemType)* iSize);
		m_iCurrUpdateSize = Max(m_iCurrUpdateSize, iOffset + iSize);
	}

	void CommitData()
	{
	}

protected:

	virtual void DoCreateBuffer(int iSize, ElemType* InitData)
	{
		CreateBufferInner(false, 0, 0);
	}

	void Reserve(int iSize, ElemType* InitData)
	{
		Check(!m_pRawData);
		if (iSize > 0)
		{
			m_iMaxSize = iSize;
			m_pRawData = new ElemType[m_iMaxSize];
			if (InitData != NULL)
			{
				CopyMemory((byte*)m_pRawData, (byte*)InitData, sizeof(ElemType)* iSize);
			}
		}
	}

	void CreateBufferInner(bool bIsDynamic, uint BindFlag = 0, uint MiscFlags = 0)
	{
		FRHIBuffer*			pBuffer = NULL;
		D3D11_BUFFER_DESC	BufferDesc;
		HRESULT				hr;

		// 确认大小和Buffer的长度必须是16的倍数
		Check(m_iMaxSize <= D3D11_REQ_CONSTANT_BUFFER_ELEMENT_COUNT && (m_iMaxSize % 16) == 0);
	
		// 清空
		ZeroMemory(&BufferDesc, sizeof(BufferDesc));

		BufferDesc.Usage = bIsDynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
		BufferDesc.ByteWidth = m_iMaxSize;
		BufferDesc.BindFlags = BindFlag;
		BufferDesc.CPUAccessFlags = bIsDynamic ? D3D11_CPU_ACCESS_WRITE : D3D11_CPU_ACCESS_READ;
		BufferDesc.MiscFlags = MiscFlags;

		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = (byte*)m_pRawData;
		InitData.SysMemPitch = 0;
		InitData.SysMemSlicePitch = 0;

		ID3D11Device* pDevice = DXUTGetD3D11Device();
		V(pDevice->CreateBuffer(&BufferDesc, &InitData, &pBuffer));

		m_pd3dBuffer = MakeCOMPtr<FRHIBuffer>(pBuffer);
	}


protected:
	TSharedPtr<FRHIBuffer>			m_pd3dBuffer;
	ElemType*						m_pRawData;
	uint							m_iCurrUpdateSize;
	uint							m_iMaxSize;
};

typedef TBuffer<byte> FBuffer;

/** 
 * CB
 */
template<typename ElemType>
class TConstantBuffer : public TBuffer<ElemType>
{
public:
	TConstantBuffer() : TBuffer() {}

protected:
	D3D11_BIND_FLAG GetBindFlag()
	{
		return D3D11_USAGE_DEFAULT;
	}
};

typedef TConstantBuffer<byte> FConstantBuffer;

/**
 * VB
 */
template<typename ElemType>
class TVertexBuffer : public TBuffer<ElemType>
{
public:
	TVertexBuffer() : TBuffer() {}

	void CreateBuffer(int iSize, ElemType* InitData = NULL)
	{

	}
};

typedef TVertexBuffer<byte> FVertexBuffer;

/**
 * IB
 */
class FIndexBuffer : public TBuffer<uint>
{
public:
	FIndexBuffer() : TBuffer() {}
};


#endif