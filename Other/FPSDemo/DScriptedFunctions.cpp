#include "DScriptedFunctions.h"

using namespace agEngine;
using namespace script;
using namespace entities;

int s_getTargetedEntity(lua_State* L)
{
    CBaseEntity* temp = CScriptManager::instance->emgr->getTargetedEntity(
        CScriptManager::instance->emgr->getEntityByName(luaL_checkstring(L, 1)));

    if (!temp)
        return 0;

    lua_pushstring(L, temp->getStringParam(nodeName).c_str());

    return 1;
}

int s_modTargetIntParameter(lua_State* L)
{
    CBaseEntity* temp = CScriptManager::instance->emgr->getEntityByName(luaL_checkstring(L, 1));

    if (temp)
        temp->modIntParam(luaL_checkint(L, 2), luaL_checkint(L, 3));

    return 0;
}

int s_modTargetFloatParameter(lua_State* L)
{
    CBaseEntity* temp = CScriptManager::instance->emgr->getEntityByName(luaL_checkstring(L, 1));

    if (temp)
        temp->modFloatParam(luaL_checkint(L, 2), luaL_checknumber(L, 3));

    return 0;
}

void s_registerScriptFunctions(lua_State* L)
{
    // Entity functions
    lua_register(L, "getTarget", s_getTargetedEntity);
    lua_register(L, "modTargetInt", s_modTargetIntParameter);
    lua_register(L, "modTargetFloat", s_modTargetFloatParameter);
}
