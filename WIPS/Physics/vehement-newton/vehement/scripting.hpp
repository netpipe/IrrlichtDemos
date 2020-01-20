#pragma once

// Vehement (C) Steven Dorman 2008
#define	 VEHEMENT_SCRIPTING_HPP

// Include nessecary libraries and headers
#include "includes.hpp"

// Using the vehe namespace
namespace vehe
{
	// Include nessecary libraries and headers
	#include "types.hpp"
	using namespace ctypes;

	// This is the state manager class
	class scripting
	{
	public:
	
		scripting ();
		~scripting(void) 
		{
			lua_close(lstate);
		};

		//Call back member functions
		void test ();

		//Register all C call backs for scripting
		void registerCallbacks();
		void runScript(char * filename);

		//Misc
		inline lua_State * getLuaState () {return lstate;};
		
	private:

		//Lua State
		lua_State*			lstate;

		//! The irrlicht device pointers
   		IrrlichtDevice*		device; 
		IGUIEnvironment*	guienv;
		IVideoDriver*		driver;
		ISceneManager*		smgr;
		ISoundEngine*		audio;
	};
};