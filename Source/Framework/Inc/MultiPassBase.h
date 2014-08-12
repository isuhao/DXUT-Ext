#ifndef _MULTI_PASS_BASE_H_
#define _MULTI_PASS_BASE_H_

#include "AppFramework.h"

class FMultiPassShader : public FShader
{
public:

	FMultiPassShader(const WString& szFileName, const String& szEntryPoint, const String& szShaderModel, EShaderType ShaderType)
		: FShader(szFileName, szEntryPoint, szShaderModel, ShaderType)
	{
	}

	void SetConstantVariables(const String& ShaderVarName, uint NumBytes, byte* RawData);

	void SetResourceVariables(const String& ShaderVarName, const TSharedPtr<FD3D11ShaderResourceView>& InSRV);

	void SetResourceVariables(const String& ShaderVarName, const TSharedPtr<FD3D11SamplerState>& InSampler);
};


enum EPassDrawType
{
	PDT_Geometry,	// ��������
	PDT_Buffer,		// ��Buffer
};

/**
* ����Model��������
*/
struct FPassModelContext
{
	String	Name;
	FMesh	Mesh;
};

/**
* ������������
*/
struct FPassConstantContext
{
	TArray<byte> RawData;

	FPassConstantContext(uint iSize, byte* Src)
	{
		RawData.reserve(iSize);
		CopyMemory(&RawData[0], Src, iSize);
	}
};

struct FPassRegisterToken
{
	static const String Default;
};

#endif