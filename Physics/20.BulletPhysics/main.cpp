/** Example 002 Quake3Map

This Tutorial shows how to load a Quake 3 map into the engine, create a
SceneNode for optimizing the speed of rendering, and how to create a user
controlled camera.

Please note that you should know the basics of the engine before starting this
tutorial. Just take a short look at the first tutorial, if you haven't done
this yet: http://irrlicht.sourceforge.net/tut001.html 

*/

///We took the Irrlicht Quake3Map example, and integrated Bullet rigid body and character controller.

#include <irrlicht.h>
#include <iostream>
#include "BspConverter.h"
#include "BspLoader.h"

#include "btBulletDynamicsCommon.h"
#include "BulletDynamics/Character/btKinematicCharacterController.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"

///downsize objects, so they are in the range 1, rather then 10

///By default, this Quake 3 Map uses units that are much bigger than meter units. 
///Scale the physics simulation world down using physicsWorldScaling, so objects are around 1 (meter) units
btScalar physicsWorldScaling = 0.1f;

///Bullet character controller
btKinematicCharacterController* character=0;
///(in Quake units, 0.1*20 = 2 meter)
btScalar characterHeight = 20.f; 
btScalar characterWidth = 10.f;
btScalar walkSpeed = 5.f;

///Optional: ColladaConverter can be used to save a physics snapshot, useful for debugging. 
///Just add ColladaConverter.cpp and link against libBulletColladaConverter, libColladaDom and libXml
///#include "ColladaConverter.h"

/*
As already written in the HelloWorld example, in the Irrlicht Engine everything
can be found in the namespace 'irr'. To get rid of the irr:: in front of the
name of every class, we tell the compiler that we use that namespace from now
on, and we will not have to write that 'irr::'. There are 5 other sub
namespaces 'core', 'scene', 'video', 'io' and 'gui'. Unlike in the HelloWorld
example, we do not call 'using namespace' for these 5 other namespaces, because
in this way you will see what can be found in which namespace. But if you like,
you can also include the namespaces like in the previous example.
*/
using namespace irr;

///a few globals, just for demo purposes
scene::ICameraSceneNode* cam=0;
scene::ISceneManager* smgr=0;
video::IVideoDriver* driver=0;
btDiscreteDynamicsWorld* gDynamicsWorld=0;
int numCreatedObjects = 0;
int numUpdatedBodies = 0;

btAlignedObjectArray<btCollisionShape*> gCollisionShapes;
bool Done = true;

///this class helps finding issues between Irrlicht and Bullet representation: problem with transform, scaling, collision shapes and constraint setup.
class IrrlichtDebugDrawer : public btIDebugDraw
{
	video::IVideoDriver*	m_driver;
	int m_debugMode;

public:
	IrrlichtDebugDrawer(video::IVideoDriver* driver):
	  m_driver(driver),
		m_debugMode(0)
	{
	}
	///most important debug drawing method, drawing lines
	virtual void	drawLine(const btVector3& from,const btVector3& to,const btVector3& color)
	{
		m_driver->draw3DLine((const irr::core::vector3df &)from,(const irr::core::vector3df &)to);
	}
	///optional debug methods
	virtual void	drawContactPoint(const btVector3& PointOnB,const btVector3& normalOnB,btScalar distance,int lifeTime,const btVector3& color) {}
	virtual void	reportErrorWarning(const char* warningString){}
	virtual void	draw3dText(const btVector3& location,const char* textString){}
	virtual void	setDebugMode(int debugMode)	{	m_debugMode = debugMode;}
	virtual int		getDebugMode() const	{		return m_debugMode;
	}
};

///BspToBulletConverter  extends the BspConverter to convert to Bullet datastructures. In this case, Irrlicht swaps Z and Y, so we do the same.
class IrrlichtBspBulletConverter : public BspConverter
{

public:

	IrrlichtBspBulletConverter()
	{
	}

		virtual void	addConvexVerticesCollider(btAlignedObjectArray<btVector3>& vertices, bool isEntity, const btVector3& entityTargetLocation)
		{
			///perhaps we can do something special with entities (isEntity)
			///like adding a collision Triggering (as example)
			
			if (vertices.size() > 0)
			{
				///irrlicht swaps Z and Y in a BSP file?
				btAlignedObjectArray<btVector3> rotatedVerts;
				for (int i=0;i<vertices.size();i++)
				{
					rotatedVerts.push_back(btVector3(vertices[i].getX(),vertices[i].getZ(),vertices[i].getY())*physicsWorldScaling);
				}
				float mass = 0.f;
				btTransform startTransform;
				//can use a shift
				startTransform.setIdentity();
				startTransform.setOrigin(btVector3(0,0,-10.f));
				//this create an internal copy of the vertices
				
				btCollisionShape* shape = new btConvexHullShape(&(rotatedVerts[0].getX()),rotatedVerts.size());
				gCollisionShapes.push_back(shape);

				btRigidBody* body = new btRigidBody(0.f,0,shape);
				//body->setCenterOfMassTransform(btTransform(btQuaternion::getIdentity(),btVector3(-1300,-144,-1249)));
				gDynamicsWorld->addRigidBody(body);
			}
		}
};


// Converts a Bullet quaternion to an Irrlicht euler angle
void QuaternionToEuler(const btQuaternion &TQuat, btVector3 &TEuler) {
	btScalar W = TQuat.getW();
	btScalar X = TQuat.getX();
	btScalar Y = TQuat.getY();
	btScalar Z = TQuat.getZ();
	float WSquared = W * W;
	float XSquared = X * X;
	float YSquared = Y * Y;
	float ZSquared = Z * Z;

	TEuler.setX(atan2f(2.0f * (Y * Z + X * W), -XSquared - YSquared + ZSquared + WSquared));
	TEuler.setY(asinf(-2.0f * (X * Z - Y * W)));
	TEuler.setZ(atan2f(2.0f * (X * Y + Z * W), XSquared - YSquared - ZSquared + WSquared));
	TEuler *= core::RADTODEG;
}


///This class sychronizes the world transform between Bullet rigid bodies and their accompanying Irrlicht nodes
class IrrMotionState : public btDefaultMotionState
{
	scene::ISceneNode* m_irrNode;
public:

	IrrMotionState(scene::ISceneNode* irrNode)
		:m_irrNode(irrNode)
	{
	}
	///synchronizes world transform from user to physics
	virtual void	getWorldTransform(btTransform& centerOfMassWorldTrans ) const 
	{
		centerOfMassWorldTrans = m_graphicsWorldTrans;
	}

	///synchronizes world transform from physics to user
	///Bullet only calls the update of worldtransform for active objects
	virtual void	setWorldTransform(const btTransform& centerOfMassWorldTrans)
	{
		numUpdatedBodies++;
		m_startWorldTrans = centerOfMassWorldTrans;
		m_graphicsWorldTrans = centerOfMassWorldTrans;

		const btVector3& Point = centerOfMassWorldTrans.getOrigin();
		m_irrNode->setPosition(core::vector3df((f32)Point[0], (f32)Point[1], (f32)Point[2])/physicsWorldScaling);

		// Set rotation
		btVector3 EulerRotation;
		QuaternionToEuler(centerOfMassWorldTrans.getRotation(), EulerRotation);
		m_irrNode->setRotation(core::vector3df(EulerRotation[0], EulerRotation[1], EulerRotation[2]));
	}

};


/// Create a box rigid body
void CreateBox(const btVector3& position, const btVector3& initialVelocity,const core::vector3df& scale, btScalar mass) 
{
	numCreatedObjects++;
	
	scene::ISceneNode *Node = smgr->addCubeSceneNode(1.0f);
	Node->setScale(scale);
	Node->setMaterialFlag(video::EMF_LIGHTING, 0);
	Node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
	Node->setMaterialTexture(0, driver->getTexture("rust0.jpg"));
	

	// Set the initial position of the object
	btTransform Transform;
	Transform.setIdentity();
	Transform.setOrigin(position*physicsWorldScaling);

	btDefaultMotionState *MotionState = new IrrMotionState(Node);
	MotionState->setWorldTransform(Transform);

	// Create the shape
	btVector3 HalfExtents(scale.X * 0.5f, scale.Y * 0.5f, scale.Z * 0.5f);
	btCollisionShape *Shape = new btBoxShape(HalfExtents*physicsWorldScaling);

	// Add mass
	btVector3 LocalInertia;
	Shape->calculateLocalInertia(mass, LocalInertia);
	//Shape->setMargin(0.01);

	// Create the rigid body object
	btRigidBody *RigidBody = new btRigidBody(mass, MotionState, Shape, LocalInertia);

	// Store a pointer to the irrlicht node so we can update it later
	RigidBody->setUserPointer((void *)(Node));
	RigidBody->setLinearVelocity(initialVelocity);

	// Add it to the world
	gDynamicsWorld->addRigidBody(RigidBody);
}

float gPlayerForwardBackward=0;
float gPlayerSideways = 0;

/// Event receiver, control Bullet character controller and shoot some boxes
class BulletEventReceiverClass : public IEventReceiver  {

public:

	virtual bool OnEvent(const SEvent &TEvent) {

		if(TEvent.EventType == EET_MOUSE_INPUT_EVENT && (TEvent.MouseInput.Event==EMIE_LMOUSE_PRESSED_DOWN)) {

			btVector3 camPos(cam->getPosition().X,cam->getPosition().Y,cam->getPosition().Z);

			///create 100 boxes at the current camera position
			{
				core::vector3df rot = cam->getRotation();
				core::matrix4 mat;
				mat.setRotationDegrees(rot);
				btVector3 forwardDir(mat[8],mat[9],mat[10]);
			
				CreateBox(camPos+forwardDir*10,forwardDir*50,core::vector3df(10,10,10),1000.);
			}

			return true;
		}

			// check if user presses the key F9
		if ((TEvent.EventType == EET_KEY_INPUT_EVENT))
		{
			
			{
				switch (TEvent.KeyInput.Key)
				{
				case KEY_UP:
					{
						if (TEvent.KeyInput.PressedDown)
						{
							gPlayerForwardBackward=walkSpeed;
						} else
						{
							gPlayerForwardBackward=0;
						}
						return true;
					}
				case KEY_DOWN:
				{
					if (TEvent.KeyInput.PressedDown)
						{
							gPlayerForwardBackward=-walkSpeed;
						} else
						{
							gPlayerForwardBackward=0;
						}
					return true;
				}
				case KEY_LEFT:
					{
						if (TEvent.KeyInput.PressedDown)
						{
							gPlayerSideways=-walkSpeed;
						} else
						{
							gPlayerSideways=0;
						}
						return true;
					}
				case KEY_RIGHT:
				{
					if (TEvent.KeyInput.PressedDown)
						{
							gPlayerSideways=walkSpeed;
						} else
						{
							gPlayerSideways=0;
						}
					return true;
				}
				default:
					{
					}
				}
			}
		}
		return false;
	}
};

/*
Ok, lets start. Again, we use the main() method as start, not the WinMain().
*/
int main()
{
	extern float gContactBreakingThreshold;
	gContactBreakingThreshold = .1f;

	BulletEventReceiverClass receiver;

	///setup the main Bullet datastructures
	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
	collisionConfiguration->setConvexConvexMultipointIterations();
	btDispatcher*				dispatcher = new btCollisionDispatcher(collisionConfiguration);
	btBroadphaseInterface*		pairCache = new btDbvtBroadphase();
	btConstraintSolver*			constraintSolver = new btSequentialImpulseConstraintSolver();
	
	///create the main Bullet dynamics world
	gDynamicsWorld  = new btDiscreteDynamicsWorld(dispatcher,pairCache,constraintSolver,collisionConfiguration);
	gDynamicsWorld->setGravity(btVector3(0,-100*physicsWorldScaling,0));
	
	///register an additional callback for characters/ghost objects
	gDynamicsWorld->getBroadphase()->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
	/*
	Like in the HelloWorld example, we create an IrrlichtDevice with
	createDevice(). The difference now is that we ask the user to select
	which video driver to use. The Software device might be
	too slow to draw a huge Quake 3 map, but just for the fun of it, we make
	this decision possible, too.
	*/

	// ask user for driver

	video::E_DRIVER_TYPE driverType;

	/*
	printf("Please select the driver you want for this example:\n"\
		" (a) Direct3D 9.0c\n (b) Direct3D 8.1\n (c) OpenGL 1.5\n"\
		" (d) Software Renderer\n (e) Burning's Software Renderer\n"\
		" (f) NullDevice\n (otherKey) exit\n\n");

	char i;
	std::cin >> i;

	switch(i)
	{
		case 'a': driverType = video::EDT_DIRECT3D9;break;
		case 'b': driverType = video::EDT_DIRECT3D8;break;
		case 'c': driverType = video::EDT_OPENGL;   break;
		case 'd': driverType = video::EDT_SOFTWARE; break;
		case 'e': driverType = video::EDT_BURNINGSVIDEO;break;
		case 'f': driverType = video::EDT_NULL;     break;
		default: return 1;
	}	
	*/

	driverType = video::EDT_OPENGL;


	// create device and exit if creation failed

	bool useFullscreen = false;

	IrrlichtDevice *device =
		createDevice(driverType, core::dimension2d<s32>(800, 600),16,useFullscreen,false,false,&receiver);

	if (device == 0)
		return 1; // could not create selected driver.

	/*
	Get a pointer to the video driver and the SceneManager so that
	we do not always have to call irr::IrrlichtDevice::getVideoDriver() and
	irr::IrrlichtDevice::getSceneManager().
	*/
	driver = device->getVideoDriver();
	
	///now we have a display drive, link it to the Bullet debug drawer
	gDynamicsWorld->setDebugDrawer(new IrrlichtDebugDrawer(driver));
	gDynamicsWorld->getDebugDrawer()->setDebugMode(btIDebugDraw::DBG_DrawWireframe);

	smgr = device->getSceneManager();

	

	/*
	To display the Quake 3 map, we first need to load it. Quake 3 maps
	are packed into .pk3 files which are nothing else than .zip files.
	So we add the .pk3 file to our irr::io::IFileSystem. After it was added,
	we are able to read from the files in that archive as if they are
	directly stored on the disk.
	*/
	if (!device->getFileSystem()->addZipFileArchive("../../media/map-20kdm2.pk3"))
	{
		device->getFileSystem()->addZipFileArchive("map-20kdm2.pk3");
	}


	io::IReadFile* file = device->getFileSystem()->createAndOpenFile("20kdm2.bsp");
	long int bspsize = file->getSize();
	void* bspbuffer = new char[bspsize];
	file->read(bspbuffer,bspsize);
	IrrlichtBspBulletConverter* convert = new IrrlichtBspBulletConverter();
	float scaling=1.;
	BspLoader bspLoader;
	bspLoader.loadBSPFile(bspbuffer);
	convert->convertBsp(bspLoader,scaling);


	///optional: export the current Bullet world into COLLADA Physics representation (requires additional libraries)
	//ColladaConverter colladaConverter(gDynamicsWorld);
	//colladaConverter.save("20kdm2.dae");

	/*
	Now we can load the mesh by calling
	irr::scene::ISceneManager::getMesh(). We get a pointer returned to an
	irr::scene::IAnimatedMesh. As you might know, Quake 3 maps are not
	really animated, they are only a huge chunk of static geometry with
	some materials attached. Hence the IAnimatedMesh consists of only one
	frame, so we get the "first frame" of the "animation", which is our
	quake level and create an OctTree scene node with it, using
	irr::scene::ISceneManager::addOctTreeSceneNode().
	The OctTree optimizes the scene a little bit, trying to draw only geometry
	which is currently visible. An alternative to the OctTree would be a
	irr::scene::IMeshSceneNode, which would always draw the complete
	geometry of the mesh, without optimization. Try it: Use
	irr::scene::ISceneManager::addMeshSceneNode() instead of
	addOctTreeSceneNode() and compare the primitives drawn by the video
	driver. (There is a irr::video::IVideoDriver::getPrimitiveCountDrawn()
	method in the irr::video::IVideoDriver class). Note that this
	optimization with the OctTree is only useful when drawing huge meshes
	consisting of lots of geometry.
	*/
	scene::IAnimatedMesh* mesh = smgr->getMesh("20kdm2.bsp");
	scene::ISceneNode* node = 0;
	
	if (mesh)
		node = smgr->addOctTreeSceneNode(mesh->getMesh(0), 0, -1, 1024);
//		node = smgr->addMeshSceneNode(mesh->getMesh(0));

	/*
	Because the level was not modelled around the origin (0,0,0), we
	translate the whole level a little bit. This is done on
	irr::scene::ISceneNode level using the methods
	irr::scene::ISceneNode::setPosition() (in this case),
	irr::scene::ISceneNode::setRotation(), and
	irr::scene::ISceneNode::setScale().
	*/
	///we don't do this, because it makes the mapping between Irrlicht and Bullet more complicated.
	///instead, just move the camera/player starting point
//	if (node)
//		node->setPosition(core::vector3df(-1300,-144,-1249));

	/*
	Now we only need a camera to look at the Quake 3 map.
	We want to create a user controlled camera. There are some
	cameras available in the Irrlicht engine. For example the
	MayaCamera which can be controlled like the camera in Maya:
	Rotate with left mouse button pressed, Zoom with both buttons pressed,
	translate with right mouse button pressed. This could be created with
	irr::scene::ISceneManager::addCameraSceneNodeMaya(). But for this
	example, we want to create a camera which behaves like the ones in
	first person shooter games (FPS) and hence use
	irr::scene::ISceneManager::addCameraSceneNodeFPS().
	*/

	cam = smgr->addCameraSceneNodeFPS();
	cam->setPosition(core::vector3df(1300,144,1249));

	//cam = smgr->addCameraSceneNode();

	///create a Bullet character and its data structures. As you can see, it is a bit complicated. 
	///Once the Bullet character controller is more mature, we simplify this.

	btPairCachingGhostObject* ghostObject= new btPairCachingGhostObject();
	btConvexShape* characterShape = new btCapsuleShape(characterWidth*physicsWorldScaling,characterHeight*physicsWorldScaling);
	btTransform trans;
	trans.setIdentity();
	trans.setOrigin(btVector3(1300,144,1249)*physicsWorldScaling);
	ghostObject->setWorldTransform(trans);
	ghostObject->setCollisionShape(characterShape);
	btScalar stepHeight = 20*physicsWorldScaling;
	int upAxis = 1;
	character = new btKinematicCharacterController (ghostObject,characterShape,stepHeight, upAxis);
	gDynamicsWorld->addCollisionObject(ghostObject,btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::StaticFilter|btBroadphaseProxy::DefaultFilter);
	gDynamicsWorld->addCharacter(character);

	/*
	The mouse cursor needs not be visible, so we hide it via the
	irr::IrrlichtDevice::ICursorControl.
	*/
	device->getCursorControl()->setVisible(false);

	
	/*
	We have done everything, so lets draw it. We also write the current
	frames per second and the primitives drawn into the caption of the
	window. The test for irr::IrrlichtDevice::isWindowActive() is optional,
	but prevents the engine to grab the mouse cursor after task switching
	when other programs are active. The call to
	irr::IrrlichtDevice::yield() will avoid the busy loop to eat up all CPU
	cycles when the window is not active.
	*/
	int lastFPS = -1;

	while(device->run())
	{
		if (device->isWindowActive())
		{
			driver->beginScene(true, true, video::SColor(255,200,200,200));
		
			
			static int ms = device->getTimer()->getTime();
			int newTime = device->getTimer()->getTime();
			int deltaTimeMs = newTime-ms;
			ms = newTime;

			///set the forward direction of the character controller
			btVector3 walkDir(0,0,0);
			if (gPlayerForwardBackward)
			{
				core::vector3df rot = cam->getRotation();
				core::matrix4 mat;
				mat.setRotationDegrees(rot);
				btVector3 forwardDir(mat[8],mat[9],mat[10]);
				walkDir += forwardDir*gPlayerForwardBackward*physicsWorldScaling;
			}
			if (gPlayerSideways)
			{
				core::vector3df rot = cam->getRotation();
				core::matrix4 mat;
				mat.setRotationDegrees(rot);
				btVector3 sideWays(mat[0],mat[1],mat[2]);
				walkDir += sideWays*gPlayerSideways*physicsWorldScaling;
			}
			character->setWalkDirection(walkDir);
			

			gDynamicsWorld->stepSimulation(deltaTimeMs*0.001f);
			btVector3 c = character->getGhostObject()->getWorldTransform().getOrigin()/physicsWorldScaling;
			core::vector3df pos (c.getX(),c.getY()+characterHeight,c.getZ());
			cam->setPosition(pos);
			
			///enable next line to get detailed Bullet performance statistics
			//CProfileManager::dumpAll();
			///enable next line to make sure graphics world matches with physics world
		
			smgr->drawAll();
			
			///uncomment next line for some Bullet wireframe debug drawing
			//gDynamicsWorld->debugDrawWorld();
			driver->endScene();
			

			int fps = driver->getFPS();

			if (lastFPS != fps)
			{
				core::stringw str = L"Irrlicht+Bullet Physics example [";
				str += driver->getName();
				str += "] FPS:";
				str += fps;
				str += " Dynamic rigid bodies: ";
				str += numCreatedObjects;
				str += " ";
				str += numUpdatedBodies;
				device->setWindowCaption(str.c_str());
				lastFPS = fps;
			}
				numUpdatedBodies=0;
		}
		else
			device->yield();
	}

	/*
	In the end, delete the Irrlicht device.
	*/
	device->drop();

	///@todo: delete Bullet data structures, left as an exercise for the reader
	return 0;
}

/*
That's it. Compile and play around with the program.
**/
