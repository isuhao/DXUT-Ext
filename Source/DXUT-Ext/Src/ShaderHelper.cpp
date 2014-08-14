#include "ShaderHelper.h"

HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
	HRESULT hr = S_OK;

	// find the file
	WCHAR str[MAX_PATH];
	V_RETURN(DXUTFindDXSDKMediaFileCch(str, MAX_PATH, szFileName));

	DWORD dwShaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;

#if defined( DEBUG ) || defined( _DEBUG )
	// Set the D3D10_SHADER_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3D10_SHADER_DEBUG;
#endif

	ID3DBlob* pErrorBlob;
	hr = D3DX11CompileFromFile(str, NULL, NULL, szEntryPoint, szShaderModel,
		dwShaderFlags, 0, NULL, ppBlobOut, &pErrorBlob, NULL);
	if (FAILED(hr))
	{
		if (pErrorBlob != NULL)
			OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
		SAFE_RELEASE(pErrorBlob);
		return hr;
	}
	SAFE_RELEASE(pErrorBlob);

	return S_OK;
}

bool GenParamMapByD3DReflection(TSharedPtr<ID3DBlob>& CompiledCode, FShaderVarialbleMap& VariableMap)
{
	HRESULT hr;
	ID3D11ShaderReflection* Reflector = NULL;

	Check(CompiledCode);
	
	V(D3DReflect(CompiledCode->GetBufferPointer(), CompiledCode->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&Reflector));

	D3D11_SHADER_DESC ShaderDesc;
	Reflector->GetDesc(&ShaderDesc);

	for (UINT ResourceIndex = 0; ResourceIndex < ShaderDesc.BoundResources; ResourceIndex++)
	{
		D3D11_SHADER_INPUT_BIND_DESC BindDesc;
		Reflector->GetResourceBindingDesc(ResourceIndex, &BindDesc);

		// 如果是常量
		if (BindDesc.Type == D3D10_SIT_CBUFFER)
		{
			const UINT CBIndex = BindDesc.BindPoint;
			ID3D11ShaderReflectionConstantBuffer* ConstantBuffer = Reflector->GetConstantBufferByName(BindDesc.Name);
			D3D11_SHADER_BUFFER_DESC CBDesc;
			ConstantBuffer->GetDesc(&CBDesc);

			// Track all of the variables in this constant buffer.
			for (UINT ConstantIndex = 0; ConstantIndex < CBDesc.Variables; ConstantIndex++)
			{
				ID3D11ShaderReflectionVariable* Variable = ConstantBuffer->GetVariableByIndex(ConstantIndex);
				D3D11_SHADER_VARIABLE_DESC VariableDesc;
				Variable->GetDesc(&VariableDesc);

				// Aeron Begin
				// @TODO: 这里有时候明明在Shader中有引用，为什么还是标记为0？
				//if (VariableDesc.uFlags & D3D10_SVF_USED)
				// Aeron End
				{
					uint StartSlot = VariableDesc.StartOffset;
					// 增加一个常量
					VariableMap.AddVariable(
						VariableDesc.Name,
						CBIndex,
						VariableDesc.StartOffset,
						VariableDesc.Size
						);
				}
			}
		}
		else if (BindDesc.Type == D3D10_SIT_TEXTURE || BindDesc.Type == D3D10_SIT_SAMPLER)
		{
			VariableMap.AddVariable(
				BindDesc.Name,
				0, // BufferIndex这里不用
				BindDesc.BindPoint,
				BindDesc.BindCount
				);
		}
	}

	Reflector->Release();

	return true;
}

void SetShaderVariable(EShaderType ShaderType, const FShaderResourceVariable& Variable, const TSharedPtr<FD3D11SamplerState>& InSampler)
{
	D3D->SetShaderResourceVariable(
		ShaderType,
		Variable.GetBindIndex(),
		InSampler
		);
}

void SetShaderVariable(EShaderType ShaderType, const FShaderResourceVariable& Variable, const TSharedPtr<FTextureBase>& InTexture2D)
{
	D3D->SetShaderResourceVariable(
		ShaderType,
		Variable.GetBindIndex(),
		InTexture2D->ShaderResourceView
		);
}

void SetShaderVariable(EShaderType ShaderType, const FShaderResourceVariable& Variable, const TSharedPtr<FD3D11ShaderResourceView>& InSRV)
{
	D3D->SetShaderResourceVariable(
		ShaderType,
		Variable.GetBindIndex(),
		InSRV
		);
}

String ShaderType2String(EShaderType Type)
{
	switch (Type)
	{
	case ST_VertexShader:
		return "VertexShader";
	case ST_PixelShader:
		return "PixelShader";
	case ST_DomainShader:
		return "DomainShader";
	case ST_HullShader:
		return "HullShader";
	case ST_GeometryShader:
		return "GeometryShader";
	case ST_ComputeShader:
		return "ComputeShader";
	default:
		return "";
	}
}