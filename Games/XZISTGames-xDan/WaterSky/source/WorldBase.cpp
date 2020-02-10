
#include "WorldBase.h"

WorldBase::WorldBase(IrrlichtDevice *device)
{
    this->device = device;
    smgr = device->getSceneManager();
    driver = device->getVideoDriver();
    guienv = device->getGUIEnvironment();
    curcon = device->getCursorControl();
    filesys = device->getFileSystem();
    timer = device->getTimer();
}

WorldBase::WorldBase(WorldBase *other)
{
    *this = *other;
}
