#include "CColorTextTag.h"
#include "IExtendedText.h"

namespace irr
{
namespace gui
{


bool CColorTextTag::match( const core::stringw& tag )
{
    return ( tag == L"col" );
}

void CColorTextTag::create( IExtendedText* text, const core::stringw& tag, const core::array<core::stringw>& params )
{
    if ( params.size() == 0 )
        text->setCurrentColor( text->getDefaultColor() );
    else
    {
        core::map<core::stringw,video::SColor>::Node* node = ColorMap.find( params[0] );
        
        if ( node != NULL )
        {
            text->setCurrentColor( node->getValue() );
        }
    }
}

void CColorTextTag::addColor( const wchar_t* name, video::SColor color )
{
    core::stringw str = name;
    ColorMap.insert( str, color );
}

void CColorTextTag::addDefaultColors()
{
    addColor( L"red",   video::SColor(255,255,0,0) );
    addColor( L"green", video::SColor(255,0,255,0) );
    addColor( L"blue",  video::SColor(255,0,0,255) );
    addColor( L"yellow", video::SColor(255,255,255,0) );
    addColor( L"orange", video::SColor(255,255,128,0) );
    addColor( L"white", video::SColor(255,255,255,255) );
    addColor( L"black", video::SColor(255,0,0,0) );
    addColor( L"gray",  video::SColor(255,128,128,128) );
}

} // namespace gui
} // namespace irr
