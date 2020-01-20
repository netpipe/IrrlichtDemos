#include "Logger.h"
#include "target.h"

using namespace std;
using namespace irr;

Target::Target(core::vector3df position, scene::ISceneManager* smgr, video::IVideoDriver* driver, Physics* physics){
    this->smgr = smgr;
    this->physics = physics;
    // scene::IMesh* mesh = smgr->getMesh("media/chest/chest.obj");
    core::vector3df scale = core::vector3df(1,1,1);
    this->irrTarget = smgr->addCubeSceneNode(1,0,-1,position, scale);
    //this->irrTarget = smgr->addMeshSceneNode(mesh,0,-1,position, scale);
    this->irrTarget->setScale(scale);
    this->irrTarget->setMaterialFlag(video::EMF_LIGHTING, false);
    this->irrTarget->setMaterialTexture(0, driver->getTexture("media/chest/chest_rare.jpg"));

    this->irrTarget->setDebugDataVisible(scene::EDS_BBOX);
    this->btTarget = physics->createTreasure(this->irrTarget,scale, position);
    logVector(1,"Created treasure at", position);
}
scene::IMeshSceneNode* Target::getIrrNode(){
    return this->irrTarget;
}
