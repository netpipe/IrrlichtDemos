#include "IrrlichtMobileEntity.hpp"

using namespace irr;
using namespace core;

using namespace desteer;
using namespace entity;
using namespace behavior;

IrrlichtMobileEntity::IrrlichtMobileEntity(
    irr::scene::ISceneNode* node, irr::core::vector3df position, float mass, float maxSpeed, float maxForce
    )
{
    _node = node;
    SetPosition(position);
    SetMass(mass);
    SetMaxSpeed(maxSpeed);
    SetMaxForce(maxForce);
};

vector3df IrrlichtMobileEntity::Position() const{
    return _node->getPosition();
}

void IrrlichtMobileEntity::SetPosition(vector3df position)
{
    _node->setPosition(position);
}

float IrrlichtMobileEntity::Radius() const{
    return (_node->getBoundingBox().getExtent().getLength() / 2);
}

vector3df IrrlichtMobileEntity::ForwardVector() const
{
	return _node->getRelativeTransformation().getRotationDegrees().rotationToDirection(vector3df(0,0,1));
}

void IrrlichtMobileEntity::SetForwardVector(irr::core::vector3df forward)
{
     _node->setRotation(forward.getHorizontalAngle());

}

vector3df IrrlichtMobileEntity::SideVector() const
{
	return _node->getRelativeTransformation().getRotationDegrees().rotationToDirection(vector3df(1,0,0));
}

void IrrlichtMobileEntity::SetSideVector(irr::core::vector3df side)
{
    return;
}

vector3df IrrlichtMobileEntity::transformWorldVectToLocal(irr::core::vector3df vec)
{
    vector3df offset = vec - Position();

    vector3df up = _node->getRelativeTransformation().getRotationDegrees().rotationToDirection(vector3df(0,1,0));

    return vector3df(offset.dotProduct(SideVector()),offset.dotProduct(up),offset.dotProduct(ForwardVector()));

}

vector3df IrrlichtMobileEntity::transformLocalVectToWorld(irr::core::vector3df vec)
{
    vector3df returnVec = vec;
    _node->getRelativeTransformation().transformVect(returnVec);
    return returnVec;
}

void IrrlichtMobileEntity::Update(float timeElapsed)
{

    //_steering->Calculate() = steering force
    //divided by mass = acceleration
    //times time = velocity

    _velocity += (_steering->Calculate()/_mass) * timeElapsed;

    //truncate velocity to max speed
    _velocity.setLength(irr::core::min_(_velocity.getLength(),_maxSpeed));

    _node->setPosition(_node->getPosition() + (_velocity * timeElapsed));

    if(_velocity.getLengthSQ() > ROUNDING_ERROR_f32)
    {
       vector3df forward = _velocity;
       SetForwardVector(forward.normalize());
    }

}
