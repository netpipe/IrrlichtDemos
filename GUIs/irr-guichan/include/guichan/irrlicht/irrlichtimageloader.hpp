
#ifndef GCN_IRRLICHTIMAGELOADER_HPP
#define GCN_IRRLICHTIMAGELOADER_HPP

#include "guichan/imageloader.hpp"
#include "guichan/platform.hpp"

#include "irrlicht.h"

namespace gcn
{
    class Image;

    /**
     * Irrlicht implementation of ImageLoader.
     */
    class GCN_EXTENSION_DECLSPEC IrrlichtImageLoader : public ImageLoader
    {
    public:
        /**
        * Constructor.
        *
        * @param device The Irrlicht IVideoDriver device to use when loading images.
        */
        IrrlichtImageLoader(irr::video::IVideoDriver* driver);

        /**
        * Destructor.
        */
        ~IrrlichtImageLoader();

        // Inherited from ImageLoader

        virtual Image* load(const std::string& filename, bool convertToDisplayFormat = true);

    protected:
        /**
        * Holds the Irrlicht IVideoDriver to use when loading images.
        */
        irr::video::IVideoDriver* mDriver;
    };
}

#endif // end GCN_IRRLICHTIMAGELOADER_HPP
