
#ifndef GCN_IRRLICHT_HPP
#define GCN_IRRLICHT_HPP

#include <guichan/irrlicht/irrlichtgraphics.hpp>
#include <guichan/irrlicht/irrlichtimage.hpp>
#include <guichan/irrlicht/irrlichtimageloader.hpp>
#include <guichan/irrlicht/irrlichtinput.hpp>

#include <guichan/platform.hpp>

extern "C"
{
    /**
     * Exists to be able to check for Guichan Irrlicht with autotools.
     */
    GCN_EXTENSION_DECLSPEC extern void gcnIrrlicht();
}

#endif // end GCN_IRRLICHT_HPP
