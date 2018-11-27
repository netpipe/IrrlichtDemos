#include <irrlicht.h>
#include "TKTerrainSceneNode.h"
#include <iostream>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

using namespace std;


TKTerrainSceneNode::TKTerrainSceneNode(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id, stringc heightMapFile)
		: scene::ISceneNode(parent, mgr, id)
{
    smgr=mgr;

    IImage *heightMap=smgr->getVideoDriver()->createImageFromFile(heightMapFile.c_str());

    cout << ">>>>>>>>>>>>>>>>>:" << heightMap->getDimension().Width << endl;

    //Para incrementar o numero de terrains será necessário corrigir o bug da memória, o mesh buffer ta gerando lixo.
    stringc name = "terrain_tikal";
    //name += rand();

    mesh=smgr->addTerrainMesh(name.c_str(),heightMap,heightMap,dimension2d<f32>(MAPSIZE,MAPSIZE),TERRAIN_HEIGHT,dimension2du(128,128));

///    this->getMaterial(0).TextureLayer[0].TextureWrap=(E_TEXTURE_CLAMP)1.0;

    heightMap->drop();
}

TKTerrainSceneNode::~TKTerrainSceneNode()
{
}

void TKTerrainSceneNode::OnRegisterSceneNode()
{
    if (IsVisible)
        SceneManager->registerNodeForRendering(this);

    ISceneNode::OnRegisterSceneNode();
}

void TKTerrainSceneNode::render()
{
    smgr->getVideoDriver()->setTransform( video::ETS_WORLD, AbsoluteTransformation );

    smgr->getVideoDriver()->setMaterial(Material);

    smgr->getVideoDriver()->drawMeshBuffer(mesh->getMeshBuffer(0));
}

const core::aabbox3d<f32>& TKTerrainSceneNode::getBoundingBox() const
{
    return mesh->getBoundingBox();
}

u32 TKTerrainSceneNode::getMaterialCount() const
{
    return 1;
}

video::SMaterial& TKTerrainSceneNode::getMaterial(u32 i)
{
    return Material;
}

IMesh* TKTerrainSceneNode::getMesh()
{
    return mesh;
}
