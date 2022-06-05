#include "CComponent.h"

CComponent::CComponent(CEntity* entity, CEntityManager* manager, E_COMPONENT_TYPE type, const irr::core::stringc& name) : Owner(entity)
{
    //ctor
    Type = type;
    Name = name;
    Manager = manager;
}

CComponent::~CComponent()
{
    //dtor
}

E_COMPONENT_TYPE CComponent::getType(void)
{
    return Type;
}

const irr::core::stringc& CComponent::getName(void)
{
    return Name;
}

CEntity* CComponent::getOwner(void)
{
    return Owner;
}
