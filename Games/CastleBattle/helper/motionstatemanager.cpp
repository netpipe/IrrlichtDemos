#include "motionstatemanager.h"
#include "bullethelper.h"
#include <iostream>
using namespace irr;
using namespace std;
MotionStateManager::MotionStateManager(btTransform &pos, scene::ISceneNode* node){
    this->initialPosition = pos;
    this->node = node;
}
void MotionStateManager::setWorldTransform(const btTransform &worldTrans){
            if(this->node == nullptr) return;
            btQuaternion rotation = worldTrans.getRotation();
            btVector3 position = worldTrans.getOrigin();
            this->node->setPosition(toIrrlichtVector(position));
            this->node->updateAbsolutePosition();
            this->node->setRotation(QuaternionToEuler(rotation));
            this->initialPosition = worldTrans;

}
void MotionStateManager::getWorldTransform(btTransform &worldTrans) const{
    worldTrans = initialPosition;
}
