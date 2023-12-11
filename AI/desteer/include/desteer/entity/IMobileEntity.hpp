#pragma once

#include "desteer/entity/IBaseEntity.hpp"
#include "desteer/entity/IEntitySpace.hpp"

//Predefining stuff to avoid circular reference.
namespace desteer { namespace controller {
class ISteeringController;
}}

namespace desteer {
namespace entity {

class IMobileEntity : public IBaseEntity, public IEntitySpace
{
public:

    virtual float Mass() const = 0;
	virtual void SetMass(float mass) = 0;

	virtual float MaxForce() const = 0;
	virtual void SetMaxForce(float maxForce) = 0;

	virtual float MaxSpeed() const = 0;
	virtual void SetMaxSpeed(float maxSpeed) = 0;

	//virtual float MaxTurnRate() const = 0;
	//virtual void SetMaxTurnRate(float maxTurnRate) = 0;

    virtual irr::core::vector3df Velocity() const = 0;

    virtual void SetSteering(controller::ISteeringController * controller) = 0;
};

}//end namespace entity
}//end namespace desteer

