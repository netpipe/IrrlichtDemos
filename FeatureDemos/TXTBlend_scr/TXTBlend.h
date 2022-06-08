#ifndef TXTBLEND_H
#define TXTBLEND_H

#include <irrlicht.h>
using namespace irr;

class IImageData
{
    u32* data;
    core::dimension2d<s32> sz;

public:
    IImageData(core::dimension2d<s32> size) : sz(size)
    {
        data = new u32[sz.Width * sz.Height];
    }

    ~IImageData() { delete [] data; }

    u32* operator () () { return data; }

    void set(s32 width, s32 height, video::SColor color)
    {
        data[height*sz.Width + width]  = color.color;
    }

    core::dimension2d<s32> size() { return sz; }
};

class TXTBlend
{
    video::IVideoDriver* driver;

    video::IImage* image1;
	video::IImage* image2;

	core::string<c8> basename;

	video::SColor interpolate(video::SColor color1, video::SColor color2, f32 amount)
    {
        f32 invamount = 1.0f - amount;

        u32 red = (u32)(color1.getRed()*invamount + color2.getRed()*amount);
        u32 green = (u32)(color1.getGreen()*invamount + color2.getGreen()*amount);
        u32 blue = (u32)(color1.getBlue()*invamount + color2.getBlue()*amount);

        video::SColor color = video::SColor(255, red, green, blue);
        return color;
    }

    void merge(video::IImage* image1, video::IImage* image2, IImageData& data, s32 method)
    {
        for(s32 j=0; j<data.size().Height; j++)
            for(s32 i=0; i<data.size().Width; i++)
            {
                f32 amount = getMergingKoeficient(data.size(), i, j, method);

                video::SColor color;
                color = interpolate(image1->getPixel(i, j), image2->getPixel(i, j), amount);

                data.set(i, j, color);
            }
    }

    f32 getMergingKoeficient(core::dimension2d<s32> size, s32 width, s32 height, s32 method)
    {
        switch(method)
        {
            case 1:
                return getMKLeft(size, width, height);
                break;
            case 2:
                return getMKRight(size, width, height);
                break;
            case 3:
                return getMKTop(size, width, height);
                break;
            case 4:
                return getMKBottom(size, width, height);
                break;

            case 5:
                return getMKTopLeft(size, width, height);
                break;
            case 6:
                return getMKTopRight(size, width, height);
                break;
            case 7:
                return getMKBottomRight(size, width, height);
                break;
            case 8:
                return getMKBottomLeft(size, width, height);
                break;

            case 9:
                return getMKTopLeftReverse(size, width, height);
                break;
            case 10:
                return getMKTopRightReverse(size, width, height);
                break;
            case 11:
                return getMKBottomRightReverse(size, width, height);
                break;
            case 12:
                return getMKBottomLeftReverse(size, width, height);
                break;

            case 13:
                return getMKTopLeftBottomRight(size, width, height);
                break;
            case 14:
                return getMKTopRightBottomLeft(size, width, height);
                break;
        }
    }

    f32 getMKLeft(core::dimension2d<s32> size, s32 width, s32 height)
    {
        size.Width--;
        return (f32)(width) / (f32)(size.Width);
    }

    f32 getMKRight(core::dimension2d<s32> size, s32 width, s32 height)
    {
        size.Width--;
        return 1.0f - (f32)(width) / (f32)(size.Width);
    }

    f32 getMKTop(core::dimension2d<s32> size, s32 width, s32 height)
    {
        size.Height--;
        return (f32)(height) / (f32)(size.Height);
    }

    f32 getMKBottom(core::dimension2d<s32> size, s32 width, s32 height)
    {
        size.Height--;
        return 1.0f - (f32)(height) / (f32)(size.Height);
    }

    f32 getMKTopLeft(core::dimension2d<s32> size, s32 width, s32 height)
    {
        size.Width--;
        f32 r = (f32)(width) / (f32)(size.Width);

        size.Height--;
        f32 p = (f32)(height) / (f32)(size.Height);

        if(r>p) return r;
        return p;
    }

    f32 getMKTopRight(core::dimension2d<s32> size, s32 width, s32 height)
    {
        size.Width--;
        f32 r = 1.0f -(f32)(width) / (f32)(size.Width);

        size.Height--;
        f32 p = (f32)(height) / (f32)(size.Height);

        if(r>p) return r;
        return p;
    }

    f32 getMKBottomRight(core::dimension2d<s32> size, s32 width, s32 height)
    {
        size.Width--;
        f32 r = 1.0f -(f32)(width) / (f32)(size.Width);

        size.Height--;
        f32 p = 1.0f - (f32)(height) / (f32)(size.Height);

        if(r>p) return r;
        return p;
    }

    f32 getMKBottomLeft(core::dimension2d<s32> size, s32 width, s32 height)
    {
        size.Width--;
        f32 r = (f32)(width) / (f32)(size.Width);

        size.Height--;
        f32 p = 1.0f - (f32)(height) / (f32)(size.Height);

        if(r>p) return r;
        return p;
    }

    f32 getMKTopLeftReverse(core::dimension2d<s32> size, s32 width, s32 height)
    {
        size.Width--;
        f32 r = (f32)(width) / (f32)(size.Width);

        size.Height--;
        f32 p = (f32)(height) / (f32)(size.Height);

        if(r<p) return r;
        return p;
    }

    f32 getMKTopRightReverse(core::dimension2d<s32> size, s32 width, s32 height)
    {
        size.Width--;
        f32 r = 1.0f -(f32)(width) / (f32)(size.Width);

        size.Height--;
        f32 p = (f32)(height) / (f32)(size.Height);

        if(r<p) return r;
        return p;
    }

    f32 getMKBottomRightReverse(core::dimension2d<s32> size, s32 width, s32 height)
    {
        size.Width--;
        f32 r = 1.0f -(f32)(width) / (f32)(size.Width);

        size.Height--;
        f32 p = 1.0f - (f32)(height) / (f32)(size.Height);

        if(r<p) return r;
        return p;
    }

    f32 getMKBottomLeftReverse(core::dimension2d<s32> size, s32 width, s32 height)
    {
        size.Width--;
        f32 r = (f32)(width) / (f32)(size.Width);

        size.Height--;
        f32 p = 1.0f - (f32)(height) / (f32)(size.Height);

        if(r<p) return r;
        return p;
    }

    f32 getMKTopLeftBottomRight(core::dimension2d<s32> size, s32 width, s32 height)
    {
        size.Width--;
        f32 r = (f32)(width) / (f32)(size.Width);

        size.Height--;
        f32 p = (f32)(height) / (f32)(size.Height);

        f32 t = 1.0f - r;

        f32 s = 1.0f - p;

        f32 f = r;
        if(f<p) f = p;

        f32 e = t;
        if(e<s) e = s;

        if(f<e) return f;
        return e;
    }

    f32 getMKTopRightBottomLeft(core::dimension2d<s32> size, s32 width, s32 height)
    {
        size.Width--;
        f32 t = (f32)(width) / (f32)(size.Width);

        size.Height--;
        f32 p = (f32)(height) / (f32)(size.Height);

        f32 r = 1.0f - t;

        f32 s = 1.0f - p;

        f32 f = r;
        if(f<p) f = p;

        f32 e = t;
        if(e<s) e = s;

        if(f<e) return f;
        return e;
    }

public:

    TXTBlend(video::IVideoDriver* idriver) : driver(idriver)
    {
        image1 = NULL;
        image2 = NULL;
    }

    ~TXTBlend()
    {
        if(image1) image1->drop();
        if(image2) image2->drop();
    }

    bool loadImage1(const c8 *filename)
    {
        if(image1) image1->drop();
        if(image1 = driver->createImageFromFile(filename)) return true;
        return false;
    }

    bool loadImage2(const c8 *filename)
    {
        if(image2) image2->drop();
        if(image2 = driver->createImageFromFile(filename)) return true;
        return false;
    }

    void setFileName(const c8 *filename)
    {
        basename = filename;
    }

    void writeFile(const c8* filetype)
    {
        if(image1 && image2 && image1->getDimension() == image2->getDimension())
        {
            video::ECOLOR_FORMAT  format = video::ECF_A8R8G8B8;

            IImageData data(image1->getDimension());

            for(s32 method=1; method<15; method++)
            {
                merge(image1, image2, data, method);
                video::IImage* image3 = driver->createImageFromData(format, data.size(), data());

                core::string<c8> count;
                if(method<10) count += "0";
                count += core::string<c8>(method);
                core::string<c8> filename = basename + count + "." + filetype;
                driver->writeImageToFile(image3, filename.c_str());
                image3->drop();
            }
        }
    }

    bool texture1Loaded()
    {
        if(image1) return true;
        return false;
    }

    bool texture2Loaded()
    {
        if(image2) return true;
        return false;
    }

    core::dimension2du getTexture1Size()
    {
        return image1->getDimension();
    }

    core::dimension2du getTexture2Size()
    {
        return image2->getDimension();
    }

    bool texturesSizeIsEqual()
    {
        if(image1->getDimension() == image2->getDimension()) return true;
        return false;
    }
};
#endif
