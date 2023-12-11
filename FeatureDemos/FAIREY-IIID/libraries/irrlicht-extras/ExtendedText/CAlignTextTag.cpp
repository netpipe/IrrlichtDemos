#include "CAlignTextTag.h"

#include "IExtendedText.h"

namespace irr
{
namespace gui
{

bool CAlignTextTag::match( const core::stringw& tag )
{
    return ( tag == L"align" );
}

void CAlignTextTag::create( IExtendedText* text, const core::stringw& tag, const core::array<core::stringw>& params )
{
    E_TEXT_ALIGN align = TEXT_ALIGN_LEFT;
    
    if ( params.size() > 0 )
    {
        if ( params[0] == L"center" || params[0] == L"c" )
        {
            align = TEXT_ALIGN_CENTER;
        }
        else
        if ( params[0] == L"right" || params[0] == L"r" )
        {
            align = TEXT_ALIGN_RIGHT;
        }
    }
    
    text->setAlignment( align );
}

} // namespace gui
} // namespace irr
