#pragma once
#include "desteer/entity/IMobileEntity.hpp"

namespace desteer{
namespace behavior{

//! ISteeringBehavior
/*! The base of all steering behaviors. These are used in conjunction with #ISteeringController.
*/
class ISteeringBehavior
{
    public:
        /*!
            Returns the calculated desired steering force to be applied to the current velocity of the entity.
            This can also be added to other steering forces returned by other behaviors in order to combine them,
            the steering controller itself will usually handle this though.
        */
        virtual irr::core::vector3df Calculate() = 0;

        /*!
            Sets the current entity this steering behavior is using to determine its calculations.
        */
        virtual void SetMobile(entity::IMobileEntity * mob) = 0;

};//end class ISteeringBehavior

} //end ns behavior
} //end ns desteer
