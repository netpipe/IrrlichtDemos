#include "TKSkySceneNode.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


TKSkySceneNode::TKSkySceneNode(ISceneNode* parent, ISceneManager* mgr, s32 id) : ISceneNode(parent, mgr, id)
{
    smgr=mgr;

    mesh=smgr->getMesh("../media/Sky/skyMesh.b3d");

    Material.TextureLayer[0].Texture = smgr->getVideoDriver()->getTexture("../media/Sky/sky.jpg");

    originalPos=vector3df(-1000,-1000,-1000);

    this->setScale(vector3df(500,300,500));
    this->setMaterialFlag(EMF_LIGHTING,false);
}

TKSkySceneNode::~TKSkySceneNode()
{
}

void TKSkySceneNode::OnRegisterSceneNode()
{
    if (IsVisible)
        SceneManager->registerNodeForRendering(this);

    ISceneNode::OnRegisterSceneNode();
}

void TKSkySceneNode::render()
{
    smgr->getVideoDriver()->setTransform( video::ETS_WORLD, AbsoluteTransformation );

    smgr->getVideoDriver()->setMaterial(Material);

    for(int i=0;i<mesh->getMeshBufferCount();i++)
    {
        smgr->getVideoDriver()->drawMeshBuffer(mesh->getMeshBuffer(i));
    }
}

const core::aabbox3d<f32>& TKSkySceneNode::getBoundingBox() const
{
    return mesh->getBoundingBox();
}

u32 TKSkySceneNode::getMaterialCount() const
{
    return 1;
}

video::SMaterial& TKSkySceneNode::getMaterial(u32 i)
{
    return Material;
}
