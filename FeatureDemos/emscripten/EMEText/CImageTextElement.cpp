#include "CImageTextElement.h"

#include <IVideoDriver.h>

#include "clipRects.h"

namespace irr
{
namespace gui
{

CImageTextElement::CImageTextElement( video::ITexture* texture, const core::dimension2d<s32>& size )
{
    Texture = texture;
    Size = size;
}

//! Returns the width and height of the text element.
const core::dimension2d<s32>& CImageTextElement::getDimension()
{
    return Size;
}

//! Called when the text element should draw itself.
//! \param area: Absolute screen coordinates where the element should draw itself.
//! \param clip: Clipping area. The element should not draw outside the clip area.
//! \param driver: Video driver used to draw with.
void CImageTextElement::draw( const core::rect<s32>& area, const core::rect<s32>& clip, video::IVideoDriver* driver )
{
    if ( Texture != NULL )
    {
        // Workaround for ignored clipping area
        core::rect<s32> dstRect = area;
        core::rect<s32> srcRect = core::rect<s32>( core::position2d<s32>(0,0), Texture->getSize() );
        
        if ( clipRects( dstRect, srcRect, clip ) )
        {
            driver->draw2DImage(
                    Texture,
                    dstRect,
                    srcRect,
                    NULL,
                    NULL,
                    true );
        }
    }
}

} // namespace gui
} // namespace irr
