#include "MultiPassRenderer.h"

void FMultiPassRenderer::OnInit()
{
	// 绑定顶点声明
	InitInputLayouts();

	// 先注册资源
	RegisterResources();

	// 注册资源之后的操作
	PostRegiester();
}

void FMultiPassRenderer::PostRegiester()
{

}

void FMultiPassRenderer::RenderPassBegin()
{
	D3D->SetDefaultBackBuffer();
}

void FMultiPassRenderer::RenderPassEnd()
{
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


void FMultiPassRenderer::RegisterShader(const String& RegisterName, const WString& FileName, const String& EntryPoint, const String& ShaderModel, EShaderType ShaderType)
{
	TSharedPtr<FMultiPassShader> NewShader = TSharedPtr<FMultiPassShader>(new FMultiPassShader(FileName, EntryPoint, ShaderModel, ShaderType) );
	m_RegisterShaders[RegisterName] = NewShader;
}

void FMultiPassRenderer::RegisterPassShader(const String& RegisterName, EShaderType ShaderType, const WString& FileName, const String& EntryPoint, const String& ShaderModel)
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
}

void FMultiPassRenderer::PassSetTarget(const String& RegisterName, uint RTIndex /* = 0 */)
{
	
}