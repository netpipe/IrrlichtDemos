#pragma once
#include "desteer/entity/IBaseEntity.hpp"

//#define ScriptFunc(IsVoid, RType, FuncName, CONST, ArgTypes)
#include "LikeMagic/Utility/ScriptFunc.hpp"

namespace desteer{
namespace entity {

class ScriptedBaseEntity : virtual public LikeMagic::MarkableObjGraph, public IBaseEntity
{
public:

    ScriptFunc(0, irr::core::vector3df, Position,           const,          )
    ScriptFunc(1, void,                 SetPosition,    /* not const */,    (irr::core::vector3df))
    ScriptFunc(0, float,                Radius,             const,          )
    ScriptFunc(1, void,                 Update,         /* not const */,    (float))
};

}//end namespace entity
}//end namespace desteer

