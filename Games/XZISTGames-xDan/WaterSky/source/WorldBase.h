
#ifndef __WorldBase_h
#define __WorldBase_h

#include <assert.h>
#include <irrlicht.h>
#include <irrKlang.h>
#include "ODEPhysics.h"
#include <xmlParser.h>

using namespace irr;

class Object;

class WorldBase
{
public:
    WorldBase(IrrlichtDevice *);
    WorldBase(WorldBase *);
    
    IrrlichtDevice *device;
    audio::ISoundEngine *sengine;
    ODEPhysics *dPhysics;
    core::array <Object *> *objects;
    XMLNode *xTranslation;
    
    // Set automatically
    scene::ISceneManager *smgr;
    video::IVideoDriver *driver;
    gui::IGUIEnvironment *guienv;
    gui::ICursorControl *curcon;
    io::IFileSystem *filesys;
    ITimer *timer;
};

#endif
