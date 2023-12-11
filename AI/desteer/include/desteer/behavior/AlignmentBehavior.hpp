#pragma once
#include "ISteeringBehavior.hpp"
#include "desteer/Types.hpp"

namespace desteer{
namespace behavior{

class AlignmentBehavior : public ISteeringBehavior
{
private:
    entity::IMobileEntity* _mob;
    EntityGroup _neighbors;

public:
    AlignmentBehavior( EntityGroup& neighbors, entity::IMobileEntity * mob = NULL);

    irr::core::vector3df Calculate();
    void SetMobile(entity::IMobileEntity * mob);
    void SetNeighbors(EntityGroup& neighbors);
};

} //end ns behavior
} //end ns desteer

