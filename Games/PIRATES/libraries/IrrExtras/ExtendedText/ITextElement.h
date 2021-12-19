#ifndef _I_TEXT_ELEMENT_H_
#define _I_TEXT_ELEMENT_H_

#include <dimension2d.h>
#include <rect.h>

namespace irr
{

namespace video
{
    class IVideoDriver;
}

namespace gui
{

class ITextElement
{
public:
    //! Virtual destructor
    virtual ~ITextElement() {}
    
    //! Returns the width and height of the text element.
    virtual const core::dimension2d<s32>& getDimension() = 0;
    
    //! Called when the text element should draw itself.
    //! \param area: Absolute screen coordinates where the element should draw itself.
    //! \param clip: Clipping area. The element should not draw outside the clip area.
    //! \param driver: Video driver used to draw with.
    virtual void draw( const core::rect<s32>& area, const core::rect<s32>& clip, video::IVideoDriver* driver ) = 0;
};

} // namespace gui
} // namespace irr

#endif
