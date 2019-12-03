#pragma once
#include "desteer/behavior/ISteeringBehavior.hpp"
#include "desteer/behavior/PursuitBehavior.hpp"

namespace desteer{
namespace behavior{

class OffsetPursuitBehavior : public ISteeringBehavior
{
    private:
    entity::IMobileEntity * _mob;
    entity::IMobileEntity * _target;
    irr::core::vector3df _offset;

    PursuitBehavior * _pursuitBehavior;

public:
    OffsetPursuitBehavior(entity::IMobileEntity* target,irr::core::vector3df offset = vector3df(0,0,0));
    irr::core::vector3df Calculate();
    void SetMobile(entity::IMobileEntity * mob);
    void SetTarget(entity::IMobileEntity * target);
};


} //end ns behavior
} //end ns desteer

