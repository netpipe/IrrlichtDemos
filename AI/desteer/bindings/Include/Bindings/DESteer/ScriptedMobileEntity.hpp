#pragma once

#include "desteer/entity/IMobileEntity.hpp"
#include "Bindings/DESteer/ScriptedBaseEntity.hpp"
#include "Bindings/DESteer/ScriptedEntitySpace.hpp"

//#define ScriptFunc(IsVoid, RType, FuncName, CONST, ArgTypes)
#include "LikeMagic/Utility/ScriptFunc.hpp"

namespace desteer {
namespace entity {

class ScriptedMobileEntity : public IMobileEntity, public LikeMagic::MarkableObjGraph
{
public:

    // For some reason deriving from ScriptedBaseEntity and ScriptedMobile entity did
    // not count as an implementation of the abstract functions in the interfaces
    // those two implement.  It's a C++ quirk I wasn't aware of.  Rather than
    // fight it it is just easier to define all the interface functions here:

    // IBaseEntity Functions
    ScriptFunc(0, irr::core::vector3df, Position,           const,          )
    ScriptFunc(1, void,                 SetPosition,    /* not const */,    (irr::core::vector3df))
    ScriptFunc(0, float,                Radius,             const,          )
    ScriptFunc(1, void,                 Update,         /* not const */,    (float))

    // IEntitySpace Functions
    ScriptFunc(0, irr::core::vector3df, SideVector,                     const,)
	ScriptFunc(1, void,                 SetSideVector,              /* not const */,    (irr::core::vector3df))
	ScriptFunc(0, irr::core::vector3df, ForwardVector,                  const,)
	ScriptFunc(1, void,                 SetForwardVector,           /* not const */,    (irr::core::vector3df))
    ScriptFunc(0, irr::core::vector3df, transformWorldVectToLocal,  /* not const */,    (irr::core::vector3df))
	ScriptFunc(0, irr::core::vector3df, transformLocalVectToWorld,  /* not const */,    (irr::core::vector3df))

    // IMobile Entity Functions
	ScriptFunc(1, void,                 SetSteering,                /* not const */,    (controller::ISteeringController*) )
    ScriptFunc(0, float,                Mass,                           const, )
	ScriptFunc(1, void,                 SetMass,                    /* not const */,    (float) )
	ScriptFunc(0, float,                MaxForce,                       const, )
	ScriptFunc(1, void,                 SetMaxForce,                /* not const */,    (float) )
	ScriptFunc(0, float,                MaxSpeed,                       const, )
	ScriptFunc(1, void,                 SetMaxSpeed,                /* not const */,    (float) )
    ScriptFunc(0, irr::core::vector3df, Velocity,                       const,  )
};

}//end namespace entity
}//end namespace desteer
