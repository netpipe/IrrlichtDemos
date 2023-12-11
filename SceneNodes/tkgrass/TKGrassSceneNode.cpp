#include "TKGrassSceneNode.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

using namespace std;


TKGrassSceneNode::TKGrassSceneNode(ISceneNode* parent, ISceneManager* mgr, s32 id, s32 material, int maxDistance) : ISceneNode(parent, mgr, id)
{

    #ifndef __ID_GRASS_CONTROL_INIT__
    #define __ID_GRASS_CONTROL_INIT__
        static int idGrassControl=0;
    #endif

    idGrass=idGrassControl++;
    if(idGrassControl > 2) idGrassControl=0;

    smgr=mgr;

    mesh=smgr->getMesh("../media/mesh.b3d");

    Material.TextureLayer[0].Texture = smgr->getVideoDriver()->getTexture("../media/grass.png");
    this->setScale(vector3df(1.2,1.2,1.2));


    Material.MaterialType = (E_MATERIAL_TYPE)material;

    Material.MaterialTypeParam = 0.3f;

    MaxDistance=maxDistance;

    originalPos=vector3df(-1000,-1000,-1000);
}

TKGrassSceneNode::~TKGrassSceneNode()
{
    mesh->drop();
}

void TKGrassSceneNode::OnRegisterSceneNode()
{
    if (IsVisible)
        SceneManager->registerNodeForRendering(this);

    ISceneNode::OnRegisterSceneNode();
}

void TKGrassSceneNode::render()
{
    smgr->getVideoDriver()->setTransform( video::ETS_WORLD, AbsoluteTransformation );
    smgr->getVideoDriver()->setMaterial(Material);

    vector3df camPos=smgr->getActiveCamera()->getPosition();//LOD
    f32 distance = camPos.getDistanceFrom(this->getAbsolutePosition());

    if(originalPos!=vector3df(-1000,-1000,-1000)) RelativeTranslation=originalPos;//


    if(distance > (MaxDistance/3)*2 && distance < MaxDistance && idGrass==0 )
    {
        smgr->getVideoDriver()->drawMeshBuffer(mesh->getMeshBuffer(0));
        originalPos=this->getPosition();
        //this->setPosition(originalPos-vector3df(0,0.66,0));
    }
    if(distance > (MaxDistance/3) && distance < (MaxDistance/3)*2)// && (idGrass==1 || idGrass==2) )
    {
        smgr->getVideoDriver()->drawMeshBuffer(mesh->getMeshBuffer(0));
        originalPos=this->getPosition();
        //this->setPosition(originalPos-vector3df(0,0.33,0));
    }
    if( distance < (MaxDistance/3) )
    {
        smgr->getVideoDriver()->drawMeshBuffer(mesh->getMeshBuffer(0));
    }
}

const core::aabbox3d<f32>& TKGrassSceneNode::getBoundingBox() const
{
    return mesh->getBoundingBox();
}

u32 TKGrassSceneNode::getMaterialCount() const
{
    return 1;
}

video::SMaterial& TKGrassSceneNode::getMaterial(u32 i)
{
    return Material;
}

