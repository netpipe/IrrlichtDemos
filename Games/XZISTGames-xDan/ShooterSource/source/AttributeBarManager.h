
#ifndef __AttributeBarManager_h
#define __AttributeBarManager_h

#include <irrlicht.h>
#include <vector>

using namespace irr;

namespace abm
{

enum {LEFT = 0x00, CENTRE = 0x01, RIGHT = 0x02, TOP = 0x03, MIDDLE = 0x04, BOTTOM = 0x05};

struct AttributeBar
{
    video::ITexture *texture;
    int *percent;
    int movingPercent;
    char hAlign,vAlign;
};

class AttributeBarManager
{
public:
    AttributeBarManager(IrrlichtDevice *);
    bool notChanging(int i);
    void render(double);
    void clear();
    void add(video::ITexture *, int *, char,char);
private:
    IrrlichtDevice *device;
    scene::ISceneManager *smgr;
    video::IVideoDriver *driver;
    double time;
    std::vector <AttributeBar> attributeBars;
    
    int margin,padding;
};

};

#endif
