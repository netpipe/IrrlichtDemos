#pragma once
#include "desteer/controller/ISteeringController.hpp"

//Include all basic behaviors.
#include "desteer/behavior/SeekBehavior.hpp"
#include "desteer/behavior/FleeBehavior.hpp"
#include "desteer/behavior/WanderBehavior.hpp"
#include "desteer/behavior/PursuitBehavior.hpp"
#include "desteer/behavior/EvadeBehavior.hpp"
#include "desteer/behavior/ArriveBehavior.hpp"
#include "desteer/behavior/HideBehavior.hpp"
#include "desteer/behavior/ObstacleAvoidanceBehavior.hpp"

namespace desteer{
namespace controller{

enum EBEHAVIOR_FLAG
{
    EBF_ARRIVE  = 0x01,
    EBF_EVADE   = 0x02,
    EBF_FLEE    = 0x04,
    EBF_HIDE    = 0x08,
    EBF_SEEK    = 0x10,
    EBF_PURSUIT = 0x20,
    EBF_WANDER  = 0x40,
    EBF_AVOID   = 0x80
};

class SimpleSteeringController : public ISteeringController
{
    private:
        entity::IMobileEntity *     _mob;
        irr::core::vector3df        _seekTarget;
        irr::core::vector3df        _arriveTarget;
        irr::core::vector3df        _fleeTarget;

        entity::IMobileEntity *     _evadeTarget;
        entity::IMobileEntity *     _hideTarget;
        entity::IMobileEntity *     _pursuitTarget;
        EntityGroup     _obstacles;

        unsigned int _behaviorFlags;

        behavior::ArriveBehavior  * _arriveBehavior;
        behavior::EvadeBehavior   * _evadeBehavior;
        behavior::FleeBehavior    * _fleeBehavior;
        behavior::HideBehavior    * _hideBehavior;
        behavior::SeekBehavior    * _seekBehavior;
        behavior::PursuitBehavior * _pursuitBehavior;
        behavior::WanderBehavior  * _wanderBehavior;
        behavior::ObstacleAvoidanceBehavior * _obsAvoidBehavior;


    public:
        SimpleSteeringController(entity::IMobileEntity *vehicle);
        irr::core::vector3df Calculate();

        void SetBehaviorFlag(EBEHAVIOR_FLAG flag, bool active);

        void SetSeekTarget(irr::core::vector3df target);
        void SetArriveTarget(irr::core::vector3df target);
        void SetHideTarget(entity::IMobileEntity *target);
        void SetPursuitTarget(entity::IMobileEntity * target);

        void SetObstacles(EntityGroup &obstacles);
    };

}//end ns controller
}//end ns desteer
