#pragma once
#include "ISteeringBehavior.hpp"

namespace desteer{
namespace behavior{

class FleeBehavior : public ISteeringBehavior
{
    private:
        irr::core::vector3df _target;
        entity::IMobileEntity * _mob;

        float _fleeDistance;
    public:
        FleeBehavior(irr::core::vector3df target, float fleeDistance = -1);
        void SetMobile(entity::IMobileEntity * mob);
        void SetTarget(irr::core::vector3df target);
        irr::core::vector3df Calculate();
};

} //end ns behavior
} //end ns desteer

