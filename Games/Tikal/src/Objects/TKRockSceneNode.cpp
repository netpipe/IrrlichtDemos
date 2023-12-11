#include "TKRockSceneNode.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

using namespace std;

TKRockSceneNode::TKRockSceneNode(ISceneNode* parent, ISceneManager* mgr, s32 id, int maxDistance) : ISceneNode(parent, mgr, id)
{
    smgr=mgr;

    mesh=smgr->getMesh("../media/Rock/rock1.b3d");

    MaxDistance=maxDistance;

    originalPos=vector3df(-1000,-1000,-1000);
}

TKRockSceneNode::~TKRockSceneNode()
{
}

void TKRockSceneNode::OnRegisterSceneNode()
{
    if (IsVisible)
        SceneManager->registerNodeForRendering(this);

    ISceneNode::OnRegisterSceneNode();
}

void TKRockSceneNode::render()
{
    smgr->getVideoDriver()->setTransform( video::ETS_WORLD, AbsoluteTransformation );

    vector3df camPos=smgr->getActiveCamera()->getPosition();
    f32 distance = camPos.getDistanceFrom(this->getAbsolutePosition());


    if( distance < MaxDistance )
    {
        for(int i=0;i<mesh->getMeshBufferCount();i++)
        {
            SMaterial mat=mesh->getMeshBuffer(i)->getMaterial();
            smgr->getVideoDriver()->setMaterial(mat);
            smgr->getVideoDriver()->drawMeshBuffer(mesh->getMeshBuffer(i));
        }
    }

}

const core::aabbox3d<f32>& TKRockSceneNode::getBoundingBox() const
{
    return mesh->getBoundingBox();
}

u32 TKRockSceneNode::getMaterialCount() const
{
    return 1;
}

video::SMaterial& TKRockSceneNode::getMaterial(u32 i)
{
    return Material;
}

