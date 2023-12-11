#include "CBlankTextElement.h"

namespace irr
{
namespace gui
{


CBlankTextElement::CBlankTextElement( const core::dimension2d<s32>& size )
{
    Size = size;
}
    
//! Returns the width and height of the text element.
const core::dimension2d<s32>& CBlankTextElement::getDimension()
{
    return Size;
}

//! Called when the text element should draw itself.
//! \param area: Absolute screen coordinates where the element should draw itself.
//! \param clip: Clipping area. The element should not draw outside the clip area.
//! \param driver: Video driver used to draw with.
void CBlankTextElement::draw( const core::rect<s32>& area, const core::rect<s32>& clip, video::IVideoDriver* driver )
{
    // Do nothing.. it is a blank element
}

} // namespace gui
} // namespace irr
