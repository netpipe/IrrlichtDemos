#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class Archer
{
public:
    Archer(IrrlichtDevice* idevice);
    void animate();
    double getDistanceFromTarget();
    IAnimatedMeshSceneNode* getMesh();
    vector3df getPosition();
    void rotateMainNode(float angle);
    void setAnimationAim(double);
    void setAnimationShoot();
    void setAnimationWait();
    void setDistanceFromTarget(double);


private:
    IrrlichtDevice* device;
    ISceneManager* smgr;

    IAnimatedMesh* archerMesh;
    IAnimatedMeshSceneNode* archer;
    IBoneSceneNode* mainNode;
};
