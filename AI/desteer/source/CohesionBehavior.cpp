#include "desteer/behavior/CohesionBehavior.hpp"
#include "desteer/entity/IMobileEntity.hpp"

using namespace irr;
using namespace core;
using namespace desteer;
using namespace entity;
using namespace behavior;

CohesionBehavior::CohesionBehavior(EntityGroup& neighbors, entity::IMobileEntity * mob)
{
    _arriveBehavior = new ArriveBehavior();
    SetNeighbors(neighbors);
    SetMobile(mob);
}

void CohesionBehavior::SetMobile(entity::IMobileEntity * mob)
{
    _mob = mob;
    _arriveBehavior->SetMobile(mob);
}
void CohesionBehavior::SetNeighbors(EntityGroup& neighbors)
{
    _neighbors = neighbors;
}

vector3df CohesionBehavior::Calculate()
{
    vector3df CenterOfMass, SteeringForce;
    if(_mob->Velocity().getLength() <= .001) return vector3df(0,0,0);

    for(int i = 0;i < _neighbors.size(); ++i)
    {
        if((_neighbors[i] != _mob))
        {
            CenterOfMass += _neighbors[i]->Position();
        }
    }
    if(_neighbors.size() - 1 > 0)
    {
        _arriveBehavior->SetTarget(CenterOfMass / (float)(_neighbors.size() - 1));
        SteeringForce = _arriveBehavior->Calculate();
    }
    return SteeringForce;
}

