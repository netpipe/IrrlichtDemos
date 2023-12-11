#include <irrlicht.h>
#pragma comment(lib, "Irrlicht.lib")

#include "IGUITreeCtrl.h"

using namespace irr;

s32 main()
{
	// create device and exit if creation failed
	IrrlichtDevice *device = createDevice(video::EDT_OPENGL, core::dimension2du(640, 480));
	video::IVideoDriver *video = device->getVideoDriver();
	gui::IGUIEnvironment *gui = device->getGUIEnvironment();
	gui::IGUISkin *skin = gui->getSkin();
//	gui::IGUIFont *font = gui->getFont("C:\\Program Files\\Irrlicht\\irrlicht-1.1\\media\\fontxenotron.bmp");
//	if(font)
//		skin->setFont(font);

	gui::IGUITreeCtrl *tree = gui::AddGUITreeCtrl(gui, core::rect<s32>(20,20,220,420));
	gui::TreeNode *node = tree->InsertNode("First!");
	node->Open = true;
	node = tree->InsertNode("Child 1", node);
	node->Open = false;
	node = tree->InsertNode("Child 1-1", node);
	node = tree->InsertNode("Second");
	gui::TreeNode *firstnode = tree->GetRootNode()->Child;
	for(s32 i = 0; i < 26; i++)
	{
		wchar_t c[25];
	//	_snwprintf(c, 24, L"Item %i", i);
		tree->InsertNode(c,firstnode);
	}

	node = tree->FindNode(15);
	node = tree->GetPrevNode(node);

	tree->InsertNode("Supercalifragalisticexmealadocious");

	skin->setColor(gui::EGDC_BUTTON_TEXT, video::SColor(0xFF000000));


	while(device->run())
		if(device->isWindowActive())
		{
			video->beginScene(true, true, video::SColor(0,122,65,171));
			gui->drawAll();
			video->endScene();
		}

	return 0;
}
