#pragma once

// Vehement (C) Steven Dorman 2008
#define	 VEHEMENT_PLAYERMANAGER_HPP

// Include nessecary libraries and headers
#include "includes.hpp"

// Using the vehe namespace
namespace vehe
{
	// Include nessecary libraries and headers
	#include "types.hpp"
	using namespace ctypes;

	// This is the state manager class
	class playermanager
	{
	public:
	
		playermanager ();
		~playermanager(void) {	};

		void newPlayer ();
		void updatePlayerManager ();

		void savePlayer (char * filename);
		void loadPlayer (char * filename);

		inline player getPlayerObject			()		{return myplayer;};
		inline void setPlayerObject (player theplayer)	{myplayer=theplayer;};		
								
	private:

		//Private Functions
		void isInGameArea ();

		//The player structure
		player			myplayer;

		//Array of items
		vector <item>	inventory;

		//! The irrlicht device pointers
   		IrrlichtDevice*		device; 
		IGUIEnvironment*	guienv;
		IVideoDriver*		driver;
		ISceneManager*		smgr;
		ISoundEngine*		audio;
	};
};