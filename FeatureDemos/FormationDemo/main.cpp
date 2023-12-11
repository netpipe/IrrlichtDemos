//Rogerborg

//minion formations
#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace video;
using namespace scene;

#pragma comment(lib, "Irrlicht.lib")

struct WrapperClass
{
 struct Unit;

 struct Formation
 {
    f32 Rotation;
    vector3df DesiredPosition;
    ISceneNode * Visual;
    f32 Speed;
    array<Unit *> Units;
    u32 MaxFiles; // Add files to each rank until full
    f32 FileWidth;
    f32 RankDepth;
    ITerrainSceneNode * Terrain;

    Formation(const vector3df & position,
              f32 speed,
              u32 maxFiles,
              int initialUnits,
              f32 fileWidth,
              f32 rankDepth,
              ITerrainSceneNode * terrain,
              ISceneManager * smgr)
    {
        Rotation = 0.f;
        Speed = speed;
        MaxFiles = maxFiles;
        FileWidth = fileWidth;
        RankDepth = rankDepth;
        Terrain = terrain;
        DesiredPosition = position;

        Visual = smgr->addAnimatedMeshSceneNode(smgr->addArrowMesh("formation arrow", SColor(255*.4, 0, 255, 0), SColor(255, 0, 255, 0), 4, 8,
                                                100.f, 60.f, 5.f, 30.f));
        Visual->setMaterialFlag(video::EMF_LIGHTING, false);
        Visual->setPosition(DesiredPosition);
        Visual->setRotation(vector3df(90, 0, 0));
        Visual->updateAbsolutePosition();

        for(int unit = 0; unit < initialUnits; ++unit)
        {
            position2di rankAndFile(unit / maxFiles, unit % maxFiles);

            Units.push_back(new Unit(this, rankAndFile, smgr));
        }
    }

    ~Formation()
    {
        for(u32 unit = 0; unit < Units.size(); ++unit)
            delete Units[unit];
    }

    void process(f32 deltaTime)
    {
        vector3df currentPosition = calculateMovePosition(Visual->getAbsolutePosition(),
                                                    DesiredPosition,
                                                    Speed,
                                                    deltaTime,
                                                    Rotation);
        currentPosition.Y += Visual->getBoundingBox().getExtent().Y;

        Visual->setPosition(currentPosition);
        Visual->setRotation(vector3df(90, Rotation, 0));
        Visual->updateAbsolutePosition();

        for(u32 unit = 0; unit < Units.size(); ++unit)
            if(Units[unit])
                Units[unit]->process(deltaTime);
    }

    vector3df getDesiredUnitPosition(const position2di & rankAndFile)
    {
        vector3df position(FileWidth * (rankAndFile.X - (.5f * MaxFiles)),
                           0.f,
                           -RankDepth * rankAndFile.Y);

        position.rotateXZBy(-Rotation); // This appears to be bass ackwards compared to getHorizontalAngle().Y

        position += Visual->getAbsolutePosition();

        return position;
    }

    vector3df calculateMovePosition(vector3df currentPosition,
                              const vector3df & desiredPosition,
                              f32 speed,
                              f32 deltaTime,
                              f32 & rotation)
    {
        const f32 availableMovement = speed * deltaTime;
        vector3df desiredMovement = desiredPosition - currentPosition;
        desiredMovement.Y = 0.f;
        const f32 desiredMovementMagnitudeSq = desiredMovement.getLengthSQ();

        if(desiredMovementMagnitudeSq > 0.f)
        {
            if(desiredMovementMagnitudeSq <= availableMovement * availableMovement)
            {
                currentPosition = desiredPosition;
            }
            else
            {
                currentPosition += desiredMovement.normalize() * availableMovement;
            }

            rotation = desiredMovement.getHorizontalAngle().Y;
        }

        currentPosition.Y = Terrain->getHeight(currentPosition.X, currentPosition.Z);

        return currentPosition;
    }
 };

 struct Unit
 {
    Formation * ParentFormation;
    ISceneNode * Visual;
    position2di RankAndFile;
    f32 Speed;
    static const f32 SpeedMultiplier;

    Unit(Formation * formation, const position2di & rankAndFile, ISceneManager * smgr)
    {
        ParentFormation = formation;
        RankAndFile = rankAndFile;
        Visual = smgr->addAnimatedMeshSceneNode(smgr->addArrowMesh("pawn", SColor(255, 255, 0, 0), SColor(255, 0, 255, 255), 4, 8,
                                                100.f, 60.f, 5.f, 30.f));
        Visual->setMaterialFlag(video::EMF_LIGHTING, false);
        Visual->setPosition(ParentFormation->getDesiredUnitPosition(RankAndFile));
        Visual->updateAbsolutePosition();

        Speed = ParentFormation->Speed * SpeedMultiplier;
        // Randomise this a little
        Speed *= .9f + 2.f * ((f32)rand() / (f32)RAND_MAX);
    }



    virtual void process(f32 deltaTime)
    {
        f32 fakeRotation;
        vector3df currentPosition = ParentFormation->calculateMovePosition(Visual->getAbsolutePosition(),
                                                                           ParentFormation->getDesiredUnitPosition(RankAndFile),
                                                                           Speed,
                                                                           deltaTime,
                                                                           fakeRotation);

        currentPosition.Y += Visual->getBoundingBox().getExtent().Y;

        Visual->setPosition(currentPosition);
    }


 }; // Unit

}; // WrapperClass

const f32 WrapperClass::Unit::SpeedMultiplier = 1.1f;


class MyEventReceiver : public IEventReceiver
{
public:

    MyEventReceiver()
    {
        ButtonClicked = false;
    }

    bool OnEvent(const SEvent& event)
    {
        if(EET_MOUSE_INPUT_EVENT == event.EventType)
            if(EMIE_LMOUSE_PRESSED_DOWN == event.MouseInput.Event && !ButtonClicked)
                ButtonClicked = true;
            else if(EMIE_LMOUSE_LEFT_UP == event.MouseInput.Event)
                ButtonClicked = false;

        return false;
    }

    bool ButtonClicked;
};

int main()
{
    IrrlichtDevice* device = createDevice(EDT_OPENGL, dimension2d<s32>(640, 480));

    if (device == 0)
        return 1; // could not create selected driver.

    IVideoDriver* driver = device->getVideoDriver();
    ISceneManager* smgr = device->getSceneManager();

    driver->setTextureCreationFlag(ETCF_ALWAYS_32_BIT, true);

    ICameraSceneNode* camera = smgr->addCameraSceneNodeFPS(0);
    camera->setPosition(vector3df(1900*2,255*2,3700*2));
    camera->setTarget(vector3df(2397*2,343*2,2700*2));
    camera->setFarValue(12000.0f);

    ITerrainSceneNode* terrain = smgr->addTerrainSceneNode(
        "../../media/terrain-heightmap.bmp",
        0,                              // parent node
        -1,                              // node id
        vector3df(0.f, 0.f, 0.f),         // position
        vector3df(0.f, 0.f, 0.f),         // rotation
        vector3df(40.f, 4.4f, 40.f),      // scale
        SColor ( 255, 255, 255, 255 ),   // vertexColor,
        5,                              // maxLOD
        ETPS_17,                     // patchSize
        4                              // smoothFactor
        );

    terrain->setMaterialFlag(EMF_LIGHTING, false);

    terrain->setMaterialTexture(0, driver->getTexture("../../media/terrain-texture.jpg"));
    terrain->setMaterialTexture(1, driver->getTexture("../../media/detailmap3.jpg"));

    terrain->setMaterialType(EMT_DETAIL_MAP);

    terrain->scaleTexture(1.0f, 20.0f);

    driver->setTextureCreationFlag(ETCF_CREATE_MIP_MAPS, false);

    smgr->addSkyBoxSceneNode(
        driver->getTexture("../../media/irrlicht2_up.jpg"),
        driver->getTexture("../../media/irrlicht2_dn.jpg"),
        driver->getTexture("../../media/irrlicht2_lf.jpg"),
        driver->getTexture("../../media/irrlicht2_rt.jpg"),
        driver->getTexture("../../media/irrlicht2_ft.jpg"),
        driver->getTexture("../../media/irrlicht2_bk.jpg"));

    driver->setTextureCreationFlag(ETCF_CREATE_MIP_MAPS, true);

    ITriangleSelector * terrainSelector = smgr->createTerrainTriangleSelector(terrain, 0);

    camera->updateAbsolutePosition();


    WrapperClass::Formation * formation = new WrapperClass::Formation(camera->getAbsolutePosition(), 100.f, 5, 30, 100.f, 100.f, terrain, smgr);



    MyEventReceiver eventReceiver;
    device->setEventReceiver(&eventReceiver);

    u32 then = device->getTimer()->getTime();

    while(device->run())
    if (device->isWindowActive())
    {
        if(eventReceiver.ButtonClicked)
        {
            const position2di clickPosition = device->getCursorControl()->getPosition();
            const line3df ray = smgr->getSceneCollisionManager()->getRayFromScreenCoordinates(clickPosition, smgr->getActiveCamera());

            vector3df desiredPosition;
            triangle3df outTriangle;

            if(smgr->getSceneCollisionManager()->getCollisionPoint(ray, terrainSelector, desiredPosition, outTriangle))
            {
                formation->DesiredPosition = desiredPosition;
            }
        }


        const u32 now = device->getTimer()->getTime();
        const f32 deltaTime = min_((f32)(now - then) / 1000.f, 0.5f);
        then = now;

        formation->process(deltaTime);

        driver->beginScene(true, true, 0 );
        smgr->drawAll();
        driver->endScene();
        device->sleep(1,0);
    }

    delete formation;
    (void)terrainSelector->drop();
    (void)device->drop();

    return 0;
}
