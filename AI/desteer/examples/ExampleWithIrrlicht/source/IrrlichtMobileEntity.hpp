#pragma once
#include "irrlicht/irrlicht.h"
#include "desteer/entity/MobileEntity.hpp"
#include "desteer/controller/SimpleSteeringController.hpp"

namespace desteer {
namespace entity {

class IrrlichtMobileEntity : public MobileEntity
{
private:
	irr::scene::ISceneNode* _node;

	controller::ISteeringController *_steering;

	irr::core::vector3df _steeringForce;
public:
    IrrlichtMobileEntity(irr::scene::ISceneNode* node,
                         irr::core::vector3df position = irr::core::vector3df(0,0,0),
                         float mass = 1,
                         float maxSpeed = 50,
                         float maxForce = 50);

	inline irr::scene::ISceneNode* Node() const { return _node; };
	inline void SetNode(irr::scene::ISceneNode *node) { _node = node; };

    virtual irr::core::vector3df Position() const;
	virtual void SetPosition(irr::core::vector3df position);

	//an approximation of the radius of the bounding box.
	virtual float Radius() const;

	virtual irr::core::vector3df PredictFuturePosition(const float predictionTime) const { return irr::core::vector3df(0,0,0); };

	virtual irr::core::vector3df SideVector() const;
	virtual void SetSideVector(irr::core::vector3df side);

	virtual irr::core::vector3df ForwardVector() const;
	virtual void SetForwardVector(irr::core::vector3df forward);

	irr::core::vector3df SteeringVector() const;

    virtual irr::core::vector3df transformWorldVectToLocal(irr::core::vector3df vec);
	virtual irr::core::vector3df transformLocalVectToWorld(irr::core::vector3df vec);

    virtual void Update(float delta);
	virtual void SetSteering(controller::ISteeringController * controller){_steering = controller;};
};

}//end namespace entity
}//end namespace desteer
