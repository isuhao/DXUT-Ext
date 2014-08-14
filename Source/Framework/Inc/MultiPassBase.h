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
	PDT_Geometry,	// 画几何体
	PDT_Buffer,		// 画Buffer
};

/**
* 关于Model的上下文
*/
struct FPassModelContext
{
	String	Name;
	FMesh	Mesh;
};

/**
* 常量的上下文
*/
struct FPassConstantContext
{
	TArray<byte> RawData;

	FPassConstantContext(uint iSize)
		: RawData(iSize)
	{
	}
};

class FMultiPassRendererVDF : public FVertexDeclarationFactory
{
public:
	FMultiPassRendererVDF()
	{
		VertexDecls = TSharedPtr<FVertexDeclaration>(new FVertexDeclaration());

		VertexDecls->AppendElementFast(0, VET_Float3, VEU_Position, 0);
		VertexDecls->AppendElementFast(0, VET_Float3, VEU_Normal, 0);
		VertexDecls->AppendElementFast(0, VET_Float2, VEU_TextureCoordinate, 0);
	}
};

// 默认的变量
struct FRegisteredVariable
{
	static const String Default;

	// 默认States
	static const String DefaultRasterizeState;
	static const String DefaultBlendState;
	static const String DefaultDepthStencilState;

	// 默认Constant
	static const String MatWorld;
	static const String MatWorldViewProjection;
};


#endif