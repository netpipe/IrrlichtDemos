

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


// irrSimplePhysics

	#include "irrSimplePhysics.h"



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

		IrrlichtDevice* device = createDevice( video::EDT_OPENGL, dimension2d<s32>(800, 600), 32,
				false, false, 0,  &receiver);

		if (device == 0)
			return 1; // could not create selected driver.

		video::IVideoDriver* driver = device->getVideoDriver();
		scene::ISceneManager* smgr = device->getSceneManager();



	// create camera

	//	smgr->addCameraSceneNode( 0, vector3df(20, 20, 20), vector3df(0,0,0) );
		smgr->addCameraSceneNodeFPS(0, 100.0f, .1f);
		device->getCursorControl()->setVisible(false);


	// make a nice big floor
	{
		scene::IAnimatedMesh* mesh = smgr->addHillPlaneMesh("myHill",
			core::dimension2d<f32>(1000,1000),
			core::dimension2d<u32>(1,1), 0, 0,
			core::dimension2d<f32>(0,0),
			core::dimension2d<f32>(100,100));

		scene::ISceneNode* node = smgr->addMeshSceneNode( mesh->getMesh(0) );
		node->setPosition( vector3df(0,-1,0) );
		node->setMaterialTexture(0, driver->getTexture("stones.jpg"));
		node->setMaterialTexture(1, driver->getTexture("water.jpg"));
		node->setMaterialType(video::EMT_REFLECTION_2_LAYER);

		node->setMaterialFlag(video::EMF_LIGHTING, false);
	}


	
	// Create irrPhysics

		CirrSimplePhysics *irrSimplePhysics = new CirrSimplePhysics
			(
				device,									// give it the irrlicht Device
				vector3df( 0, -9.81f, 0)				// default Gravity (if not provided, defaults to NO gravity = vector3df(0,0,0))
			);



// ********************************************************************************************************************

		bool useFORCES = true;			// if TRUE, input keys will set the FORCES, otherwise absolute VELOCITIES
										// try both to see the effect of the behaviour

// ********************************************************************************************************************
		



	// "Character" SceneNode to affect with irrPhysics

		u32 ipObject;	// stores indices returned by irrPhysics objects, so we can edit it in the future



	// create an object [as a sphere or cube]
		ISceneNode *node = smgr->addSphereSceneNode( 2 ); 

		if( node )
		{
		// set initial parameters
			node->setMaterialTexture(0, driver->getTexture("fire.bmp"));
			node->setMaterialFlag(video::EMF_LIGHTING, false);

		// tell irrPhysics about it so it can apply Gravity to it automatically, and deal with forces etc
			f32 mass = 20; 
			ipObject = irrSimplePhysics->addNodeForPhysics( node, mass );	

		// manually disable gravity on this node to begin with
			irrSimplePhysics->setObjectAffectedByGravity( ipObject, false );		
		}
		else return 1;	// error obviously occured
	


	int lastFPS = -1;


	while(device->run())
	{
	// Update Physics! has to be done before ANYTHING else in the frame, or all hell breaks loose
		irrSimplePhysics->UPDATE();


	// pause / unpause irrPhysics this frame

		if(receiver.IsKeyDown(irr::KEY_KEY_P))
		{
			irrSimplePhysics->setSimPaused( true );
		}
		else if(receiver.IsKeyDown(irr::KEY_KEY_O))
		{
			irrSimplePhysics->setSimPaused( false );
		}


	// Reset physics on object, but KEEP the node associated with it

		if(receiver.IsKeyDown(irr::KEY_KEY_R))
		{
			irrSimplePhysics->resetPhysicsForObject( ipObject, false );		// reset physics parameters
			irrSimplePhysics->setObjectAffectedByGravity( ipObject, false );			// manually disable gravity on this node
			node->setPosition( vector3df( 0,0,0 ) );						// reset node position
			node->updateAbsolutePosition();
		}



	// Apply User-Forces to Node
		
	// get the node associated with this object
		ISceneNode *node = irrSimplePhysics->getNodeFromObject( ipObject );

		if( node )
		{
		// A / D to rotate the Node
			if(receiver.IsKeyDown(irr::KEY_KEY_A))
			{
				vector3df rot = node->getRotation();
					rot.Y += 0.6f;
				node->setRotation( rot );
			}
			else if(receiver.IsKeyDown(irr::KEY_KEY_D))
			{
				vector3df rot = node->getRotation();
					rot.Y -= 0.6f;
				node->setRotation( rot );
			}


			

		// reset current velocity. only need to do this when dealing with absolute velocities

			if( !useFORCES )
			{
			// ignore Y-velocity to keep Gravity and Jumping forces valid
				vector3df vel = irrSimplePhysics->getObjectVelocity( ipObject );
					vel.X = 0;
					vel.Z = 0;
				irrSimplePhysics->setObjectVelocity( ipObject, vel );
			}
			



		// W / S to Apply forwards / backwards user force

			if(receiver.IsKeyDown(irr::KEY_KEY_W))
			{
				vector3df vecForceF = node->getRotation().rotationToDirection().normalize();


			// do one or the other, *NOT* both

			// Force
				if( useFORCES )
				{
					irrSimplePhysics->setObjectUserForce( ipObject, vector3df( 50.0f * vecForceF ) );		// apply force
				}

			// Velocity
				if( !useFORCES )
				{
					vector3df Vec = 15.0f * vecForceF;

				// keep Y-velocity of Gravity
					Vec.Y = irrSimplePhysics->getObjectVelocity( ipObject ).Y;
					irrSimplePhysics->setObjectVelocity( ipObject, Vec );		// set velocity
				}
			}
			else if(receiver.IsKeyDown(irr::KEY_KEY_S))
			{
				vector3df vecForceB = -node->getRotation().rotationToDirection().normalize();


			// do one or the other, *NOT* both

			// Force
				if( useFORCES )
				{
					irrSimplePhysics->setObjectUserForce( ipObject, vector3df( 50.0f * vecForceB ) );		// apply force
				}

			// Velocity
				if( !useFORCES )
				{
					vector3df Vec = 15.0f * vecForceB;

				// keep Y-velocity of Gravity
					Vec.Y = irrSimplePhysics->getObjectVelocity( ipObject ).Y;
					irrSimplePhysics->setObjectVelocity( ipObject, Vec );		// set velocity
				}
			}

		// SPACE-BAR to "jump" by applying an additional force against gravity
			if(receiver.IsKeyDown(irr::KEY_SPACE))
			{
				irrSimplePhysics->addObjectUserForce( ipObject, vector3df( 0, 20, 0 ) );
			}


		// cap the velocity. only need to do this when dealing with Forces
			if( useFORCES )
			{
				if( abs( irrSimplePhysics->getObjectVelocity( ipObject ).getLength() ) > 30 )
				{
					irrSimplePhysics->getObjectVelocity( ipObject ).setLength( 30 );
				}
			}


		// disable gravity if near ground, but leaves global gravity acting anyway
			if( node->getAbsolutePosition().Y < 0 )
			{
				irrSimplePhysics->setObjectAffectedByGravity( ipObject, false );
				vector3df pos = node->getAbsolutePosition();
					pos.Y = 0;
				node->setPosition( pos );
				node->updateAbsolutePosition();
			}

		// enable gravity if slightly airborne
			if( node->getAbsolutePosition().Y > 0 )
			{
				irrSimplePhysics->setObjectAffectedByGravity( ipObject, true );
			}

		}




	// do Irrlicht stuff

		driver->beginScene(true, true, video::SColor(255,113,113,133));

		smgr->drawAll(); // draw the 3d scene
		device->getGUIEnvironment()->drawAll(); // draw the gui environment (the logo)

		driver->endScene();

		int fps = driver->getFPS();

		if (lastFPS != fps)
		{
			core::stringw tmp(L"irrSimplePhysics Example 2 [");
			tmp += driver->getName();
			tmp += L"] fps: ";
			tmp += fps;

			device->setWindowCaption(tmp.c_str());
			lastFPS = fps;
		}
	}


	// show cursor
		device->getCursorControl()->setVisible(true);


	// cleanup physics
		delete irrSimplePhysics;

	device->drop();
	
	return 0;
}
