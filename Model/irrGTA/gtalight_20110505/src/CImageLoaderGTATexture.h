// Copyright (C) 2007-2011 David Kment
// For license see license.txt

#ifndef _CIMAGELOADERGTATEXTURE_H_
#define _CIMAGELOADERGTATEXTURE_H_

#include "irrlicht.h"
#include "IImageLoader.h"

using namespace irr;
using namespace core;
using namespace io;
using namespace video;

#define FORMAT_DEFAULT 0x0000
#define FORMAT_1555 0x0100 // (1 bit alpha, RGB 5 bits each; also used for DXT1 with alpha)
#define FORMAT_565 0x0200 // (5 bits red, 6 bits green, 5 bits blue; also used for DXT1 without alpha)
#define FORMAT_4444 0x0300 // (RGBA 4 bits each; also used for DXT3)
#define FORMAT_LUM8 0x0400 // (gray scale)
#define FORMAT_8888 0x0500 // (RGBA 8 bits each)
#define FORMAT_888 0x0600 // (RGB 8 bits each)
#define FORMAT_555 0x0A00 // (RGB 5 bits each - rare, use 565 instead)
#define FORMAT_EXT_AUTO_MIPMAP 0x1000 // (RW generates mipmaps)
#define FORMAT_EXT_PAL8 0x2000 // (2^8 = 256 palette colors)
#define FORMAT_EXT_PAL4 0x4000 // (2^4 = 16 palette colors)
#define FORMAT_EXT_MIPMAP 0x8000 // (mipmaps included)

struct SGTATextureHeader
{
    u32 platformId;
    u16 filterFlags;
    u8 textureWrapV;
    u8 textureWrapU;
    u8 diffuseTextureName[32];
    u8 alphaTextureName[32];
    u32 rasterFormat;
    u32 alphaOrFourCC;
    u16 imageWidth;
    u16 imageHeight;
    u8 bitsPerPixel;
    u8 mipMapCount;
    u8 rasterType;
    u8 dxtCompressionType;
};

class CImageLoaderGTATexture : public irr::video::IImageLoader
{
public:
	CImageLoaderGTATexture(IrrlichtDevice *device);
	virtual bool isALoadableFileExtension(const path& filename) const;
	virtual bool isALoadableFileFormat(irr::io::IReadFile* file) const;
	virtual irr::video::IImage* loadImage(irr::io::IReadFile* file) const;
private:
    IrrlichtDevice *Device;
};

#endif /* _CIMAGELOADERGTATEXTURE_H_ */
