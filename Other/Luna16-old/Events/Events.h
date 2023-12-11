#ifndef EVENTS_H
#define EVENTS_H

#include <queue>

#include <irrlicht.h>
#include "../Scene/customNodes.h"

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class EventRec : public IEventReceiver
{
	public:
	EventRec();
		virtual bool OnEvent ( const SEvent &event );
		void initialize();

		void registerCamera ( CCameraSceneNode *camera );

		bool quit;
		bool login;
		bool conRefOK;
		bool wrongPassOK;

		// Mouse coordinates
		unsigned short mousex;
		unsigned short mousey;

		// Mouse click coords
		std::queue<position2d<s32> > lmouseClicks;
		std::queue<position2d<s32> > rmouseClicks;

		// Mouse button usage
		bool lmouse;
		bool rmouse;

		// developer login
		bool devLogin;
	private:
		CCameraSceneNode *camera;
};


#endif
