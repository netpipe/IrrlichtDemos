#include "ball.h"
#include <iostream>
using namespace irr;

Ball::Ball(IrrlichtDevice* device, scene::ISceneManager* smgr,  video::IVideoDriver* driver, Physics* physics, core::vector3df position){
    this->irrBall = smgr->addSphereSceneNode(this->radius,32,0,-1,position);
    this->irrBall->setMaterialFlag(video::EMF_LIGHTING, false);
    this->irrBall->setMaterialTexture(0,driver->getTexture("media/cannon/cannonballtex.png"));
    this->btBall = physics->createCannonBall(this->irrBall,position, radius);
    this->btBall->activate(true);
    #if (DEBUG_OUTPUT_MASK & 2)
      this->irrBall->setDebugDataVisible((scene::E_DEBUG_SCENE_TYPE)scene::EDS_BBOX_BUFFERS);
    #endif
    this->device = device;
    this->smgr = smgr;
}
Ball::~Ball(){

}
//}
void Ball::setCamera(scene::ICameraSceneNode* camera, core::vector3df rotation){
    assert(camera != 0);
    this->parentCamera = camera;
   // camera->setPosition(this->irrBall->getAbsolutePosition());

    core::vector3df offset = this->cameraStartPosition = camera->getAbsolutePosition();
    this->camera = new Camera(offset,rotation,this->smgr,0);
    this->camera->camera->setTarget(rotation);
    this->camera->camera->setRotation(rotation);
    this->camera->camera->bindTargetAndRotation(true);
//    this->camera = new Camera(camera,this->smgr);

//    this->points.push_front(camera->getAbsolutePosition());

}
bool Ball::moveCamera(){
    assert(this->camera != 0);
    if(!this->camera){
        return false;
    }

    if(toIrrlichtVector(this->btBall->getLinearVelocity()) == core::vector3df(0,0,0) && this->camera){
        this->deleteCamera();
        return false;
    }
    int sig = sign(cos(this->parentCamera->getRotation().Y*core::DEGTORAD64));

    assert(this->camera->camera != 0);
    core::vector3df diff = core::vector3df(
            this->irrBall->getAbsolutePosition() - core::vector3df(0,-0.4f,sig*CAMERA_OFFSET_BALL));
    this->camera->camera->setPosition(diff);
    this->camera->camera->setTarget(this->irrBall->getAbsolutePosition());
    this->irrBall->updateAbsolutePosition();
    this->camera->camera->updateAbsolutePosition();
//    this->points.push_front(this->camera->camera->getAbsolutePosition());
    return true;

}
void Ball::deleteCamera(){
    if(this->camera){

//    core::array<core::vector3df> p;
//
//    core::vector3df position = this->camera->camera->getAbsolutePosition();
//
//    p.push_front(position);
//
//    for(int i = 0; i < 35; i++){
//        position+=core::vector3df(0,0.314f,0);
//        p.push_back(position);
//    }
//
//    scene::ISceneNodeAnimator* animator = this->smgr->createFollowSplineAnimator(this->device->getTimer()->getTime(), p,20.f,false);
//    this->camera->camera->addAnimator(animator);
//    animator->drop();
    delete this->camera;
    this->camera = 0;
    assert(this->parentCamera != 0);
    this->smgr->setActiveCamera(this->parentCamera);
    }
}
