// This example is part of irrBullet by Josiah Hartzell (fighterstw@hotmail.com or josiah.hartzell@gmail.com)

#include "liquidbodyexample.h"
#include <irrlicht.h>
#include <irrBullet.h>

using namespace irr;
using namespace core;
using namespace video;
using namespace scene;
using namespace io;
using namespace std;


CLiquidbodyExample::CLiquidbodyExample()
{
}



bool CLiquidbodyExample::OnEvent(const SEvent& event)
{
	if (!device)
		return false;

    switch(event.EventType)
    {
        case EET_MOUSE_INPUT_EVENT:
        {
            if(event.MouseInput.Event==EMIE_LMOUSE_PRESSED_DOWN)
            {
                IRigidBody* body = shootCube(vector3df(10,10,10), 2, "crate.jpg");

                irr::f32 t = 0.5f;
                irr::f32 buoyancy = 0.8f;

                irr::core::array<SBuoyancyPoint> points;
                //points.push_back(SBuoyancyPoint(irr::core::vector3df(0,0,0), 180.0f));
                points.push_back(SBuoyancyPoint(irr::core::vector3df(t,t,t), buoyancy));
                points.push_back(SBuoyancyPoint(irr::core::vector3df(-t,t,t), buoyancy));
                points.push_back(SBuoyancyPoint(irr::core::vector3df(-t,t,-t), buoyancy));
                points.push_back(SBuoyancyPoint(irr::core::vector3df(t,t,-t), buoyancy));

                points.push_back(SBuoyancyPoint(irr::core::vector3df(-t,-t,t), buoyancy));
                points.push_back(SBuoyancyPoint(irr::core::vector3df(t,-t,t), buoyancy));
                points.push_back(SBuoyancyPoint(irr::core::vector3df(-t,-t,-t), buoyancy));
                points.push_back(SBuoyancyPoint(irr::core::vector3df(t,-t,-t), buoyancy));


                body->setBuoyancyPoints(points);
                return true;
            }

            else
            if(event.MouseInput.Event==EMIE_RMOUSE_PRESSED_DOWN)
            {
                IRigidBody* body = shootSphere(vector3df(0.2,0.2,0.2), 0.2);

                body->getBuoyancyPointByIndex(0).buoyancy = 0.0f;
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

void CLiquidbodyExample::runExample()
{
    debugDraw = true;
    drawProperties = true;
    drawWireFrame = false;

    int rows=2, columns=2;

    device =
        createDevice( video::EDT_OPENGL, dimension2d<u32>(640, 480), 16,
            false, false, false, this);

    printf("Please enter the number of rows and columns of floating objects to create: \n");

    cin >> rows;
    cin >> columns;


    device->setWindowCaption(L"irrBullet Liquidbody Example - Josiah Hartzell");

    device->getFileSystem()->addFolderFileArchive("../../media/");


    ILightSceneNode* light = device->getSceneManager()->addLightSceneNode(0, vector3df(20, 40, -50), SColorf(1.0f, 1.0f, 1.0f), 4000.0f);
    light->setLightType(ELT_DIRECTIONAL);
    light->setRotation(vector3df(0,200,30));

    device->getSceneManager()->setAmbientLight(SColor(100,100,100,100));

    camera = device->getSceneManager()->addCameraSceneNodeFPS();
	camera->setPosition(vector3df(50,15,4));
	camera->setTarget(vector3df(0,0,0));


    ////////////////////////////
    // Create irrBullet World //
    ////////////////////////////
    world = createIrrBulletWorld(device, true, debugDraw);

    world->setDebugMode(EPDM_DrawAabb |
            EPDM_DrawContactPoints);

    world->setGravity(vector3df(0,-10,0));

    ILiquidBody* water = world->addLiquidBody(vector3df(-5000,0,5000),irr::core::aabbox3df(0, -10000, 0, 10000, 0, 10000), 500.0f, 200.0f);
    water->setCurrentDirection(vector3df(0,0,0));
    water->setGlobalWaveChangeIncrement(0.01f);
    water->setGlobalWaveUpdateFrequency(1.0f);
    water->setMaxGlobalWaveHeight(4.0f);
    water->setMinGlobalWaveHeight(-1.0f);
    water->setLocalWaveValues(10,1,0.5f);
    water->setInfinite(true);
    water->setInfiniteDepth(true);
    water->setLiquidDensity(0.1f);
    //water->setDebugDrawEnabled(false);


    IAnimatedMesh* mesh = device->getSceneManager()->addHillPlaneMesh( "myHill",
		core::dimension2d<f32>(20,20),
		core::dimension2d<u32>(40,40), 0, 0,
		core::dimension2d<f32>(0,0),
		core::dimension2d<f32>(1000,1000));

	ISceneNode* node = device->getSceneManager()->addWaterSurfaceSceneNode(mesh->getMesh(0), 0.0f, 300.0f, 30.0f);
	node->setPosition(core::vector3df(0,5,0));

	node->setMaterialTexture(0, device->getVideoDriver()->getTexture("water.jpg"));
	node->setScale(vector3df(1000,1,1000));

	node->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);
	node->setMaterialFlag(EMF_BACK_FACE_CULLING, false);


	for(u32 i=0; i < rows; i++)
	{
	    for(u32 j=0; j < columns; j++)
        {
            IRigidBody* body = addCube(vector3df(i*15,0,j*15), vector3df(10,10,10), 1, "crate.jpg");

            irr::f32 t = 0.5f;
            irr::f32 buoyancy = 0.2f;

            irr::core::array<SBuoyancyPoint> points;
            //points.push_back(SBuoyancyPoint(irr::core::vector3df(0,0,0), 180.0f));
            points.push_back(SBuoyancyPoint(irr::core::vector3df(t,t,t), buoyancy));
            points.push_back(SBuoyancyPoint(irr::core::vector3df(-t,t,t), buoyancy));
            points.push_back(SBuoyancyPoint(irr::core::vector3df(-t,t,-t), buoyancy));
            points.push_back(SBuoyancyPoint(irr::core::vector3df(t,t,-t), buoyancy));

            points.push_back(SBuoyancyPoint(irr::core::vector3df(-t,-t,t), buoyancy));
            points.push_back(SBuoyancyPoint(irr::core::vector3df(t,-t,t), buoyancy));
            points.push_back(SBuoyancyPoint(irr::core::vector3df(-t,-t,-t), buoyancy));
            points.push_back(SBuoyancyPoint(irr::core::vector3df(t,-t,-t), buoyancy));


            /*ICollisionObjectAffectorBuoyancy* affector = new ICollisionObjectAffectorBuoyancy(points,
                irr::core::aabbox3df(0, -100, 0, 10000, 0, 10000), 1);
            affector->setDebugDrawing(true);
            body->addAffector(affector);*/
            body->setBuoyancyPoints(points);

            //body->setActivationState(EAS_DISABLE_DEACTIVATION);
        }
	}

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
        //static_cast<ISoftBody*>(world->getCollisionObjectByName("SOFTBODY1"))->addForce(vector3df(-2,0,0));


        //world->debugDrawWorld(debugDraw);
        // This call will draw the technical properties of the physics simulation
        // to the GUI environment.
        world->debugDrawProperties(drawProperties);


        device->getSceneManager()->drawAll();
        device->getGUIEnvironment()->drawAll();

        device->getVideoDriver()->endScene();
    }
    //delete Liquid;

    // We're done with the IrrBullet world, so we free the memory that it takes up.
    if(world)
        delete world;

    if(device)
        device->drop();
}


CLiquidbodyExample::~CLiquidbodyExample()
{
    //dtor
}
