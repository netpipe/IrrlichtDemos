#ifndef _C_H_LINE_TEXT_ELEMENT_H_
#define _C_H_LINE_TEXT_ELEMENT_H_

#include "ITextElement.h"
#include <SColor.h>

namespace irr
{
namespace gui
{

class CHLineTextElement : public ITextElement
{
public:
    CHLineTextElement( const core::dimension2d<s32>& size, video::SColor color, s32 lineWidth );
    
    //! Returns the width and height of the text element.
    virtual const core::dimension2d<s32>& getDimension();
    
    //! Called when the text element should draw itself.
    //! \param area: Absolute screen coordinates where the element should draw itself.
    //! \param clip: Clipping area. The element should not draw outside the clip area.
    //! \param driver: Video driver used to draw with.
    virtual void draw( const core::rect<s32>& area, const core::rect<s32>& clip, video::IVideoDriver* driver );

private:
    core::dimension2d<s32> Size;
    video::SColor Color;
    s32 LineWidth;
};

} // namespace gui
} // namespace irr

#endif
