#include "desteer/behavior/WanderBehavior.hpp"

using namespace desteer;
using namespace behavior;

using namespace irr;
using namespace core;

f64 RandomClamped()
{
    return ((rand()%10000) - 5000) * .0001;
}

WanderBehavior::WanderBehavior(float wanderRadius , float wanderDistance , float wanderJitter)
{
    _wanderRadius = wanderRadius;
    _wanderDistance = wanderDistance;
    _wanderJitter = wanderJitter;
}

vector3df WanderBehavior::Calculate()
{
    //_wanderJitter *= RandomClamped();

    vector3df _wanderTarget = vector3df(RandomClamped() * _wanderJitter,0,RandomClamped() * _wanderJitter);
    (_wanderTarget.normalize()) *= _wanderRadius;

    return _mob->transformLocalVectToWorld(_wanderTarget + vector3df(0,0,_wanderDistance)) - _mob->Position();
}

void WanderBehavior::SetMobile(entity::IMobileEntity * mob)
{
    _mob = mob;
}

