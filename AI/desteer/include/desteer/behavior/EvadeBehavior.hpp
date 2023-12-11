#pragma once
#include "desteer/behavior/ISteeringBehavior.hpp"
#include "desteer/behavior/FleeBehavior.hpp"

namespace desteer{
namespace behavior{

class EvadeBehavior : public ISteeringBehavior
{
    private:
    entity::IMobileEntity * _mob;
    entity::IMobileEntity * _target;

    FleeBehavior * _fleeBehavior;

public:
    EvadeBehavior(entity::IMobileEntity* target);
    irr::core::vector3df Calculate();
    void SetMobile(entity::IMobileEntity * mob);
    void SetTarget(entity::IMobileEntity * target);
};


} //end ns behavior
} //end ns desteer
