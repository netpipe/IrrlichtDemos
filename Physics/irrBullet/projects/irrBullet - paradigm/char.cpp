#include "main.h"
class Cparadigm;
class Cevents;
class Cmap;
class Cchar;
class Cgui;

static Cparadigm *instance = &Cparadigm::Instance();
static Cevents *myevents = &Cevents::Instance();
static Cmap *mymap = &Cmap::Instance();
static Cchar *mychar = &Cchar::Instance();
static Cgui *mygui = &Cgui::Instance();

int Cchar::init()
{
        mychar->setRunning(true);
        mychar->loadChar = false;

        return 0;
};


void Cchar::loadchar()
{
    if (!mychar->loadChar)
    {
        mychar->loadChar = true;
        // add camera
        scene::ICameraSceneNode* camera =
            instance->smgr->addCameraSceneNodeFPS();
    }
    else
        cout << "Character already loaded.." << endl;
};

