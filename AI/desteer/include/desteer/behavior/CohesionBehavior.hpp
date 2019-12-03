
#pragma once
#include "ISteeringBehavior.hpp"
#include "ArriveBehavior.hpp"
#include "desteer/Types.hpp"

namespace desteer{
namespace behavior{

class CohesionBehavior : public ISteeringBehavior
{
private:
    entity::IMobileEntity* _mob;
    EntityGroup _neighbors;
    ArriveBehavior* _arriveBehavior;

public:
    CohesionBehavior( EntityGroup& neighbors, entity::IMobileEntity * mob = NULL);

    irr::core::vector3df Calculate();
    void SetMobile(entity::IMobileEntity * mob);
    void SetNeighbors(EntityGroup& neighbors);
};

} //end ns behavior
} //end ns desteer

