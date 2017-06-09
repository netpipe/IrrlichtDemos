//
// Flag Scene-Node Demo
//
// Created by FMX for the Irrlicht community
// zlib licence
//


// irrlicht
	#include <irrlicht.h>

	using namespace irr;

	using namespace core;
	using namespace scene;
	using namespace video;
	using namespace io;
	using namespace gui;

	IVideoDriver	*driver = 0;
	ISceneManager	*smgr	= 0;
	IGUIEnvironment	*guienv = 0;
	IrrlichtDevice	*device = 0;


// flag scene node
	#include "CFlagSceneNode.h"
	CFlagSceneNode	*irrFlagNode = 0;

// demo veriables
	vector3df	GravityVec(0, -9.8f, 0);

	bool	doWind 			= true;
	f32		WindWave 		= 0;
	bool	showWire		= false;
	bool	pausePhysics	= false;



// Generic Input Event Reciever from Tutorial 04
	class MyEventReceiver : public IEventReceiver
	{
	public:
		virtual bool OnEvent(const SEvent& event)
		{
			if (event.EventType == irr::EET_KEY_INPUT_EVENT) KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
			return false;
		}
		virtual bool IsKeyDown(EKEY_CODE keyCode) const{ return KeyIsDown[keyCode]; }
		MyEventReceiver(){ for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i) KeyIsDown[i] = false; }

	private:
		bool KeyIsDown[KEY_KEY_CODES_COUNT];
	};


int main()
{
	// create an Input event Reciever
		MyEventReceiver 	receiver;

	// create irrlicht device
		device = createDevice( video::EDT_OPENGL, dimension2d<u32>(800, 600), 32, false, false, false, &receiver);
		if (!device)return 1;

		device->setWindowCaption(L"Flag SceneNode Demo by FMX");

		driver = device->getVideoDriver();
		smgr = device->getSceneManager();
		guienv = device->getGUIEnvironment();


	// create a camera
		smgr->addCameraSceneNode(0, core::vector3df(0,125,-250), core::vector3df(0,0,0));
//smgr->addCameraSceneNodeFPS();

	// define initial material properties (can be modified later too)
		SMaterial Mat;
		Mat.Lighting		= false;
		Mat.BackfaceCulling = false;

	// Create the Flag Scene Node (dont forget to drop() it afterwards)
		irrFlagNode = new CFlagSceneNode
		(
			10, 20,		// flag dimensions (change to whatever size required, or scale the node later)
			10, 20,		// flag tesselation (good default value)

			Mat,		// initial material properties
			GravityVec,	// gravity (depends on the scale of the scene and the tesselation)

			2,			// iterations (change at your own peril)
			1.99f,		// spring-stiffness (change at your own peril)

			smgr->getRootSceneNode(), smgr, -1	// parent, irrlicht scene manager, ID
		);

		if( !irrFlagNode ) return 0; // unknown error


	// assign a texture and enable alpha transparency (id is always 0)
		irrFlagNode->getMaterial(0).setTexture( 0, driver->getTexture( "irrlichtlogo.png" ) );
		irrFlagNode->getMaterial(0).MaterialType = EMT_TRANSPARENT_ALPHA_CHANNEL;


	// node can be manipulated like any other irrlicht node
		irrFlagNode->setScale( vector3df(10) );

		// create an animator to make it move in circles



	// render loop
	while(device->run())
	{
		// check key input

			// Pause Physics
			if(receiver.IsKeyDown(irr::KEY_KEY_Q)){ pausePhysics = true; }
			if(receiver.IsKeyDown(irr::KEY_KEY_E)){ pausePhysics = false; }

			// Wind
			if(receiver.IsKeyDown(irr::KEY_KEY_A)){ doWind = true; }
			if(receiver.IsKeyDown(irr::KEY_KEY_D)){ doWind = false; }

			// Wireframe
			if(receiver.IsKeyDown(irr::KEY_KEY_W)){ irrFlagNode->getMaterial(0).Wireframe = true; }
			if(receiver.IsKeyDown(irr::KEY_KEY_S)){ irrFlagNode->getMaterial(0).Wireframe = false; }


		// simulate wind by adding a force every frame
		if( doWind )
		{
			WindWave += 0.01f;
			if( WindWave > 1 ) { WindWave = 0; }

			vector3df Wind = vector3df(
							 (sin( WindWave * 3 ) + 0.5f) * 5,
							 (sin( WindWave ) * 0.5f) * 40,
							 (cos( WindWave * 3)) * 5
							 );

			// Apply the Force
			irrFlagNode->applyForce( Wind );
		}


//irrFlagNode->UpdatePhysics( 0.02f ); // dt
		// update irrFlag Physics (must provide an accurate dt, to pause it simply dont update it)
		//if( !pausePhysics )
			//irrFlagNode->UpdatePhysics( .0085f);

		// render as usual
		driver->beginScene(true, true, SColor(255,100,101,140));

			smgr->drawAll();
			guienv->drawAll();
		driver->endScene();
		device->sleep(5,0);
	}

	irrFlagNode->drop();	// remove flag scenenode

	device->drop();

	return 0;
}
