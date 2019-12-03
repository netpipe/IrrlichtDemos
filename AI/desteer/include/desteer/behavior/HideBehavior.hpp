#pragma once
#include "ISteeringBehavior.hpp"
#include "desteer/Types.hpp"
#include "desteer/behavior/EvadeBehavior.hpp"
#include "desteer/behavior/ArriveBehavior.hpp"

namespace desteer{
namespace behavior{

class HideBehavior : public ISteeringBehavior
{
private:
    entity::IMobileEntity * _mob;
    entity::IMobileEntity * _target;

    float _hideDistanceFromObj;

    EntityGroup _obstacles;

    EvadeBehavior * _evadeBehavior;
    ArriveBehavior  * _arriveBehavior;

    irr::core::vector3df GetHidingPosition(const irr::core::vector3df& targetPos, const entity::IBaseEntity *obstacle);
public:
    HideBehavior(entity::IMobileEntity* target, EntityGroup & obstacles, float hideDistanceFromObstacle = 20);
    irr::core::vector3df Calculate();
    void SetMobile(entity::IMobileEntity * mob);
    void SetTarget(entity::IMobileEntity * target);
    void SetObstacles(EntityGroup &obstacles);
};

} //end ns behavior
} //end ns desteer

