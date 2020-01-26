// hudIcons.h
////////////////

#ifndef __HUDICONS_H__
#define __HUDICONS_H__

#include <irrlicht.h>
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;
using namespace std;

class HudIcons {
public:
       HudIcons(IGUIEnvironment* guienv, IVideoDriver* driver);
       ~HudIcons();
       void SetIcons(bool billSunk[]);
private:
       IGUIEnvironment* guienv;   
	   IVideoDriver* driver;
       IGUIImage*  blist[15];
       
       unsigned size;// = 32;
       unsigned spacer;// = 4;
};

#endif
