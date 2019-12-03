#pragma once
#include "desteer/entity/IMobileEntity.hpp"
#include "desteer/controller/SimpleSteeringController.hpp"

//#define ScriptFunc(IsVoid, RType, FuncName, CONST, ArgTypes)
#include "LikeMagic/Utility/ScriptFunc.hpp"

namespace desteer {
namespace entity {

class ScriptedEntitySpace : virtual public LikeMagic::MarkableObjGraph, public IEntitySpace
{
public:
    ScriptFunc(0, irr::core::vector3df, SideVector,                     const,)
	ScriptFunc(1, void,                 SetSideVector,              /* not const */,    (irr::core::vector3df))
	ScriptFunc(0, irr::core::vector3df, ForwardVector,                  const,)
	ScriptFunc(1, void,                 SetForwardVector,           /* not const */,    (irr::core::vector3df))
    ScriptFunc(0, irr::core::vector3df, transformWorldVectToLocal,  /* not const */,    (irr::core::vector3df))
	ScriptFunc(0, irr::core::vector3df, transformLocalVectToWorld,  /* not const */,    (irr::core::vector3df))
};

}//end namespace entity
}//end namespace desteer
