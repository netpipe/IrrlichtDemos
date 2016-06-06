#include <irrlicht.h>

#include "CGUIEditorControl.h"

#include <ctime>
#include <cstdlib>

using namespace irr;
using namespace gui;
using namespace core;
using namespace video;



s32 main()
{
	IrrlichtDevice *device = createDevice(video::EDT_OPENGL, core::dimension2d<s32>(1024, 768));


	IVideoDriver *video		= device->getVideoDriver();
	IGUIEnvironment *gui	= device->getGUIEnvironment();

	new CGUIEditorControl(gui, rect<s32>(10,10, 400, 600), 0, 12);

	while(device->run())
		if(device->isWindowActive())
		{
			static s32 OldFPS = -1;
			s32 FPS = video->getFPS();
			if(FPS != OldFPS)
			{
				wchar_t c[64];
				// How many FPS do YOU have?
				wprintf(c, 63, L"TreeView Control Demo: %i FPS", FPS);
				device->setWindowCaption(c);
				OldFPS = FPS;
			}
			video->beginScene(true, true, video::SColor(0,122,65,171));
			gui->drawAll();
			video->endScene();
		}

	return 0;
}
