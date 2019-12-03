#include "ISteeringBehavior.hpp"

namespace desteer{ namespace behavior{

class InterposeBehavior : ISteeringBehavior
{
private:
    entity::IMobileEntity *_vehicle1;
    entity::IMobileEntity *_vehicle2;

    entity::IMobileEntity *_mob;
public:
    InterposeBehavior(entity::IMobileEntity *mob, entity::IMobileEntity *vehicle1, entity::IMobileEntity *vehicle2)
    : _vehicle1(vehicle1), _vehicle2(vehicle2), _mob(mob){}

    virtual irr::core::vector3df Calculate();
    virtual void SetMobile(entity::IMobileEntity * mob);

};

}   //end namespace behavior
}   //end namespace desteer
