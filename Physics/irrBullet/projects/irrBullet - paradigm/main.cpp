#define LOADMAP 0
#define LOADCHAR 1

#include "main.h"
class Cparadigm;
class Cevents;
class Cmap;
class Cchar;
class Cgui;
class MyEventReceiver;

static Cparadigm *instance = &Cparadigm::Instance();
static Cevents *myevents = &Cevents::Instance();
static Cmap *mymap = &Cmap::Instance();
static Cchar *mychar = &Cchar::Instance();
static Cgui *mygui = &Cgui::Instance();


int main ()
{
    if(instance->init()==1)
    {
        cout << "Error Initializing Device" << endl;
    }

    return 0;
};

int Cparadigm::init()
{
        instance->setRunning(true);

       	// ask user for driver
        video::E_DRIVER_TYPE driverType=driverChoiceConsole();
        if (driverType==video::EDT_COUNT)
            return 1;

        // create device with full flexibility over creation parameters
        // you can add more parameters if desired, check irr::SIrrlichtCreationParameters
        irr::SIrrlichtCreationParameters params;
        params.DriverType=driverType;
        params.WindowSize=core::dimension2d<u32>(800 , 600);
        instance->device = createDeviceEx(params);

        if (!instance->device)
            return 1;

        instance->device->setWindowCaption(L"Paradigm");
        instance->driver = instance->device->getVideoDriver();
        instance->smgr = instance->device->getSceneManager();
        instance->guienv = instance->device->getGUIEnvironment();
        instance->mymenu = instance->smgr->createNewSceneManager();

        //set fonts
        IGUISkin* skin = instance->guienv->getSkin();
        instance->guienv->getSkin()->setFont(instance->guienv->getFont("./data/fontlucida.png"));
        skin->setFont(instance->guienv->getBuiltInFont(), EGDF_TOOLTIP);

        // create event receiver
        rec = new MyEventReceiver();
        instance->device->setEventReceiver(rec);

        // Init all our engines
        myevents->init();
        mymap->init();
        mygui->init();

        // load the first map
        myevents->addEvent(LOADMAP);

         // load the character
        myevents->addEvent(LOADCHAR);

        // Start the engine into its main loop
        instance->step ();

        return 0;
};

int Cparadigm::step()
{
    // Set our delta time and time stamp
    u32 TimeStamp = instance->device->getTimer()->getTime();
    u32 DeltaTime = 0;
   	while( instance->device->run() && instance->getRunning())
	{
	    if (instance->device->isWindowActive())
	    {
           instance->driver->beginScene(true, true, SColor(0,0,0,0));

            DeltaTime = device->getTimer()->getTime() - TimeStamp;
            TimeStamp = device->getTimer()->getTime();

            // irrbullet related
            mymap->world->stepSimulation(DeltaTime*0.001f, 120);
            mymap->world->debugDrawWorld(true);
            mymap->world->debugDrawProperties(true);

	    }

            // Run out events
           myevents->events();

        if (instance->device->isWindowActive())
	    {
            // Are we inside the main menu? if not draw our scene

            if (!mygui->mainmenu)
                instance->smgr->drawAll();
            else
                instance->mymenu->drawAll();

           instance->guienv->drawAll();

           instance->driver->endScene();
	    }
	}

    delete mymap->world;
        return 0;
};
