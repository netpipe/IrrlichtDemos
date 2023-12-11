#ifndef _C_IMAGE_TEXT_ELEMENT_H_
#define _C_IMAGE_TEXT_ELEMENT_H_

#include "ITextElement.h"

namespace irr
{
namespace video
{
    class ITexture;
}
namespace gui
{

class CImageTextElement : public ITextElement
{
public:
    CImageTextElement( video::ITexture* texture, const core::dimension2d<s32>& size );
    
    //! Returns the width and height of the text element.
    const core::dimension2d<s32>& getDimension();
    
    //! Called when the text element should draw itself.
    //! \param area: Absolute screen coordinates where the element should draw itself.
    //! \param clip: Clipping area. The element should not draw outside the clip area.
    //! \param driver: Video driver used to draw with.
    void draw( const core::rect<s32>& area, const core::rect<s32>& clip, video::IVideoDriver* driver );
    
private:
    video::ITexture* Texture;
    core::dimension2d<s32> Size;
};

} // namespace gui
} // namespace irr

#endif
