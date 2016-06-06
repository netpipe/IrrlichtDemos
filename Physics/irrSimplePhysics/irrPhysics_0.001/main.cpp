

// Irrlicht

	#include <irrlicht.h>

	using namespace irr;

	using namespace core;
	using namespace scene;
	using namespace video;
	using namespace io;
	using namespace gui;


	#ifdef _IRR_WINDOWS_
		#pragma comment(lib, "Irrlicht.lib")
		#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
	#endif



// needed for random-number-generation
	#include <cstdlib>
	#include <ctime>
	#include <iostream>

	using namespace std;


// irrPhysics

	#include "irrPhysics.h"



// Event Receiver

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

	private:
		// We use this array to store the current state of each key
		bool KeyIsDown[KEY_KEY_CODES_COUNT];
	};



// MAIN

int main()
{
	// create Irrlicht device
		MyEventReceiver receiver;

		IrrlichtDevice* device = createDevice( video::EDT_OPENGL, dimension2du(800, 600), 32,
				false, false, 0,  &receiver);

		if (device == 0)
			return 1; // could not create selected driver.

		video::IVideoDriver* driver = device->getVideoDriver();
		scene::ISceneManager* smgr = device->getSceneManager();



	// create camera

		smgr->addCameraSceneNodeFPS(0, 100.0f, .1f);
		device->getCursorControl()->setVisible(false);



	// attach billboard "force-target"

		ISceneNode *billboard = 0;
		billboard = smgr->addBillboardSceneNode(smgr->getRootSceneNode(), core::dimension2d<f32>(10, 10));
		billboard->setMaterialFlag(video::EMF_LIGHTING, false);
		billboard->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
		billboard->setMaterialTexture(0, driver->getTexture("particlewhite.bmp"));

		scene::ISceneNodeAnimator* anim = 0;
			anim = smgr->createFlyCircleAnimator (core::vector3df(0,-33,0), 20, 0.001f);
			billboard->addAnimator(anim);
		anim->drop();

	// attach another billboard "force-target"

		ISceneNode *billboard2 = 0;
		billboard2 = smgr->addBillboardSceneNode(smgr->getRootSceneNode(), core::dimension2d<f32>(10, 10));
		billboard2->setMaterialFlag(video::EMF_LIGHTING, false);
		billboard2->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
		billboard2->setMaterialTexture(0, driver->getTexture("particlewhite.bmp"));

		scene::ISceneNodeAnimator* anim2 = 0;
			anim2 = smgr->createFlyCircleAnimator (core::vector3df(0,33,0), 20, 0.001f, vector3df(0,-1,0) );
			billboard2->addAnimator(anim2);
		anim2->drop();



	// randomise seed
		srand((unsigned)time(0));


	// Create irrPhysics

		CirrPhysics *irrPhysics = new CirrPhysics
			(
				smgr->getRootSceneNode(), smgr, -1,		// usual SceneNode parameters
				device,									// give it the irrlicht Device
				vector3df( 0, -9.81f, 0)				// default Gravity (if not provided, defaults to NO gravity = vector3df(0,0,0))
			);



	// array of objects to affect with irrPhysics

		array<u32>ipObjects;	// stores indices returned by irrPhysics objects, so we can alter them in the future
		ipObjects.set_used( 0 );

		bool isSphere = true;

		for( int i = 0;  i < 300;  i++ )
		{
		// create an object [as a sphere or cube]
			ISceneNode *node = 0;

			isSphere = !isSphere;

			if( isSphere )	{ node = smgr->addSphereSceneNode( 1 ); }
			else			{ node = smgr->addCubeSceneNode( 1 );   }

			if( node )
			{
			// set initial parameters
				if( isSphere )	{ node->setMaterialTexture(0, driver->getTexture("water.jpg")); }
				else			{ node->setMaterialTexture(0, driver->getTexture("stones.jpg")); }

				node->setMaterialFlag(video::EMF_LIGHTING, false);

			// random starting position
				f32 posThresh = 10;
				f32 posX = f32( -posThresh + int(int(posThresh*2)*rand()/(RAND_MAX + 1.0)) );
				f32 posY = f32( -50 + int(20*rand()/(RAND_MAX + 1.0)) );
				f32 posZ = f32( -posThresh + int(int(posThresh*2)*rand()/(RAND_MAX + 1.0)) );

				node->setPosition(core::vector3df(posX,0,posZ));

			// tell irrPhysics about it so it can apply Gravity to it automatically, and deal with forces etc
				f32 mass = f32( 100 + int(1000*rand()/(RAND_MAX + 1.0)) );
				ipObjects.push_back( irrPhysics->addNodeForPhysics( node, mass ) );
			}
			else return 1;	// error obviously occured
		}


	int lastFPS = -1;


	while(device->run())
	{

	// pause / unpause irrPhysics

		if(receiver.IsKeyDown(irr::KEY_KEY_P))
		{
			irrPhysics->setSimPaused( true );
		}
		else if(receiver.IsKeyDown(irr::KEY_KEY_O))
		{
			irrPhysics->setSimPaused( false );
		}



	// flip gravity

		if(receiver.IsKeyDown(irr::KEY_KEY_K))
		{
			irrPhysics->setGravity( vector3df( 0, 9.81f, 0 ) );
		}
		else if(receiver.IsKeyDown(irr::KEY_KEY_L))
		{
			irrPhysics->setGravity( vector3df( 0, -9.81f, 0 ) );
		}


	// go through all irrPhysics objects

		for( u16 i = 0;  i < ipObjects.size();  i++ )
		{
		// get the node associated with this object
			ISceneNode *node = irrPhysics->getNodeFromObject( i );
			if( !node ) continue;

		// Apply a user force towards the Billboard [only for a brief period of time]
			if( (node->getAbsolutePosition().Y < -20) && (node->getAbsolutePosition().Y > -33)  )
			{
				vector3df vecToBB = vector3df(billboard->getAbsolutePosition() - node->getAbsolutePosition()).normalize();
				irrPhysics->setObjectUserForce( i, vector3df( 200.0f * vecToBB ) );
			}
			if( (node->getAbsolutePosition().Y > 20) && (node->getAbsolutePosition().Y < 33)  )
			{
				vector3df vecToBB = vector3df(billboard2->getAbsolutePosition() - node->getAbsolutePosition()).normalize();
				irrPhysics->setObjectUserForce( i, vector3df( 200.0f * vecToBB ) );
			}

		// reset them once they've gone past a certain point
			if( (node->getAbsolutePosition().Y < -100) || (node->getAbsolutePosition().Y > 100) )
			{
			// reset physics parameters, but KEEP the node associated with it
				irrPhysics->resetPhysicsForObject( i, false );

			// random starting position
				f32 posThresh = 10;
				f32 posX = f32( -posThresh + int(int(posThresh*2)*rand()/(RAND_MAX + 1.0)) );
				f32 posZ = f32( -posThresh + int(int(posThresh*2)*rand()/(RAND_MAX + 1.0)) );
				node->setPosition(core::vector3df(posX,0,posZ));
			}
		}




		driver->beginScene(true, true, video::SColor(255,113,113,133));

		smgr->drawAll(); // draw the 3d scene
		device->getGUIEnvironment()->drawAll(); // draw the gui environment (the logo)

		driver->endScene();

		int fps = driver->getFPS();

		if (lastFPS != fps)
		{
			core::stringw tmp(L"irrPhysics Example [");
			tmp += driver->getName();
			tmp += L"] fps: ";
			tmp += fps;

			device->setWindowCaption(tmp.c_str());
			lastFPS = fps;
		}
	}


	irrPhysics->drop();
	device->drop();

	return 0;
}
