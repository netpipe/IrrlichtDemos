#ifndef _C_H_LINE_TEXT_TAG_H_
#define _C_H_LINE_TEXT_TAG_H_

#include "ITextTag.h"

namespace irr
{
namespace gui
{

class CHLineTextTag : public ITextTag
{
public:
    virtual bool match( const core::stringw& tag );
    
    virtual void create( IExtendedText* text, const core::stringw& tag, const core::array<core::stringw>& params );
};

} // namespace gui
} // namespace irr

#endif
