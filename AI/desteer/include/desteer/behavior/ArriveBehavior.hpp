#pragma once
#include "ISteeringBehavior.hpp"

namespace desteer{
namespace behavior{

class ArriveBehavior : public ISteeringBehavior
{
private:
    entity::IMobileEntity * _mob;

    irr::core::vector3df _target;
    float _arriveTolerance;
    float _decceleration;

public:
    ArriveBehavior(
                   irr::core::vector3df target  = irr::core::vector3df(0,0,0),
                   entity::IMobileEntity * mob  = NULL,
                   float arriveTolerance        = 10,
                   float decceleration          = 2.0
                  );

    irr::core::vector3df Calculate();
    void SetMobile(entity::IMobileEntity * mob);
    void SetTarget(irr::core::vector3df target);
};

} //end ns behavior
} //end ns desteer
