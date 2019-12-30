#ifndef SCRIPTING_H
#define SCRIPTING_H

#include <string>
#include <irrlicht.h>

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

extern "C"
{
	#include "../Lua/lua.h"
	#include "../Lua/lualib.h"
	#include "../Lua/lauxlib.h"
}

#include "luna.h"

class ScriptInterface
{
public:
	ScriptInterface(ISceneManager *smgr);
	~ScriptInterface();

	void loadScript(const std::string &scriptName);
private:
	lua_State *vm;
	ISceneManager *smgr;
};

#define method(class, name) {#name, &class::name}

#endif
