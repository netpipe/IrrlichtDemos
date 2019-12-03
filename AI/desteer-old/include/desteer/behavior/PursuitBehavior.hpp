#pragma once
#include "desteer/behavior/ISteeringBehavior.hpp"
#include "desteer/behavior/SeekBehavior.hpp"

namespace desteer{
namespace behavior{

class PursuitBehavior : public ISteeringBehavior
{
    private:
    entity::IMobileEntity * _mob;
    entity::IMobileEntity * _target;

    SeekBehavior * _seekBehavior;

public:
    PursuitBehavior(entity::IMobileEntity* target);
    irr::core::vector3df Calculate();
    void SetMobile(entity::IMobileEntity * mob);
    void SetTarget(entity::IMobileEntity * target);
};


} //end ns behavior
} //end ns desteer
