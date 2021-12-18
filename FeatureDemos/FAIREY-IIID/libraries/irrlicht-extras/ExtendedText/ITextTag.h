#ifndef _I_TEXT_TAG_H_
#define _I_TEXT_TAG_H_

#include <IUnknown.h>
#include <irrString.h>
#include <irrArray.h>

namespace irr
{
namespace gui
{

class IExtendedText;

class ITextTag : virtual public IUnknown
{
public:
    virtual bool match( const core::stringw& tag ) = 0;
    
    virtual void create( IExtendedText* text, const core::stringw& tag, const core::array<core::stringw>& params ) = 0;
};

} // namespace gui
} // namespace irr

#endif
