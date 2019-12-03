#include "BaseIncludes.hpp"

class ObstacleAvoidenceDemo
{
    public:
    static void run(){
	IrrlichtDevice *device = createDevice( video::EDT_OPENGL, dimension2d<u32>(1024, 768), 32, false, false, true, 0);
	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();

	smgr->addCameraSceneNodeFPS();

    //Rand length is used to place stuff onto the actual ground mesh.
    const s32 randLength = 1024;
    srand(device->getTimer()->getRealTime());

    //Creating the grid for unit measure, etc
    u32 tileNumber = 128;
	IAnimatedMesh* groundMesh = smgr->addHillPlaneMesh("", dimension2d<float>(8,8),dimension2d<u32>(tileNumber,tileNumber),0,0.0f,dimension2df(0,0),dimension2df(tileNumber,tileNumber));
    IAnimatedMeshSceneNode * groundNode = smgr->addAnimatedMeshSceneNode(groundMesh);
    groundNode->setMaterialTexture(0,driver->getTexture(resPath("../../media/grid2.png")));
    groundNode->setMaterialFlag(EMF_LIGHTING,false);
    groundNode->setPosition(vector3df(0,-2,0));

    //obstacles for entity to avoid.
    EntityGroup obstacles;

    for(int i = 0; i < 60; i++)
    {
        ISceneNode* s = smgr->addSphereSceneNode(20);
        IrrlichtBaseEntity * e = new IrrlichtBaseEntity(s);
        s->setPosition(vector3df( (rand() % randLength) - (randLength/2), 0, (rand() % randLength) - (randLength/2)));
        obstacles.push_back(e);
    }

    //Nodes for vehicles
    ISceneNode * cube = smgr->addCubeSceneNode(4);
    cube->setMaterialFlag(EMF_LIGHTING,false);
    cube->setMaterialTexture(0,driver->getTexture(resPath("../../media/v1-solid.png")));
    //Creating the actual vehicles
    IrrlichtMobileEntity * Entity1 = new IrrlichtMobileEntity(cube ,vector3df(0,0,0), 1, 100, 25);

    //Creating the steering conrollers, constructor also sets steering on entity
    SimpleSteeringController* Entity1Steering = new SimpleSteeringController(Entity1);

    //Setting up other params for behaviors
    Entity1Steering->SetObstacles(obstacles);
    Entity1Steering->SetSeekTarget(vector3df(0,0,513));

    //Enabling behaviors
    Entity1Steering->SetBehaviorFlag(EBF_AVOID,true);
    Entity1Steering->SetBehaviorFlag(EBF_SEEK,true);

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

        //Wrap X and Z coords so its like astroids.
        vector3df entityPos = Entity1->Position();

        if(entityPos.X > 512)
        {
            Entity1->SetPosition(vector3df(-512,0,entityPos.Z));
        }
        if(entityPos.X < -512)
        {
            Entity1->SetPosition(vector3df(512,0,entityPos.Z));
        }
        if(entityPos.Z > 512)
        {
            Entity1->SetPosition(vector3df(entityPos.X,0,-512));
        }
        if(entityPos.Z < -512)
        {
            Entity1->SetPosition(vector3df(entityPos.X,0,512));
        }


        SColor velocityColor = SColor(0,255,0,0);
        SColor steeringForceColor = SColor(0,0,255,0);
        SColor headingColor = SColor(0,0,0,255);

        driver->beginScene(true, true, SColor(255,100,101,140));
            smgr->drawAll();
            driver->setTransform(video::ETS_WORLD, core::IdentityMatrix);
            entityPos = Entity1->Position();
            driver->draw3DLine(entityPos,entityPos + Entity1->Velocity(), velocityColor);
            driver->draw3DLine(entityPos,entityPos + Entity1->SteeringVector(), steeringForceColor);
            driver->draw3DLine(entityPos,entityPos + Entity1->ForwardVector(), headingColor);
		driver->endScene();
	}

    //Clean up irrlicht.
	device->drop();
    }
};
