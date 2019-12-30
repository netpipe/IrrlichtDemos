#ifndef _C_IMAGE_TEXT_TAG_H_
#define _C_IMAGE_TEXT_TAG_H_

#include "ITextTag.h"
#include <dimension2d.h>
#include "irrMap.h"

namespace irr
{
namespace video
{
    class ITexture;
}
namespace gui
{

class CImageTextTag : public ITextTag
{
public:
    virtual bool match( const core::stringw& tag );
    
    virtual void create( IExtendedText* text, const core::stringw& tag, const core::array<core::stringw>& params );
    
    void addImage( const wchar_t* name, video::ITexture* texture, const core::dimension2d<s32>& size );
    
private:
    struct SInlineImage
    {
        video::ITexture* Texture;
        core::dimension2d<s32> Size;
    };
    core::map<core::stringw,SInlineImage> ImageMap;
};

} // namespace gui
} // namespace irr

#endif
