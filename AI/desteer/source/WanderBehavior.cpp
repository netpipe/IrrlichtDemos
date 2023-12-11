#include "desteer/behavior/WanderBehavior.hpp"

using namespace desteer;
using namespace behavior;

using namespace irr;
using namespace core;

f64 RandomClamped()
{
    //random number from 1 to -1
    return ((rand() % 100) - 50) * .02;
}

WanderBehavior::WanderBehavior(float wanderRadius, float wanderDistance, float wanderJitter)
{
    _wanderRadius = wanderRadius; // default 10
    _wanderDistance = wanderDistance; //default 20
    _wanderJitter = wanderJitter; //default 10
    _wanderTarget = vector3df(0,0,0);
}

vector3df WanderBehavior::Calculate()
{
    //get random point on unit circle scaled by amount of change allowed per update
    _wanderTarget += vector3df(RandomClamped(), 0, RandomClamped());

    //Scale up to radius for smoothing out changes (i.e. bigger circle = wider turns)
    vector3df scaledTarget = _wanderTarget.normalize() * _wanderRadius;

    //Change local vector back to world vector for desired velocity
    return _mob->transformLocalVectToWorld(scaledTarget + _wanderDistance) - _mob->Position();
}

void WanderBehavior::SetMobile(entity::IMobileEntity * mob)
{
    _mob = mob;
}

