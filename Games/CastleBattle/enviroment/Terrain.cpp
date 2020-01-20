#include <iostream>
#include "Terrain.h"
using namespace irr;
Terrain::Terrain(scene::ISceneManager* smgr, video::IVideoDriver* driver, const core::stringw* pathTexture, const core::vector3df &scale){
 this->terrain = smgr->addTerrainSceneNode(
    pathTexture[0],
    0,
    1,
    core::vector3df(0.f,0.f,0.f),
    core::vector3df(0.f,0.f,0.f),
    scale,
    video::SColor(255,255,255,255),
    5,
    scene::ETPS_17,
    4,
    true
    );
 this->terrain->setMaterialTexture(0,driver->getTexture(pathTexture[0]));
 this->terrain->setMaterialTexture(1,driver->getTexture(pathTexture[1]));
 this->terrain->setMaterialType(irr::video::EMT_DETAIL_MAP);
 this->terrain->setMaterialFlag(irr::video::EMF_LIGHTING, false);
 this->terrain->setMaterialFlag(irr::video::EMF_FOG_ENABLE, false);
 this->terrain->setMaterialFlag(irr::video::EMF_NORMALIZE_NORMALS, true);
 this->terrain->scaleTexture(1.0f, 1000.0f);
 #if (DEBUG_OUTPUT_MASK & 2)
  this->terrain->setDebugDataVisible((scene::E_DEBUG_SCENE_TYPE)scene::EDS_BBOX_BUFFERS);
 #endif

}
Terrain::~Terrain(){
    delete this->terrain;
    
}
core::vector3df Terrain::getCenter(){

            return this->terrain->getTerrainCenter();
        }
void Terrain::collisionResponseAnimator(scene::ISceneManager* smgr,scene::ISceneNode* obj){

//        scene::ITriangleSelector* selector = 0;
//        selector = smgr->createTerrainTriangleSelector(this->terrain);
//        this->terrain->setTriangleSelector(selector);
//
//    if(selector){
//       const core::aabbox3d<f32>& box = this->terrain->getBoundingBox();
//        core::vector3df radius = box.MaxEdge - box.getCenter();
//        scene::ISceneNodeAnimator* anim = smgr->createCollisionResponseAnimator(
//            selector,obj,core::vector3df(radius.X,1.f,radius.Z), core::vector3df(0,-5.f,0));
//            selector->drop();
//            obj->addAnimator(anim);
//            anim->drop();
//    }


}
void Terrain::addCollisionNode(scene::ISceneManager* smgr,scene::IAnimatedMeshSceneNode* obj){
        scene::ITriangleSelector* selector = smgr->createTriangleSelector(obj);
        obj->setTriangleSelector(selector);
        selector->drop();

}
core::aabbox3df Terrain::getTerrainBox(){
    return this->terrain->getBoundingBox();
}
scene::ITerrainSceneNode* Terrain::getTerrain(){ return this->terrain; }
