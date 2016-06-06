#ifndef _C_COLOR_TEXT_TAG_H_
#define _C_COLOR_TEXT_TAG_H_

#include "ITextTag.h"
#include "irrMap.h"
#include <SColor.h>

namespace irr
{
namespace gui
{

class CColorTextTag : public ITextTag
{
public:
    virtual bool match( const core::stringw& tag );
    
    virtual void create( IExtendedText* text, const core::stringw& tag, const core::array<core::stringw>& params );
    
    void addColor( const wchar_t* name, video::SColor color );
    
    void addDefaultColors();

private:
    core::map<core::stringw,video::SColor> ColorMap;
};

} // namespace gui
} // namespace irr

#endif
