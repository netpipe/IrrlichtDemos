#pragma once

// Vehement (C) Steven Dorman 2008
#define	 VEHEMENT_FACTORY_HPP

// Include nessecary libraries and headers
#include "includes.hpp"

// Include facotry objects
#include "statemanager.hpp"

// Include facotry objects
#include "mapeditor.hpp"

// Include factory objects
#include "playermanager.hpp"

// Include factory objects
#include "scripting.hpp"

// Using the vehe namespace
namespace vehe
{
	//Include the types
	#include "types.hpp"
	using namespace ctypes;

	// This is the main factory class
	class dfactory
	{
	protected:
		//! Pointer to the factory
		static dfactory *dinstance;

		//! Factory constructor
		dfactory() 
		{
			activecamera = 0;
			idebug=false;
			ipause=true;
			firstload=true;
		};

		//! Factory destructor
		~dfactory(void)
		{
			p_world->destroyWorld();
			smgr->clear();
			device->drop();
			audio->drop();
		};

	public:
		//! Creates or returns the singleton instance
		//! Use vehe::GetDevice!
		static dfactory *instance();

		//! Creates the irrlicht window, creates pointers
		//! Use vehe::CreateDevice!
		void init (int width, int height, bool fullscr,	IEventReceiver*	recv);

		//! Drawing Functions
		void beginDraw ();
		void finishDraw ();

		//! Camera Functions
		void createCamera (vector3df pos);
		void resetCamera ();
		void roamingCamera ();
		void freeMouse();

		//! Map Functions
		void loadMap (char * filename);
		void newMap  (char * filename);
		void saveMap (char * filename);

		/* BEGIN GAME SPECIFIC FUNCTION */

		//! Push an object with your mind
		void pushObject(int myid);

		/* END GAME SPECIFIC FUNCTION */
			
		//! Create the game entity for the last object
		void buildObject(object newobject);

		//! Update an existing game entity
		void updateObject(object newobject, int myid);

		//! Delete an existing game entity and refresh their ID's
		void deleteObject(int myid);

		//! Move an existing game entity 
		void moveObject(int myid);

		//! Clone an existing game entity 
		void cloneObject(int myid);

		//! Setup the Skydome
		void createSkyDome ();

		//! Save the current maps state
		void saveMapState (char * filename);

		//! Load the state of the current map
		void loadMapState (char * filename);

		//! Deletes a Directory
		bool deleteDirectory(LPCTSTR lpszDir, bool noRecycleBin = true);

		inline IrrlichtDevice * getIrrDevice    () { return device; };
		inline ISoundEngine   * getSoundDevice  () { return audio; };
		inline statemanager   * getStateManager () { return gamestate; };
		inline playermanager   * getPlayerManager () { return tplayer; };
		inline mapeditor	  * getMapEditor	() { return editor; };
		inline scripting	  * getScripting	() { return tscript; };
		inline ICameraSceneNode	* getCameraNode	() { return camera; };
		inline IBillboardSceneNode * getBillBoardNode	() { return bill; };
		inline mapobj			getMapObject	() { return currentmap; }
		inline object			getObject		(int i) { return objects.at(i); };
		inline entity			getEntity		(int i) { return entities.at(i); };
		
		inline bool getDrawingPhysicsDebug		()		{ return idebug; };
		inline void setDrawingPhysicsDebug (bool din)	{ idebug=din; };  
		inline bool getIsPaused					()		{ return ipause; };
		inline void setIsPaused			   (bool din)	{ ipause=din; };

		private:

		//! Start the engine
		void start ();

		//! Create the terrain object
		void createTerrain (mapobj data);

		//! Populate the world with objects loaded from the file
		void populateObjects ();

		//! Add billboard for editor target
		void createBillBoard();

		//Game State Manager
		statemanager*		gamestate;

		//mapeditor class
		mapeditor*			editor;

		//Player Manager class
		playermanager*		tplayer;

		//Scripting Manager class
		scripting*			tscript;

		//! The irrlicht device pointers
   		IrrlichtDevice*		device; 
		IGUIEnvironment*	guienv;
		IVideoDriver*		driver;
		ISceneManager*		smgr;
		ISoundEngine*		audio;

		//Newton game world
		newton::IWorld*		p_world;

		//Scene Nodes
		ICameraSceneNode*			camera;
		ICameraSceneNode*			camera2;
		ICameraSceneNode*			camera3;
		ITerrainSceneNode*			terrain;
		IBillboardSceneNode*		bill;
		
		//Global Irrlicht objects
		IMetaTriangleSelector*		meta; 

		//Variables
		int							activecamera;	//This is the active camera
		bool						idebug;			//For drawing the debug lines
		bool						ipause;			//Is the game currently paused
		bool						firstload;		//Did the engine just start up
		mapobj						currentmap;		//The active maps map object
		entity						eterrain;		//The active maps entity object
		vector <object>				objects;		//The array of object structures
		vector <entity>				entities;		//The array of game entities
		vector <cpos>				objectstate;	//For saving the game states
	};

};