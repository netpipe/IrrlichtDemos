#pragma once
#include "ISteeringBehavior.hpp"
#include "desteer/Types.hpp"

namespace desteer{
namespace behavior{

class ObstacleAvoidanceBehavior : public ISteeringBehavior
{
private:
    entity::IMobileEntity * _mob;

    EntityGroup _obstacles;

public:
    ObstacleAvoidanceBehavior(EntityGroup & obstacles);
    irr::core::vector3df Calculate();
    void SetMobile(entity::IMobileEntity * mob);
    void SetObstacles(EntityGroup &obstacles);
};

} //end ns behavior
} //end ns desteer
