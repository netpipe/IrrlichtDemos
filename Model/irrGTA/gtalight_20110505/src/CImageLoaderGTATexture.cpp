// Copyright (C) 2007-2011 David Kment
// For license see license.txt

#include "CImageLoaderGTATexture.h"
#include "IImage.h"
#include "dimension2d.h"
#include "IVideoDriver.h"
#include "IFileSystem.h"
#include "IReadFile.h"

#include "s3tc.h"

CImageLoaderGTATexture::CImageLoaderGTATexture(IrrlichtDevice *device)
{
    Device = device;
}

bool CImageLoaderGTATexture::isALoadableFileExtension(const path& filename) const
{
    return core::hasFileExtension(filename, "gtatexture");
}

bool CImageLoaderGTATexture::isALoadableFileFormat(irr::io::IReadFile* file) const
{
    // since gtatexture is a virtual file inside TXD archive, never to be caught in the wild,
    // we can skip this part and return false
    return false;
}

IImage* CImageLoaderGTATexture::loadImage(irr::io::IReadFile* file) const
{
    SGTATextureHeader header;

    file->seek(0);

    if (file->read(&header, sizeof(SGTATextureHeader)) != sizeof(SGTATextureHeader))
    {
        return 0;
    }

    bool unsupported = false;

    u32 rasterFormat = header.rasterFormat % 0x1000;
    u32 rasterFormatExt = header.rasterFormat - rasterFormat;

    ECOLOR_FORMAT colorFormat = ECF_A8R8G8B8;
    u8 bytesPerPixel = 0;

    if(rasterFormat == FORMAT_DEFAULT)
    {
        colorFormat = ECF_A8R8G8B8;
        bytesPerPixel = 4;
    }
    else if(rasterFormat == FORMAT_1555)
    {
        colorFormat = ECF_A1R5G5B5;
        bytesPerPixel = 2;
    }
    else if(rasterFormat == FORMAT_565)
    {
        colorFormat = ECF_R5G6B5;
        bytesPerPixel = 2;
    }
    else if(rasterFormat == FORMAT_4444)
    {
        colorFormat = ECF_A8R8G8B8;
        bytesPerPixel = 2;
    }
    else if(rasterFormat == FORMAT_LUM8)
    {
        colorFormat = ECF_R8G8B8;
        bytesPerPixel = 1;
    }
    else if(rasterFormat == FORMAT_8888)
    {
        colorFormat = ECF_A8R8G8B8;
        bytesPerPixel = 4;
    }
    else if(rasterFormat == FORMAT_888)
    {
        colorFormat = ECF_R8G8B8;
        bytesPerPixel = 3;
    }
    else if(rasterFormat == FORMAT_555)
    {
        colorFormat = ECF_R5G6B5;
        bytesPerPixel = 2;
    }
    else
    {
        printf("unknown raster format: 0x%x\n", rasterFormat);
        unsupported = true;
    }

    if(rasterFormatExt == FORMAT_EXT_AUTO_MIPMAP)
    {
        // auto mipmaps
    }
    else if(rasterFormatExt == FORMAT_EXT_MIPMAP)
    {
        // mipmaps included
    }

    if(unsupported)
    {
        return 0;
    }

    if(header.dxtCompressionType != 0)
    {
        colorFormat = ECF_A8R8G8B8;
        bytesPerPixel = 4;
    }

    u32 imageSize = header.imageWidth * header.imageHeight * bytesPerPixel;
    u32 rasterSize = file->getSize() - sizeof(SGTATextureHeader);

	u8* raster = new u8[rasterSize];
	u8* data = new u8[imageSize];

	file->read(raster, rasterSize);

    u8 *fourCC = (u8 *) &header.alphaOrFourCC;
    if(fourCC[0] == 'D' && fourCC[1] == 'X' && fourCC[2] == 'T')
    {
        if(fourCC[3] == '1') {header.dxtCompressionType = 1;}
        else if(fourCC[3] == '3') {header.dxtCompressionType = 3;}
        else if(fourCC[3] == '5') {header.dxtCompressionType = 5;}
    }

    if(rasterFormatExt == FORMAT_EXT_PAL8 || rasterFormatExt == FORMAT_EXT_PAL4)
    {
        // paletted pixels
        u32 paletteSize = 256;
        if(rasterFormatExt == FORMAT_EXT_PAL4) {paletteSize = 16;}

        u8 *palette = new u8[paletteSize*4];

        u32 readoffset = 0, writeoffset = 0;

        for(readoffset=0; readoffset < paletteSize*4; readoffset++)
        {
            palette[readoffset] = raster[readoffset];
        }

        // skip 4 bytes of unneeded raster size data
        readoffset += 4;

        if(bytesPerPixel == 3)
        {
            for(; writeoffset < imageSize; readoffset++)
            {
                data[writeoffset++] = palette[(raster[readoffset]*4)];
                data[writeoffset++] = palette[(raster[readoffset]*4)+1];
                data[writeoffset++] = palette[(raster[readoffset]*4)+2];
            }
        }
        else if(bytesPerPixel == 4)
        {
            for(; writeoffset < imageSize; readoffset++)
            {
                data[writeoffset++] = palette[(raster[readoffset]*4)];
                data[writeoffset++] = palette[(raster[readoffset]*4)+1];
                data[writeoffset++] = palette[(raster[readoffset]*4)+2];
                data[writeoffset++] = palette[(raster[readoffset]*4)+3];
            }
        }

        delete [] palette;
    }
    else if(rasterFormatExt == FORMAT_4444)
    {
        u32 readoffset = 0, writeoffset = 0;

        u8 colorRG;
        u8 colorBA;

        u8 r, g, b, a;

        // skip 4 bytes of unneeded raster size data
        readoffset += 4;

        for(; readoffset < rasterSize;)
        {
			colorRG = raster[readoffset++];
			colorBA = raster[readoffset++];

			r = colorRG >> 4 << 4;
			g = colorRG << 4;
			b = colorBA >> 4 << 4;
			a = colorBA << 4;

            data[writeoffset++] = raster[readoffset++];
        }
	}
    else if(rasterFormatExt == FORMAT_LUM8)
    {
        u32 readoffset = 0, writeoffset = 0;

        // skip 4 bytes of unneeded raster size data
        readoffset += 4;

        for(; readoffset < rasterSize;)
        {
            u8 grey = raster[readoffset++];

            data[writeoffset++] = grey;
            data[writeoffset++] = grey;
            data[writeoffset++] = grey;
        }
	}
    else if(header.dxtCompressionType != 0)
    {
        if(header.dxtCompressionType == 1)
        {
			ff_decode_dxt1(raster+4, data, header.imageWidth, header.imageHeight, header.imageWidth * 4);
        }
        else if(header.dxtCompressionType == 3)
        {
			ff_decode_dxt3(raster+4, data, header.imageWidth, header.imageHeight, header.imageWidth * 4);
        }
        else if(header.dxtCompressionType == 5)
        {
			printf("DXT5 is currently not supported\n");
        }
    }
    else
    {
        // don't even know if uncompressed non-palette data exists, but i support it just in case

        u32 readoffset = 0, writeoffset = 0;

        // skip 4 bytes of unneeded raster size data
        readoffset += 4;

        for(; writeoffset < imageSize;)
        {
            data[writeoffset++] = raster[readoffset++];
        }
    }

    IImage *image = Device->getVideoDriver()->createImageFromData(colorFormat, core::dimension2d<u32>(header.imageWidth, header.imageHeight), data);

    // data gets deleted by CImage when not needed anymore, so no need to delete it
	delete [] raster;

	return image;
}
