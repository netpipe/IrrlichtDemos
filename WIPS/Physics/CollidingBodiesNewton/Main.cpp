#include "stdafx.h"
#include <iostream>
#include <Newton.h>
#include <irrlicht.h>
#include <memory.h>
#include <string>
#include <iostream>
#include <irrKlang.h>
#include "CIrrEventReceiver.h"

// irrKlang namespace
using namespace irrklang;

// irrlicht namespaces
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

// standard namespace
using namespace std;

// constants declarations
#define NEWTONTOIRR 32.0f			  // conversion factor between the two
#define IRRTONEWTON 1.0f / NEWTONTOIRR
#define CLIPPING_DISTANCE 100000.0f // the distance where everything can still be seen.
#define STARTDISTANCE 350.0f		// cam distance start
#define CAMERASPEED 0.0060f		  // Camerarotationspeed
#define STARTANGLEY 2.36f			  // cam angle
#define STARTANGLEZ 1.3f			  // cam angle
#define GRAVITY -50.0f
#define PI 3.14159265f

// irrKlang declaration
ISoundEngine* engine = NULL;

// irrlicht declarations
IVideoDriver* driver = NULL;
ISceneManager* smgr = NULL;
//IGUIEnvironment* guienv = NULL;
ICameraSceneNode* camera = NULL;

CIrrEventReceiver rv;

ISceneNode* sphereOne = NULL;
ISceneNode* sphereTwo = NULL;
ISceneNode* ground = NULL;
ISceneNode* leftWall = NULL;
ISceneNode* rightWall = NULL;
ISceneNode* backWall = NULL;
ISceneNode* frontWall = NULL;

// newton physics declarations
static NewtonWorld * nWorld = NULL;
NewtonCollision * collision = NULL;

NewtonBody* sphereOneBody = NULL;
NewtonBody* sphereTwoBody = NULL;
NewtonBody* groundBody = NULL;
NewtonBody* leftWallBody = NULL;
NewtonBody* rightWallBody = NULL;
NewtonBody* backWallBody = NULL;
NewtonBody* frontWallBody = NULL;

vector3df lastTargetPos; // used for 3th person camera view.

float CamSphereDistance = STARTDISTANCE; // Distance from sphere to Camera
float CamRotationAngleY = STARTANGLEY;	 // Cam rotation 
float CamRotationAngleZ = STARTANGLEZ;

bool updated = false;

void InitializePhysicsEngine();
void InitializeGraphicsWorld();
void UpdateCameraPosition();
void CreateSpheres();
void CreateWalls();


// callback functions for the newton bodies
static void PhysicsSetTransform(const NewtonBody* body, const dFloat* matrix);
static void PhysicsApplyForceAndTorque(const NewtonBody* body);
static void PhysicsBodyDestructor(const NewtonBody* body);

// callback functions for the 2 colliding bodies.
static int beginCallback(const NewtonMaterial* material, const NewtonBody* body0, const NewtonBody* body1);
static int processCallback(const NewtonMaterial* material, const NewtonContact* contact);
static void endCallback(const NewtonMaterial* material);


// struct for colliding bodies.
struct CollidingBodies
{
   const NewtonBody *colBody0, *colBody1;
	 bool SphereWithSphere;
} geomCollision;

// allocation of memory from Newton
void* _cdecl NewtonAlloc(int sizeInBytes)
{
	return new char[sizeInBytes];
}

// destruction of memory from Newton
void _cdecl NewtonFree(void *ptr, int sizeInBytes)
{
	char *tmp;
	tmp = (char*) ptr;
	delete[] tmp;
}

int main(int argc, char **argv)
{
	// setting vsync to true helps the physics engine be more accurate.
	IrrlichtDevice* device = createDevice(EDT_OPENGL, dimension2d<s32>(800,600), 32, false, true, true, &rv);
	driver = device->getVideoDriver();
	smgr = device->getSceneManager();
	IGUIEnvironment* guienv = device->getGUIEnvironment();

	// start the sound engine with default parameters
  engine = createIrrKlangDevice();

	// initialize the physics engine.
	InitializePhysicsEngine();
	
	// initialize the graphics world. It creates all walls and spheres.
	InitializeGraphicsWorld();
	
	float CamRotationBaseY;
	int lastFPS = 0;
	int force = 0;
	
	// play a music during the game
	ISound* music = engine->play3D("data/ophelia.mp3", vec3df(0,0,0), true, false, true);

	IGUIFont* guiFont = device->getGUIEnvironment()->getFont("data/fontlucida.png");

	camera = smgr->addCameraSceneNode(0, vector3df(30.0, 38.0, 2.0));
	
	if(camera)
	{
		camera->setTarget(sphereOne->getPosition());
		camera->setFarValue(CLIPPING_DISTANCE);
	}
		
	while(device->run())
	{
		if (device->isWindowActive()) 
		{
			// end process events
			rv.endEventProcess();

			// update newton (physics) world
			NewtonUpdate(nWorld, 1.0f/60);

			// Draw fps counter
			int fps = driver->getFPS();
			
			if (lastFPS != fps)
			{
				wchar_t tmp[1024];
				swprintf(tmp, 1024, L"Irrlicht & Newton Example [fps: %d] [triangles: %d]", 
								 fps, driver->getPrimitiveCountDrawn());
				
				device->setWindowCaption(tmp);
				lastFPS = fps;
			}
			
			core::stringw displayForce = "Force: ";
			displayForce += force;

			driver->beginScene(true, true, video::SColor(0,0,0,0)); 
			smgr->drawAll();

			guiFont->draw(displayForce.c_str(), core::rect<s32>(25,20,300,60), video::SColor(255,100,101,140));
			
			guienv->drawAll();
			driver->endScene();
			
			// updates the cam position for 3th person camera view.
			UpdateCameraPosition();
			
			// power up a force by pressing and holding down the right mouse button (MB_RIGHT)
			if(rv.isKeyPressed(KEY_KEY_F))
			{
				if(force < 100)
					force += 5.0;
			}

			// if mouse has moved you'll enter this if statement.
			// its used for the 3th person camera view.
			if(rv.isMouseButtonDown(MB_RIGHT))
			{
				if(rv.mouseMoved())
				{
					int DeltaX = rv.getDeltaMousePosX();
					int DeltaY = rv.getDeltaMousePosY();
					
					updated = true;

					// rotate camera							
					if (DeltaX > 0)
					{
						CamRotationAngleY += (DeltaX * CAMERASPEED);
						while(CamRotationAngleY > 2 * PI)
							CamRotationAngleY -= 2 * PI;
						CamRotationBaseY = CamRotationAngleY;
					}

					else
					{
						CamRotationAngleY += (DeltaX * CAMERASPEED);
						while(CamRotationAngleY < 2 * PI)
							CamRotationAngleY += 2 * PI;
						CamRotationBaseY = CamRotationAngleY;
					}
				}
			}
				
			// launch the sphere in the direction of the camera's view with a force
			if(rv.isMouseButtonDown(MB_LEFT))
			{
				vector3df camViewDirection = (camera->getTarget() - camera->getPosition()).normalize() * force;
				camViewDirection.Y = 0;
				vector3df nVeloc(camViewDirection);
				NewtonBodySetVelocity(sphereOneBody, &nVeloc.X);
			}
			
			// destroy everything and exits the game when esc is pressed.
			if(rv.isKeyPressed(KEY_ESCAPE))
			{
				// release music stream.
				music->drop();

				// delete irrKlang engine
				engine->drop(); 

				// Release Irrlicht device
				device->drop();

				// Destroy Physics World
				NewtonDestroy(nWorld);

				return 0;
			}
			
			// start processing the events of keyboard/mouse button presses.
			rv.startEventProcess();
		}
	}
	
	// release music stream.
	music->drop(); 

	// delete irrKlang engine
	engine->drop(); 

	// Release Irrlicht device
	device->drop();

	// Destroy Physics World
	NewtonDestroy(nWorld);

	return 0;
}

void InitializePhysicsEngine()
{
	nWorld = NewtonCreate(NewtonAlloc, NewtonFree);
	
	// set the linear solver model for faster speed 
  NewtonSetSolverModel (nWorld, 1);

	// set the adaptive friction model for faster speed 
  NewtonSetFrictionModel (nWorld, 1);

	// Set up default material properties for newton
	int i = NewtonMaterialGetDefaultGroupID(nWorld);
	NewtonMaterialSetDefaultFriction   (nWorld, i, i, 0.4f, 0.6f);
	NewtonMaterialSetDefaultElasticity (nWorld, i, i, 0.75f);
	NewtonMaterialSetDefaultSoftness   (nWorld, i, i, 0.0f);
	NewtonMaterialSetCollisionCallback (nWorld, i, i, sphereOneBody, beginCallback, processCallback, endCallback);
	NewtonMaterialSetCollisionCallback (nWorld, i, i, sphereTwoBody, beginCallback, processCallback, endCallback);
}

// Newton callback
static void PhysicsApplyForceAndTorque(const NewtonBody* body)
{
	float mass;
	float Ixx;
	float Iyy;
	float Izz;

	NewtonBodyGetMassMatrix(body, &mass, &Ixx, &Iyy, &Izz);

	vector3df force(0.0f , GRAVITY * mass , 0.0f);
	vector3df torque(0.0f, 0.0f, 0.0f);

	NewtonBodySetForce(body, &force.X);
}

// set the transformation of a rigid body
// You have to do this so that the Newton body and irrLicht scene node are in sync,
// and you actually see the changes created by the physics simulation
void PhysicsSetTransform(const NewtonBody* body, const float* matrix)
{
	scene::ISceneNode* primitive;

	// get the graphic object form the rigid body
	// Newton can store a pointer to the scene node in the body object.
	primitive = (scene::ISceneNode*) NewtonBodyGetUserData (body);

	// set the transformation matrix for this rigid body
	dMatrix& mat = *((dMatrix*)matrix);
	
	// Create a matrix to store the irrLicht transformation that will correspond
	// to the Newton transformation
	core::matrix4 irrMatrix;

	// There's no easy way to get the information from one to the other, so a memory copy is required.
	// The matrix is a 4x4 float
	memcpy (irrMatrix.pointer(), &mat, 16 * sizeof(float));

	// In order to get the transform right, you have to apply the translation and the rotation
	// from the Newton body to the irrLicht scene node.
	core::vector3df position (irrMatrix.getTranslation());
	position *= NEWTONTOIRR;

	core::vector3df rotation;
	NewtonGetEulerAngle(irrMatrix.pointer(), &rotation.X);
	rotation = rotation * (180.0 / 3.1416);
	
	primitive->setPosition(position);
	primitive->setRotation(rotation);
}

// check if bodies are almost colliding
static int beginCallback(const NewtonMaterial* material, const NewtonBody* body0, const NewtonBody* body1)
{
   geomCollision.colBody0 = body0;
   geomCollision.colBody1 = body1;
   return true; // telling newton we accept this collision
}

// process the collided bodies
static int processCallback(const NewtonMaterial* material, const NewtonContact* contact)
{
	if(geomCollision.colBody0 == sphereOneBody || geomCollision.colBody1 == sphereOneBody)
	{
		if(geomCollision.colBody0 == sphereTwoBody || geomCollision.colBody1 == sphereTwoBody)
		{
			geomCollision.SphereWithSphere = true;
		}
	}
	
	return true; // telling newton we accept this collision
}

// end the callback and play sound
static void endCallback(const NewtonMaterial* material)
{
	if(geomCollision.SphereWithSphere)
	{
		const char* filename;
		filename = "data/explosion.wav";
		engine->play3D(filename, vector3df(0.0,0.0,0.0));
		// set boolean to false or you'll hear the explosion.wav the whole time.
		geomCollision.SphereWithSphere = false;
	}
}

// rigid body destructor
static void PhysicsBodyDestructor(const NewtonBody* body)
{
	// set the body callback and user data to NULL to prevent infinite recursion
  NewtonBodySetDestructorCallback(body, NULL); 
  NewtonBodySetUserData(body, NULL);
}

void InitializeGraphicsWorld()
{
	// create the sky box
	smgr->addSkyBoxSceneNode
	(
		driver->getTexture("data/irrlicht2_up.jpg"),
		driver->getTexture("data/irrlicht2_dn.jpg"),
		driver->getTexture("data/irrlicht2_lf.jpg"),
		driver->getTexture("data/irrlicht2_rt.jpg"),
		driver->getTexture("data/irrlicht2_ft.jpg"),
		driver->getTexture("data/irrlicht2_bk.jpg")
	);

	CreateSpheres();
	CreateWalls();
}

void CreateSpheres()
{
	/************** creat sphereOne **************/
	dVector size(60.0f, 2.0f, 60.0f);
	dMatrix location(GetIdentityMatrix());
	location.m_posit.m_y = 0.0f;

	float irrSize1 = size.m_x * NEWTONTOIRR;

	// set the initial size for the spheres
	dVector sphereSize = dVector(1.0f, 1.0f, 1.0f);

	// irrlich takes only 1 parameter for the size of the radius so we have to scale it down a bit.
	irrSize1 = sphereSize.m_x * NEWTONTOIRR;

	// create the collision 
	collision = NewtonCreateSphere(nWorld, sphereSize.m_x, sphereSize.m_y, sphereSize.m_z, NULL);
	
	// position the sphere in the world
	location.m_posit.m_x = 20.0f;
	location.m_posit.m_y = 1.5f;
	location.m_posit.m_z = -20.0f;
	
	// create a graphic sphere.
	sphereOne = smgr->addSphereSceneNode(irrSize1, 32);
	
	if(sphereOne)
	{
		sphereOne->setMaterialFlag(video::EMF_LIGHTING, false);
		sphereOne->setMaterialTexture(0, driver->getTexture("data/earth.bmp"));
	}

	//create the rigid body
	sphereOneBody = NewtonCreateBody(nWorld, collision);
	
	// wont make the sphere being disabled.
	NewtonBodySetAutoFreeze(sphereOneBody, false);
	
	// save the pointer to the graphic object with the body.
	NewtonBodySetUserData(sphereOneBody, sphereOne);

	// set a destructor for this rigid body
	NewtonBodySetDestructorCallback(sphereOneBody, PhysicsBodyDestructor);

	// set the transform call back function
	NewtonBodySetTransformCallback(sphereOneBody, PhysicsSetTransform);

	// set the force and torque call back function
	NewtonBodySetForceAndTorqueCallback(sphereOneBody, PhysicsApplyForceAndTorque);

	// set the mass matrix
	NewtonBodySetMassMatrix(sphereOneBody, 2.0f, 1.0f, 1.0f, 1.0f);
	
	// set the matrix for both the rigid body and the graphic body
	NewtonBodySetMatrix(sphereOneBody, &location[0][0]);
	PhysicsSetTransform(sphereOneBody, &location[0][0]);

	location.m_posit.m_y += size.m_y * 2.0f;

	// release the collision geometry when not needed
	NewtonReleaseCollision(nWorld, collision);
	/************** end creation of sphereOne **************/


	/************** creat sphereTwo **************/
	dVector size2(60.0f, 2.0f, 60.0f);
	dMatrix location2(GetIdentityMatrix());
	location2.m_posit.m_y = 0.0f;

	float irrSize2 = size2.m_x * NEWTONTOIRR;

	// set the initial size for the spheres
	dVector sphereSize2 = dVector(1.0f, 1.0f, 1.0f);
	irrSize2 = sphereSize2.m_x * NEWTONTOIRR;

	// create the collision 
	collision = NewtonCreateSphere(nWorld, sphereSize2.m_x, sphereSize2.m_y, sphereSize2.m_z, NULL);
	
	// position sphere in the world
	location2.m_posit.m_x = -20.0f;
	location2.m_posit.m_y = 1.5f;
	location2.m_posit.m_z = 20.0f; 

	// create a graphic sphere.
	sphereTwo = smgr->addSphereSceneNode(irrSize2, 32);
	
	if(sphereTwo)
	{
		sphereTwo->setMaterialFlag(video::EMF_LIGHTING, false);
		sphereTwo->setMaterialTexture(0, driver->getTexture("data/earth.bmp"));
	}

	// create the rigid body
	sphereTwoBody = NewtonCreateBody(nWorld, collision);
	
	// wont make the sphere being disabled.
	NewtonBodySetAutoFreeze(sphereTwoBody, false);
	
	// save the pointer to the graphic object with the body.
	NewtonBodySetUserData(sphereTwoBody, sphereTwo);

	// set a destructor for this rigid body
	NewtonBodySetDestructorCallback(sphereTwoBody, PhysicsBodyDestructor);

	// set the transform call back function
	NewtonBodySetTransformCallback(sphereTwoBody, PhysicsSetTransform);

	// set the force and torque call back function
	NewtonBodySetForceAndTorqueCallback(sphereTwoBody, PhysicsApplyForceAndTorque);

	// set the mass matrix
	NewtonBodySetMassMatrix(sphereTwoBody, 2.0f, 1.0f, 1.0f, 1.0f);
	
	// set the matrix for both the rigid body and the graphic body
	NewtonBodySetMatrix(sphereTwoBody, &location2[0][0]);
	PhysicsSetTransform(sphereTwoBody, &location2[0][0]);

	location2.m_posit.m_y += size2.m_y * 2.0f;

	// release the collision geometry when not needed
	NewtonReleaseCollision(nWorld, collision);
	/************** end creation of sphereTwo **************/
}

void CreateWalls()
{
	/****************** create the floor ******************/
	dVector size(80.0f, 1.0f, 80.0f);
	dMatrix location(GetIdentityMatrix());
	location.m_posit.m_x = 0.0f;
	location.m_posit.m_y = 0.0f;
	location.m_posit.m_z = 0.0f;

	// create a box for ground 
	// Uses the addCubeSceneNode function, but since it only takes one size parameter, 
  // you have to squish it using the scale vector
	core::vector3df position(location.m_posit.m_x, location.m_posit.m_y, location.m_posit.m_z);
	position *= NEWTONTOIRR;
	
	float irrSize = size.m_x * NEWTONTOIRR;
	
	ground = smgr->addCubeSceneNode(irrSize, 0, -1, position,
																	core::vector3df(0.0, 0.0, 0.0),
																	core::vector3df(1.0, 1.0 / 100.0, 1.0));
	
	if(ground)
	{
		ground->setMaterialFlag(video::EMF_LIGHTING, false);
		ground->setMaterialTexture(0, driver->getTexture("data/grass.jpg"));
		ground->getMaterial(0).getTextureMatrix(0).setTextureScale(32,32);
	}

	// create the the ground collision, and body with default values
	collision = NewtonCreateBox(nWorld, size.m_x, size.m_y, size.m_z, NULL); 
	groundBody = NewtonCreateBody(nWorld, collision);
  NewtonReleaseCollision(nWorld, collision);

	// wont make the wall being disabled.
	NewtonBodySetAutoFreeze(groundBody, false);

	// set a destructor for this rigid body
	NewtonBodySetDestructorCallback(groundBody, PhysicsBodyDestructor);

	// set the transformation for this rigid body
	NewtonBodySetMatrix(groundBody, &location[0][0]);

	// save the pointer to the graphic object with the body.
	NewtonBodySetUserData(groundBody, ground);
	/****************** end creation of the floor ******************/

	/****************** create the left wall ******************/
	// drawing the left wall for collision detection
	dVector size2(1.0f, 80.0f, 80.0f);
	dMatrix location2(GetIdentityMatrix());
	location2.m_posit.m_x = -40.0f;
	location2.m_posit.m_y = 40.0f;
	location2.m_posit.m_z = 0.0f;
	
	// create a box for wall 
	// Uses the addCubeSceneNode function, but since it only takes one size parameter, 
  // you have to squish it using the scale vector
	core::vector3df position2(location2.m_posit.m_x, location2.m_posit.m_y, location2.m_posit.m_z);
	
	position2 *= NEWTONTOIRR;
	
	float irrSize2 = size2.m_y * NEWTONTOIRR;

	leftWall = smgr->addCubeSceneNode(irrSize2, 0, -1, position2,
						 core::vector3df(0.0, 0.0, 0.0),
						 core::vector3df(1.0 / 100.0, 1.0, 1.0));
	
	if(leftWall)
	{
		leftWall->setMaterialFlag(video::EMF_LIGHTING, false);
		leftWall->setVisible(false); // if you want to see the wall set it to true.
		leftWall->setMaterialTexture(0, driver->getTexture("data/terrain-texture.jpg"));
	}

	// create the the leftWall collision, and body with default values
	collision = NewtonCreateBox(nWorld, size2.m_x, size2.m_y, size2.m_z, NULL); 
	leftWallBody = NewtonCreateBody(nWorld, collision);
	NewtonReleaseCollision(nWorld, collision);

	// wont make the wall being disabled.
	NewtonBodySetAutoFreeze(leftWallBody, false);

	// set a destructor for this rigid body
	NewtonBodySetDestructorCallback(leftWallBody, PhysicsBodyDestructor);
	
	// set the transformation for this rigid body
	NewtonBodySetMatrix(leftWallBody, &location2[0][0]);

	// save the pointer to the graphic object with the body.
	NewtonBodySetUserData(leftWallBody, leftWall);
	/****************** end creation of the left wall ******************/

	
	/****************** create the right wall ******************/
	// drawing the right wall for collision detection
	dVector size3(1.0f, 80.0f, 80.0f);
	dMatrix location3(GetIdentityMatrix());
	location3.m_posit.m_x = 40.0f;
	location3.m_posit.m_y = 40.0f;
	location3.m_posit.m_z = 0.0f;
	
	// create a box for wall 
	// Uses the addCubeSceneNode function, but since it only takes one size parameter, 
  // you have to squish it using the scale vector
	core::vector3df position3(location3.m_posit.m_x, location3.m_posit.m_y, location3.m_posit.m_z);
	
	position3 *= NEWTONTOIRR;
	
	float irrSize3 = size3.m_y * NEWTONTOIRR;

	rightWall = smgr->addCubeSceneNode(irrSize2, 0, -1, position3,
						 core::vector3df(0.0, 0.0, 0.0),
						 core::vector3df(1.0 / 100.0, 1.0, 1.0));
	
	if(rightWall)
	{
		rightWall->setMaterialFlag(video::EMF_LIGHTING, false);
		rightWall->setVisible(false);
		rightWall->setMaterialTexture(0, driver->getTexture("data/terrain-texture.jpg"));
	}

	// create the the leftWall collision, and body with default values
	collision = NewtonCreateBox(nWorld, size3.m_x, size3.m_y, size3.m_z, NULL); 
	rightWallBody = NewtonCreateBody(nWorld, collision);
	NewtonReleaseCollision(nWorld, collision);

	// wont make the wall being disabled.
	NewtonBodySetAutoFreeze(rightWallBody, false);
	
	// set a destructor for this rigid body
	NewtonBodySetDestructorCallback(rightWallBody, PhysicsBodyDestructor);

	// set the transformation for this rigid body
	NewtonBodySetMatrix(rightWallBody, &location3[0][0]);

	// save the pointer to the graphic object with the body.
	NewtonBodySetUserData(rightWallBody, rightWall);
	/****************** end creation of the right wall ******************/


	/****************** create the back wall ******************/
	// drawing the back wall for collision detection
	dVector size4(80.0f, 80.0f, 1.0f);
	dMatrix location4(GetIdentityMatrix());
	location4.m_posit.m_x = 0.0f;
	location4.m_posit.m_y = 40.0f;
	location4.m_posit.m_z = 40.0f;
	
	// create a box for wall 
	// Uses the addCubeSceneNode function, but since it only takes one size parameter, 
  // you have to squish it using the scale vector
	core::vector3df position4(location4.m_posit.m_x, location4.m_posit.m_y, location4.m_posit.m_z);
	
	position4 *= NEWTONTOIRR;
	
	float irrSize4 = size4.m_x * NEWTONTOIRR;

	backWall = smgr->addCubeSceneNode(irrSize4, 0, -1, position4,
						 core::vector3df(0.0, 0.0, 0.0),
						 core::vector3df(1.0, 1.0, 1.0 / 100.0));
	
	if(backWall)
	{
		backWall->setMaterialFlag(video::EMF_LIGHTING, false);
		backWall->setVisible(false);
		backWall->setMaterialTexture(0, driver->getTexture("data/terrain-texture.jpg"));
	}

	// create the the leftWall collision, and body with default values
	collision = NewtonCreateBox(nWorld, size4.m_x, size4.m_y, size4.m_z, NULL); 
	backWallBody = NewtonCreateBody(nWorld, collision);
	NewtonReleaseCollision(nWorld, collision);

	// wont make the wall being disabled.
	NewtonBodySetAutoFreeze(backWallBody, false);
	
	// set a destructor for this rigid body
	NewtonBodySetDestructorCallback(backWallBody, PhysicsBodyDestructor);

	// set the transformation for this rigid body
	NewtonBodySetMatrix(backWallBody, &location4[0][0]);

	// save the pointer to the graphic object with the body.
	NewtonBodySetUserData(backWallBody, backWall);
	/****************** end creation of the back wall ******************/


	/****************** create the front wall ******************/
	// drawing the front wall for collision detection
	dVector size5(80.0f, 80.0f, 1.0f);
	dMatrix location5(GetIdentityMatrix());
	location5.m_posit.m_x = 0.0f;
	location5.m_posit.m_y = 40.0f;
	location5.m_posit.m_z = -40.0f;
	
	// create a box for wall 
	// Uses the addCubeSceneNode function, but since it only takes one size parameter, 
  // you have to squish it using the scale vector
	core::vector3df position5(location5.m_posit.m_x, location5.m_posit.m_y, location5.m_posit.m_z);
	
	position5 *= NEWTONTOIRR;
	
	// irrlich takes only 1 parameter for the size of the cube so we have to scale it down a bit.
	float irrSize5 = size5.m_x * NEWTONTOIRR;

	frontWall = smgr->addCubeSceneNode(irrSize5, 0, -1, position5,
						 core::vector3df(0.0, 0.0, 0.0),
						 core::vector3df(1.0, 1.0, 1.0 / 100.0));
	
	if(frontWall)
	{
		frontWall->setMaterialFlag(video::EMF_LIGHTING, false);
		frontWall->setVisible(false);
		frontWall->setMaterialTexture(0, driver->getTexture("data/terrain-texture.jpg"));
	}

	// create the the leftWall collision, and body with default values
	collision = NewtonCreateBox(nWorld, size5.m_x, size5.m_y, size5.m_z, NULL); 
	frontWallBody = NewtonCreateBody(nWorld, collision);
	NewtonReleaseCollision(nWorld, collision);
	
	// wont make the wall being disabled.
	NewtonBodySetAutoFreeze(frontWallBody, false);

	// set a destructor for this rigid body
	NewtonBodySetDestructorCallback(frontWallBody, PhysicsBodyDestructor);

	// set the transformation for this rigid body
	NewtonBodySetMatrix(frontWallBody, &location5[0][0]);

	// save the pointer to the graphic object with the body.
	NewtonBodySetUserData(frontWallBody, frontWall);
	/****************** end creation of the front wall ******************/
}

// this function is used for the 3th person camera view.
void UpdateCameraPosition() 
{ 
	if(lastTargetPos != sphereOne->getPosition()) 
	{
		updated = true;
		lastTargetPos = sphereOne->getPosition();
	}
	
	if(updated) 
	{
		updated = false; // reset updated
		irr::core::vector3df cameraPos = lastTargetPos;
		cameraPos.X += CamSphereDistance * sin(CamRotationAngleY) * sin(CamRotationAngleZ);
		cameraPos.Z += CamSphereDistance * cos(CamRotationAngleY) * sin(CamRotationAngleZ);					
		cameraPos.Y += CamSphereDistance * cos(CamRotationAngleZ);
		camera->setPosition(cameraPos);
		camera->setTarget(lastTargetPos);		
	}
}