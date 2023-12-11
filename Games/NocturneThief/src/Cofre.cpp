#include "Cofre.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


Cofre::Cofre(ISceneManager* mgr)
{
    smgr=mgr;

    mesh=smgr->getMesh("../media/cofre.b3d");
    node=smgr->addAnimatedMeshSceneNode(mesh);

    node->setTriangleSelector(smgr->createTriangleSelectorFromBoundingBox(node));

    meshShadow=smgr->getMesh("../media/sombraCofre.b3d");
    nodeShadow=smgr->addAnimatedMeshSceneNode(meshShadow,node);
    nodeShadow->setPosition(vector3df(0,0.02,0));
    nodeShadow->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL);
}

void Cofre::setEstado(ESTADO_COFRE estado)
{
    if(estado==COFRE_ABERTO)
    {
        node->setFrameLoop(1,1);
    }

    if(estado==COFRE_FECHADO)
    {
        node->setFrameLoop(0,0);
    }
}

void Cofre::setPosition(vector3df pos)
{
    node->setPosition(pos);
}

vector3df Cofre::getPosition()
{
    return node->getPosition();
}

ITriangleSelector* Cofre::getTriangleSelector()
{
    return node->getTriangleSelector();
}
