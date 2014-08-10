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

void FDynamicRHI::SetShaderConstantVariable(EShaderType ShaderType, uint BufferIndex, uint BindIndex, uint NumBytes, byte* Data)
{
	TArray<FConstantBuffer>& CB = m_ConstantBuffers[ShaderType];
	Check(BufferIndex < uint(CB.GetSize()));
	CB[BufferIndex].UpdateData(Data, BindIndex, NumBytes);
}

void FDynamicRHI::SetShaderResourceVariable(EShaderType ShaderType, uint BindIndex, const TSharedPtr<FRHISamplerState>& Sampler)
{
	SetSamplerState(ShaderType, BindIndex, Sampler);
}

void FDynamicRHI::SetShaderResourceVariable(EShaderType ShaderType, uint BindIndex, const TSharedPtr<FTextureBase>& Texture)
{
	Check(Texture && Texture->ShaderResourceView);
	FRHIShaderResourceView* pSRV = Texture->ShaderResourceView.get();
	ShaderSetShaderResourcesFuncs[ShaderType](m_pd3dImmediateContext, BindIndex, 1, &pSRV);
}

void FDynamicRHI::CommitConstantBuffer()
{
	for (byte ShaderType = ST_VertexShader; ShaderType < ST_NumShaderTypes; ++ShaderType)
	{
		m_pd3dImmediateContext->VSSetConstantBuffers(0, 1, NULL);
	}
}

// 开始渲染，做一些初始化工作
void FDynamicRHI::BeginRender()
{
}

// 结束渲染，清空所有渲染状态
void FDynamicRHI::EndRender()
{
	// 清空RTV和DSV
	for (uint i = 0; i < MAX_RT_COUNT; ++i)
	{
		m_pCurrRTVs[i] = 0;
	}
	m_pCurrDSV = 0;

	m_pd3dImmediateContext->OMSetRenderTargets(MAX_RT_COUNT, &m_pCurrRTVs[0], m_pCurrDSV);


	for (byte ShaderType = ST_VertexShader; ShaderType < ST_NumShaderTypes; ++ShaderType)
	{
		// 清空所有CB
		FRHIBuffer* NullBuffer = NULL;
		for (uint i = 0; i < MAX_CB_COUNT; ++i)
		{
			ShaderSetConstantBuffersFuncs[ShaderType](m_pd3dImmediateContext, i, 1, &NullBuffer);
		}
	
		// 清空所有SRV
		FRHIShaderResourceView* NullSRV = NULL;
		for (uint i = 0; i < MAX_SR_COUNT; ++i)
		{
			ShaderSetShaderResourcesFuncs[ShaderType](m_pd3dImmediateContext, i, 1, &NullSRV);
		}
	}

	// 清空Shader
	m_pd3dImmediateContext->VSSetShader(NULL, NULL, 0);
	m_pd3dImmediateContext->PSSetShader(NULL, NULL, 0);
	m_pd3dImmediateContext->DSSetShader(NULL, NULL, 0);
	m_pd3dImmediateContext->HSSetShader(NULL, NULL, 0);
	m_pd3dImmediateContext->GSSetShader(NULL, NULL, 0);
	m_pd3dImmediateContext->CSSetShader(NULL, NULL, 0);

	// 清空VB
	for (uint StreamIndex = 0; StreamIndex < 16; StreamIndex++)
	{
		FRHIBuffer* NullBuffer = NULL;
		uint Strides = 0;
		uint Offsets = 0;
		m_pd3dImmediateContext->IASetVertexBuffers(StreamIndex, 1, &NullBuffer, &Strides, &Offsets);
	}
	// 清空IB
	m_pd3dImmediateContext->IASetIndexBuffer(NULL, DXGI_FORMAT_R32_UINT, 0);
}