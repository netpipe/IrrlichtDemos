#ifndef _CCore
#define _CCore

#include "include.h"

//controls the user and the island
#include "CController.h"
#include "ThirdPersonCamera.h"

namespace island
{
	class CCore
	{
	public:
		CCore();
		~CCore();

		void setup();
		void render();
		void stop();
		void restart();

		////// return irrlicht default pointers //////
		irr::gui::ICursorControl* getCursor();
		irr::scene::ISceneManager* getSceneManager();
		irr::gui::IGUIEnvironment* getGUIEnvironment();
		irr::video::IVideoDriver* getVideoDriver();
		irr::s32 getRealTime();
		
		ThirdPersonCamera* cam;
	private:
		irr::IrrlichtDevice* device;

		CController* controller;

		irr::scene::ISceneNode* island;
	};
}

#endif