#include "MultiPassRenderer.h"

void FMultiPassRenderer::OnInit()
{
	// 绑定顶点声明
	InitInputLayouts();

	// 先注册资源, Shader/ShaderVariables等等
	RegisterResources();

	// 注册默认的常量
	RegisterDefaultConstants();

	// 注册默认的States
	RegisterDefaultStates();

	// 注册资源之后的操作
	PostRegister();
}

void FMultiPassRenderer::PostRegister()
{

}

void FMultiPassRenderer::RegisterDefaultConstants()
{

}

void FMultiPassRenderer::PassSetDefaultConstants()
{

}

// 注册默认的States
void FMultiPassRenderer::RegisterDefaultStates()
{
	RegisterRasterizeState(FRegisteredVariable::DefaultRasterizeState);
	RegisterBlendState(FRegisteredVariable::DefaultBlendState);
	RegisterDepthStencilState(FRegisteredVariable::DefaultDepthStencilState);
}

// 设置默认的States
void FMultiPassRenderer::PassSetDefaultStates()
{
	PassSetRasterizeState(FRegisteredVariable::DefaultRasterizeState);
	PassSetBlendState(FRegisteredVariable::DefaultBlendState);
	PassSetDepthStencilState(FRegisteredVariable::DefaultDepthStencilState);
}

void FMultiPassRenderer::PassSetRasterizeState(const String& RegisterName)
{
	if (m_RegisterRasterizeStates.find(RegisterName) != m_RegisterRasterizeStates.end())
	{
		D3D->SetRasterizerState(m_RegisterRasterizeStates[RegisterName]);
	}
}

void FMultiPassRenderer::PassSetBlendState(const String& RegisterName)
{
	if (m_RegisterBlendStates.find(RegisterName) != m_RegisterBlendStates.end())
	{
		D3D->SetBlendState(m_RegisterBlendStates[RegisterName]);
	}
}

void FMultiPassRenderer::PassSetDepthStencilState(const String& RegisterName)
{
	if (m_RegisterDepthStencilStates.find(RegisterName) == m_RegisterDepthStencilStates.end())
	{
		D3D->SetDepthStencilState(m_RegisterDepthStencilStates[RegisterName]);
	}
}

void FMultiPassRenderer::RenderPassBegin()
{
	D3D->SetDefaultBackBuffer();
	D3D->Clear(true, FLinearColor::Black, true, 1, false, 0);

	// 设置一下默认的常量
	PassSetDefaultConstants();

	// 设置一下默认的状态
	PassSetDefaultStates();
}

void FMultiPassRenderer::RenderPassEnd()
{
	// 调用Mesh->Render
	//for (auto Itr = m_RegisterModels.begin(); Itr != m_RegisterModels.end(); ++Itr)
	//{
	//	TSharedPtr<FMesh> Mesh = Itr->second;
	//	Mesh->Render();
	//}

	auto MeshPtr = m_RegisterModels.find(m_CurrRenderingMesh);
	if (MeshPtr != m_RegisterModels.end())
	{
		MeshPtr->second->Render();
	}
}

void FMultiPassRenderer::OnRender(float fDeltaSeconds)
{
	D3D->BeginRender();

	RenderPasses();

	D3D->EndRender();
}

// 相应GUI事件，处理ConstantValue的变化等等
void FMultiPassRenderer::OnGUIEvent(uint nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext)
{
	// @TODO: SetConstantValue
}

void FMultiPassRenderer::InitInputLayouts()
{

}

void FMultiPassRenderer::PassSetConstantValue(EShaderType ShaderType, const String& ShaderVarName, const String& RegisterName)
{
	TMap<String, TSharedPtr<FPassConstantContext> >::iterator Itr = m_RegisterConstants.find(RegisterName);
	if (Itr != m_RegisterConstants.end())
	{
		TSharedPtr<FPassConstantContext>& ConstPtr = Itr->second;
		if (ConstPtr)
		{
			PassSetConstantValue<byte>(ShaderType, ShaderVarName, ConstPtr->RawData.size(), &(ConstPtr->RawData[0]));
			return;
		}
	}

	Check(0);
}

void FMultiPassRenderer::PassSetModel(const String& RegisterName)
{
	m_CurrRenderingMesh = RegisterName;
}

void FMultiPassRenderer::RegisterFrameBuffer(const String& RegisterName, uint Width, uint Height, EPixelFormat PixFormat)
{
	TSharedPtr<FFrameBuffer> NewFB = D3D->CreateFrameBuffer(Width, Height, PixFormat);
	m_RegisterFrameBuffers[RegisterName] = NewFB;
}

void FMultiPassRenderer::RegisterFrameBuffer(const String& RegisterName)
{
	uint Width	= 0;
	uint Height = 0;

	D3D->GetBackBufferDesc(Width, Height);
	RegisterFrameBuffer(RegisterName, Width, Height, PF_A8R8G8B8);
}

void FMultiPassRenderer::RegisterShader(const String& RegisterName, EShaderType ShaderType, const WString& FileName, const String& EntryPoint, const String& ShaderModel)
{
	// 先在Registers中找
	TSharedPtr<FD3D11BoundShaderState> FoundBoundShader;
	TMap<String, TSharedPtr<FD3D11BoundShaderState> >::iterator Itr = m_RegisterBoundShaders.find(RegisterName);
	if (Itr != m_RegisterBoundShaders.end())
	{
		FoundBoundShader = Itr->second;
	}
	else
	{
		FoundBoundShader = TSharedPtr<FD3D11BoundShaderState>(new FD3D11BoundShaderState);
		m_RegisterBoundShaders[RegisterName] = FoundBoundShader;
	}

	// 不能为空
	Check(FoundBoundShader);

	// 现在是创建新的Shader
	TSharedPtr<FMultiPassShader> NewShader = TSharedPtr<FMultiPassShader>(new FMultiPassShader(FileName, EntryPoint, ShaderModel, ShaderType));
	// 先初始化
	NewShader->InitShader();

	switch (ShaderType)
	{
	case ST_VertexShader:
		{
			ID3D11Device* pDevice = D3D->GetDevice();
			HRESULT hr;
			FoundBoundShader->VertexShader = NewShader->GetVertexShader();
			if (m_pVertexDeclaration)
			{
				ID3D11InputLayout* pInputLayout = NULL;
				D3D11_INPUT_ELEMENT_DESC* Elements = m_pVertexDeclaration->GetElements();
				UINT NumElements = m_pVertexDeclaration->GetElementNum();
				ID3DBlob* pBlob = NewShader->GetCode().get();
				V(pDevice->CreateInputLayout(Elements, NumElements, pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &pInputLayout));
				FoundBoundShader->InputLayouts = MakeCOMPtr<ID3D11InputLayout>(pInputLayout);
			}
		}
		break;
	case ST_PixelShader:
		FoundBoundShader->PixelShader = NewShader->GetPixelShader();
		break;
	case ST_DomainShader:
		FoundBoundShader->DomainShader = NewShader->GetDomainShader();
		break;
	case ST_HullShader:
		FoundBoundShader->HullShader = NewShader->GetHullShader();
		break;
	case ST_GeometryShader:
		FoundBoundShader->GeometryShader = NewShader->GetGeometryShader();
		break;
	case ST_ComputeShader:
		FoundBoundShader->ComputeShader = NewShader->GetComputeShader();
		break;
	case ST_NumShaderTypes:
		break;
	default:
		break;
	}
}

void FMultiPassRenderer::PassSetShader(const String& RegisterName)
{
	TMap<String, TSharedPtr<FD3D11BoundShaderState> >::iterator Itr = m_RegisterBoundShaders.find(RegisterName);
	if (Itr != m_RegisterBoundShaders.end())
	{
		D3D->SetBoundShaderState(Itr->second);
	}
	else
	{
		// Shader不正确
		Check(0);
	}

	// 设置当前的
	m_CurrBoundShader = RegisterName;
}

void FMultiPassRenderer::PassSetTarget(const String& RegisterName, uint RTIndex /* = 0 */)
{
	
}

void FMultiPassRenderer::RegisterRasterizeState(const String& RegisterName, ERasterizerFillMode FillMode /* = FM_Solid */, ERasterizerCullMode CullMode /* = CM_CCW */,
	bool bAllowMSAA /* = true */, bool bFrontCounterClockwise /* = false */, INT DepthBias /* = 0 */, float SlopeScaledDepthBias /* = 0 */)
{
	if (m_RegisterRasterizeStates.find(RegisterName) == m_RegisterRasterizeStates.end())
	{
		TSharedPtr<FD3D11RasterState> RasterizeState = D3D->CreateRasterizerState(FillMode, CullMode, bAllowMSAA, bFrontCounterClockwise, DepthBias, SlopeScaledDepthBias);
		m_RegisterRasterizeStates[RegisterName] = RasterizeState;
	}
}

void FMultiPassRenderer::RegisterBlendState(const String& RegisterName, EBlendOperation ColorBlendOp /* = BO_Add */, EBlendFactor ColorSrcBlend /* = BF_One */,
	EBlendFactor ColorDstBlend /* = BF_Zero */, EBlendOperation AlphaBlendOp /* = BO_Add */, EBlendFactor AlphaSrcBlend /* = BF_One */, EBlendFactor AlphaDstBlend /* = BF_Zero */,
	EBlendColorWriteEnable ColorWriteEnable /* = CWE_All */)
{
	if (m_RegisterBlendStates.find(RegisterName) == m_RegisterBlendStates.end())
	{
		TSharedPtr<FD3D11BlendState> BlendState = D3D->CreateBlendState(ColorBlendOp, ColorSrcBlend, ColorDstBlend, AlphaBlendOp, AlphaSrcBlend, AlphaDstBlend, ColorWriteEnable);
		m_RegisterBlendStates[RegisterName] = BlendState;
	}
}

void FMultiPassRenderer::RegisterDepthStencilState(const String& RegisterName, bool DepthEnable)
{
	if (m_RegisterDepthStencilStates.find(RegisterName) == m_RegisterDepthStencilStates.end())
	{
		TSharedPtr<FD3D11DepthStencilState> DepthStencilState = D3D->CreateDepthStencilState(DepthEnable);
		m_RegisterDepthStencilStates[RegisterName] = DepthStencilState;
	}
}

// Mesh
void FMultiPassRenderer::RegisterModel(const String& RegisterName, const TSharedPtr<FMesh>& InMesh)
{
	// 如果还没初始化，先初始化吧
	if (!InMesh->IsInitialized())
	{
		InMesh->Init();
	}

	if (m_RegisterModels.find(RegisterName) == m_RegisterModels.end())
	{
		m_RegisterModels[RegisterName] = InMesh;
	}
}

void FMultiPassRenderer::RegisterModel(const String& RegisterName, ESimplePrimitiveType PrimType)
{
	switch (PrimType)
	{
	case SPT_Quad:
		RegisterModel(RegisterName, FQuad::Create());
		break;
	default:
		break;
	}
}