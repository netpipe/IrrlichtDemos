#include <irrlicht.h>
#include <irrBullet.h>
#include "loadsceneexample.h"


using namespace irr;
using namespace core;
using namespace video;
using namespace scene;
using namespace gui;
using namespace io;
using namespace std;


/*
    This example will show you how to load .irr scene files and use them with irrBullet.
    It is very simple and is only to be used as an example of how you can do it yourself.
*/


CLoadSceneExample::CLoadSceneExample()
{
}


bool CLoadSceneExample::OnEvent(const SEvent& event)
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

            else
            if(event.KeyInput.Key == KEY_KEY_R && event.KeyInput.PressedDown == false)
            {
                /*while(world->getNumCollisionObjects() > 0)
                {
                    world->removeCollisionObject(world->getCollisionObjectByIndex(0));
                }
                createGround();
                createBoxes();*/
                return true;
            }
        }
        break;
        default:
            break;
    }
    return false;


}


void CLoadSceneExample::runExample()
{
    debugDraw = true;
    drawProperties = true;
    drawWireFrame = false;


    device =
        createDevice( video::EDT_OPENGL, dimension2d<u32>(640, 480), 16,
            false, false, false, this);


    device->setWindowCaption(L"irrBullet Load Scene");

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


	device->getSceneManager()->loadScene("Scenes/SimpleScene.irr");

	core::array<scene::ISceneNode *> nodes;
	device->getSceneManager()->getSceneNodesFromType(scene::ESNT_ANY, nodes); // Find all nodes

	for (u32 i=0; i < nodes.size(); ++i)
	{
		scene::ISceneNode* node = nodes[i];
		stringc name = node->getName();
		const stringc prefix = name.subString(0,name.findFirst('_'));
		const stringc suffix = name.subString(name.findFirst('_')+1, name.size()-name.findFirst('_'));

		//printf("PREFIX: %s\n", prefix.c_str());
		//printf("SUFFIX: %s\n", suffix.c_str());

		if(node->getType() == scene::ESNT_MESH)
		{
		    if(prefix == "rigid")
            {
                ICollisionShape* shape = 0;

                if(suffix == "mesh")
                    shape = new IGImpactMeshShape(node, static_cast<IMeshSceneNode*>(node)->getMesh(), node->getBoundingBox().getVolume()*0.001);

                else if(suffix == "box")
                    shape = new IBoxShape(node, node->getBoundingBox().getVolume()*0.001);

                else if(suffix == "sphere")
                    shape = new ISphereShape(node, node->getBoundingBox().getVolume()*0.001);

                world->addRigidBody(shape);
            }

            else if(prefix == "static")
            {
                IBvhTriangleMeshShape* shape = new IBvhTriangleMeshShape(node, static_cast<IMeshSceneNode*>(node)->getMesh(), 0.0f);
                IRigidBody* body = world->addRigidBody(shape);
            }

            else if(prefix == "soft")
            {
                ISoftBody* softbody = world->addSoftBody(static_cast<IMeshSceneNode*>(node));
                softbody->setTotalMass(0.1f, false);
                softbody->setActivationState(EAS_DISABLE_DEACTIVATION);
                node->setMaterialFlag(EMF_BACK_FACE_CULLING, false);
                node->setAutomaticCulling(EAC_OFF);



                softbody->getConfiguration().liftCoefficient = 0.0;
                softbody->getConfiguration().dragCoefficient = 0.0;
                softbody->getConfiguration().dampingCoefficient = 0.0;
                softbody->getConfiguration().poseMatchingCoefficient = 0.0f;
                softbody->getConfiguration().positionsSolverIterations = 56;
                softbody->updateConfiguration();
            }
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


        //world->debugDrawWorld(debugDraw);
        // This call will draw the technical properties of the physics simulation
        // to the GUI environment.
        world->debugDrawProperties(true);


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


CLoadSceneExample::~CLoadSceneExample()
{
}
