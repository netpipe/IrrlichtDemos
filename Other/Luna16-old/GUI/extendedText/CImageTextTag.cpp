#include "CImageTextTag.h"
#include "IExtendedText.h"
#include "CImageTextElement.h"

namespace irr
{
namespace gui
{

bool CImageTextTag::match( const core::stringw& tag )
{
    return ( tag == L"img" );
}

void CImageTextTag::create( IExtendedText* text, const core::stringw& tag, const core::array<core::stringw>& params )
{
    if ( params.size() == 0 )
        return;
    
    core::map<core::stringw,SInlineImage>::Node* node = ImageMap.find( params[0] );
    
    if ( node != NULL )
    {
        CImageTextElement* element = new CImageTextElement( node->getValue().Texture, node->getValue().Size );
        
        // Left anchor?
        if ( params.size() > 1 && params[1] == L"left" )
        {
            text->insertAnchoredTextElement( element, true );
        }
        // Right anchor?
        else if ( params.size() > 1 && params[1] == L"right" )
        {
            text->insertAnchoredTextElement( element, false );
        }
        // Normal inline image
        else
        {
            text->insertTextElement( element );
        }
    }
}

void CImageTextTag::addImage( const wchar_t* name, video::ITexture* texture, const core::dimension2d<s32>& size )
{
    SInlineImage img;
    
    img.Texture = texture;
    img.Size = size;
    
    ImageMap.insert( core::stringw(name), img );
}

} // namespace gui
} // namespace irr
