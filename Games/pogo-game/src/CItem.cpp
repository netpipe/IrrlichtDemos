#include "../include/CItem.h"


CItem::CItem(int mtype,scene::ISceneManager* mgr,s32 mat,ITimer*mtimer,scene::ISceneNode*mLevelNode,scene::ITriangleSelector* mSelector)
{
    smgr=mgr;
    type=mtype;
    driver=smgr->getVideoDriver();

    levelNode=mLevelNode;
    selector=mSelector;
    timer=mtimer;
    worldBox=levelNode->getBoundingBox();

    //coin node setup
    scene::IAnimatedMesh* faerie = smgr->getMesh("coin.obj");
    faerie->setHardwareMappingHint(scene::EHM_STATIC);
    node = smgr->addAnimatedMeshSceneNode(faerie);
    node->setMaterialTexture(0, driver->getTexture("coin_a.tga"));
    node->setMaterialTexture(1, driver->getTexture("coin_n.tga"));
    node->setMaterialTexture(2, driver->getTexture("coin_refl.tga"));
    node->setMaterialType((video::E_MATERIAL_TYPE)mat);
    node->setScale(core::vector3df(100,100,100));
    scene::ISceneNodeAnimator*anim;
    //evil coin spins in reverse
    if (type==0)
        anim=smgr->createRotationAnimator(core::vector3df(0,-0.5,0));
    else
        anim=smgr->createRotationAnimator(core::vector3df(0,0.5,0));
    //setup special coin 3d sound
    if (type==49)
    {
//        twinkle= engine->play3D("chimes.wav",
    //                            vec3df(node->getPosition().X,node->getPosition().Y,node->getPosition().Z), true, false, true);
    //    twinkle->setMinDistance(1000.0f);
    }
    else
        //only nice ocins get the animator
        node->addAnimator(anim);

    this->spwan();

}

CItem::~CItem()
{
    //dtor
}


