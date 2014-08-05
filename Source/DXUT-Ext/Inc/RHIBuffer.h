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
		m_pD3DBuffer.reset();
	}

	~TBuffer() { Destroy(); }

	virtual void CreateBuffer(int iSize, ElemType* InitData = NULL)
	{
		CreateBufferWithFlag(iSize, InitData);
	}

	void CreateBufferWithFlag(int iSize, ElemType* InitData = NULL, bool bIsDynamic = false, uint BindFlag = 0, uint MiscFlag = 0)
	{
		Reserve(iSize, InitData);
		CreateBufferInner(bIsDynamic, BindFlag, MiscFlag);
	}

	void Destroy()
	{
		SAFE_DELETE_ARRAY(m_pRawData);
		m_pD3DBuffer.reset();
		m_iCurrUpdateSize = m_iMaxSize = 0;
	}

	const TSharedPtr<FRHIBuffer>& GetBuffer()
	{
		return m_pD3DBuffer;
	}

	void UpdateData(ElemType* InData, uint iOffset, uint iSize)
	{
		Check(iOffset + iSize <= m_iMaxSize);
		CopyMemory((byte*)m_pRawData + iOffset * sizeof(ElemType), (byte*)InData, sizeof(ElemType)* iSize);
		m_iCurrUpdateSize = Max(m_iCurrUpdateSize, iOffset + iSize);
	}

	void CommitData()
	{
		if (!m_pD3DBuffer || m_iCurrUpdateSize <= 0)
		{
			return;
		}

		ID3D11DeviceContext* pd3dImmediateContext = DXUTGetD3D11DeviceContext();
		if (!pd3dImmediateContext)
		{
			return;
		}

		D3D11_BUFFER_DESC TheBufferDesc;
		m_pD3DBuffer->GetDesc(&TheBufferDesc);

		if (TheBufferDesc.Usage == D3D11_USAGE_DEFAULT)
		{
			pd3dImmediateContext->UpdateSubresource(m_pD3DBuffer.get(), 0, 0, (void*)m_pRawData, 0, 0);
		}
		else if (TheBufferDesc.Usage == D3D11_USAGE_DYNAMIC)
		{
			D3D11_MAPPED_SUBRESOURCE MappedResource;
			pd3dImmediateContext->Map(m_pD3DBuffer.get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
			uint TotalUpdateSize = sizeof(ElemType)* m_iCurrUpdateSize;
			if (MappedResource.pData != NULL)
			{
				CopyMemory(MappedResource.pData, (byte*)m_pRawData, TotalUpdateSize);
			}
			pd3dImmediateContext->Unmap(m_pD3DBuffer.get(), 0);
		}
		else
		{
			HRESULT		hr;
			FRHIBuffer* pNewBuffer;
			ID3D11Device* pDevice = DXUTGetD3D11Device();
			Check(pDevice);

			D3D11_BUFFER_DESC NewBufferDesc;
			ZeroMemory(&NewBufferDesc, sizeof(NewBufferDesc));
			NewBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			NewBufferDesc.ByteWidth = m_iMaxSize * sizeof(ElemType);

			D3D11_SUBRESOURCE_DATA InitData;
			InitData.pSysMem = m_pRawData;
			InitData.SysMemPitch = 0;
			InitData.SysMemSlicePitch = 0;

			V(pDevice->CreateBuffer(&NewBufferDesc, &InitData, &pNewBuffer));
			pd3dImmediateContext->CopyResource(m_pD3DBuffer.get(), pNewBuffer);

			SAFE_RELEASE(pNewBuffer);
		}
	}

protected:

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

		uint				ByteSize = m_iMaxSize * sizeof(ElemType);

		// 确认大小和Buffer的长度必须是16的倍数
		Check(ByteSize <= D3D11_REQ_CONSTANT_BUFFER_ELEMENT_COUNT && (ByteSize % 16) == 0);
	
		// 清空
		ZeroMemory(&BufferDesc, sizeof(BufferDesc));

		BufferDesc.Usage = bIsDynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
		BufferDesc.ByteWidth = ByteSize;
		BufferDesc.BindFlags = BindFlag;
		BufferDesc.CPUAccessFlags = bIsDynamic ? D3D11_CPU_ACCESS_WRITE : 0;
		BufferDesc.MiscFlags = MiscFlags;

		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = (byte*)m_pRawData;
		InitData.SysMemPitch = 0;
		InitData.SysMemSlicePitch = 0;

		ID3D11Device* pDevice = DXUTGetD3D11Device();
		V(pDevice->CreateBuffer(&BufferDesc, &InitData, &pBuffer));

		m_pD3DBuffer = MakeCOMPtr<FRHIBuffer>(pBuffer);
	}

protected:
	TSharedPtr<FRHIBuffer>			m_pD3DBuffer;
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

	virtual void CreateBuffer(int iSize, ElemType* InitData = NULL)
	{
		CreateBufferWithFlag(iSize, InitData, false, D3D11_BIND_CONSTANT_BUFFER, 0);
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

	virtual void CreateBuffer(int iSize, ElemType* InitData = NULL)
	{
		CreateBufferWithFlag(iSize, InitData, true, D3D11_BIND_VERTEX_BUFFER, 0);
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

	virtual void CreateBuffer(int iSize, uint* InitData = NULL)
	{
		CreateBufferWithFlag(iSize, InitData, true, D3D11_BIND_INDEX_BUFFER, 0);
	}
};

#endif