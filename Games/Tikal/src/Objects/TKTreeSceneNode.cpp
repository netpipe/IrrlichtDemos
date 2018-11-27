#include "TKTreeSceneNode.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

using namespace std;

TKTreeSceneNode::TKTreeSceneNode(ISceneNode* parent, ISceneManager* mgr, s32 id, int maxDistance) : ISceneNode(parent, mgr, id)
{
    smgr=mgr;

int type = rand()%2;


        mesh=smgr->getMesh("../media/Vegetacao/treeMesh.b3d");///TODO: randomizar tipo de arvore
        folha=smgr->getMesh("../media/Vegetacao/treeMeshT.b3d");



    Material.MaterialType = EMT_TRANSPARENT_ALPHA_CHANNEL;


    MaxDistance=maxDistance;

    originalPos=vector3df(-1000,-1000,-1000);
}

TKTreeSceneNode::~TKTreeSceneNode()
{

}

void TKTreeSceneNode::OnRegisterSceneNode()
{
    if (IsVisible)
        SceneManager->registerNodeForRendering(this);

    ISceneNode::OnRegisterSceneNode();
}

void TKTreeSceneNode::render()
{
    smgr->getVideoDriver()->setTransform( video::ETS_WORLD, AbsoluteTransformation );

    vector3df camPos=smgr->getActiveCamera()->getPosition();//LOD
    f32 distance = camPos.getDistanceFrom(this->getAbsolutePosition());


    if( distance < MaxDistance )
    {
        if(DETAIL_HIGH)
        for(int i=0;i<mesh->getMeshBufferCount();i++)
        {
            SMaterial mat=mesh->getMeshBuffer(i)->getMaterial();
            mat.MaterialType = EMT_TRANSPARENT_ALPHA_CHANNEL_REF;
            smgr->getVideoDriver()->setMaterial(mat);
            smgr->getVideoDriver()->drawMeshBuffer(mesh->getMeshBuffer(i));
        }

        for(int i=0;i<folha->getMeshBufferCount();i++)
        {
            SMaterial mat=folha->getMeshBuffer(i)->getMaterial();
            mat.MaterialType = EMT_TRANSPARENT_ALPHA_CHANNEL;
            smgr->getVideoDriver()->setMaterial(mat);
            smgr->getVideoDriver()->drawMeshBuffer(folha->getMeshBuffer(i));
        }
    }

}

const core::aabbox3d<f32>& TKTreeSceneNode::getBoundingBox() const
{
    return mesh->getBoundingBox();
}

u32 TKTreeSceneNode::getMaterialCount() const
{
    return 1;
}

video::SMaterial& TKTreeSceneNode::getMaterial(u32 i)
{
    return Material;
}

IAnimatedMesh* TKTreeSceneNode::getMesh()
{
    return mesh;
}

