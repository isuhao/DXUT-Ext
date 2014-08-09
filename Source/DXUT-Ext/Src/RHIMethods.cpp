#include "RHI.h"
#include <functional>

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

std::function<void(ID3D11DeviceContext*, UINT, UINT, ID3D11ShaderResourceView * const *)> ShaderSetShaderResourcesFuncs[ST_NumShaderTypes] =
{
	std::mem_fn(&ID3D11DeviceContext::VSSetShaderResources),
	std::mem_fn(&ID3D11DeviceContext::PSSetShaderResources),
	std::mem_fn(&ID3D11DeviceContext::DSSetShaderResources),
	std::mem_fn(&ID3D11DeviceContext::HSSetShaderResources),
	std::mem_fn(&ID3D11DeviceContext::GSSetShaderResources),
	std::mem_fn(&ID3D11DeviceContext::CSSetShaderResources),
};

std::function<void(ID3D11DeviceContext*, UINT, UINT, ID3D11SamplerState * const *)> ShaderSetSamplersFuncs[ST_NumShaderTypes] =
{
	std::mem_fn(&ID3D11DeviceContext::VSSetSamplers),
	std::mem_fn(&ID3D11DeviceContext::PSSetSamplers),
	std::mem_fn(&ID3D11DeviceContext::DSSetSamplers),
	std::mem_fn(&ID3D11DeviceContext::HSSetSamplers),
	std::mem_fn(&ID3D11DeviceContext::GSSetSamplers),
	std::mem_fn(&ID3D11DeviceContext::CSSetSamplers),
};

std::function<void(ID3D11DeviceContext*, UINT, UINT, ID3D11Buffer * const *)> ShaderSetConstantBuffersFuncs[ST_NumShaderTypes] =
{
	std::mem_fn(&ID3D11DeviceContext::VSSetConstantBuffers),
	std::mem_fn(&ID3D11DeviceContext::PSSetConstantBuffers),
	std::mem_fn(&ID3D11DeviceContext::DSSetConstantBuffers),
	std::mem_fn(&ID3D11DeviceContext::HSSetConstantBuffers),
	std::mem_fn(&ID3D11DeviceContext::GSSetConstantBuffers),
	std::mem_fn(&ID3D11DeviceContext::CSSetConstantBuffers),
};

void FDynamicRHI::SetSamplerState(EShaderType ShaderType, int SamplerIndex, const TSharedPtr<FRHISamplerState>& SamplerState)
{
	FRHISamplerState* pSamp = SamplerState.get();
	ShaderSetSamplersFuncs[ShaderType](m_pd3dImmediateContext, SamplerIndex, 1, &pSamp);
}

