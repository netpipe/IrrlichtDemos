#ifndef _C_TAB_TEXT_TAG_H_
#define _C_TAB_TEXT_TAG_H_

#include "ITextTag.h"

namespace irr
{
namespace gui
{

class CTabTextTag : public ITextTag
{
public:
    //! Creates a new tab text tag. The default syntax is #tab# for a 12-pixel tabulator, and #tab:xxxx# for a 'xxxx'-pixel tabulator.
    //! \param tag: The tag used to identify the tab. Default is "tab".
    //! \param tabSize: Default size of the tab if no parameter is given (#tab:15# will always use 15-pixels, for example).
    CTabTextTag( const wchar_t* tag=L"tab", s32 tabSize=12 );
    
    virtual bool match( const core::stringw& tag );
    
    virtual void create( IExtendedText* text, const core::stringw& tag, const core::array<core::stringw>& params );
    
private:
    core::stringw Tag;
    s32 TabSize;
};

} // namespace gui
} // namespace irr

#endif
