#ifndef _SHADER_MANAGER_H_
#define _SHADER_MANAGER_H_

#include "Core.h"
#include "Shader.h"

/** 
 * ����FShader
 */
extern HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);

/**
 * ����VariableMap
 */
extern bool GenParamMapByD3DReflection(TSharedPtr<ID3DBlob>& CompiledCode, FShaderVarialbleMap& VariableMap);


template <typename ValueType>
void SetShaderVariable(EShaderType ShaderType, const FShaderConstantVarialble& Variable, ValueType& InValue)
{
	uint NumBytes = Min(sizeof(ValueType), Variable.GetNumBytes());

	D3D->SetShaderConstantVariable(
		ShaderType,
		Variable.GetBufferIndex(),
		Variable.GetBindIndex(),
		NumBytes,
		(byte*)(&InValue)
		);
}

template <typename ValueType>
void SetShaderVariable(EShaderType ShaderType, const FShaderConstantVarialble& Variable, ValueType* InValue, uint NumElements = 1)
{
	uint NumBytes = Min(sizeof(ValueType), Variable.GetNumBytes());

	D3D->SetShaderConstantVariable(
		ShaderType,
		Variable.GetBufferIndex(),
		Variable.GetBindIndex(),
		NumBytes * NumElements,
		(byte*)(InValue)
		);
}

extern void SetShaderVariable(EShaderType ShaderType, const FShaderResourceVariable& Variable, const TSharedPtr<FD3D11SamplerState>& InSampler);

extern void SetShaderVariable(EShaderType ShaderType, const FShaderResourceVariable& Variable, const TSharedPtr<FTextureBase>& InTexture2D);

extern void SetShaderVariable(EShaderType ShaderType, const FShaderResourceVariable& Variable, const TSharedPtr<FD3D11ShaderResourceView>& InSRV);


extern String ShaderType2String(EShaderType Type);

#endif