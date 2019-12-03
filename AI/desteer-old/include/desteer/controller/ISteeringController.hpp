#pragma once
#include "irrvector/vector3d.h"
#include "desteer/Types.hpp"
#include "desteer/entity/IMobileEntity.hpp"

namespace desteer{
namespace controller{

//! ISteeringController
/*! The base of all steering controllers, you should implement your own way of accessing behaviors through this interface
    or use the prebuilt SimpleSteeringController or ModularSteeringController, which both should cover many needs.
*/
class ISteeringController
{
    public:
    /*!
        Returns the calculated sum of all the behaviors this controller is using.
    */
    virtual irr::core::vector3df Calculate() = 0;
};

} //end ns controller
} //end ns desteer
