#ifndef __SCRIPT_H_INCLUDED__
#define __SCRIPT_H_INCLUDED__

#include <irrArray.h>
#include <IReadFile.h>

#include "token.h"

namespace irr
{
namespace scene
{
namespace quake3
{

class Script : public virtual IReferenceCounted
{
public:
    Script(io::IReadFile* file, u32 size=0, s32 offset = 0);

    Script(const c8* name, const c8* script, u32 size=0, u32 offset = 0);

    ~Script() {}

    void setScript(io::IReadFile* file, u32 size=0, s32 offset = 0);

    void setScript(const c8* name, const c8* script, u32 size=0, u32 offset = 0);

    const c8* getName() const { return Name.c_str(); }

    void setToLower(bool value = true) { ToLower = value; }

    bool getToLower() { return ToLower; }

    void setAllowLineBreaks(bool value = true) { AllowLineBreaks = value; }

    bool getAllowLineBreaks() { return AllowLineBreaks; }

    core::stringc toString() { return ( Data.size() ? core::stringc(Data.pointer()) : core::stringc("") ); }

    void clear();

    void begin();

    E_Q3_TOKEN nextToken();

    const Token& token() const { return CurrentToken; }

    int line() const { return COM_Lines; }

private:
    core::stringc Name;
    core::array<c8> Data;
    c8* BeginData;
    char COM_Token[MAX_TOKEN_CHARS];
    int  COM_Lines;
    bool ToLower;
    bool AllowLineBreaks;
    Token CurrentToken;
};

} // end namespace quake3
} // end namespace scene
} // end namespace irr

#endif

