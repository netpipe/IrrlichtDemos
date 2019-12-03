#include "desteer/behavior/FleeBehavior.hpp"

using namespace desteer;
using namespace behavior;
using namespace entity;
using namespace irr;
using namespace core;

FleeBehavior::FleeBehavior(irr::core::vector3df target, float fleeDistance)
{
    _target = target;
    _fleeDistance = fleeDistance;
}

void FleeBehavior::SetMobile(IMobileEntity * mob)
{
    _mob = mob;
}

void FleeBehavior::SetTarget(vector3df target)
{
    _target = target;
}

vector3df FleeBehavior::Calculate()
{
    //if fleedistance = -1, flee always.  -1 means infinite distance;
    if(_fleeDistance == -1 || _target.getDistanceFromSQ(_mob->Position()) < _fleeDistance * _fleeDistance)
    {
        vector3df desiredVelocity = ( _mob->Position() - _target).normalize() * _mob->MaxSpeed();
        return (desiredVelocity - _mob->Velocity());
    }
    return vector3df(0,0,0);
}

