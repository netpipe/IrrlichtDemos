#include "desteer/behavior/SeekBehavior.hpp"

using namespace desteer;
using namespace behavior;
using namespace entity;
using namespace irr;
using namespace core;

SeekBehavior::SeekBehavior(vector3df target, IMobileEntity * mob)
{
    _target = target;
    _mob = mob;
}

void SeekBehavior::SetTarget(vector3df target)
{
    _target = target;
}

void SeekBehavior::SetMobile(IMobileEntity * mob)
{
    _mob = mob;
}

vector3df SeekBehavior::Calculate()
{
    vector3df desiredVelocity = (_target - _mob->Position()).normalize() * _mob->MaxSpeed();
    return (desiredVelocity - _mob->Velocity());
}
