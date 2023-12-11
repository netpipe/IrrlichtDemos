#include <stdio.h>
#include <math.h>
#include <wchar.h>
#include <irrlicht.h>
#include <IGUIFont.h>
#include "../src/irrWeatherManager.h"

using namespace irr;
using namespace core;
using namespace video;
using namespace gui;
using namespace scene;

/*
    This example is not meant to show good coding style, and it is not recommended that you
    set up projects this way; it's only there to show you how to use irrWeatherManager.
*/

scene::ISceneNode* node = 0;
IrrlichtDevice* device = 0;
bool camEnabled=true;
scene::ISceneManager* smgr;
scene::ICameraSceneNode* camera = 0;
scene::ICameraSceneNode* camera2 = 0;
irrWeatherManager *weatherManager = 0;

int rand2(int lim, int seed)
{
    static long a = seed; // could be made the seed value

    a = (a * 32719 + 3) % 32749;

    return ((a % lim) + 1);
}


class MyEventReceiver : public IEventReceiver
{
public:
   virtual bool OnEvent(const SEvent& event)
   {
        if (event.EventType == irr::EET_KEY_INPUT_EVENT&&
            event.KeyInput.PressedDown)
        {
            switch(event.KeyInput.Key)
            {
                case KEY_ESCAPE:{device->closeDevice();}
                case KEY_SPACE:
                {
                    camEnabled=!camEnabled;
                    device->getCursorControl()->setVisible(!camEnabled);
                    core::vector3df target;
                    if(camEnabled)
                    {
                        device->getCursorControl()->setPosition(0.5f,0.5f);
                        smgr->setActiveCamera(camera);
                        camera->setInputReceiverEnabled(true);
                    }
                    else
                    {
                        camera->setInputReceiverEnabled(false);
                        target=camera->getTarget();
                        camera2->setTarget(target);
                        camera2->setPosition(camera->getPosition());
                        smgr->setActiveCamera(camera2);
                    }
            }
            case KEY_KEY_S:
            {
                //   core::vector3df v = node->getPosition();
                //   v.Y += event.KeyInput.Key == KEY_KEY_W ? 2.0f : -2.0f;
                //   node->setPosition(v);
            }
            default:
            break;

            return true;
            }
        }

        if (event.EventType == EET_GUI_EVENT)
        {
            s32 id = event.GUIEvent.Caller->getID();
            //IGUIEnvironment* env = device->getGUIEnvironment();

            switch(event.GUIEvent.EventType)
            {

                case EGET_SCROLL_BAR_CHANGED:
                if (id == 104)
                {
                    s32 pos = ((IGUIScrollBar*)event.GUIEvent.Caller)->getPos();
                    weatherManager->getAtmosphere()->setDaysPerDay((f64)pos/10.0f);//day speed
                }
                break;
                default:
                    break;
            }
            return true;
        }


      return false;
   }
};


/*void createCloud(const vector3df &pos, const vector3df &scale, u32 extraSeed)
{
    // create a cloud node
    CCloudSceneNode* clouds = new CCloudSceneNode(
            device->getSceneManager()->getRootSceneNode(), device->getSceneManager(),
                device->getTimer(), -1, pos, core::vector3df(0,0,0), core::vector3df(1,1,1));

    // load the texture
    video::ITexture * txture = device->getVideoDriver()->getTexture("media/cloud4.png");

    // set any old random seed
    srand(device->getTimer()->getRealTime() + extraSeed);

    // set the level of details to draw at what distance
    clouds->setLOD(10.0f);

    // set the maximum detail level when recursing
    clouds->setMaxDepth(3);

    // we dont use lighting yet. all the normals face the camera and it looks wierd
    // we could edit the colour of the particles in the list in realtime though!
    clouds->setMaterialFlag(video::EMF_LIGHTING, true);


    clouds->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);

    // set any old random seed
    srand(rand());

    // call a "here's one I made earlier" function
    clouds->makeRandomCloud(60);

    clouds->setLOD(10.0f);
    clouds->setMaxDepth(1);


    clouds->setScale(scale);

    clouds->setMaterialTexture(0, txture );


    // cheap movement just so the clouds are not static
    scene::ISceneNodeAnimator* anim = 0;
	anim = smgr->createFlyStraightAnimator(clouds->getPosition(), clouds->getPosition() + vector3df(0,0,80000), 456400, true, true);
	clouds->addAnimator(anim);
	anim->drop();
}*/

void createUserInterface(IGUIEnvironment* const env, IGUISkin* const skin, IGUIFont* const font)
{
    IGUIScrollBar* scrollbar = env->addScrollBar(true, rect<s32>(150, 45, 350, 60), 0, 104);
    scrollbar->setMax(50000);
    scrollbar->setPos(600);

    IGUIStaticText *textinfo;

    textinfo = env->addStaticText(L"Dayspeed in minutes", rect<s32>(150,25,350,40), true);

    textinfo = env->addStaticText(L"Press spacebar to enable or disable camera controls to use the GUI. Arrows to move.",
        rect<s32>(50,5,550,20), true);

    textinfo = env->addStaticText(L"Dayspeed is how much time completes a virtual day(1440 min) in minutes.\nExample: 2880(dayspeed) / 1440(day in minutes) = 2 days per 1 minute",
        rect<s32>(50,70,550,100), true);
}


int main()
{

    MyEventReceiver receiver;
    device = createDevice(video::EDT_OPENGL, core::dimension2d<u32>(800, 600),
        32,false, true, false, &receiver);//EDT_DIRECT3D9

    device->setEventReceiver(&receiver);
    video::IVideoDriver* driver = device->getVideoDriver();

    IGUIEnvironment* env = device->getGUIEnvironment();
    driver->setTextureCreationFlag(ETCF_ALWAYS_32_BIT,true);
    smgr = device->getSceneManager();

    IGUISkin* skin = env->getSkin();
    IGUIFont* font = env->getFont("media/fonthaettenschweiler.bmp");
    if (font)
        skin->setFont(font);

    createUserInterface(env, skin, font);

    driver->setFog(SColor(0, 255, 255, 255), EFT_FOG_LINEAR, 8000.0f, 40000.0f,
		0.01f, true, true);

    camera = smgr->getActiveCamera();
    if (camera)
    {
        smgr->setActiveCamera(0);
        camera->remove();
    }
    camera2 = smgr->addCameraSceneNode();
    camera2->setFOV(45.0f*180.0f/irr::core::DEGTORAD);//make correct FOV
    camera2->setFarValue(120000.0f);
    camera = smgr->addCameraSceneNodeFPS(0, 200, 5);
    camera->setFOV(45.0f*180.0f/irr::core::DEGTORAD);
    camera->setFarValue(1200000.0f);
    camera->setPosition(core::vector3df(0,2040,20));

    weatherManager = new irrWeatherManager(device);
    weatherManager->getAtmosphere()->setUpdateFog(true);


    scene::ITerrainSceneNode* terrain = smgr->addTerrainSceneNode(
      "media/terrain-heightmap.bmp");

    terrain->setScale(core::vector3df(24000, 50, 24000));
    terrain->setMaterialFlag(EMF_FOG_ENABLE, true);
    terrain->setMaterialFlag(video::EMF_LIGHTING, true);
    terrain->setPosition(core::vector3df(-180000,-1400,-180000));
    terrain->setMaterialType(terrain->getMaterial(0).MaterialType == video::EMT_SOLID ?
    video::EMT_DETAIL_MAP : video::EMT_SOLID);
    terrain->setMaterialTexture(0, driver->getTexture("media/terrain-texture.jpg"));
    terrain->setMaterialTexture(1, driver->getTexture("media/detailmap3.jpg"));
    //terrain->setMaterialType(video::EMT_DETAIL_MAP);
    terrain->scaleTexture(1.0f, 50.0f);


    /*u32 numClouds = 40;

    for(u32 i=0; i < numClouds; i++)
    {
        f32 x = rand() / 0.4 - (rand() / 0.4);
        f32 y = rand() / 0.4 - (rand() / 0.4);
        f32 z = rand() / 0.4 - (rand() / 0.4); // 50.0

        createCloud(vector3df(x, 6000, z), vector3df(0.15,0.2,0.3), i * 22);
    }*/

    irr::core::array<stringc> textureNames;

    SCloudCreationInfo info;

    info.setDefaults();


    info.textureNames.push_back(stringc("media/cumulus2.png"));
    info.textureNames.push_back(stringc("media/cumulus3.png"));
    info.textureName = "media/cumulus2.png";
    info.seed = device->getTimer()->getTime();
    info.numParticles = 10;


    // We'll only set the center of the layer and let the default parameters set the rest
    //weatherManager->createCloudLayer(vector3df(0,6000,0), vector3df(0.15,0.2,0.3), "media/cumulus2.png", 8);

    ICloudLayer* layer = weatherManager->addCloudLayer(vector3df(0,19000,0), info, 1000, false);

    for(u32 i=0; i < layer->getCloudCount(); i++)
    {
        //layer->getCloudByIndex(i)->setImposterEnabled(false);
        //layer->getCloudByIndex(i)->setDebugDataVisible(1);
        //layer->getCloudByIndex(i)->setDefaultColor(SColor(255,255,255,255));
    }

    /*info.textureNames.clear();
    info.textureName = "media/cumulus1.png";
    weatherManager->addCloudLayer(vector3df(0,49000,0), info, 300, false);*/
    //weatherManager->addCloudLayer(vector3df(0,9000,0), info, 50);

    //weatherManager->setLightningFrequency(20.0f);

    //weatherManager->removeCloudLayer(weatherManager->getCloudLayerByIndex(1));
    //weatherManager->removeAllCloudLayers(true);



    device->getCursorControl()->setVisible(false);
    int lastFPS = -1;

    /*camera->setPosition(vector3df(terrain->getPosition().X + (terrain->getScale().X * 0.5),
        10.0f, terrain->getPosition().Z - (terrain->getScale().Z * 0.5)));*/

    //weatherManager->getAtmosphere()->getCompoundCelestialBodySceneNode()->setSunSize(dimension2df(400.0f,400.0f));

    while(device->run())
    {
        driver->beginScene(true, true, weatherManager->getAtmosphere()->getClearColor());
        weatherManager->updateWeather(); // Update the weather manager
        smgr->drawAll();
        env->drawAll();
        driver->endScene();



        int fps = driver->getFPS();
        wchar_t tmp[1024];int fp;
        if (lastFPS != fps)
        {
            fp=fps;
            lastFPS = fps;
        }

        swprintf(tmp, 1024, L" [irrWeatherManager Sample] FPS: %d Dayspeed: %5.3f",
                fp,weatherManager->getAtmosphere()->getDayspeed());


        stringw str = "FPS= ";
        str += fp;

        font->draw(str.c_str(),rect<s32>(5,5,30,30),SColor(255,255,0,100));

        device->setWindowCaption(tmp);

    }

   delete weatherManager;

   device->drop();

   return 0;
}
