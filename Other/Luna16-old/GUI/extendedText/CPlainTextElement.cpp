#include "CPlainTextElement.h"

#include <IGUIFont.h>

namespace irr
{
namespace gui
{

CPlainTextElement::CPlainTextElement( const wchar_t* text, const core::dimension2d<s32>& dim, video::SColor color, IGUIFont* font )
{
    Text = text;
    Size = dim;
    Color = color;
    Font = font;
}

//! Returns the width and height of the text element.
const core::dimension2d<s32>& CPlainTextElement::getDimension()
{
    return Size;
}

//! Called when the text element should draw itself.
//! \param area: Absolute screen coordinates where the element should draw itself.
//! \param clip: Clipping area. The element should not draw outside the clip area.
//! \param driver: Video driver used to draw with.
void CPlainTextElement::draw( const core::rect<s32>& area, const core::rect<s32>& clip, video::IVideoDriver* driver )
{
    Font->draw( Text.c_str(), area, Color, false, false, &clip );
}
    
//! Appends text to this element.
void CPlainTextElement::append( const wchar_t* text, const core::dimension2d<s32>& dim )
{
    Text += text;
    Size.Width += dim.Width;
    if ( Size.Height < dim.Height )
        Size.Height = dim.Height;
}

} // namespace gui
} // namespace irr
