#include "RHI.h"

void FDynamicRHI::SetVertexBuffer(int BufferIndex, const TSharedPtr<FRHIBuffer>& VertexBuffer, UINT Stride)
{
	ID3D11Buffer* pVB = VertexBuffer.get();
	UINT Offset = 0;
	m_pd3dImmediateContext->IASetVertexBuffers(BufferIndex, 1, &pVB, &Stride, &Offset);
}

void FDynamicRHI::SetIndexBuffer(int BufferIndex, const TSharedPtr<FRHIBuffer>& IndexBuffer, DXGI_FORMAT Format)
{
	m_pd3dImmediateContext->IASetIndexBuffer(IndexBuffer.get(), Format, 0);
}

void FDynamicRHI::DrawIndexedPrimitive(const TSharedPtr<FRHIBuffer>& IndexBuffer, EPrimitiveType PrimitiveType)
{

}

//TSharedPtr<FRHITexture2D> FDynamicRHI::CreateTexture2D(uint Width, uint Height, byte Format, uint NumMips, uint Flags)
//{
//
//}