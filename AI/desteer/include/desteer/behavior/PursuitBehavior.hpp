#pragma once
#include "desteer/behavior/ISteeringBehavior.hpp"
#include "desteer/behavior/ArriveBehavior.hpp"

namespace desteer{
namespace behavior{

class PursuitBehavior : public ISteeringBehavior
{
    private:
    entity::IMobileEntity * _mob;
    entity::IMobileEntity * _target;
    irr::core::vector3df _offset;
    ArriveBehavior * _arriveBehavior;

public:
    PursuitBehavior(entity::IMobileEntity* target, irr::core::vector3df offset = irr::core::vector3df(0,0,0));
    irr::core::vector3df Calculate();
    void SetMobile(entity::IMobileEntity * mob);
    void SetTarget(entity::IMobileEntity * target);
    void SetOffset(irr::core::vector3df offset);
};


} //end ns behavior
} //end ns desteer
