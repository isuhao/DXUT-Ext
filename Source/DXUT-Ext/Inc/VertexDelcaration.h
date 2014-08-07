#ifndef _VERTEX_DECLARATION_H_
#define _VERTEX_DECLARATION_H_

#include "Core.h"

enum EVertexElementType
{
	VET_None,
	VET_Float1,
	VET_Float2,
	VET_Float3,
	VET_Float4,
	VET_PackedNormal,
	VET_UByte4,
	VET_UByte4N,
	VET_Color,
	VET_Short2,
	VET_Short2N,
	VET_Half2,
	VET_MAX
};

enum EVertexElementUsage
{
	VEU_Position,
	VEU_TextureCoordinate,
	VEU_BlendWeight,
	VEU_BlendIndices,
	VEU_Normal,
	VEU_Tangent,
	VEU_Binormal,
	VEU_Color
};

/**
 * 顶点声明，对DX11 InputLayout的封装
 * 跟Shader解耦，更加灵活
 */
class FVertexDeclaration
{
public:
	FVertexDeclaration()
	{
		Clear();
	}

	D3D11_INPUT_ELEMENT_DESC* GetElements()
	{
		return &m_Layouts.GetAt(0);
	}

	UINT GetElementNum()
	{
		return m_Layouts.GetSize();
	}

	void AppendElement(UINT StreamIndex, UINT Offset, EVertexElementType ElemType, EVertexElementUsage Usage, UINT UsageIndex,
		bool bUseInstanceIndex = false, UINT NumVerticesPerInstance = 0);

	void AppendElementFast(UINT StreamIndex, EVertexElementType ElemType, EVertexElementUsage Usage, UINT UsageIndex, bool bResetOffset = false,
		bool bUseInstanceIndex = false, UINT NumVerticesPerInstance = 0);

	static UINT GetFormatByteSize(EVertexElementType UType);

	void Clear();

private:
	UINT										m_nCurrOffset; // 这个值只有AppendElementFast才有用
	TArray<D3D11_INPUT_ELEMENT_DESC>			m_Layouts;
};

/** 
 * 顶点声明的工厂类, 子类构造的时候初始化顶点声明内容
 */
class FVertexDeclarationFactory
{
public:
	const TSharedPtr<FVertexDeclaration>& GetVertexDeclaration()
	{
		return VertexDecls;
	}

protected:
	TSharedPtr<FVertexDeclaration> VertexDecls;
};

#endif