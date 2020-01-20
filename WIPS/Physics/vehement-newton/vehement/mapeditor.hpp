#pragma once

// Vehement (C) Steven Dorman 2008
#define	 VEHEMENT_MAPEDITOR_HPP

// Include nessecary libraries and headers
#include "includes.hpp"

// Using the vehe namespace
namespace vehe
{
	// This is the map editor class
	class mapeditor
	{
	public:
		mapeditor ();
		~mapeditor(void) {	};

		//Show the map editor window
		void createEditor ();

		//Process GUI Input
		void processGUI (int x, int y);

		//Draw selected debug infos
		void drawSelectedCollision();

		inline void setActive (bool din) {active=din;};
		inline bool getActive () {return active;};
		inline bool getIsChildOpen () {return child; };
		inline int	getSelectedEditorNode () {return picked;};
		inline void	setSelectedEditorNode (int id) {picked=id;};

	private:

		//Create object from GUI window or edit the selected one
		void makeObject();
		
		//Add an object
		void addObject ();

		//Edit an object
		void editObject ();

		//Update an object
		void updateObject ();

		//Root GUI Node
		IGUIElement		*	root;
		IGUIWindow		*	window;
		IGUIWindow		*	window_add;
		IGUIWindow		*	window_edit;

		//Variables
		bool	active;
		bool	child;
		int		picked;
		int		lpicked;
	
		//! The irrlicht device pointers
   		IrrlichtDevice*		device; 
		IGUIEnvironment*	guienv;
		IVideoDriver*		driver;
		ISceneManager*		smgr;
		ISoundEngine*		audio;
	};

};