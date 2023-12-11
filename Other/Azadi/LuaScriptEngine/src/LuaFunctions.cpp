#include "LuaEngine.h"

#include <cmath>
#include <unistd.h>
#ifdef WIN32
#include <windows.h>
#endif

int l_print(lua_State *L)
{
    LuaEngine::getInstance()->getAzadiInstance()->outputConsole(std::string(lua_checkstring(L, 1)));
    return 1;
}

int l_includeScript(lua_State *L)
{
    LuaEngine::getInstance()->includeFile(lua_checkstring(L, 1));
    return 1;
}

/*============================================

    Misc functions

=============================================*/

int l_wait(lua_State *L)
{
    #ifdef WIN32
    timeBeginPeriod(1);
    Sleep(unsigned(lua_checknumber(L, 1)));
    timeEndPeriod(1);
    #else
    usleep(unsigned(lua_checknumber(L, 1)) * 1000);
    #endif
    return 1;
}

int l_getfps(lua_State *L)
{
    lua_pushnumber(L, LuaEngine::getInstance()->getDrawingEngine()->getFps());
    return 1;
}

int l_getTicks(lua_State *L)
{
    lua_pushnumber(L, LuaEngine::getInstance()->getAzadiInstance()->getTicks());
    return 1;
}

/*===========================================

    Cursor functions

===========================================*/

int l_captureCursor(lua_State *L)
{
    LuaEngine::getInstance()->getAzadiInstance()->captureCursor();
    return 1;
}

int l_releaseCursor(lua_State *L)
{
    LuaEngine::getInstance()->getAzadiInstance()->releaseCursor();
    return 1;
}

/*===========================================

    Building functions

 ===========================================*/

int l_newBuilding(lua_State *L)
{
    lua_pushnumber(L, LuaEngine::getInstance()->newBuilding(std::string(lua_checkstring(L, 1)), std::string(lua_checkstring(L, 2))));
    return 1;
}

int l_setBuildingScr(lua_State *L)
{
    LuaEngine::getInstance()->setBuildingScript(unsigned(lua_checknumber(L, 1)), std::string(lua_checkstring(L, 2)));
    return 1;
}

int l_setBuildingPos(lua_State *L)
{
    LuaEngine::getInstance()->moveBuilding(unsigned(lua_checknumber(L, 1)), lua_checknumber(L, 2), lua_checknumber(L, 3), lua_checknumber(L, 4));
    return 1;
}

int l_setBuildingRot(lua_State *L)
{
    LuaEngine::getInstance()->rotateBuilding(unsigned(lua_checknumber(L, 1)), lua_checknumber(L, 2), lua_checknumber(L, 3), lua_checknumber(L, 4));
    return 1;
}

int l_flushBuildings(lua_State *L)
{
    LuaEngine::getInstance()->flushBuildings();
    return 1;
}

int l_executeBuildingScripts(lua_State *L)
{
    LuaEngine::getInstance()->runBuildingScripts();
    return 1;
}

int l_setBuildingAnimation(lua_State *L)
{
    if (!(lua_toboolean(L, 4)))
        LuaEngine::getInstance()->setBuildingAnim(unsigned(lua_checknumber(L, 1)), lua_checkstring(L, 2), lua_checknumber(L, 3));
    else
        LuaEngine::getInstance()->setBuildingAnimCycle(unsigned(lua_checknumber(L, 1)), lua_checkstring(L, 2), lua_checknumber(L, 3));

    return 1;
}

int l_stopBuildingAnimation(lua_State *L)
{
    LuaEngine::getInstance()->stopBuildingAnim(unsigned(lua_checknumber(L, 1)), lua_checkstring(L, 2));

    return 1;
}

/*==========================================

    Unit related functions

==========================================*/

int l_newUnit(lua_State *L)
{
    lua_pushnumber(L, LuaEngine::getInstance()->newUnit(std::string(lua_checkstring(L, 1)), std::string(lua_checkstring(L, 2))));
    return 1;
}

int l_setUnitScr(lua_State *L)
{
    LuaEngine::getInstance()->setUnitScript(unsigned(lua_checknumber(L, 1)), std::string(lua_checkstring(L, 2)));
    return 1;
}

int l_setUnitPos(lua_State *L)
{
    LuaEngine::getInstance()->moveUnit(unsigned(lua_checknumber(L, 1)), lua_checknumber(L, 2), lua_checknumber(L, 3), lua_checknumber(L, 4));
    return 1;
}

int l_setUnitRot(lua_State *L)
{
    LuaEngine::getInstance()->rotateUnit(unsigned(lua_checknumber(L, 1)), lua_checknumber(L, 2), lua_checknumber(L, 3), lua_checknumber(L, 4));
    return 1;
}

int l_flushUnits(lua_State *L)
{
    LuaEngine::getInstance()->flushUnits();
    return 1;
}

int l_executeUnitScripts(lua_State *L)
{
    LuaEngine::getInstance()->runUnitScripts();
    return 1;
}

int l_setUnitAnimation(lua_State *L)
{
    if (!(lua_toboolean(L, 4)))
        LuaEngine::getInstance()->setUnitAnim(unsigned(lua_checknumber(L, 1)), lua_checkstring(L, 2), lua_checknumber(L, 3));
    else
        LuaEngine::getInstance()->setUnitAnimCycle(unsigned(lua_checknumber(L, 1)), lua_checkstring(L, 2), lua_checknumber(L, 3));

    return 1;
}

int l_stopUnitAnimation(lua_State *L)
{
    LuaEngine::getInstance()->stopUnitAnim(unsigned(lua_checknumber(L, 1)), lua_checkstring(L, 2));

    return 1;
}

/*==========================================

    Input functions

 =========================================*/

int l_setInputObject(lua_State *L)
{
    LuaEngine::getInstance()->setInputObjectBuilding(int(lua_checknumber(L, 1)));
    return 1;
}

/*==========================================

    Texture related functions

 ==========================================*/

int l_loadTexture(lua_State *L)
{
    LuaEngine::getInstance()->getTextureFactory()->loadTexture(lua_checkstring(L, 1));
    return 1;
}

int l_deleteTexture(lua_State *L)
{
    return 1;
}

int l_flushTextures(lua_State *L)
{
    LuaEngine::getInstance()->getTextureFactory()->flushFactory();
    return 1;
}

int l_setAnisotropicFactor(lua_State *L)
{
    LuaEngine::getInstance()->getDrawingEngine()->toggleAnisotropicFiltering(float(lua_checknumber(L, 1)));
    return 1;
}

/*=========================================

    Drawing functions

==========================================*/

int l_setAntiAliasSamples(lua_State *L)
{
    LuaEngine::getInstance()->getAzadiInstance()->toggleAntiAlias(short(lua_checknumber(L, 1)));
    return 1;
}

int l_drawTexturedRectWithColor(lua_State *L)
{
    LuaEngine::getInstance()->getDrawingEngine()->drawTexturedRectWithColor(int(lua_checknumber(L, 1)), int(lua_checknumber(L, 2)), int(lua_checknumber(L, 3)), int(lua_checknumber(L, 4)), std::string(lua_checkstring(L, 5)), float(lua_checknumber(L, 6)), float(lua_checknumber(L, 7)), float(lua_checknumber(L, 8)));
    return 1;
}

int l_drawTexturedRect(lua_State *L)
{
    LuaEngine::getInstance()->getDrawingEngine()->drawTexturedRect(int(lua_checknumber(L, 1)), int(lua_checknumber(L, 2)), int(lua_checknumber(L, 3)), int(lua_checknumber(L, 4)), std::string(lua_checkstring(L, 5)));
    return 1;
}

int l_clearScreen(lua_State *L)
{
    LuaEngine::getInstance()->getDrawingEngine()->clearScreen(float(lua_checknumber(L, 1)), float(lua_checknumber(L, 2)), float(lua_checknumber(L, 3)), float(lua_checknumber(L, 4)));
    return 1;
}

int l_flipBuffers(lua_State *L)
{
    LuaEngine::getInstance()->getDrawingEngine()->flipBuffers();
    return 1;
}

int l_prepare2D(lua_State *L)
{
    LuaEngine::getInstance()->getDrawingEngine()->prepare2D();
    return 1;
}

int l_prepare3D(lua_State *L)
{
    LuaEngine::getInstance()->getDrawingEngine()->prepare3D();
    return 1;
}

/*=========================================

    Sound functions

=========================================*/

int l_soundTick(lua_State *L)
{
    LuaEngine::getInstance()->getSoundFactory()->tick();
    return 1;
}

int l_loadSound(lua_State *L)
{
    LuaEngine::getInstance()->getSoundFactory()->loadSound(std::string(lua_checkstring(L, 1)));
    return 1;
}

int l_unloadSound(lua_State *L)
{
    LuaEngine::getInstance()->getSoundFactory()->unloadSound(unsigned(lua_checknumber(L, 1)));
    return 1;
}

int l_playSound(lua_State *L)
{
    LuaEngine::getInstance()->getSoundFactory()->playSound(unsigned(lua_checknumber(L, 1)), bool(lua_toboolean(L, 2)), float(lua_checknumber(L, 3)), float(lua_checknumber(L, 4)));
    return 1;
}

int l_stopSound(lua_State *L)
{
    LuaEngine::getInstance()->getSoundFactory()->stopSound(unsigned(lua_checknumber(L, 1)));
    return 1;
}

/*=========================================

    Return functions

=========================================*/

int l_returnShort(lua_State *L)
{
    LuaEngine::getInstance()->returnShort(short(lua_checknumber(L, 1)));
    return 1;
}

int l_returnInt(lua_State *L)
{
    LuaEngine::getInstance()->returnInt(int(lua_checknumber(L, 1)));
    return 1;
}

int l_returnLong(lua_State *L)
{
    LuaEngine::getInstance()->returnLong(long(lua_checknumber(L, 1)));
    return 1;
}

int l_returnFloat(lua_State *L)
{
    LuaEngine::getInstance()->returnFloat(float(lua_checknumber(L, 1)));
    return 1;
}

int l_returnDouble(lua_State *L)
{
    LuaEngine::getInstance()->returnDouble(double(lua_checknumber(L, 1)));
    return 1;
}

int l_returnBool(lua_State *L)
{
    LuaEngine::getInstance()->returnBool(bool(lua_toboolean(L, 1)));
    return 1;
}

int l_returnString(lua_State *L)
{
    LuaEngine::getInstance()->returnString(std::string(lua_checkstring(L, 1)));
    return 1;
}

/*================================================

    Debug functions

  =================================================*/

int l_setter(lua_State *L)
{
    LuaEngine::getInstance()->getAzadiInstance()->setter(int(lua_checknumber(L, 1)));
    return 1;
}

void LuaEngine::registerLuaFunctions()
{
    // Base functions
    lua_register(luavm, "print", l_print);
    lua_register(luavm, "include", l_includeScript);

    // Misc functions
    lua_register(luavm, "wait", l_wait);
	lua_register(luavm, "getfps", l_getfps);
	lua_register(luavm, "getTicks", l_getTicks);

	// Cursor functions
	lua_register(luavm, "captureCursor", l_captureCursor);
	lua_register(luavm, "releaseCursor", l_releaseCursor);

    // Building related functions
	lua_register(luavm, "newBuilding", l_newBuilding);
	lua_register(luavm, "setBuildingScr", l_setBuildingScr);
	lua_register(luavm, "setBuildingPos", l_setBuildingPos);
	lua_register(luavm, "setBuildingRot", l_setBuildingRot);
	lua_register(luavm, "flushBuildings", l_flushBuildings);
	lua_register(luavm, "executeBuildingScripts", l_executeBuildingScripts);
	lua_register(luavm, "setBuildingAnimation", l_setBuildingAnimation);
	lua_register(luavm, "stopBuildingAnimation", l_stopBuildingAnimation);

	// Unit related functions
	lua_register(luavm, "newUnit", l_newUnit);
	lua_register(luavm, "setUnitScr", l_setUnitScr);
	lua_register(luavm, "setUnitPos", l_setUnitPos);
	lua_register(luavm, "setUnitRot", l_setUnitRot);
	lua_register(luavm, "flushUnits", l_flushUnits);
	lua_register(luavm, "executeUnitScripts", l_executeUnitScripts);
	lua_register(luavm, "setUnitAnimation", l_setUnitAnimation);
	lua_register(luavm, "stopUnitAnimation", l_stopUnitAnimation);

	// Input object functions
	lua_register(luavm, "setInputObject", l_setInputObject);

	// Texture related functions
	lua_register(luavm, "loadTexture", l_loadTexture);
	lua_register(luavm, "deleteTexture", l_deleteTexture);
	lua_register(luavm, "flushTextures", l_flushTextures);
	lua_register(luavm, "setAnisotropicFactor", l_setAnisotropicFactor);

	// Drawing functions
	lua_register(luavm, "setAntiAliasSamples", l_setAntiAliasSamples);
	lua_register(luavm, "drawTexturedRectWithColor", l_drawTexturedRectWithColor);
	lua_register(luavm, "drawTexturedRect", l_drawTexturedRect);
	lua_register(luavm, "flipBuffers", l_flipBuffers);
	lua_register(luavm, "prepare2D", l_prepare2D);
	lua_register(luavm, "prepare3D", l_prepare3D);
	lua_register(luavm, "clearScreen", l_clearScreen);

	// Sound functions
	lua_register(luavm, "soundTick", l_soundTick);
	lua_register(luavm, "loadSound", l_loadSound);
	lua_register(luavm, "unloadSound", l_unloadSound);
	lua_register(luavm, "playSound", l_playSound);
	lua_register(luavm, "stopSound", l_stopSound);

	// Return functions
	lua_register(luavm, "returnShort", l_returnShort);
	lua_register(luavm, "returnInt", l_returnInt);
	lua_register(luavm, "returnLong", l_returnLong);
	lua_register(luavm, "returnFloat", l_returnFloat);
	lua_register(luavm, "returnDouble", l_returnDouble);
	lua_register(luavm, "returnBool", l_returnBool);
	lua_register(luavm, "returnString", l_returnString);

	// Debug functions
	lua_register(luavm, "setter", l_setter);

	// Register the current building/unit handles
	execute("currentBuilding = {};");
	execute("currentUnit = {};");
}

