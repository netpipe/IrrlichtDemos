#include "CHLineTextElement.h"

#include <IVideoDriver.h>

namespace irr
{
namespace gui
{


CHLineTextElement::CHLineTextElement( const core::dimension2d<s32>& size, video::SColor color, s32 lineWidth )
{
    Size = size;
    Color = color;
    LineWidth = lineWidth;
}

//! Returns the width and height of the text element.
const core::dimension2d<s32>& CHLineTextElement::getDimension()
{
    return Size;
}

//! Called when the text element should draw itself.
//! \param area: Absolute screen coordinates where the element should draw itself.
//! \param clip: Clipping area. The element should not draw outside the clip area.
//! \param driver: Video driver used to draw with.
void CHLineTextElement::draw( const core::rect<s32>& area, const core::rect<s32>& clip, video::IVideoDriver* driver )
{
    s32 h = area.getHeight();

    core::rect<s32> drawArea = core::rect<s32>(
            area.UpperLeftCorner.X,
            area.UpperLeftCorner.Y + (h - LineWidth) / 2,

            area.LowerRightCorner.X,
            area.LowerRightCorner.Y - (h - LineWidth + (LineWidth%2)) / 2 );

    driver->draw2DRectangle( drawArea, Color, Color, Color, Color, &clip );
}

} // namespace gui
} // namespace irr
