
#include <stdlib.h>
#include <irrlicht.h>
#include "Game.h"
///#include <irrKlang.h>
#include "OS.h"

using namespace std;
using namespace irr;

int main(int argc, char *argv[])
{
    freopen("log.txt", "wt", stdout);
    freopen("errlog.txt", "wt", stderr);

    IrrlichtDevice *device = createDevice(video::EDT_OPENGL,
        core::dimension2d<irr::u32>(1024,768),
        32,
        false,
        false,false);

    device->setWindowCaption(L"People Shooter");
    device->getFileSystem()->addZipFileArchive("media.zip");
    //device->getFileSystem()->addFolderFileArchive("media_working");
    scene::ISceneManager *smgr = device->getSceneManager();
    device->getCursorControl()->setVisible(false);
    srand(device->getTimer()->getTime());

///    audio::ISoundEngine *sengine = createIrrKlangDevice(audio::ESOD_WIN_MM);

    video::IVideoDriver *driver = device->getVideoDriver();
    gui::IGUIEnvironment *guienv = device->getGUIEnvironment();

    // preload stuff
    smgr->getMesh("walls.b3d");
    smgr->getMesh("person.b3d");
    smgr->getMesh("duck.x");
    smgr->getMesh("ufo.x");
    smgr->getMesh("demon.b3d");
    driver->getTexture("splatscreen.png");
    guienv->getFont("scorefont.xml");
    guienv->getFont("smallfont.xml");
    driver->getTexture("bar.png");
    driver->getTexture("wall3.jpg");
    driver->getTexture("intro.png");
    driver->getTexture("bang.tga");
    device->getGUIEnvironment()->getFont("font1.bmp");
    smgr->getMesh("shell.b3d");
    driver->getTexture("blood_dark.tga");
    driver->getTexture("bloodstain0.tga");
    driver->getTexture("bloodstain1.tga");
    driver->getTexture("bloodstain2.tga");
    smgr->getMesh("person_gib0.b3d");
    smgr->getMesh("person_gib1.b3d");
    smgr->getMesh("person_gib2.b3d");
    driver->getTexture("tubing0.tga");
    driver->getTexture("tubing1.tga");
    driver->getTexture("blood.tga");
    driver->getTexture("feather.tga");
    driver->getTexture("spider.tga");

    // lights
	scene::ILightSceneNode *light = smgr->addLightSceneNode(NULL, core::vector3df(20,50.0,0),
        video::SColorf(0.8, 0.8, 0.8), 40.0f);
	video::SLight &slight = light->getLightData();
	slight.AmbientColor = video::SColorf(0.8,0.8,0.8);
	slight.SpecularColor = video::SColorf(0.6,0.6,0.6);

    // camera
    scene::ICameraSceneNode *camera = smgr->addCameraSceneNode();
    //scene::ICameraSceneNode *camera = smgr->addCameraSceneNodeFPS(0, 100.0, 10.0);
    camera->setPosition(core::vector3df(0,1,10));
    camera->setTarget(core::vector3df(0,4,0));
    //camera->setPosition(core::vector3df(4,9,4));
    //camera->setTarget(core::vector3df(0,9,0));

int sengine=0; //tecan hack
    // action!
    Game *game = new Game(device);
    bool showWebsite = game->run();
    delete game;

    device->drop();
//    sengine->drop();

    //if (showWebsite) os::openWebpage("http://games.xzist.org/shooter/buy/");

    return 0;
}
