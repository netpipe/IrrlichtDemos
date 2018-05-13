#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class Arrow
{
private:
    IrrlichtDevice* device;
    ISceneManager* smgr;

    IAnimatedMesh* arrowMesh;
    //IAnimatedMeshSceneNode *arrow;
    IMeshSceneNode* arrow;

public:
    Arrow(IrrlichtDevice* idevice);
    void hideArrow();
    void showArrow();
    vector3df getPosition();
    vector3df getRotation();
    void setRotation(vector3df);
    void setPosition(vector3df);
    void setPosition(double X ,double Y,double Z);
};
