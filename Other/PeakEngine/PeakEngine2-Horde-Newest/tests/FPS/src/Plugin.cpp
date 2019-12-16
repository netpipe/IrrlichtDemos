
#include "Plugin.h"
#include "core/GameEngine.h"
#include "core/Script.h"
#include "Player.h"
#include "Weapon.h"

#include <stdio.h>

extern "C"
{
#include "lualib.h"
}

int tolua_plugin_open(lua_State*);

bool initPlugin(void)
{
	printf("Arizona: initPlugin called!\n");
	printf("Root directory: %s\n", peak::GameEngine::get()->getRootDirectory().c_str());
	peak::Script::addBindings(tolua_plugin_open);
	peak::EntityFactory::registerFactory(&PlayerFactory::factory, "player");
	peak::EntityFactory::registerFactory(&WeaponFactory::factory, "weapon");
	return true;
}
bool unloadPlugin(void)
{
	printf("Arizona: unloadPlugin called!\n");
	return true;
}

