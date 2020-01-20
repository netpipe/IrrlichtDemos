#include <iostream>
#include <cassert>
#include "camera.h"

using namespace irr;
Camera::Camera(scene::ICameraSceneNode* camera, scene::ISceneManager* smgr){
    this->camera = camera;
    this->smgr = smgr;
}
Camera::Camera(scene::ICameraSceneNode* camera, scene::ISceneNode* parent, scene::ISceneManager* smgr){
    this->smgr = smgr;
    this->camera = smgr->addCameraSceneNode(parent, camera->getAbsolutePosition());
    this->camera->bindTargetAndRotation(true);


}
Camera::Camera(core::vector3df offset, core::vector3df rotation, scene::ISceneManager* smgr, scene::ISceneNode* parent){
this->camera = smgr->addCameraSceneNode(parent, offset,rotation);
this->camera->bindTargetAndRotation(true);
this->camera->setRotation(rotation);
this->camera->bindTargetAndRotation(true);
this->camera->setFarValue(3000.f);
}
void Camera::activeCamera(){
    assert(this->camera);
    this->smgr->setActiveCamera(this->camera);
}
Camera::~Camera(){
    this->camera->remove();
}
scene::ICameraSceneNode* Camera::getCamera(){
    return this->camera;
}
