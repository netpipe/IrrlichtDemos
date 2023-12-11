#include "motionstate.h"

IMotionState::IMotionState(const btTransform &initialPos)
{
    worldTransform = initialPos;
}

void IMotionState::getWorldTransform(btTransform &worldTrans) const
{
    /*if(object->hasParent == true)
    {
        btTransform tempTrans = object->body->getWorldTransform();

        btVector3 pos = object->getParent()->body->getWorldTransform().getOrigin() - object->body->getWorldTransform().getOrigin();

        tempTrans.setOrigin(pos);

        worldTrans = tempTrans;
    }*/



    worldTrans = worldTransform;
}

void IMotionState::setWorldTransform(const btTransform &worldTrans)
{
    if(node != 0)
    {
        /*if(object->hasParent == true)
        {
            btTransform tempTrans = worldTrans;

            btVector3 pos = object->getParent()->body->getWorldTransform().getOrigin() + object->body->getWorldTransform().getOrigin();

            tempTrans.setOrigin(pos);

            applyTransformToVisualModel(tempTrans);

            worldTransform = tempTrans;

            //printf(object->worldObjectName.c_str());
        }*/

        applyTransformToVisualModel(worldTrans);

        worldTransform = worldTrans;
    }

    else
    {
        if(failed == false)
        {
            std::string str = "Object (";
            str += node->getName();
            str += ") could not be updated.";
//this does not work on linux            MessageBox(0,str.c_str(),"Critical Error",MB_OK | MB_ICONERROR);
            failed = true;
        }
    }
}

void IMotionState::applyTransformToVisualModel(const btTransform &transform)
{
    // Set position
    btVector3 Point = transform.getOrigin();
    node->setPosition(vector3df((f64)Point[0], (f64)Point[1], (f64)Point[2]));

    node->updateAbsolutePosition();

    // Set rotation
    btVector3 EulerRotation;
    QuaternionToEuler(transform.getRotation(), EulerRotation);
    node->setRotation(vector3df(EulerRotation[0], EulerRotation[1], EulerRotation[2]));

    node->updateAbsolutePosition();
}


IMotionState::~IMotionState()
{
}

