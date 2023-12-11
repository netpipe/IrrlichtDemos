
/*
 * For comments regarding functions please see the header file.
 */

#include "guichan/irrlicht/irrlichtimageloader.hpp"

#include "guichan/exception.hpp"
#include "guichan/irrlicht/irrlichtimage.hpp"

namespace gcn
{
    IrrlichtImageLoader::IrrlichtImageLoader(irr::video::IVideoDriver* driver) : mDriver(driver)
    {
        if (mDriver)
        {
            mDriver->grab();
        }
    }
    
    IrrlichtImageLoader::~IrrlichtImageLoader()
    {
        if (mDriver)
        {
            mDriver->drop();
        }
    }

    Image* IrrlichtImageLoader::load(const std::string& filename, bool convertToDisplayFormat)
    {
        irr::video::IImage* image = mDriver->createImageFromFile(filename.c_str());

        if (image == NULL)
        {
            throw GCN_EXCEPTION(std::string("Unable to load image file: ") + filename);
        }

        return new IrrlichtImage(image, mDriver, filename, true, convertToDisplayFormat);
    }
}
