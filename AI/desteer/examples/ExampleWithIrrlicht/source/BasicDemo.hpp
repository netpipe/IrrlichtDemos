
#include "BaseIncludes.hpp"

class ObstacleAvoidenceDemo
{
    public:
    static void run(){
	IrrlichtDevice *device = createDevice( video::EDT_OPENGL, dimension2d<u32>(1024, 768), 32, false, false, true, 0);
	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();

	smgr->addCameraSceneNodeFPS();

    const s32 randLength = 1024;
    srand(device->getTimer()->getRealTime());


    //Creating the grid for unit measure, etc
    u32 tileNumber = 128;
	IAnimatedMesh* groundMesh = smgr->addHillPlaneMesh("", dimension2d<float>(8,8),dimension2d<u32>(tileNumber,tileNumber),0,0.0f,dimension2df(0,0),dimension2df(tileNumber,tileNumber));
    IAnimatedMeshSceneNode * groundNode = smgr->addAnimatedMeshSceneNode(groundMesh);
    groundNode->setMaterialTexture(0,driver->getTexture(resPath("../../media/grid2.png")));
    groundNode->setMaterialFlag(EMF_LIGHTING,false);
    groundNode->setPosition(vector3df(0,-2,0));


    //obstacles for stuff
    EntityGroup obstacles;

    for(int i = 0; i < 20; i++)
    {
        ISceneNode* s = smgr->addSphereSceneNode(20);
        IrrlichtBaseEntity * e = new IrrlichtBaseEntity(s);
        s->setPosition(vector3df(rand()%randLength - (randLength/2),0,rand()%randLength - (randLength/2)));
        obstacles.push_back(e);
    }

    //Nodes for vehicles
    ISceneNode * cube = smgr->addCubeSceneNode(4);
    ISceneNode * cube2 = smgr->addCubeSceneNode(4);
    cube->setMaterialFlag(EMF_LIGHTING,false);
    cube->setMaterialTexture(0,driver->getTexture(resPath("../../media/v1-solid.png")));
    cube2->setMaterialFlag(EMF_LIGHTING,false);
    cube2->setMaterialTexture(0,driver->getTexture(resPath("../../media/v2-solid.png")));

    //Creating the actual vehicles
    IrrlichtMobileEntity * Entity1 = new IrrlichtMobileEntity(cube ,vector3df(0,0,0), 1, 60, 150);
    IrrlichtMobileEntity * Entity2 = new IrrlichtMobileEntity(cube2,vector3df(0,0,300), 1, 60, 150);

    //Creating the steering conrollers, constructor also sets steering on entity
    SimpleSteeringController* Entity1Steering = new SimpleSteeringController(Entity1);
    SimpleSteeringController * Entity2Steering = new SimpleSteeringController(Entity2);

    //Setting up other params for behaviors
    Entity1Steering->SetObstacles(obstacles);
    Entity1Steering->SetHideTarget(Entity2);
    Entity1Steering->SetBehaviorFlag(EBF_HIDE,true);
    Entity1Steering->SetBehaviorFlag(EBF_AVOID,true);

    Entity2Steering->SetObstacles(obstacles);
    Entity2Steering->SetPursuitTarget(Entity1);
    Entity2Steering->SetBehaviorFlag(EBF_PURSUIT,true);
    Entity2Steering->SetBehaviorFlag(EBF_AVOID,true);

    //vars for tracking time between frames. This allows framerate independent motion state updates.
    u32 then = device->getTimer()->getTime();
    float timeUpdate = 0;

    while(device->run())
	{
	    const u32 now = device->getTimer()->getTime();
        const float frameDeltaTime = (float)(now - then) / 1000.f; // Time in seconds
        then = now;
        timeUpdate += frameDeltaTime;

        if(timeUpdate > 1)
        {
            core::stringw str = L"desteer v0.0.1  FPS: ";
            str += (s32)driver->getFPS();
			device->setWindowCaption(str.c_str());
			timeUpdate = 0;
		}

        //Do behavior updates before the rendering.
        Entity1->Update(frameDeltaTime);
        Entity2->Update(frameDeltaTime);

        driver->beginScene(true, true, SColor(255,100,101,140));
            smgr->drawAll();
		driver->endScene();
	}
    //Clean up irrlicht.
	device->drop();
    }
};
