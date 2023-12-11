
#ifndef IRR_EXT_RENDER_BIG_IMAGES_H_INCLUDED
#define IRR_EXT_RENDER_BIG_IMAGES_H_INCLUDED

#include <irrlicht.h>

namespace irr
{

bool renderToImage(
    IrrlichtDevice* device,
    video::IImage* dst,
    s32 nSamples = 1,
    const video::SColor& clearColor = video::SColor(0,0,0,0),
    bool renderGUI = false );

bool blitImageToImage(
	video::IImage* dst,
	core::position2di dstPos,
	video::IImage* src,
	const core::recti& srcRect );

} // end namespace irr

#endif
