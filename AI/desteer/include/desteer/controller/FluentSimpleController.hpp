#pragma once
#include "SimpleSteeringController.hpp"

namespace desteer { namespace controller{

class FluentSimpleController : private SimpleSteeringController
{
    public:
    FluentSimpleController();
    FluentSimpleController(entity::IMobileEntity *vehicle);
    irr::core::vector3df Calculate();

    FluentSimpleController * WithSeek();
    FluentSimpleController * WithSeek(irr::core::vector3df target);
    FluentSimpleController * WithArrive();
    FluentSimpleController * WithArrive(irr::core::vector3df target);
    FluentSimpleController * WithHide();
    FluentSimpleController * WithHide(entity::IMobileEntity *target);
    FluentSimpleController * WithPursuit();
    FluentSimpleController * WithPursuit(entity::IMobileEntity * target);
    FluentSimpleController * WithObstacles(EntityGroup &obstacles);
};

} //end ns desteer
} //end ns controller
