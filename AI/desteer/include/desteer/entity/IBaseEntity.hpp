#pragma once
#include "irrvector/vector3d.h"

namespace desteer {
namespace entity {

class IBaseEntity {
public:
	virtual irr::core::vector3df Position() const = 0;
	virtual void SetPosition(irr::core::vector3df position) = 0;

	virtual float Radius() const = 0;

	virtual void Update(float timeElapsed) = 0;
};

}//end namespace entity
}//end namespace desteer
