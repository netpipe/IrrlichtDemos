#include "TKGroundVegetationSceneNode.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

using namespace std;

TKGroundVegetationSceneNode::TKGroundVegetationSceneNode(ISceneNode* parent, ISceneManager* mgr, s32 id, int maxDistance) : ISceneNode(parent, mgr, id)
{
    smgr=mgr;

    int type = rand()%3;

    mesh=smgr->getMesh("../media/Vegetacao/foll01.b3d");

    if(type==0) Material.setTexture(0,smgr->getVideoDriver()->getTexture("../media/Vegetacao/vegetation0.png"));
    if(type==1) Material.setTexture(0,smgr->getVideoDriver()->getTexture("../media/Vegetacao/vegetation1.png"));
    if(type==2) Material.setTexture(0,smgr->getVideoDriver()->getTexture("../media/Vegetacao/vegetation2.png"));

    Material.MaterialType = EMT_TRANSPARENT_ALPHA_CHANNEL;

    MaxDistance=maxDistance;

    originalPos=vector3df(-1000,-1000,-1000);
}

TKGroundVegetationSceneNode::~TKGroundVegetationSceneNode()
{
}

void TKGroundVegetationSceneNode::OnRegisterSceneNode()
{
    if (IsVisible)
        SceneManager->registerNodeForRendering(this);

    ISceneNode::OnRegisterSceneNode();
}

void TKGroundVegetationSceneNode::render()
{
    smgr->getVideoDriver()->setTransform( video::ETS_WORLD, AbsoluteTransformation );

    vector3df camPos=smgr->getActiveCamera()->getPosition();
    f32 distance = camPos.getDistanceFrom(this->getAbsolutePosition());


    if( distance < MaxDistance )
    {
        for(int i=0;i<mesh->getMeshBufferCount();i++)
        {
            //SMaterial mat=mesh->getMeshBuffer(i)->getMaterial();
            Material.MaterialType = EMT_TRANSPARENT_ALPHA_CHANNEL_REF;
            smgr->getVideoDriver()->setMaterial(Material);
            smgr->getVideoDriver()->drawMeshBuffer(mesh->getMeshBuffer(i));
        }

        if(DETAIL_HIGH)
        for(int i=0;i<mesh->getMeshBufferCount();i++)///TODO: boolean para controlar HIGH e LOW
        {
            Material.MaterialType = EMT_TRANSPARENT_ALPHA_CHANNEL;
            smgr->getVideoDriver()->setMaterial(Material);
            smgr->getVideoDriver()->drawMeshBuffer(mesh->getMeshBuffer(i));
        }
    }

}

const core::aabbox3d<f32>& TKGroundVegetationSceneNode::getBoundingBox() const
{
    return mesh->getBoundingBox();
}

u32 TKGroundVegetationSceneNode::getMaterialCount() const
{
    return 1;
}

video::SMaterial& TKGroundVegetationSceneNode::getMaterial(u32 i)
{
    return Material;
}



