#include "CFontTextTag.h"

#include <IGUIFont.h>
#include "IExtendedText.h"

namespace irr
{
namespace gui
{


bool CFontTextTag::match( const core::stringw& tag )
{
    return ( tag == L"font" );
}

void CFontTextTag::create( IExtendedText* text, const core::stringw& tag, const core::array<core::stringw>& params )
{
    if ( params.size() == 0 )
    {
        text->setCurrentFont( text->getDefaultFont() );
    }
    else
    {
        core::map<core::stringw,IGUIFont*>::Node* node = FontMap.find( params[0] );
        
        if ( node != NULL )
        {
            text->setCurrentFont( node->getValue() );
        }
    }
}

void CFontTextTag::addFont( const wchar_t* name, IGUIFont* font )
{
    if ( font != NULL )
    {
        FontMap.insert( core::stringw(name), font );
    }
}

} // namespace gui
} // namespace irr
