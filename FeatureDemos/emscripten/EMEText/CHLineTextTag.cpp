#include "CHLineTextTag.h"

#include "IExtendedText.h"
#include "CHLineTextElement.h"

#include <stdlib.h>

namespace irr
{
namespace gui
{

bool CHLineTextTag::match( const core::stringw& tag )
{
    return ( tag == L"hline" );
}

void CHLineTextTag::create( IExtendedText* text, const core::stringw& tag, const core::array<core::stringw>& params )
{
    s32 size = 1;

  //  if ( params.size() > 0 )
  //  {
    //    size = _wtoi( params[0].c_str() );

       // if ( size <= 0 )
   //         size = 1;
   // }

    s32 lineSpace = text->getTotalLineSpace();

    s32 lineHeight = text->getLineHeight();

    if ( lineHeight < size )
        lineHeight = size;

    text->insertTextElement( new CHLineTextElement( core::dimension2d<s32>( lineSpace, lineHeight ), text->getCurrentColor(), size ) );
}

} // namespace gui
} // namespace irr
