#include "CTabTextTag.h"

#include "IExtendedText.h"
#include "CBlankTextElement.h"

#include <stdlib.h>

namespace irr
{
namespace gui
{

//! Creates a new tab text tag. The default syntax is #tab# for a 12-pixel tabulator, and #tab:xxxx# for a 'xxxx'-pixel tabulator.
//! \param tag: The tag used to identify the tab. Default is "tab".
//! \param tabSize: Default size of the tab if no parameter is given (#tab:15# will always use 15-pixels, for example).
CTabTextTag::CTabTextTag( const wchar_t* tag, s32 tabSize )
{
    Tag = tag;
    TabSize = tabSize;

    if ( TabSize <= 0 ) // Avoid division by zero later
        TabSize = 1;
}

bool CTabTextTag::match( const core::stringw& tag )
{
    return ( Tag == tag );
}

void CTabTextTag::create( IExtendedText* text, const core::stringw& tag, const core::array<core::stringw>& params )
{
    s32 size = TabSize;


    // Find the space used to far
    s32 remaining = text->getLineSpaceRemaining();

    if ( remaining <= size )
    {
        text->insertLineBreak();
    }
    else
    {
        s32 used = text->getTotalLineSpace() - remaining;

        s32 indent = size - ( used % size );

        text->insertTextElement( new CBlankTextElement( core::dimension2d<s32>( indent, 1 ) ) );
    }
}

} // namespace gui
} // namespace irr
