//#include "physics.h"
//#include <btBulletDynamicsCommon.h>
#include <iostream>
#include <vector>
#include "physics.h"
#include "objectWithNode.h"
#include "eventreceiver.cpp"


using namespace std;

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

//Yay for functions being in an unorganized place
btRigidBody* addplane()
{
	btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0,1,0),1);
	btDefaultMotionState* groundMotionState =
		new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,-10,0))); //the location of the plane
	btRigidBody::btRigidBodyConstructionInfo
		groundRigidBodyCI(0,groundMotionState,groundShape); //This creates the rigid body. The mass is 0 so it is immovable.
	btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);

	return groundRigidBody;

}


int main()
{
	//IRRLICHT
	MyEventReceiver receiver;
	IrrlichtDevice* device = createDevice(video::EDT_OPENGL, dimension2d<s32>(640,480), 16, false, false, false, &receiver);
	if (!device) {return 1;}

	//hide the cursor
	device->getCursorControl()->setVisible(false);
	//more irrlicht stuff
	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();
	IGUIEnvironment* guienv = device->getGUIEnvironment();

	//woot for random lighting
	ISceneNode* light = smgr->addLightSceneNode(0, vector3df(0,200,-50), SColor((u32) 255, (u32)255, (u32)0, (u32)0));
	ISceneNode* light2 = smgr->addLightSceneNode(0, vector3df(50,100,0), SColor((u32) 255, (u32)255, (u32)255, (u32)0));
	ISceneNode* light3 = smgr->addLightSceneNode(0, vector3df(100,50,50), SColor((u32) 255, (u32)255, (u32)0, (u32)255));

	ICameraSceneNode* camera = smgr->addCameraSceneNodeFPS();
	camera->setPosition(vector3df(0,6,0));
	//BULLET
	btDiscreteDynamicsWorld* pworld = makeworld();
	pworld->setGravity(btVector3(0,-10.0,0)); //- means down

	pworld->addRigidBody(addplane()); //add the invisible floor

	vector<ISceneNode*> nodevector;
	vector<Object*> objectvector;

	btScalar friction(0.05);
	btVector3 gravity(0,-10,0);

	btScalar xtowerstart(0.0);// The towers will start at x=0

	//Find the initial time
	u32 then = device->getTimer()->getTime();



	while(device->run())
	{

		driver->beginScene(true, true, SColor(255,100,101,140));



		if (objectvector.size() < 2500)
		{
			//if the left mouse button has been pressed shoot  cube
			if(receiver.HasKeyBeenPressed(irr::KEY_LBUTTON))
			{

				//You don't want those cubes shooting from the middle of nowhere, find the camera!
				vector3df startpos = camera->getPosition();
				vector3df startrot = camera->getRotation();

				//Make a new object with node with the size being 5,5,5,
				//the camera angle and position and a mass of 10kg
				ObjectWithNode* newobjectnode = new ObjectWithNode(smgr, btVector3(5,5,5),
					btTransform(btQuaternion(startrot.X, startrot.Y, startrot.Z), //Doesn't work, need to fix
					btVector3(startpos.X, startpos.Y, startpos.Z))
					, 10); //Note: The class should've been named box node

				//Store a pointer to the new object in our handy vector
				objectvector.push_back(newobjectnode);
				//Add the rigid body of our object to the physics world
				pworld->addRigidBody(newobjectnode->getBody());

	/*I'm not really sure how the math works in this part. I borrowed it from Erwin's (the
	creator of Bullet's demo. However, I know what it does.*/
				matrix4 mat; //Create an irrlicht matrix
				mat.setRotationDegrees(startrot); //take the angle of the camera and fill the matrix
				btVector3 forwardDir(mat[8],mat[9],mat[10]); //get the quaternion values of the matrix?
				newobjectnode->getBody()->setLinearVelocity(forwardDir * 400); //Set the velocity. The 400 is the speed

				newobjectnode->getBody()->setFriction(0.05); //How about trying to change the friction at runtime? Make it happen!

			}

			//Towers
			if (receiver.HasKeyBeenPressed(irr::KEY_BACK))
			{

				for (int y = -5; y<300; y+=10)
				{
					vector3df startpos(xtowerstart,y, 0);
					vector3df startrot(0,0,0);
					ObjectWithNode* newobjectnode = new ObjectWithNode(smgr, btVector3(5,5,5),
						btTransform(btQuaternion(startrot.X, startrot.Y, startrot.Z),
						btVector3(startpos.X, startpos.Y, startpos.Z))
						, 10);
					objectvector.push_back(newobjectnode);
					pworld->addRigidBody(newobjectnode->getBody());

				//A generalized function qould be great
				}

				xtowerstart+=11.5; // the next tower will spawn 11.5 on the x axis units away from the last tower
				std::cout << "Making Tower: " << xtowerstart/11.5 <<  std::endl;
			}
		}

		else { std::cout << "CAPPED" << std::endl;} // Spam CAPPED if they reach the limit for run

		//if + has been pressed
		if (receiver.HasKeyBeenPressed(irr::KEY_PLUS))
		{
			//add 10 to gravity in the y direction
			//Note: adding means gravity will be going upwards if it is positive
			gravity += btVector3(0,10,0);
			pworld->setGravity(gravity);
			cout << pworld->getGravity().getY() << endl;
		}
		//Same as above but subtract
		if (receiver.HasKeyBeenPressed(irr::KEY_MINUS))
		{
			gravity -= btVector3(0,10,0);
			pworld->setGravity(gravity);
			cout << pworld->getGravity().getY() << endl;
		}

		//If N has been pressed
		if (receiver.HasKeyBeenPressed(irr::KEY_KEY_N))
		{
			//print the size of objectvector - how many objects are in the world
			std::cout << "Number of Cubes: " << objectvector.size() << std::endl;
		}

		//cout << driver->getFPS() << endl;

		//Get the now time, get the difference, set then to now
		u32 now = device->getTimer()->getTime();
		float timeSinceLastCall = (float)(now - then) / 1000.f; // Time in seconds
		then = now;

		//What this does is allow fps independent physics.
		//Tell Bullet how much time has passed and step the simulation
		pworld->stepSimulation(timeSinceLastCall, 1);


		//The usual Irrlicht stuff
		smgr->drawAll();
		guienv->drawAll();
		driver->endScene();
	}

//Has anybody ordered a memory leak? Silly Bullet doesn't clean up for us.
	return 0;
}


