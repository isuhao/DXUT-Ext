#ifndef _D3D_TEXTURE_H_
#define _D3D_TEXTURE_H_

#include "Core.h"
#include "D3DResources.h"

/** 
 * ÌùÍ¼µÄ»ùÀà
 */
class FTextureBase
{
public:
	TSharedPtr<FD3D11ShaderResourceView>	ShaderResourceView;
	const uint Width;
	const uint Height;
	const uint Depth;

	FTextureBase(
		uint InWidth,
		uint InHeight,
		uint InDepth,
		const TSharedPtr<FD3D11ShaderResourceView>& InDSV
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
	TSharedPtr<FD3D11Texture2D>		Resource;

	FTexture2D(
		uint InWidth,
		uint InHeight,
		uint InDepth,
		const TSharedPtr<FD3D11ShaderResourceView>& InDSV,
		const TSharedPtr<FD3D11Texture2D>& InResource
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