// This example is part of irrBullet by Josiah Hartzell (fighterstw@hotmail.com or josiah.hartzell@gmail.com)

#include <irrlicht.h>
#include <irrBullet.h>
#include "affectorsexample.h"


using namespace irr;
using namespace core;
using namespace video;
using namespace scene;
using namespace gui;
using namespace io;
using namespace std;


/*
This example is low-skill level, and will show you how to create simple cube shapes and
show you the basics of irrBullet rigid body physics.
*/
CAffectorsExample::CAffectorsExample()
{
}


bool CAffectorsExample::OnEvent(const SEvent& event)
{
	if (!device)
		return false;

    switch(event.EventType)
    {
        case EET_MOUSE_INPUT_EVENT:
        {
            if(event.MouseInput.Event==EMIE_LMOUSE_PRESSED_DOWN)
            {
                IRigidBody* body = shootCube(vector3df(2,2,2), 1);

                makeAffector(body);
                return true;
            }

            else
            if(event.MouseInput.Event==EMIE_RMOUSE_PRESSED_DOWN)
            {
                IRigidBody* body = shootSphere(vector3df(0.2,0.2,0.2), 0.2);

                makeAffector(body);
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

            else
            if(event.KeyInput.Key == KEY_KEY_R && event.KeyInput.PressedDown == false)
            {
                while(world->getNumCollisionObjects() > 0)
                {
                    world->removeCollisionObject(world->getCollisionObjectByIndex(0));
                }
                createGround();
                return true;
            }
        }
        break;
        default:
            break;
    }
    return false;


}


void CAffectorsExample::runExample()
{
    debugDraw = true;
    drawProperties = true;
    drawWireFrame = false;

    AffectorType = 1;

    device =
        createDevice( video::EDT_OPENGL, dimension2d<u32>(640, 480), 16,
            false, false, false, this);

    printf("Affector type (1=deletion,2=attraction,3=repulsion):\n");
    cin >> AffectorType;


    device->setWindowCaption(L"irrBullet Affectors Example - Josiah Hartzell");

    device->getFileSystem()->addFolderFileArchive("../../media/");


    device->getSceneManager()->addLightSceneNode(0, vector3df(20, 40, -50), SColorf(1.0f, 1.0f, 1.0f), 4000.0f);



    ////////////////////////////
    // Create irrBullet World //
    ////////////////////////////
    world = createIrrBulletWorld(device, true, debugDraw);

    world->setDebugMode(EPDM_DrawAabb |
            EPDM_DrawContactPoints);

    world->setGravity(vector3df(0,-10,0));


    camera = device->getSceneManager()->addCameraSceneNodeFPS();
	camera->setPosition(vector3df(50,15,200));


	createGround();
	createBoxes();



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

void CAffectorsExample::createBoxes()
{
    for(u32 j=0; j < 10; j++)
    {
        for(u32 i=0; i < 10; i++)
        {
            IRigidBody* body = addCube(vector3df(3 * i, 0 + 3 * j + 3, 0), vector3df(3,3,3), 3);
            makeAffector(body);
        }
    }
}

void CAffectorsExample::createGround()
{
    ISceneNode *Node = device->getSceneManager()->addCubeSceneNode(1.0);
	Node->setScale(vector3df(300,3,300)); // 400, 3, 400
	Node->setPosition(vector3df(20,0,10));
	Node->setMaterialFlag(video::EMF_LIGHTING, true);
	Node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
	Node->setMaterialTexture(0, device->getVideoDriver()->getTexture("rockwall.jpg"));

    if(drawWireFrame)
        Node->setMaterialFlag(EMF_WIREFRAME, true);

	ICollisionShape *shape = new IBoxShape(Node, 0, false);

	//shape->setMargin(0.01);

	IRigidBody *body;
	body = world->addRigidBody(shape);
}

void CAffectorsExample::makeAffector(IRigidBody* const body)
{
    ICollisionObjectAffector* affector = 0;
    switch(AffectorType)
    {
        case 1:
            affector = new ICollisionObjectAffectorDelete(2000);
        break;

        case 2:
            affector = new ICollisionObjectAffectorAttract(device->getSceneManager()->getActiveCamera(), 200.0f);
        break;

        case 3:
            affector = new ICollisionObjectAffectorAttract(device->getSceneManager()->getActiveCamera(), -50.0f);
        break;

        default:
            break;
    }
    body->addAffector(affector);
}


CAffectorsExample::~CAffectorsExample()
{
}
