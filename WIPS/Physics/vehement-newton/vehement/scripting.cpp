// Vehement (C) Steven Dorman 2008
#define	 VEHEMENT_SCRIPTING_CPP

// Include the class definition
#include "scripting.hpp"

// Reference the factory class
#include "factory.hpp"
class dfactory;

// Using the vehe namespace
namespace vehe 
{
	using namespace ctypes;

	//Initializer
	scripting::scripting ()	
	{
		// Store device pointers for use
		device = dfactory::instance()->getIrrDevice();
		
		guienv = dfactory::instance()->getIrrDevice()->getGUIEnvironment();
		
		driver = dfactory::instance()->getIrrDevice()->getVideoDriver();

		smgr  = dfactory::instance()->getIrrDevice()->getSceneManager();

		audio =  dfactory::instance()->getSoundDevice();

		//Initialize the lua engine
		lstate = lua_open();
		luaL_openlibs(lstate);

		//Register all the lua callbacks
		scripting::registerCallbacks();

		//Run the configuration scripts
		luaL_dofile(lstate, "./data/scripts/config.sc");
	};

		/* Test Local Function */
		void scripting::test()
		{
			cout << "Scripting Engine Initialized!" << endl;
		}

		/* Call Back */
		int Ltest(lua_State* L){
			dfactory::instance()->getScripting()->test();
			return 0;
		}
		
	//Run Script
	void scripting::runScript(char * filename)
	{
		luaL_dofile(lstate, filename);
	}

	//Register all the function callbacks
	void scripting::registerCallbacks()
	{
		//Register Function Callbacks
		lua_pushcfunction(lstate, Ltest);	
		lua_setglobal(lstate, "test");
	}

};