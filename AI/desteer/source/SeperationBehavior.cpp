#include "desteer/behavior/SeperationBehavior.hpp"
#include "desteer/entity/IMobileEntity.hpp"

using namespace irr;
using namespace core;
using namespace desteer;
using namespace entity;
using namespace behavior;

SeperationBehavior::SeperationBehavior(EntityGroup& neighbors, entity::IMobileEntity * mob)
{
    SetNeighbors(neighbors);
    SetMobile(mob);
}

void SeperationBehavior::SetMobile(entity::IMobileEntity * mob)
{
    _mob = mob;
}
void SeperationBehavior::SetNeighbors(EntityGroup& neighbors)
{
    _neighbors = neighbors;
}

irr::core::vector3df SeperationBehavior::Calculate()
{
    if(_mob->Velocity().getLength() <= 1) return vector3df(0,0,0);
    vector3df SteeringForce = vector3df(0,0,0);

    for(int i = 0; i < _neighbors.size(); ++i)
    {
        if((_neighbors[i] != _mob))
        {
            vector3df ToNeighbor = _mob->Position() - _neighbors[i]->Position();
            float distanceToNeighbor = ToNeighbor.getLength();
            SteeringForce += ToNeighbor.normalize() / (distanceToNeighbor);
        }
    }

    return SteeringForce;
}

