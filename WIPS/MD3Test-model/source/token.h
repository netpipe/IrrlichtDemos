#ifndef __C_TOKEN_H_INCLUDED__
#define __C_TOKEN_H_INCLUDED__

#include <irrString.h>

namespace irr
{
namespace scene
{
namespace quake3
{

#define	MAX_TOKEN_CHARS		1024

enum E_Q3_TOKEN
{
    ETK_TOKEN_UNRESOLVED = 0,
    ETK_TOKEN_EOF = 1,
    ETK_TOKEN_START_LIST,
    ETK_TOKEN_END_LIST,
    ETK_TOKEN_ENTITY,
    ETK_TOKEN_TOKEN,
    ETK_TOKEN_EOL
};


class Token : public virtual IReferenceCounted
{
public:
    Token() : Type(ETK_TOKEN_UNRESOLVED), Value("") {}

    Token(E_Q3_TOKEN type, core::stringc value = "")  : Type(type), Value(value) {}

    Token(E_Q3_TOKEN type, const c8 * value = 0)  : Type(type), Value(value) {}

    void setType(E_Q3_TOKEN type) { Type = type; }

    E_Q3_TOKEN getType() const { return Type; }

    void setValue(const c8 * type) { Value = type; }

    void setValue(core::stringc value) { Value = value; }

    const core::stringc & getValue() const { return Value; }

private:
    E_Q3_TOKEN Type;
    core::stringc Value;
};

} // end namespace quake3
} // end namespace scene
} // end namespace irr

#endif


