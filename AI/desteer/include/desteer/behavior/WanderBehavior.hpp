#pragma once
#include "ISteeringBehavior.hpp"

namespace desteer{
namespace behavior{

class WanderBehavior : public ISteeringBehavior
{
private:
    entity::IMobileEntity * _mob;

    float _wanderRadius;
    float _wanderDistance;
    float _wanderJitter;
    irr::core::vector3df _wanderTarget;

public:
    WanderBehavior(float wanderRadius = 10, float wanderDistance = 20, float wanderJitter = 10 );
    irr::core::vector3df Calculate();
    void SetMobile(entity::IMobileEntity * mob);
};

} //end ns behavior
} //end ns desteer

