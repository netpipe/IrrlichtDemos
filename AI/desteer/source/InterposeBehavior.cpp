#include "desteer/behavior/InterposeBehavior.hpp"

using namespace desteer;
using namespace behavior;
using namespace entity;
using namespace irr;
using namespace core;

vector3df InterposeBehavior::Calculate()
{
    vector3df midPoint = (_vehicle1->Position() + _vehicle1->Position()) / 2.0;

    double timeToReachMidPoint = _mob->Position().getDistanceFrom(midPoint) / _mob->MaxSpeed();

    vector3df v1Pos = _vehicle1->Position() + _vehicle1->Velocity() * timeToReachMidPoint;
    vector3df v2Pos = _vehicle2->Position() + _vehicle2->Velocity() * timeToReachMidPoint;

    midPoint = (v1Pos + v2Pos) / 2.0;

    //return Arrive(midPoint, fast);
    return vector3df(0,0,0);
}

void InterposeBehavior::SetMobile(entity::IMobileEntity * mob)
{
    _mob = mob;
}
