#include "MultiPassBase.h"

void FMultiPassShader::SetConstantVariables(const String& ShaderVarName, uint NumBytes, byte* RawData)
{
	uint BufferIndex = 0;
	uint BindIndex = 0;
	uint OutNumBytes = 0;

	float M[4][4];
	float4x4* pGetData = reinterpret_cast<float4x4*>(RawData);
	if (pGetData != NULL)
	{
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				M[i][j] = pGetData->m[i][j];
			}
		}
	}

	// 如果有这个变量才设进去
	if (m_VariableMap.GetVariable(ShaderVarName, BufferIndex, BindIndex, OutNumBytes))
	{
		// !!!!!!!!!!!!!!!
		// @TODO: 需要处理数据必须16对齐的问题
		// @TODO: 可以的话对Variable做一个缓存，这样就不用每次都查Map了！
		D3D->SetShaderConstantVariable(
			m_ShaderType,
			BufferIndex,
			BindIndex,
			NumBytes,
			RawData
			);
	}
}

void FMultiPassShader::SetResourceVariables(const String& ShaderVarName, const TSharedPtr<FD3D11ShaderResourceView>& InSRV)
{
	uint BufferIndex = 0;
	uint BindIndex = 0;
	uint NumBytes = 0;

	m_VariableMap.GetVariable(ShaderVarName, BufferIndex, BindIndex, NumBytes);

	D3D->SetShaderResourceVariable(
		m_ShaderType,
		BindIndex,
		InSRV
		);
}


void FMultiPassShader::SetResourceVariables(const String& ShaderVarName, const TSharedPtr<FD3D11SamplerState>& InSampler)
{
	uint BufferIndex = 0;
	uint BindIndex = 0;
	uint NumBytes = 0;

	m_VariableMap.GetVariable(ShaderVarName, BufferIndex, BindIndex, NumBytes);

	D3D->SetShaderResourceVariable(
		m_ShaderType,
		BindIndex,
		InSampler
		);
}

const String FRegisteredVariable::Default					= "Default";
const String FRegisteredVariable::DefaultRasterizeState		= "DefaultRasterizeState";
const String FRegisteredVariable::DefaultBlendState			= "DefaultBlendState";
const String FRegisteredVariable::DefaultDepthStencilState	= "DefaultDepthStencilState";
const String FRegisteredVariable::MatWorld					= "matWorld";
const String FRegisteredVariable::MatWorldViewProjection	= "matWorldViewProjection";