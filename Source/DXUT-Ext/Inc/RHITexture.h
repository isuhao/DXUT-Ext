#ifndef _RHI_TEXTURE_H_
#define _RHI_TEXTURE_H_

#include "Core.h"
#include "RHIResources.h"

/** 
 * ��ͼ�Ļ���
 */
class FTextureBase
{
public:
	TSharedPtr<FRHIShaderResourceView>	ShaderResourceView;
	const uint Width;
	const uint Height;
	const uint Depth;

	FTextureBase(
		uint InWidth,
		uint InHeight,
		uint InDepth,
		const TSharedPtr<FRHIShaderResourceView>& InDSV
		)
		: Width(InWidth)
		, Height(InHeight)
		, Depth(InDepth)
		, ShaderResourceView(InDSV)
	{
	}
};

class FTexture2D : public FTextureBase
{
public:
	TSharedPtr<FRHITexture2D>		Resource;

	FTexture2D(
		uint InWidth,
		uint InHeight,
		uint InDepth,
		const TSharedPtr<FRHIShaderResourceView>& InDSV,
		const TSharedPtr<FRHITexture2D>& InResource
		)
		: FTextureBase
			( InWidth
			, InHeight
			, InDepth
			, InDSV
			)
		, Resource(InResource)
	{
	}

	~FTexture2D()
	{
	}
};

extern FPixelFormatInfo GPixelFormats[PF_MaxSize];

#endif