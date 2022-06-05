#include "CApplication.h"
#include "CStateManager.h"
#include "CStateFactory.h"
#include "CMainMenu.h"
#include "CGame.h"
#include "CEditor.h"

CApplication::CApplication()
{
    //ctor
    Manager = NULL;
    Device = NULL;
}

CApplication::~CApplication()
{
    //dtor
    if (Device)
    {
        Device->closeDevice();
        Device->drop();
        Device = NULL;
    }
}

void CApplication::init(int x, int y)
{
    if (Device)
        return;
    Device = irr::createDevice(irr::video::EDT_OPENGL, irr::core::dimension2du(x,y));
    Manager = new CStateManager(Device);

    registerStates();
}

void CApplication::registerStates(void)
{
    Manager->registerState(new CSimpleStateFactory<CMainMenu>("MainMenu"));
    Manager->registerState(new CSimpleStateFactory<CGame>("Game"));
    Manager->registerState(new CSimpleStateFactory<CEditor>("Editor"));
    Manager->createState("MainMenu");
}

void CApplication::run(void)
{
    while (Device && Device->run())
    {
        //draw stuff
        Device->getVideoDriver()->beginScene();
        Device->getSceneManager()->drawAll();
        Device->getGUIEnvironment()->drawAll();
        Device->getVideoDriver()->endScene();

        //update states
        if (!Manager->update())
            break;
    }
}
