#pragma once
#include "ISteeringBehavior.hpp"

namespace desteer{
namespace behavior{

class SeekBehavior : public ISteeringBehavior
{
    private:
        irr::core::vector3df _target;
        entity::IMobileEntity * _mob;
    public:
        SeekBehavior(entity::IMobileEntity * mob = NULL,irr::core::vector3df target = irr::core::vector3df(0,0,0));
        void SetMobile(entity::IMobileEntity * mob);
        void SetTarget(irr::core::vector3df target);
        void SetTarget(entity::IMobileEntity * target) {return;};
        irr::core::vector3df Calculate();
};

} //end ns behavior
} //end ns desteer
