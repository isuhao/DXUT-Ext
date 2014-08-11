#include "VertexDelcaration.h"


void FVertexDeclaration::Clear()
{
	m_Layouts.clear();
	m_nCurrOffset = 0;
}

void FVertexDeclaration::AppendElement(UINT StreamIndex, UINT Offset, EVertexElementType ElemType, EVertexElementUsage Usage, UINT UsageIndex,
	bool bUseInstanceIndex /* = false */, UINT NumVerticesPerInstance /* = 0 */)
{
	D3D11_INPUT_ELEMENT_DESC D3DElement;

	D3DElement.InputSlot = StreamIndex;
	D3DElement.AlignedByteOffset = Offset;
	switch (ElemType)
	{
	case VET_Float1:		D3DElement.Format = DXGI_FORMAT_R32_FLOAT; break;
	case VET_Float2:		D3DElement.Format = DXGI_FORMAT_R32G32_FLOAT; break;
	case VET_Float3:		D3DElement.Format = DXGI_FORMAT_R32G32B32_FLOAT; break;
	case VET_Float4:		D3DElement.Format = DXGI_FORMAT_R32G32B32A32_FLOAT; break;
	case VET_PackedNormal:	D3DElement.Format = DXGI_FORMAT_R8G8B8A8_UNORM; break;
	case VET_UByte4:		D3DElement.Format = DXGI_FORMAT_R8G8B8A8_UINT; break;
	case VET_UByte4N:		D3DElement.Format = DXGI_FORMAT_R8G8B8A8_UNORM; break;
	case VET_Color:			D3DElement.Format = DXGI_FORMAT_R8G8B8A8_UNORM; break;
	case VET_Short2:		D3DElement.Format = DXGI_FORMAT_R16G16_SINT; break;
	case VET_Short2N:		D3DElement.Format = DXGI_FORMAT_R16G16_SNORM; break;
	case VET_Half2:			D3DElement.Format = DXGI_FORMAT_R16G16_FLOAT; break;
	default:
		// @TODO: Aeron,  printf error 
		break;
	};

	switch (Usage)
	{
	case VEU_Position:			D3DElement.SemanticName = "POSITION"; break;
	case VEU_TextureCoordinate:	D3DElement.SemanticName = "TEXCOORD"; break;
	case VEU_BlendWeight:		D3DElement.SemanticName = "BLENDWEIGHT"; break;
	case VEU_BlendIndices:		D3DElement.SemanticName = "BLENDINDICES"; break;
	case VEU_Normal:			D3DElement.SemanticName = "NORMAL"; break;
	case VEU_Tangent:			D3DElement.SemanticName = "TANGENT"; break;
	case VEU_Binormal:			D3DElement.SemanticName = "BINORMAL"; break;
	case VEU_Color:				D3DElement.SemanticName = "COLOR"; break;
	};

	D3DElement.SemanticIndex = UsageIndex;
	D3DElement.InputSlotClass = bUseInstanceIndex ? D3D11_INPUT_PER_INSTANCE_DATA : D3D11_INPUT_PER_VERTEX_DATA;
	D3DElement.InstanceDataStepRate = bUseInstanceIndex ? 1 : 0;

	m_Layouts.push_back(D3DElement);
}

/**
* 获取一个顶点说明的数据宽度
*/
UINT FVertexDeclaration::GetFormatByteSize(EVertexElementType UType)
{
	UINT FormatSize = 0;
	switch (UType)
	{
	case VET_Float1:       return sizeof(FLOAT)* 1; break;
	case VET_Float2:       return sizeof(FLOAT)* 2; break;
	case VET_Float3:       return sizeof(FLOAT)* 3; break;
	case VET_Float4:       return sizeof(FLOAT)* 4; break;
	case VET_PackedNormal: return sizeof(BYTE)* 4;  break;
	case VET_UByte4:       return sizeof(BYTE)* 4;  break;
	case VET_UByte4N:      return sizeof(BYTE)* 4;  break;
	case VET_Color:        return sizeof(BYTE)* 4;  break;
	case VET_Short2:       return sizeof(WORD)* 2;  break;
	case VET_Short2N:      return sizeof(WORD)* 2;  break;
	case VET_Half2:        return sizeof(WORD)* 2;  break;
	}
	Check(FormatSize > 0);
	return FormatSize;
}

/**
* 在同一个Stream中快速加入一个顶点说明，自动计算偏移量
* @param bResetOffset 是否重置Offset
*/
void FVertexDeclaration::AppendElementFast(UINT StreamIndex, EVertexElementType ElemType, EVertexElementUsage Usage, UINT UsageIndex, bool bResetOffset /* = false */,
	bool bUseInstanceIndex /* = false */, UINT NumVerticesPerInstance /* = 0 */)
{
	if (bResetOffset)
	{
		m_nCurrOffset = 0;
	}

	AppendElement(StreamIndex, m_nCurrOffset, ElemType, Usage, UsageIndex, bUseInstanceIndex, NumVerticesPerInstance);

	m_nCurrOffset += GetFormatByteSize(ElemType);
}