
/*
 * For comments regarding functions please see the header file.
 */

#include "guichan/irrlicht/irrlichtimage.hpp"

#include "guichan/exception.hpp"

namespace gcn
{
    IrrlichtImage::IrrlichtImage(irr::video::IImage* image,
                                 irr::video::IVideoDriver* driver,
                                 const std::string& name,
                                 bool autoFree,
                                 bool convertToDisplayFormat)
    {
        mTexture = NULL;
        mImage = image;
        mDriver = driver;
        mName = name;
        mAutoFree = autoFree;

        if (mDriver)
        {
            driver->grab();
        }

        if (convertToDisplayFormat)
        {
            IrrlichtImage::convertToDisplayFormat();
        }
    }

    IrrlichtImage::~IrrlichtImage()
    {
        if (mAutoFree)
        {
            free();
        }

        if (mDriver)
        {
            mDriver->drop();
        }
    }

    irr::video::ITexture* IrrlichtImage::getTexture() const
    {
        return mTexture;
    }

    int IrrlichtImage::getWidth() const
    {
        if (mTexture)
        {
            return mTexture->getSize().Width;
        }
        else if (mImage)
        {
            return mImage->getDimension().Width;
        }

        throw GCN_EXCEPTION("Trying to get the width of a non loaded image.");
    }

    int IrrlichtImage::getHeight() const
    {
        if (mTexture)
        {
            return mTexture->getSize().Height;
        }
        else if (mImage)
        {
            return mImage->getDimension().Height;
        }

        throw GCN_EXCEPTION("Trying to get the height of a non loaded image.");
    }

    Color IrrlichtImage::getPixel(int x, int y)
    {
        if (mImage == NULL)
        {
            throw GCN_EXCEPTION("Image has been converted to display format.");
        }

        irr::video::SColor color = mImage->getPixel(x, y);
        return Color(color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());
    }

    void IrrlichtImage::putPixel(int x, int y, const Color& color)
    {
        if (mImage == NULL)
        {
            throw GCN_EXCEPTION("Image has been converted to display format.");
        }

        mImage->setPixel(x, y, irr::video::SColor(color.a, color.r, color.g, color.b));
    }

    void IrrlichtImage::convertToDisplayFormat()
    {
        if (mTexture != NULL)
        {
            return;
        }

        if (mImage == NULL)
        {
            throw GCN_EXCEPTION("Trying to convert a non loaded image to display format.");
        }

        mTexture = mDriver->addTexture(mName.c_str(), mImage);

        if (mTexture == NULL)
        {
            throw GCN_EXCEPTION("Unable to convert image to display format!");
        }

        mDriver->makeColorKeyTexture(mTexture, irr::video::SColor(0,255,0,255));

        mImage->drop();
        mImage = NULL;
    }

    void IrrlichtImage::free()
    {
        if (mImage != NULL)
        {
            mImage->drop();
        }
    }
}
