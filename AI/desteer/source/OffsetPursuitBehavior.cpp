#include "desteer/behavior/OffsetPursuitBehavior.hpp"

namespace desteer{
namespace behavior{

OffsetPursuitBehavior::OffsetPursuitBehavior(entity::IMobileEntity* target, irr::core::vector3df offset)
{
    _target = target;
    _offset = offset;

    _pursuitBehavior = new PursuitBehavior(_target);
    _pursuitBehavior->SetMobile(_mob);
}

irr::core::vector3df OffsetPursuitBehavior::Calculate()
{
    return _pursuitBehavior->Calculate();
}

void OffsetPursuitBehavior::SetMobile(entity::IMobileEntity * mob)
{
    _mob = mob;
}

void OffsetPursuitBehavior::SetTarget(entity::IMobileEntity * target)
{
    _target = target;
}


} //end ns behavior
} //end ns desteer


