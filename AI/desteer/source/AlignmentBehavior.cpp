#include "desteer/behavior/AlignmentBehavior.hpp"
#include "desteer/entity/IMobileEntity.hpp"

using namespace irr;
using namespace core;
using namespace desteer;
using namespace entity;
using namespace behavior;

AlignmentBehavior::AlignmentBehavior(EntityGroup& neighbors, entity::IMobileEntity * mob)
{
    SetNeighbors(neighbors);
    SetMobile(mob);
}

void AlignmentBehavior::SetMobile(entity::IMobileEntity * mob)
{
    _mob = mob;
}
void AlignmentBehavior::SetNeighbors(EntityGroup& neighbors)
{
    _neighbors = neighbors;
}

irr::core::vector3df AlignmentBehavior::Calculate()
{
    vector3df AverageHeading;

    for(int a = 0; a < _neighbors.size(); ++a)
    {
        if((_neighbors[a]!=_mob))
        {
            IMobileEntity * neighbor = dynamic_cast<IMobileEntity*>(_neighbors[a]);
            if(neighbor){
                AverageHeading += neighbor->ForwardVector();
            }
        }
    }

    if(_neighbors.size() - 1 > 0)
    {
        AverageHeading /= (float)(_neighbors.size() - 1);
        AverageHeading -= _mob->ForwardVector();
    }
    return AverageHeading;
}

