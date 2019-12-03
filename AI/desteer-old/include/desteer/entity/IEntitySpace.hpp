#pragma once
#include "irrvector/vector3d.h"

namespace desteer{
namespace entity {

class IEntitySpace
{
    public:
	virtual irr::core::vector3df SideVector() const = 0;
	virtual void SetSideVector(irr::core::vector3df side) = 0;

	virtual irr::core::vector3df ForwardVector() const = 0;
	virtual void SetForwardVector(irr::core::vector3df forward) = 0;

    virtual irr::core::vector3df transformWorldVectToLocal(irr::core::vector3df vec) = 0;
    virtual irr::core::vector3df transformLocalVectToWorld(irr::core::vector3df vec) = 0;

};

}//end namespace entity
}//end namespace desteer
