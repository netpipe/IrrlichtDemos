#include "desteer/controller/ModularSteeringController.hpp"

using namespace desteer;
using namespace behavior;
using namespace controller;
using namespace entity;
using namespace irr;
using namespace core;

ModularSteeringController::ModularSteeringController(IMobileEntity* mob)
{
    _mob = mob;
}

vector3df ModularSteeringController::Calculate()
{
    vector3df steeringForce = vector3df(0,0,0);
    float maxForceSQ = _mob->MaxForce() * _mob->MaxForce();

    // TODO (Kaeles#1#): Allow user to set summing algorithm in modular controller
    for(BehaviorIterator it = _behaviors.begin(); it != _behaviors.end(); ++it)
    {
        if(steeringForce.getLengthSQ() < maxForceSQ)
        {
            steeringForce += (*it)->Calculate();
        }
        else
        {
            break;
        }
    }

    return steeringForce;

}

void ModularSteeringController::AddBehavior(ISteeringBehavior* behavior)
{
    _behaviors.push_back(behavior);
    behavior->SetMobile(_mob);
}
