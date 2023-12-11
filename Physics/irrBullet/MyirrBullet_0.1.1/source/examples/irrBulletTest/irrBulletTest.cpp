#include <iostream>
#include <irrlicht.h>
#include <irrbullet.h>



using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

ISceneManager *smgr;
IVideoDriver *driver;
IrrlichtDevice *device = 0;

irrBulletWorld *world;

ICameraSceneNode *camera;

// Here we set up our collision masks.
// These enumerations are for the type of object that the needed rigid body will be.
enum MyGameMasks
{
    EMGM_NONE = 0,
    EMGM_GROUND = BIT(1),
    EMGM_PROPS = BIT(2)
};

// These are the types that any given rigid body can actually interact with.
// If you set the 2nd parameter in world->addRigidBody() to EMGM_PROPS,
// and set the 3rd parameter to collideWithRigidBody, it would only collide with other
// rigid bodies using that mask. If you set it to EMGM_GROUND, it would only collide
// with the rigid bodies using the EMGM_GROUND mask.
u32 collideWithGround = EMGM_GROUND;
u32 collideWithRigidBody = EMGM_PROPS;
u32 collideWithAll = EMGM_GROUND | EMGM_PROPS;


// To add a rigid body with collision masks, simply add two extra paremeters to the creation code:
// world->addRigidBody(shape, EMGM_GROUND, collideWithRigidBody);
// instead of world->addRigidBody(shape)
void createGround()
{
    ISceneNode *Node = smgr->addCubeSceneNode(1.0f);
	Node->setScale(vector3df(400,3,400));
	Node->setPosition(vector3df(0,0,0));
	Node->setMaterialFlag(video::EMF_LIGHTING, true);
	Node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
	Node->setMaterialTexture(0, driver->getTexture("rockwall.jpg"));

	ICollisionShape *shape = new IBoxShape(Node, 0);

	IRigidBody *body;
	body = world->addRigidBody(shape, EMGM_GROUND, collideWithRigidBody);
}

void addCube(const vector3df &pos, const vector3df &scale, f64 mass)
{
    ISceneNode *Node = smgr->addCubeSceneNode(1.0f);
	Node->setScale(scale);
	Node->setPosition(pos);
	Node->setMaterialFlag(video::EMF_LIGHTING, true);
	Node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
	Node->setMaterialTexture(0, driver->getTexture("crate.jpg"));

	ICollisionShape *shape = new IBoxShape(Node, mass);

	shape->setMargin(0.1);

	IRigidBody *body;
	body = world->addRigidBody(shape, EMGM_PROPS, collideWithAll);
}

void shootCube(const vector3df &scale, f64 mass)
{
	vector3df pos(camera->getPosition().X,camera->getPosition().Y,camera->getPosition().Z);

	ISceneNode *Node = smgr->addCubeSceneNode(1.0f);
	Node->setScale(scale);
	Node->setPosition(pos);
	Node->setMaterialFlag(video::EMF_LIGHTING, true);
	Node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
	Node->setMaterialTexture(0, driver->getTexture("rockwall.jpg"));

	ICollisionShape *shape = new IBoxShape(Node, mass);

	shape->setMargin(0.1);

	IRigidBody *body = world->addRigidBody(shape, EMGM_PROPS, collideWithAll);

	core::vector3df rot = camera->getRotation();
    core::matrix4 mat;
    mat.setRotationDegrees(rot);
    vector3df forwardDir(vector3df(mat[8],mat[9],mat[10]) * 120);

    body->setLinearVelocity(forwardDir);
}

void shootSphere(const vector3df &scale, f64 mass)
{
	vector3df pos(camera->getPosition().X,camera->getPosition().Y,camera->getPosition().Z);

	ISceneNode *Node = smgr->addSphereSceneNode();
	Node->setScale(scale);
	Node->setPosition(pos);
	Node->setMaterialFlag(video::EMF_LIGHTING, true);
	Node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
	Node->setMaterialTexture(0, driver->getTexture("rockwall.jpg"));

	ICollisionShape *shape = new ISphereShape(Node, mass);

	shape->setMargin(0.1);

	IRigidBody *body = world->addRigidBody(shape, EMGM_PROPS, collideWithAll);

    // Since we use a "bumpy" triangle mesh for the terrain, we need to set the CCD radius and
    // motion threshold to ensure that we don't have any tunneling problems.
    // This doesn't work very well for box shapes.
	// ALSO: If you use setLocalScaling() on the collision shape, be sure to call this again
	// after scaling to make sure that it matches the actual object.
	body->autoCalculateCcdValues();

	core::vector3df rot = camera->getRotation();
    core::matrix4 mat;
    mat.setRotationDegrees(rot);
    vector3df forwardDir(vector3df(mat[8],mat[9],mat[10]) * 120);

    body->setLinearVelocity(forwardDir);
}



class EventReceiver : public IEventReceiver  {

public:

	virtual bool OnEvent(const SEvent &TEvent) {

		if(TEvent.EventType == EET_MOUSE_INPUT_EVENT && (TEvent.MouseInput.Event==EMIE_LMOUSE_PRESSED_DOWN))
		{

			shootCube(vector3df(2,2,2), 1);

			return true;
		}

		else
		if(TEvent.EventType == EET_MOUSE_INPUT_EVENT && (TEvent.MouseInput.Event==EMIE_RMOUSE_PRESSED_DOWN))
		{

			shootSphere(vector3df(0.2,0.2,0.2), 0.2);

			return true;
		}
		return false;
	}
};


int main()
{
    EventReceiver receiver;
    device =
        createDevice( video::EDT_OPENGL, dimension2d<u32>(640, 480), 16,
            false, false, false, &receiver);

    if (!device)
        return 1;

    device->setWindowCaption(L"irrBullet Example");


    driver = device->getVideoDriver();
    smgr = device->getSceneManager();
    IGUIEnvironment* env = device->getGUIEnvironment();

    // Create the irrBullet world with the Irrlicht device as a parameter.
    world = createIrrBulletWorld(device);
    createGround();

    u32 rows = 10; // 10
    u32 height = 20; // 20
    for(u32 j=0; j < height; j++)
    {
        for(u32 i=0; i < rows; i++)
        {
            addCube(vector3df(3 * i, 300 + 3 * j + 3, 0), vector3df(3,3,3), 3);
        }
    }

    smgr->addLightSceneNode(0, vector3df(20, 40, -50), SColorf(1.0f, 1.0f, 1.0f), 4000.0f);


	camera = smgr->addCameraSceneNodeFPS();
	camera->setPosition(vector3df(15,15,4));


	ISceneNode *Node = smgr->addMeshSceneNode(smgr->getMesh("terrainMain.b3d"));
	Node->setPosition(vector3df(0,0,0));
	Node->setMaterialFlag(video::EMF_LIGHTING, false);

    // For the terrain, instead of adding a cube or sphere shape, we are going to
    // add a BvhTriangleMeshShape. This is the standard trimesh shape
    // for static objects. The first parameter is of course the node to control,
    // the second parameter is the collision mesh, incase you want a low-poly collision mesh,
    // and the third parameter is the mass.
	ICollisionShape *shape = new IBvhTriangleMeshShape(Node, smgr->getMesh("terrainMain.b3d"), 0.0);

	shape->setMargin(0.0);

    // The rigid body will be placed at the origin of the node that the collision shape is controlling,
    // so we do not need to set the position after positioning the node.
	IRigidBody *terrain = world->addRigidBody(shape, EMGM_GROUND, collideWithRigidBody);
	terrain->getPointer()->setCollisionFlags(1);

	// When setting a rigid body to a static object, please be sure that you have
	// that object's mass set to 0.0. Otherwise, undesired results will occur.
	terrain->setCollisionFlags(ECF_STATIC_OBJECT);


	SMaterial material;
	material.Lighting = false;

    u32 TimeStamp = device->getTimer()->getTime(), DeltaTime = 0;
	while(device->run())
    {
        driver->beginScene(true, true, SColor(255,100,101,140));

        DeltaTime = device->getTimer()->getTime() - TimeStamp;
		TimeStamp = device->getTimer()->getTime();

        // Step the simulation with our delta time
        world->stepSimulation(DeltaTime*0.001f, 120);

        driver->setMaterial(material);
        driver->setTransform(irr::video::ETS_WORLD, irr::core::matrix4());

        // This call will draw the technical properties of the physics simulation
        // to the GUI environment.
        world->debugDrawProperties(true, SColor(255,255,255,255));

        smgr->drawAll();
        env->drawAll();

        driver->endScene();
    }
    // We're done with the IrrBullet world, so we free the memory that it takes up.
    delete world;
    device->drop();

    return 0;
}

