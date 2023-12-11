#ifndef _C_ALIGN_TEXT_TAG_H_
#define _C_ALIGN_TEXT_TAG_H_

#include "ITextTag.h"

namespace irr
{
namespace gui
{

class CAlignTextTag : public ITextTag
{
public:
    virtual bool match( const core::stringw& tag );
    
    virtual void create( IExtendedText* text, const core::stringw& tag, const core::array<core::stringw>& params );
};

} // namespace gui
} // namespace irr

#endif
