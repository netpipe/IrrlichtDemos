#include "Scripting.h"

ScriptInterface::ScriptInterface(ISceneManager *smg)
{
	vm = lua_open();
	smgr = smg;
}

ScriptInterface::~ScriptInterface()
{
	lua_close(vm);
}

void ScriptInterface::loadScript(const std::string &scriptName)
{
	if (luaL_loadfile(vm, scriptName.c_str()) || lua_pcall(vm, 0, 0, 0))
	{
		lua_pop(vm, 1);
	}
}