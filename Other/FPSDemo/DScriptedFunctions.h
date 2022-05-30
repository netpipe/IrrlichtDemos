#ifndef _DSCRIPTEDFUNCTIONS_H_
#define _DSCRIPTEDFUNCTIONS_H_

#include <irrlicht.h>
extern "C"
{
#ifdef WIN32
#include "lua/lua.h"
#include "lua/lauxlib.h"
#include "lua/lualib.h"
#else
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#endif
}

#include "CScriptManager.h"

/************************************************************

    Function prototypes accessible to the scripting engine

*************************************************************/

int s_getTargetedEntity(lua_State* L);
int s_modTargetIntParameter(lua_State* L);
int s_modTargetFloatParameter(lua_State* L);

void s_registerScriptFunctions(lua_State* L);

#endif // _DSCRIPTEDFUNCTIONS_H_

