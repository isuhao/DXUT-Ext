#ifndef _RHI_TEXTURE_H_
#define _RHI_TEXTURE_H_

#include "Core.h"
#include "RHIResources.h"

class FTextureBase
{
public:
	FTextureBase() {}

protected:
	TSharedPtr<FRHIShaderResourceView>	SRV;
};

class FTexture2D : public FTextureBase
{
public:

	FTexture2D()
	{
	}

protected:
	TSharedPtr<FRHITexture2D>		Resource;
};

extern FPixelFormatInfo GPixelFormats[];

#endif