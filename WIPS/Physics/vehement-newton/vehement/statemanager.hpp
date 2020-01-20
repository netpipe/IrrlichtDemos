#pragma once

// Vehement (C) Steven Dorman 2008
#define	 VEHEMENT_STATEMANAGER_HPP

// Include nessecary libraries and headers
#include "includes.hpp"

// Using the vehe namespace
namespace vehe
{
	// This is the state manager class
	class statemanager
	{
	public:
	
		statemanager ();
		~statemanager(void) {	};

		//Public Functions
		void doState	();
		void processGUI (int x, int y);
		
		//Set the current game state
		inline void setState (int din) { mystate=din; };

		//Root GUI Node
		IGUIElement		*	root;
		
	private: 
		//Private Functions
		void loadGUI	();
		void loadHUD	();

		//Private Variables
		int		mystate;
		bool	newgame;
		bool	startingup;

		//Array of gui elements
		vector <IGUIElement*>	elements;
		vector <int>			stateswitch;

		//! The irrlicht device pointers
   		IrrlichtDevice*		device; 
		IGUIEnvironment*	guienv;
		IVideoDriver*		driver;
		ISceneManager*		smgr;
		ISoundEngine*		audio;
	};
};