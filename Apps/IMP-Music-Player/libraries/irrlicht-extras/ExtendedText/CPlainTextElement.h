#ifndef _C_PLAIN_TEXT_ELEMENT_H_
#define _C_PLAIN_TEXT_ELEMENT_H_

#include "ITextElement.h"
#include <SColor.h>
#include <irrTypes.h>
#include <irrString.h>

namespace irr
{
namespace gui
{

class IGUIFont;

class CPlainTextElement : public ITextElement
{
public:
    CPlainTextElement( const wchar_t* text, const core::dimension2d<s32>& dim, video::SColor color, IGUIFont* font );
    
    //! Returns the width and height of the text element.
    virtual const core::dimension2d<s32>& getDimension();
    
    //! Called when the text element should draw itself.
    //! \param area: Absolute screen coordinates where the element should draw itself.
    //! \param clip: Clipping area. The element should not draw outside the clip area.
    //! \param driver: Video driver used to draw with.
    virtual void draw( const core::rect<s32>& area, const core::rect<s32>& clip, video::IVideoDriver* driver );
    
    //! Appends text to this element.
    void append( const wchar_t* text, const core::dimension2d<s32>& dim );
    
private:
    core::stringw Text;
    core::dimension2d<s32> Size;
    video::SColor Color;
    IGUIFont* Font;
};

} // namespace gui
} // namespace irr

#endif
