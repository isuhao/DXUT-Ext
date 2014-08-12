#include "D3DDriver.h"
#include <functional>

void FD3D11Driver::SetVertexBuffer(int BufferIndex, const TSharedPtr<FD3D11Buffer>& VertexBuffer, UINT Stride)
{
	ID3D11Buffer* pVB = VertexBuffer.get();
	UINT Offset = 0;
	m_pd3dImmediateContext->IASetVertexBuffers(BufferIndex, 1, &pVB, &Stride, &Offset);
}

void FD3D11Driver::SetIndexBuffer(int BufferIndex, const TSharedPtr<FD3D11Buffer>& IndexBuffer, DXGI_FORMAT Format)
{
	m_pd3dImmediateContext->IASetIndexBuffer(IndexBuffer.get(), Format, 0);
}

// @TODO: DrawIndex的封装
void FD3D11Driver::DrawIndexedPrimitive(const TSharedPtr<FD3D11Buffer>& IndexBuffer, EPrimitiveType PrimitiveType)
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

void FD3D11Driver::SetSamplerState(EShaderType ShaderType, int SamplerIndex, const TSharedPtr<FD3D11SamplerState>& SamplerState)
{
	FD3D11SamplerState* pSamp = SamplerState.get();
	ShaderSetSamplersFuncs[ShaderType](m_pd3dImmediateContext, SamplerIndex, 1, &pSamp);
}

void FD3D11Driver::SetShaderConstantVariable(EShaderType ShaderType, uint BufferIndex, uint BindIndex, uint NumBytes, byte* Data)
{
	TArray<TSharedPtr<FConstantBuffer> >& CB = m_ConstantBuffers[ShaderType];
	Check(BufferIndex < CB.size());
	CB[BufferIndex]->UpdateData(Data, BindIndex, NumBytes);
}

void FD3D11Driver::SetShaderResourceVariable(EShaderType ShaderType, uint BindIndex, const TSharedPtr<FD3D11SamplerState>& Sampler)
{
	SetSamplerState(ShaderType, BindIndex, Sampler);
}

void FD3D11Driver::SetShaderResourceVariable(EShaderType ShaderType, uint BindIndex, const TSharedPtr<FD3D11ShaderResourceView>& SRV)
{
	FD3D11ShaderResourceView* pSRV = SRV.get();
	ShaderSetShaderResourcesFuncs[ShaderType](m_pd3dImmediateContext, BindIndex, 1, &pSRV);
}

void FD3D11Driver::CommitConstantBuffer()
{
	for (byte ShaderType = ST_VertexShader; ShaderType < ST_NumShaderTypes; ++ShaderType)
	{
		TArray<TSharedPtr<FConstantBuffer> >& CB = m_ConstantBuffers[ShaderType];
		for (uint i = 0; i < CB.size(); ++i)
		{
			if (CB[i]->CommitData())
			{
				FD3D11Buffer* pBuffer = CB[i]->GetBuffer().get();
				ShaderSetConstantBuffersFuncs[ShaderType](m_pd3dImmediateContext, i, 1, &pBuffer);
			}
		}
	}
}

// 开始渲染，做一些初始化工作
void FD3D11Driver::BeginRender()
{
}

// 结束渲染，清空所有渲染状态
void FD3D11Driver::EndRender()
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
		FD3D11Buffer* NullBuffer = NULL;
		for (uint i = 0; i < MAX_CB_COUNT; ++i)
		{
			ShaderSetConstantBuffersFuncs[ShaderType](m_pd3dImmediateContext, i, 1, &NullBuffer);
		}
	
		// 清空所有SRV
		FD3D11ShaderResourceView* NullSRV = NULL;
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
		FD3D11Buffer* NullBuffer = NULL;
		uint Strides = 0;
		uint Offsets = 0;
		m_pd3dImmediateContext->IASetVertexBuffers(StreamIndex, 1, &NullBuffer, &Strides, &Offsets);
	}
	// 清空IB
	m_pd3dImmediateContext->IASetIndexBuffer(NULL, DXGI_FORMAT_R32_UINT, 0);
}

void FD3D11Driver::GetBackBufferDesc(uint& OutWidth, uint OutHeight)
{
	const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc = DXUTGetDXGIBackBufferSurfaceDesc();
	OutWidth  = pBackBufferSurfaceDesc->Width;
	OutHeight = pBackBufferSurfaceDesc->Height;
}