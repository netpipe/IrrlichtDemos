// This example is part of irrBullet by Josiah Hartzell (fighterstw@hotmail.com or josiah.hartzell@gmail.com)

#include "collisionexample.h"
#include <irrlicht.h>
#include <irrbullet.h>

using namespace irr;
using namespace core;
using namespace video;
using namespace scene;
using namespace io;

CCollisionExample::CCollisionExample()
{
}

// To add a rigid body with collision masks, simply add two extra parameters to the creation code:
// world->addRigidBody(shape, EMGM_GROUND, collideWithRigidBody);
// instead of world->addRigidBody(shape)
void CCollisionExample::createGround()
{
    ISceneNode *Node = device->getSceneManager()->addCubeSceneNode(1.0);
	Node->setScale(vector3df(600,3,600)); // 400, 3, 400
	Node->setPosition(vector3df(200,0,100));
	Node->setMaterialFlag(video::EMF_LIGHTING, true);
	Node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
	Node->setMaterialTexture(0, device->getVideoDriver()->getTexture("rockwall.jpg"));

    if(drawWireFrame)
        Node->setMaterialFlag(EMF_WIREFRAME, true);

	ICollisionShape *shape = new IBoxShape(Node, 0, false);

	IRigidBody *body;
	body = world->addRigidBody(shape);
}

bool CCollisionExample::OnEvent(const SEvent& event)
{
	if (!device)
		return false;

    switch(event.EventType)
    {
        case EET_MOUSE_INPUT_EVENT:
        {
            if(event.MouseInput.Event==EMIE_LMOUSE_PRESSED_DOWN)
            {
                shootCube(vector3df(2,2,2), 1);
                return true;
            }

            else
            if(event.MouseInput.Event==EMIE_RMOUSE_PRESSED_DOWN)
            {
                shootSphere(vector3df(0.2,0.2,0.2), 0.2);
                return true;
            }
        }
        break;

        case EET_KEY_INPUT_EVENT:
        {
            if(event.KeyInput.Key == KEY_KEY_P && event.KeyInput.PressedDown == false)
            {
                world->pauseSimulation(!world->simulationPaused());
                return true;
            }
        }
        break;
        default:
            break;
    }
    return false;
}

void CCollisionExample::runExample()
{
    debugDraw = true;
    drawProperties = true;
    drawWireFrame = false;

    u32 rows = 10;
	u32 columns = 10;

    device =
        createDevice( video::EDT_OPENGL, dimension2d<u32>(640, 480), 16,
            false, false, false, this);


    device->setWindowCaption(L"irrBullet Collision Example - Josiah Hartzell");

    device->getFileSystem()->addFolderFileArchive("../../media/");


    device->getSceneManager()->addLightSceneNode(0, vector3df(20, 40, -50), SColorf(1.0f, 1.0f, 1.0f), 4000.0f);



    ////////////////////////////
    // Create irrBullet World //
    ////////////////////////////
    world = createIrrBulletWorld(device, true, debugDraw);

    world->setDebugMode(EPDM_DrawAabb |
            EPDM_DrawContactPoints);

    world->setGravity(vector3df(0,-10,0));

    createGround();


	for(int i=0; i < 8; i++)
    {
        srand(device->getTimer()->getTime());

        u32 force = rand() % 40 + 20;

        createTank(L"JagdPanther.b3d", L"JagdPanther.b3d", vector3df((i*40),5,-120), 12);
        static_cast<IRigidBody*>(world->getCollisionObjectByIndex(world->getNumCollisionObjects()-1))->setLinearVelocity(vector3df(0,0,force));
    }

    camera = device->getSceneManager()->addCameraSceneNodeFPS();
	camera->setPosition(vector3df(50,15,4));
	camera->setTarget(static_cast<IRigidBody*>(world->getCollisionObjectByName("Tank"))->getWorldTransform().getTranslation());


    // Set our delta time and time stamp
    u32 TimeStamp = device->getTimer()->getTime();
    u32 DeltaTime = 0;
    while(device->run())
    {
        device->getVideoDriver()->beginScene(true, true, SColor(255,100,101,140));

        DeltaTime = device->getTimer()->getTime() - TimeStamp;
		TimeStamp = device->getTimer()->getTime();

		// Step the simulation with our delta time
        world->stepSimulation(DeltaTime*0.001f, 120);

        handleCollisions();


        world->debugDrawWorld(debugDraw);
        // This call will draw the technical properties of the physics simulation
        // to the GUI environment.
        world->debugDrawProperties(drawProperties);


        device->getSceneManager()->drawAll();
        device->getGUIEnvironment()->drawAll();

        device->getVideoDriver()->endScene();
    }

    // We're done with the IrrBullet world, so we free the memory that it takes up.
    if(world)
        delete world;

    if(device)
        device->drop();
}

void CCollisionExample::handleCollisions()
{
    for(int i=0; i < world->getNumManifolds(); i++)
    {
        ICollisionCallbackInformation *info = world->getCollisionCallback(i);

        bool hasCollideAttribute = (info->getBody0()->getAttributes()->existsAttribute("collide"));
        ICollisionObject* obj = (hasCollideAttribute) ? info->getBody0() : info->getBody1();

        if(obj->getAttributes()->existsAttribute("collide") && obj->getAttributes()->getAttributeAsBool("collide") == true)
        {
            obj->getAttributes()->setAttribute("collide", false);
            world->addToDeletionQueue(obj);
        }

        obj = (hasCollideAttribute) ? info->getBody1() : info->getBody0();
        if(obj->getAttributes()->existsAttribute("vehicle"))
        {
            if(obj->getAttributes()->existsAttribute("destroyable"))
            {
                if(obj->getAttributes()->getAttributeAsBool("isDestroyed") == false)
                {
                    obj->getAttributes()->setAttribute("isDestroyed", true);

                    IParticleSystemSceneNode* ps = createParticleSystem(vector3df(0.0f,0.02f,0.0f), 80, 100,
                        SColor(0,255,255,255), SColor(0,255,255,255), 800, 2000, 0,
                        dimension2df(5.f,5.f), dimension2df(10.f,10.f), "fire.bmp",
                        0.0, false, EMT_TRANSPARENT_VERTEX_ALPHA, true);
                    ps->setPosition(static_cast<IRigidBody*>(obj)->getWorldTransform().getTranslation());

                    // Since we set a reference to the raycast vehicle of each tank, it will be removed automatically
                    // by irrBullet when the object is removed!
                    world->removeCollisionObject(obj);
                }
            }
        }

        delete info;
    }

    // If you wanted to get all contact points, you could do something like this:
    /*
    for(int i=0; i < DynamicsWorld->getNumManifolds(); i++)
    {
        ICollisionCallbackInformation *info = DynamicsWorld->getCollisionCallback(i);

        int numContacts = info->getPointer()->getNumContacts();
        for(int j=0; j < numContacts; j++)
        {
            if(verifyCollisionCallback(info))
            {
                if(info->getContactPoint(j).getDistance()<1.5f && info->getContactPoint(j).getLifeTime() < 2.0f)
                {
                    // Handle contact point
                }
            }
        }
        info->getPointer()->clearManifold();
    }
    */
}


CCollisionExample::~CCollisionExample()
{
    //dtor
}
