#ifndef __I_PARSER_H_INCLUDED__
#define __I_PARSER_H_INCLUDED__

#include "script.h"

namespace irr
{
namespace scene
{
namespace quake3
{

template <class T>
class IParser : public virtual IReferenceCounted
{
public:

    virtual const Script & script() const { return *PScript; }

    virtual bool isValid() const { return (this->size() > 0); }

    virtual u32 size() const = 0;

    virtual s32 find(const c8 * name) const = 0;

    virtual const T * get(u32 index) const = 0;

    virtual const T * get(s32 index) const
    {
        return ( index >= 0 ? get( (u32) index ) : (const T *) NULL );
    }

    virtual bool exists(const c8 * name) const { return ( find(name) >= 0 ); }

    virtual bool notExists(const c8 * name) const { return ( find(name) < 0 ); }

protected:
    IParser(Script *script) : PScript(script) { if (PScript) PScript->grab(); }

    virtual ~IParser() { if (PScript) PScript->drop(); }

    virtual void parse() = 0;

    virtual Script* getScript() { return PScript; }

private:
    Script *PScript;
};

} // end namespace quake3
} // end namespace scene
} // end namespace irr

#endif
