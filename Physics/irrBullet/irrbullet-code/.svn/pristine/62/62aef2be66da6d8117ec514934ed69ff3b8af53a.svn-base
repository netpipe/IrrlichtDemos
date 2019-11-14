// This example is part of irrBullet by Josiah Hartzell (fighterstw@hotmail.com or josiah.hartzell@gmail.com)

#include "softbodyexample.h"
#include <irrlicht.h>
#include <irrBullet.h>

using namespace irr;
using namespace core;
using namespace video;
using namespace scene;
using namespace io;
using namespace std;

/*
    Note: The flag example in this demo is only as good as its wind generator, and
    in this demo it is a very simple wind generator in one direction; however, it
    still achieves decent results.
*/

CSoftbodyExample::CSoftbodyExample()
{
}



bool CSoftbodyExample::OnEvent(const SEvent& event)
{
	if (!device)
		return false;

    switch(event.EventType)
    {
        case EET_MOUSE_INPUT_EVENT:
        {
            if(event.MouseInput.Event==EMIE_LMOUSE_PRESSED_DOWN)
            {
                IRigidBody* body = shootCube(vector3df(10,10,10), 2);

                ICollisionObjectAffectorDelete *deleteAffector = new ICollisionObjectAffectorDelete(4000);
                body->addAffector(deleteAffector);
                return true;
            }

            else
            if(event.MouseInput.Event==EMIE_RMOUSE_PRESSED_DOWN)
            {
                IRigidBody* body = shootSphere(vector3df(0.2,0.2,0.2), 0.2);

                ICollisionObjectAffectorDelete *deleteAffector = new ICollisionObjectAffectorDelete(4000);
                body->addAffector(deleteAffector);
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

void CSoftbodyExample::runExample()
{
    debugDraw = true;
    drawProperties = true;
    drawWireFrame = false;

    int choice = 5;
    int rows = 7;
    int columns = 1;
    softVersusSoft = 1;

    printf("Select the softbody type: \n");
    printf("1 = Sphere\n2 = Newspaper(Aero)\n3 = Cloth\n4 = Feather(Aero)\n5 = Flag(Aero)\n");
    cin >> choice;
    printf("Select the number of rows: \n");
    cin >> rows;
    printf("Select the number of columns (only for newspaper and feather, else 1): \n");
    cin >> columns;
    printf("Enable soft-soft collision?\n");
    cin >> softVersusSoft;

    device =
        createDevice( video::EDT_OPENGL, dimension2d<u32>(640, 480), 16,
            false, false, false, this);


    device->setWindowCaption(L"irrBullet Softbody Example - Josiah Hartzell");

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

    world->getSoftBodyWorldInfo().air_density = 0.05;

    createGround();


    for(u32 i=0; i < rows; i++)
    {
        for(u32 j=0; j < columns; j++)
            createSoftbodyType(choice, vector3df(56*(j+i), 200+i, 56*(j+i)));
    }
    //loadRoomScene();

    /*for(u32 i=1; i < 10; i++)
        createSoftbodyType(3, vector3df(106*i, 200+i, 106*i));*/

    //createSoftbodyType(4, vector3df(0,100,0));

    // Set our delta time and time stamp
    u32 TimeStamp = device->getTimer()->getTime();
    u32 DeltaTime = 0;
    u32 lastWindUpdate = 0;
    f32 xWind = 0.0f;
    f32 zWind = 0.0f;
    while(device->run())
    {
        device->getVideoDriver()->beginScene(true, true, SColor(255,100,101,140));

        DeltaTime = device->getTimer()->getTime() - TimeStamp;
		TimeStamp = device->getTimer()->getTime();

		// Step the simulation with our delta time
        world->stepSimulation(DeltaTime*0.001f, 120);

        if(choice == 5)
        {
            if(device->getTimer()->getTime()-lastWindUpdate > 1000)
            {
                srand(device->getTimer()->getTime());
                xWind = f32(rand()%(8 - 1)*0.0000001);
                zWind = f32(rand()%(8 - 1)*0.0000001);
                lastWindUpdate = device->getTimer()->getTime();
            }

            if(!world->simulationPaused())
            {
                for(u32 i=0; i < world->getNumCollisionObjects(); ++i)
                {
                    ICollisionObject* object = world->getCollisionObjectByIndex(i);
                    if(object->getObjectType() == ECOT_SOFT_BODY)
                        static_cast<ISoftBody*>(object)->addForce(vector3df(xWind,0,zWind));
                }
            }
        }


        //world->debugDrawWorld(debugDraw);
        // This call will draw the technical properties of the physics simulation
        // to the GUI environment.
        world->debugDrawProperties(drawProperties);


        device->getSceneManager()->drawAll();
        device->getGUIEnvironment()->drawAll();

        device->getVideoDriver()->endScene();
    }
    //delete water;

    // We're done with the IrrBullet world, so we free the memory that it takes up.
    if(world)
        delete world;

    if(device)
        device->drop();
}

// 1 = sphere, 2 = newspaper(aero), 3 = cloth, 4 = feather(aero), 5 = flag(aero)
ISoftBody* CSoftbodyExample::createSoftbodyType(u32 type, const vector3df& position)
{
    IMeshSceneNode* softbodyNode;
    switch(type)
    {
        case 1:
        {
            device->getSceneManager()->getMeshCache()->removeMesh(device->getSceneManager()->getMeshCache()->getMeshByName("sphere.b3d"));
            IMesh* mesh = device->getSceneManager()->getMesh("sphere.b3d");

            softbodyNode = device->getSceneManager()->addMeshSceneNode(mesh);
            softbodyNode->setMaterialTexture(0, device->getVideoDriver()->getTexture("detailmap3.jpg"));
            softbodyNode->setAutomaticCulling(EAC_OFF);
            softbodyNode->setScale(vector3df(10,10,10));
            softbodyNode->setMaterialFlag(EMF_NORMALIZE_NORMALS, true);
        }
        break;

        case 2:
        {
            device->getSceneManager()->getMeshCache()->removeMesh(device->getSceneManager()->getMeshCache()->getMeshByName("newspaperMesh"));

            IMesh* mesh = device->getSceneManager()->addHillPlaneMesh("newspaperMesh",
                core::dimension2d<f32>(22,22),
                core::dimension2d<u32>(3,3), 0, 0,
                core::dimension2d<f32>(0,0),
                core::dimension2d<f32>(1,1));



            softbodyNode = device->getSceneManager()->addMeshSceneNode(mesh);
            softbodyNode->setMaterialFlag(EMF_BACK_FACE_CULLING, false);
            softbodyNode->setMaterialTexture(0, device->getVideoDriver()->getTexture("newspaper.png"));

            softbodyNode->setAutomaticCulling(EAC_OFF);
        }
        break;

        case 3:
        {
            device->getSceneManager()->getMeshCache()->removeMesh(device->getSceneManager()->getMeshCache()->getMeshByName("clothMesh"));

            IMesh* mesh = device->getSceneManager()->addHillPlaneMesh("clothMesh",
                core::dimension2d<f32>(10,10),
                core::dimension2d<u32>(10,10), 0, 0,
                core::dimension2d<f32>(0,0),
                core::dimension2d<f32>(1,1));


            softbodyNode = device->getSceneManager()->addMeshSceneNode(mesh);
            softbodyNode->setMaterialFlag(EMF_BACK_FACE_CULLING, false);
            softbodyNode->setMaterialTexture(0, device->getVideoDriver()->getTexture("irrbullet_1024x768.png"));

            softbodyNode->setRotation(vector3df(0,-90,-90));

            softbodyNode->setAutomaticCulling(EAC_OFF);
        }
        break;

        case 4:
        {
            device->getSceneManager()->getMeshCache()->removeMesh(device->getSceneManager()->getMeshCache()->getMeshByName("featherMesh"));

            //IMesh* mesh = device->getSceneManager()->getMesh("newspaper.obj");

            IMesh* mesh = device->getSceneManager()->addHillPlaneMesh("featherMesh",
                core::dimension2d<f32>(22,22),
                core::dimension2d<u32>(1,4), 0, 0,
                core::dimension2d<f32>(0,0),
                core::dimension2d<f32>(1,1));

            //mesh->getMeshBuffer(0)->getMaterial().TextureLayer[0].TextureWrapU = ETC_CLAMP_TO_BORDER;
            //mesh->getMeshBuffer(0)->getMaterial().TextureLayer[0].TextureWrapV = ETC_CLAMP_TO_BORDER;


            softbodyNode = device->getSceneManager()->addMeshSceneNode(mesh);
            softbodyNode->setMaterialFlag(EMF_BACK_FACE_CULLING, false);
            softbodyNode->setMaterialTexture(0, device->getVideoDriver()->getTexture("feather.png"));
            softbodyNode->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL);


            softbodyNode->setAutomaticCulling(EAC_OFF);
        }
        break;

        case 5:
        {
            device->getSceneManager()->getMeshCache()->removeMesh(device->getSceneManager()->getMeshCache()->getMeshByName("clothMesh"));

            IMesh* mesh = device->getSceneManager()->addHillPlaneMesh("clothMesh",
                core::dimension2d<f32>(1.5,1),
                core::dimension2d<u32>(10,10), 0, 0,
                core::dimension2d<f32>(0,0),
                core::dimension2d<f32>(1,1));


            softbodyNode = device->getSceneManager()->addMeshSceneNode(mesh);
            softbodyNode->setMaterialFlag(EMF_BACK_FACE_CULLING, false);
            softbodyNode->setMaterialTexture(0, device->getVideoDriver()->getTexture("flag_usa.png"));

            softbodyNode->setRotation(vector3df(0,-90,-90));

            softbodyNode->setAutomaticCulling(EAC_OFF);
        }
        break;
    }
    softbodyNode->setPosition(position);
    //softbodyNode->setRotation(vector3df(0,0,0));
    softbodyNode->setDebugDataVisible(EDS_BBOX);

    ISoftBody* softbody = world->addSoftBody(softbodyNode);

    softbody->setName("SOFTBODY1");
    softbody->setActivationState(EAS_DISABLE_DEACTIVATION);

    if(softVersusSoft)
    {
        softbody->generateClusters(60.0f);
        softbody->addCollisionFlag(ESBCM_CLUSTER_SOFT_VERSUS_SOFT);
    }

    switch(type)
    {
        case 1:
        {
            softbody->getConfiguration().poseMatchingCoefficient = 0.0;
            softbody->updateConfiguration();

            softbody->generateBendingConstraints(2);
            softbody->randomizeConstraints();
        }
        break;

        case 2:
        {
            //softbody->getConfiguration().aeroModel = btSoftBody::eAeroModel::V_TwoSided;

            softbody->getConfiguration().liftCoefficient = 1.004;
            softbody->getConfiguration().dragCoefficient = 0.0003;
            softbody->getConfiguration().dampingCoefficient = 0.0;
            softbody->getConfiguration().aeroModel = ESBAM_VERTEX_TWO_SIDED;
            softbody->updateConfiguration();

            softbody->setTotalMass(0.001);
            softbody->setScale(vector3df(0.1,0.1,0.1));

            softbody->generateBendingConstraints(2);
            softbody->randomizeConstraints();
        }
        break;

        case 3:
        {
            IRigidBody* body = addCube(bulletToIrrlichtVector(softbody->getPointer()->m_nodes[0].m_x), vector3df(1,1,1), 0.0f);
            softbody->appendAnchor(0, body, true);

            body = addCube(bulletToIrrlichtVector(softbody->getPointer()->m_nodes[11*10].m_x), vector3df(1,1,1), 0.0f);
            softbody->appendAnchor(11*10, body, true);
        }
        break;

        case 4:
        {
            //softbody->getConfiguration().aeroModel = btSoftBody::eAeroModel::V_TwoSided;

            softbody->getConfiguration().liftCoefficient = 1.004;
            softbody->getConfiguration().dragCoefficient = 0.0;
            softbody->getConfiguration().dampingCoefficient = 0.0;
            softbody->getConfiguration().aeroModel = ESBAM_VERTEX_TWO_SIDED;
            softbody->updateConfiguration();

            softbody->setTotalMass(0.00001);
            softbody->setScale(vector3df(0.1,0.1,0.1));

            softbody->generateBendingConstraints(2);
            softbody->randomizeConstraints();
        }
        break;

        case 5:
        {
            IRigidBody* body = addCube(bulletToIrrlichtVector(softbody->getPointer()->m_nodes[0].m_x), vector3df(0.1,0.1,0.1), 0.0f);
            softbody->appendAnchor(0, body, true);

            /*body = addCube(bulletToIrrlichtVector(softbody->getPointer()->m_nodes[6].m_x), vector3df(0.1,0.1,0.1), 0.0f);
            softbody->appendAnchor(6, body, true);

            body = addCube(bulletToIrrlichtVector(softbody->getPointer()->m_nodes[10].m_x), vector3df(0.1,0.1,0.1), 0.0f);
            softbody->appendAnchor(10, body, true);

            body = addCube(bulletToIrrlichtVector(softbody->getPointer()->m_nodes[14].m_x), vector3df(0.1,0.1,0.1), 0.0f);
            softbody->appendAnchor(14, body, true);*/

            body = addCube(bulletToIrrlichtVector(softbody->getPointer()->m_nodes[20].m_x), vector3df(0.1,0.1,0.1), 0.0f);
            softbody->appendAnchor(20, body, true);

            softbody->getConfiguration().dragCoefficient = 0.0;
            softbody->getConfiguration().dampingCoefficient = 0.0;
            softbody->getConfiguration().aeroModel = ESBAM_VERTEX_TWO_SIDED;
            softbody->updateConfiguration();

            softbody->setTotalMass(0.00001);
        }
        break;
    }

    return softbody;

}


CSoftbodyExample::~CSoftbodyExample()
{
    //dtor
}
