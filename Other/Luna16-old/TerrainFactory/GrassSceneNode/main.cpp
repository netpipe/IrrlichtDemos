
#include <irrlicht.h>
#include <iostream>

using namespace irr;

#include "CGrassPatchSceneNode.h"
#include "CWindGenerator.h"

core::array<video::E_MATERIAL_TYPE> modes;
video::ITexture *tex1;
video::ITexture *tex2;

scene::IWindGenerator *wind;


#define width  10
#define height 10
#define total  width*height

class MyEventReceiver : public IEventReceiver
{
public:

    MyEventReceiver(scene::ISceneNode* terrain, scene::CGrassPatchSceneNode** grass)
    {
        // store pointer to terrain so we can change its drawing mode
        Terrain = terrain;
        Grass   = &grass[0];
        mode    = 0;
        tex     = false;
    }

    bool OnEvent(const SEvent &event)
    {
        // check if user presses the key 'W' or 'D'
        if (event.EventType == irr::EET_KEY_INPUT_EVENT && !event.KeyInput.PressedDown)
        {
            switch (event.KeyInput.Key)
            {
            case irr::KEY_KEY_W: // switch wire frame mode
                Terrain->setMaterialFlag(video::EMF_WIREFRAME, !Terrain->getMaterial(0).Wireframe);
                return true;
            case irr::KEY_KEY_D: // toggle detail map
                Terrain->setMaterialType(
                    Terrain->getMaterial(0).MaterialType == video::EMT_SOLID ?
                    video::EMT_DETAIL_MAP : video::EMT_SOLID);
                return true;
            case irr::KEY_KEY_Z: // change draw distance
                for (int n=0; n<total; ++n)
                {
                    if (Grass[n]->getDrawDistance() > GRASS_PATCH_SIZE + 50)
                        Grass[n]->setDrawDistance(Grass[n]->getDrawDistance()- 50.0f);
                }
                return true;
            case irr::KEY_KEY_X: // change draw distance
                for (int n=0; n<total; ++n)
                {
                    Grass[n]->setDrawDistance(Grass[n]->getDrawDistance()+ 50.0f);
                }
                return true;
            case irr::KEY_KEY_C: // change max density
                for (int n=0; n<total; ++n)
                {
                    if (Grass[n]->getMaxDensity() > 55 )
                        Grass[n]->setMaxDensity(Grass[n]->getMaxDensity()- 50);
                }
                return true;
            case irr::KEY_KEY_V: // change max density
                for (int n=0; n<total; ++n)
                {
                    Grass[n]->setMaxDensity(Grass[n]->getMaxDensity()+ 50);
                }
                return true;
            case irr::KEY_KEY_B: // change mode
                if (mode > 0)
                   --mode;
                for (int n=0; n<total; ++n)
                {
                    Grass[n]->setMaterialType(modes[mode]);
                }
                return true;
            case irr::KEY_KEY_N: // change mode
                if (mode < modes.size())
                   ++mode;
                for (int n=0; n<total; ++n)
                {
                    Grass[n]->setMaterialType(modes[mode]);
                }
                return true;
            case irr::KEY_KEY_M: // change mode
                tex = !tex;
                for (int n=0; n<total; ++n)
                {
                    Grass[n]->setMaterialTexture(0,tex ? tex1:tex2);
                }
                return true;

            case irr::KEY_KEY_S: // switch terrain invisible
                Terrain->setVisible( !Terrain->isVisible());
                return true;

            case irr::KEY_KEY_K: // less wind
                 if ( wind->getStrength() > 5 )
                    wind->setStrength( wind->getStrength() - 5 );
                 return true;

            case irr::KEY_KEY_L: // more wind
                 if ( wind->getStrength() < 100 )
                    wind->setStrength( wind->getStrength() + 5 );
                 return true;

            case irr::KEY_KEY_O: // blustery wind
                 if ( wind->getRegularity() > 1 )
                    wind->setRegularity( wind->getRegularity() - 1 );
                 return true;

            case irr::KEY_KEY_P: // regular wind
                 if ( wind->getRegularity() < 20 )
                    wind->setRegularity( wind->getRegularity() + 1 );
                 return true;

            case irr::KEY_KEY_U: // decrease wind resolution
                if (Grass[0]->getWindRes() > 1)
                {
                    for (int n=0; n<total; ++n)
                    {
                        Grass[n]->setWindRes(Grass[n]->getWindRes() - 1);
                    }
                }
                return true;

            case irr::KEY_KEY_I: // decrease wind resolution
                if (Grass[0]->getWindRes() < 20)
                {
                    for (int n=0; n<total; ++n)
                    {
                        Grass[n]->setWindRes(Grass[n]->getWindRes() + 1);
                    }
                }
                return true;

            case irr::KEY_KEY_T: // decrease refresh delay
                if (Grass[0]->getRefreshDelay() > 4)
                {
                    for (int n=0; n<total; ++n)
                    {
                        Grass[n]->setRefreshDelay(Grass[n]->getRefreshDelay() - 10);
                    }
                }
                return true;

            case irr::KEY_KEY_Y: // increase refresh delay
                if (Grass[0]->getRefreshDelay() < 500)
                {
                    for (int n=0; n<total; ++n)
                    {
                        Grass[n]->setRefreshDelay(Grass[n]->getRefreshDelay() + 10);
                    }
                }
                return true;

            }
        }

        return false;
    }

private:
    scene::ISceneNode* Terrain;
    u32 mode;
    bool tex;
    scene::CGrassPatchSceneNode** Grass;
};


/*
The start of the main function starts like in most other example. We ask the user
for the desired renderer and start it up.
*/
int main()
{

	c8 i;
    std::cin >> i;


    modes.set_used(4);
    modes[0] = video::EMT_TRANSPARENT_ALPHA_CHANNEL;
    modes[1] = video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF;
    modes[2] = video::EMT_TRANSPARENT_ADD_COLOR;
    modes[3] = video::EMT_TRANSPARENT_VERTEX_ALPHA;

    // create device
    IrrlichtDevice* device = createDevice(video::EDT_OPENGL, core::dimension2di(1000, 760));

    if (device == 0)
        return 1; // could not create selected driver.

    video::IVideoDriver* driver = device->getVideoDriver();
    scene::ISceneManager* smgr = device->getSceneManager();
    gui::IGUIEnvironment* env = device->getGUIEnvironment();

    driver->setTextureCreationFlag(video::ETCF_ALWAYS_32_BIT, true);

    // add irrlicht logo
    env->addImage(driver->getTexture("../../media/irrlichtlogoalpha.tga"),
        core::position2d<s32>(10,10));

    // add some help text
    gui::IGUIStaticText* text = env->addStaticText(
        L"Press 'S' to toggle terrain\nPress 'W' to toggle wireframe mode\nPress 'D' to toggle detail map\nZ and X for distance\nC and V for max density\nB and N to change render mode\nM to swap texture\nK and L to change wind strength\nO and P to change wind regularity\nU and I for wind resolution\nT and Y for buffer refresh rate",
        core::rect<s32>(10,620,200,750), true, true, 0, -1, true);

    // add camera
    scene::ICameraSceneNode* camera =
        smgr->addCameraSceneNodeFPS(0,100.0f,.20f);

    camera->setPosition(core::vector3df(1000,500,1000));
    camera->setFarValue(12000.0f);

    // create wind...
    wind = scene::createWindGenerator( 30.0f, 3.0f );

    // disable mouse cursor
    device->getCursorControl()->setVisible(false);

    // add terrain scene node
    scene::ITerrainSceneNode* terrain = smgr->addTerrainSceneNode("media/terrain-heightmap.bmp");
    video::IImage* heightMap  = driver->createImageFromFile ("media/terrain-heightmap.bmp");
    video::IImage* textureMap = driver->createImageFromFile ("media/terrain-grasscol.bmp");
    video::IImage* grassMap   = driver->createImageFromFile ("media/terrain-grassmap.png");

    core::vector3df ts(40, 4.0f, 40);
    terrain->setScale(ts);
    terrain->setPosition( core::vector3df(0,0,0) );

    terrain->setMaterialFlag(video::EMF_LIGHTING, false);

    terrain->setMaterialTexture(0, driver->getTexture("media/terrain-texture.jpg"));
    terrain->setMaterialTexture(1, driver->getTexture("media/detailmap3.jpg"));

    terrain->setMaterialType(video::EMT_DETAIL_MAP);

    terrain->scaleTexture(1.0f, 20.0f);

    // create triangle selector for the terrain
    scene::ITriangleSelector* selector
        = smgr->createTerrainTriangleSelector(terrain, 0);
    terrain->setTriangleSelector(selector);
    selector->drop();

    // create collision response animator and attach it to the camera
    scene::ISceneNodeAnimator* anim = smgr->createCollisionResponseAnimator(
        selector, camera, core::vector3df(60,100,60),
        core::vector3df(0,-3,0),
        core::vector3df(0,50,0));
    camera->addAnimator(anim);
    anim->drop();

    // create event receiver
    scene::CGrassPatchSceneNode *grass[width*height];
    MyEventReceiver receiver(terrain,&grass[0]);
    device->setEventReceiver(&receiver);


    int lastFPS = -1;


    tex1 = driver->getTexture("media/grass.png");
    tex2 = driver->getTexture("media/grass2.bmp");
    driver->makeColorKeyTexture(tex2, core::position2d<s32>(0,0));

    //now we add some grass nodes
    for (int x=0; x<width; ++x)
      for (int z=0; z<height; ++z)
      {
        // add a grass node
        grass[x*width + z] = new scene::CGrassPatchSceneNode(terrain, smgr, -1, core::vector3d<s32>(x,0,z), "grass", heightMap, textureMap, grassMap, wind);
        grass[x*width + z]->setMaterialFlag(video::EMF_LIGHTING,false);
        grass[x*width + z]->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
        grass[x*width + z]->setMaterialTexture(0,tex1);
        grass[x*width + z]->drop();
    }


    while(device->run())
    if (device->isWindowActive())
    {

        driver->beginScene(true, true, video::SColor(0,50,50,200) );

        smgr->drawAll();
        env->drawAll();

        driver->endScene();
        if (camera->getPosition().Y < -300.0f)
            camera->setPosition(core::vector3df(1000,500,1000));
device->sleep(20);
        // display frames per second in window title
        int fps = driver->getFPS();
        if (lastFPS != fps)
        {
            core::stringw str = L"Grass! [";
            str += driver->getName();
            str += "] FPS:";
            str += fps;
            str += ", Tri:";
            str += s32(driver->getPrimitiveCountDrawn());
            str += ", Draw Distance:";
            str += s32(grass[0]->getDrawDistance());
            str += ", Max density:";
            str += s32(grass[0]->getMaxDensity());
            str += ", Wind Reg:";
            str += s32(wind->getRegularity());
            str += ", Wind Str:";
            str += s32(wind->getStrength());
            str += ", Wind Res:";
            str += s32(grass[0]->getWindRes());
            str += ", DelayMS:";
            str += s32(grass[0]->getRefreshDelay());

            device->setWindowCaption(str.c_str());
            lastFPS = fps;
        }
    }

	heightMap->drop();
    textureMap->drop();
    grassMap->drop();

    device->drop();

    return 0;
}

