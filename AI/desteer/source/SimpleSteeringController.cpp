#include "desteer/controller/SimpleSteeringController.hpp"

#include "desteer/Types.hpp"

using namespace irr;
using namespace core;
using namespace desteer;
using namespace behavior;
using namespace controller;
using namespace entity;

SimpleSteeringController::SimpleSteeringController(IMobileEntity* mob)
{
    _mob = mob;
    _arriveTarget   = vector3df(0,0,0);
    _seekTarget     = vector3df(0,0,0);
    _fleeTarget     = vector3df(0,0,0);

    _evadeTarget    = NULL;
    _hideTarget     = NULL;
    _pursuitTarget   = NULL;

    _behaviorFlags  = 0;

    _seekBehavior       = new SeekBehavior(_seekTarget,mob);
    _arriveBehavior     = new ArriveBehavior(_seekTarget,mob,.08);
    _fleeBehavior       = new FleeBehavior(_fleeTarget);
    _hideBehavior       = new HideBehavior(_hideTarget,_obstacles);
    _wanderBehavior     = new WanderBehavior();
    _evadeBehavior      = new EvadeBehavior(_evadeTarget);
    _pursuitBehavior    = new PursuitBehavior(_pursuitTarget);
    _obsAvoidBehavior   = new ObstacleAvoidanceBehavior(_obstacles);


    _seekBehavior->SetMobile(_mob);
    _arriveBehavior->SetMobile(_mob);
    _fleeBehavior->SetMobile(_mob);
    _hideBehavior->SetMobile(_mob);
    _wanderBehavior->SetMobile(_mob);
    _evadeBehavior->SetMobile(_mob);
    _pursuitBehavior->SetMobile(_mob);
    _obsAvoidBehavior->SetMobile(_mob);

    _mob->SetSteering(this);
}

irr::core::vector3df SimpleSteeringController::Calculate()
{
    irr::core::vector3df steeringForce = vector3df(0,0,0);
    float maxForceSQ = (_mob->MaxForce() * _mob->MaxForce());

    if(_behaviorFlags & EBF_AVOID)
    {
        _obsAvoidBehavior->SetObstacles(_obstacles);
        steeringForce += _obsAvoidBehavior->Calculate();
        if(steeringForce.getLengthSQ() > maxForceSQ)
        {
            steeringForce.setLength(_mob->MaxForce());
            return steeringForce;
        }
    }

    if(_behaviorFlags & EBF_HIDE)
    {
        _hideBehavior->SetTarget(_hideTarget);
        _hideBehavior->SetObstacles(_obstacles);
        steeringForce += _hideBehavior->Calculate();
        if(steeringForce.getLengthSQ() > maxForceSQ)
        {
            steeringForce.setLength(_mob->MaxForce());
            return steeringForce;
        }
    }

    if(_behaviorFlags & EBF_PURSUIT)
    {
        _pursuitBehavior->SetTarget(_pursuitTarget);
        steeringForce += _pursuitBehavior->Calculate();
        if(steeringForce.getLengthSQ() > maxForceSQ)
        {
            steeringForce.setLength(_mob->MaxForce());
            return steeringForce;
        }
    }

    if(_behaviorFlags & EBF_EVADE)
    {
        _evadeBehavior->SetTarget(_evadeTarget);
        steeringForce += _evadeBehavior->Calculate();
        if(steeringForce.getLengthSQ() > maxForceSQ)
        {
            steeringForce.setLength(_mob->MaxForce());
            return steeringForce;
        }
    }

    if(_behaviorFlags & EBF_SEEK)
    {
        _seekBehavior->SetTarget(_seekTarget);
        steeringForce += _seekBehavior->Calculate();
        if(steeringForce.getLengthSQ() > maxForceSQ)
        {
            steeringForce.setLength(_mob->MaxForce());
            return steeringForce;
        }
    }

    if(_behaviorFlags & EBF_ARRIVE)
    {
        _arriveBehavior->SetTarget(_arriveTarget);
        steeringForce += _arriveBehavior->Calculate();
        if(steeringForce.getLengthSQ() > maxForceSQ)
        {
            steeringForce.setLength(_mob->MaxForce());
            return steeringForce;
        }
    }

    if(_behaviorFlags & EBF_WANDER)
    {
        steeringForce += _wanderBehavior->Calculate();
        if(steeringForce.getLengthSQ() > maxForceSQ)
        {
            steeringForce.setLength(_mob->MaxForce());
            return steeringForce;
        }
    }

    return steeringForce;
};

void SimpleSteeringController::SetSeekTarget(vector3df target)
{
    _seekTarget = target;
}

void SimpleSteeringController::SetArriveTarget(vector3df target)
{
    _arriveTarget = target;
}

void SimpleSteeringController::SetHideTarget(entity::IMobileEntity *target)
{
    _hideTarget = target;
}

void SimpleSteeringController::SetPursuitTarget(entity::IMobileEntity *target)
{
    _pursuitTarget = target;
}

void SimpleSteeringController::SetObstacles(EntityGroup &obstacles)
{
    _obstacles = obstacles;
}

void SimpleSteeringController::SetBehaviorFlag(EBEHAVIOR_FLAG flag, bool active)
{
    _behaviorFlags = active ? _behaviorFlags | flag :  _behaviorFlags ^ flag;
}
