#include <irrlicht.h>
#include "listFileSwitch.h"
using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


class MyEventReceiver : public IEventReceiver
{
public:
	// This is the one method that we have to implement
	virtual bool OnEvent(const SEvent& event)
	{
		// Remember whether each key is down or up
		if (event.EventType == irr::EET_KEY_INPUT_EVENT)
			KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;

		return false;
	}

	// This is used to check whether a key is being held down
	virtual bool IsKeyDown(EKEY_CODE keyCode) const
	{
		return KeyIsDown[keyCode];
	}

	MyEventReceiver()
	{
		for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
			KeyIsDown[i] = false;
	}
	   //pf is CNrpPictureFlow
 //  IGUIElement* elm = CNrpEngine::Instance().GetGuiEnvironment()->getFocus();
  //       if( elm == pf )
   //        elm->OnEvent( event );

private:
	// We use this array to store the current state of each key
	bool KeyIsDown[KEY_KEY_CODES_COUNT];
};




int main()
{
    MyEventReceiver receiver;

	IrrlichtDevice *device =
		createDevice( video::EDT_SOFTWARE, dimension2d<u32>(640, 480), 16,
			false, false, false, &receiver);

	if (!device)
		return 1;

	device->setWindowCaption(L"Hello World! - Irrlicht Engine Demo");

	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();
	IGUIEnvironment* guienv = device->getGUIEnvironment();

	guienv->addStaticText(L"Hello World! This is the Irrlicht Software renderer!",
		rect<s32>(10,10,260,22), true);

	//IAnimatedMesh* mesh = smgr->getMesh("untitled.x");
	//if (!mesh)
	//{
	//	device->drop();
	//	return 1;
	//}
	//IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode( mesh );


//	if (node)
//	{
	  //  node->setFrameLoop (0, 120);

//	    node->setAnimationSpeed(1333);
//	    node->setLoopMode(true);
	//	node->setMaterialFlag(EMF_LIGHTING, false);
	//	node->setMD2Animation("ObAction");
	//	node->setMaterialTexture( 0, driver->getTexture("../../media/sydney.bmp") );
//	}


	smgr->addCameraSceneNode(0, vector3df(0,10,-10), vector3df(0,5,0));



   CNrp2DPictureFlow *pf = new CNrp2DPictureFlow( device , guienv,
                                                 guienv->getRootGUIElement(), core::recti( 0, 0, 400, 400 ), core::recti( 0, 0, 100, 100 ),-1 );


	int ammount =15;

   for( int k=0; k < ammount; k++ )
      pf->addItem( driver->getTexture( "stones.jpg" ), L"" );

int delay = 0;
int direction =0;
int icount=0;
	while(device->run())
	{
		if (delay > 80){
			delay=0;
			if (pf->getcurrent() < ammount-1 & direction == 0){
					pf->Next(1);
					} else {pf->Next(-1); direction == 1;}
		} else {delay++;}

		driver->beginScene(true, true, SColor(255,100,101,140));

		smgr->drawAll();
		guienv->drawAll();

		driver->endScene();
		device->sleep(20);
	}


	device->drop();

	return 0;
}

/*
That's it. Compile and run.
**/
