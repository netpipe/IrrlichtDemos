#ifndef _C_FONT_TEXT_TAG_H_
#define _C_FONT_TEXT_TAG_H_

#include "ITextTag.h"
#include "irrMap.h"

namespace irr
{
namespace gui
{

class IGUIFont;

class CFontTextTag : public ITextTag
{
public:
    virtual bool match( const core::stringw& tag );
    
    virtual void create( IExtendedText* text, const core::stringw& tag, const core::array<core::stringw>& params );
    
    void addFont( const wchar_t* name, IGUIFont* font );
    
private:
    core::map<core::stringw,IGUIFont*> FontMap;
};

} // namespace gui
} // namespace irr

#endif
