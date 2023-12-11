#include "desteer/behavior/PursuitBehavior.hpp"

using namespace desteer;
using namespace behavior;
using namespace entity;

using namespace irr;
using namespace core;


PursuitBehavior::PursuitBehavior(IMobileEntity * target, vector3df offset)
{
    _offset = offset;
    _target = target;
    _arriveBehavior = new ArriveBehavior();
}

vector3df PursuitBehavior::Calculate()
{
    vector3df offsetTarget = _target->Position() + (_target->Position() - _target->transformLocalVectToWorld(_offset));
    vector3df toTarget = (offsetTarget - _mob->Position()) ;
    float heading = _mob->ForwardVector().dotProduct(_target->ForwardVector());

    //if target forward and mob forward are within 18 degs, just seek.
    if((toTarget.dotProduct( _mob->ForwardVector()) > 0) && (heading < 0.95)) //acos(0.95) = 18 degs
    {
        _arriveBehavior->SetTarget(offsetTarget);
        return _arriveBehavior->Calculate();
    }

    float lookAheadTime = toTarget.getLength() / ( _mob->MaxSpeed() + _target->Velocity().getLength() );

    _arriveBehavior->SetTarget(offsetTarget + (_target->Velocity() * lookAheadTime));
    return _arriveBehavior->Calculate();
}

void PursuitBehavior::SetMobile(entity::IMobileEntity * mob)
{
    _mob = mob;

    _arriveBehavior->SetMobile(_mob);
}

void PursuitBehavior::SetTarget(entity::IMobileEntity *target)
{
    _target = target;
}

void PursuitBehavior::SetOffset(vector3df offset)
{
    _offset = offset;
}
