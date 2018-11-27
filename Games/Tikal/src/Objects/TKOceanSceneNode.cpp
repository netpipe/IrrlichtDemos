#include <irrlicht.h>
#include "TKOceanSceneNode.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


TKOceanSceneNode::TKOceanSceneNode(ISceneNode* parent, ISceneManager* mgr, s32 id) : ISceneNode(parent, mgr, id)
{
    smgr=mgr;

    originalPos=vector3df(-1000,-1000,-1000);

    IImage *heightMap=smgr->getVideoDriver()->createImageFromFile("../media/Ocean/ocean.png");

    //mesma coisa que terrain...
    stringc name = "ocean_tikal";
    //name+=rand();
    mesh=smgr->addTerrainMesh(name.c_str(),heightMap,heightMap,dimension2d<f32>(MAPSIZE,MAPSIZE),5,dimension2du(128,128));

    heightMap->drop();
}

TKOceanSceneNode::~TKOceanSceneNode()
{
}

void TKOceanSceneNode::OnRegisterSceneNode()
{
    if (IsVisible)
        SceneManager->registerNodeForRendering(this);

    ISceneNode::OnRegisterSceneNode();
}

void TKOceanSceneNode::render()
{
    smgr->getVideoDriver()->setTransform( video::ETS_WORLD, AbsoluteTransformation );

    vector3df camPos=smgr->getActiveCamera()->getPosition();
    f32 distance = camPos.getDistanceFrom(this->getAbsolutePosition());


    for(int i=0;i<mesh->getMeshBufferCount();i++)
    {
        //SMaterial mat=mesh->getMeshBuffer(i)->getMaterial();
        smgr->getVideoDriver()->setMaterial(Material);
        smgr->getVideoDriver()->drawMeshBuffer(mesh->getMeshBuffer(i));
    }

}

const core::aabbox3d<f32>& TKOceanSceneNode::getBoundingBox() const
{
    return mesh->getBoundingBox();
}

u32 TKOceanSceneNode::getMaterialCount() const
{
    return 1;
}

video::SMaterial& TKOceanSceneNode::getMaterial(u32 i)
{
    return Material;
}

void TKOceanSceneNode::setHeight(f32 height)
{
    setPosition(getPosition()+vector3df(0,height,0));
}


