#ifndef __COFRE__
#define __COFRE__

#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

enum ESTADO_COFRE
{
    COFRE_ABERTO, COFRE_FECHADO
};

class Cofre
{
public:
    Cofre(ISceneManager* mgr);

    void setEstado(ESTADO_COFRE estado);
    void setPosition(vector3df pos);

    vector3df getPosition();
    ITriangleSelector* getTriangleSelector();
private:
    ISceneManager* smgr;

    IAnimatedMesh* mesh;
    IAnimatedMeshSceneNode* node;

    IAnimatedMesh* meshShadow;
    IAnimatedMeshSceneNode* nodeShadow;
};

#endif
