#include "CHealthComponent.h"
#include "CEntityManager.h"
#include "CStateManager.h"

CHealthComponent::CHealthComponent(CEntity* entity, CEntityManager* manager) :  CComponent(entity, manager, ECT_DATASHEET, "DataSheet"), Health(getPropertyValue<int>("Health"))
{
    getOwner()->registerCallback("receiveDamage", this, &CHealthComponent::receiveDamage);
}

CHealthComponent::~CHealthComponent()
{
    getOwner()->unregisterCallback("receiveDamage", this);
}

void CHealthComponent::update(irr::f32 timeDiff)
{
    if (Health <= 0)
    {
        getPropertyValue<bool>("Alive") = false;
        Manager->getManager()->FireEvent("EnemyKilled", 20, getOwner());
        getOwner()->remove();
    }
}

void CHealthComponent::receiveDamage(Base_Property* property, CEntity* sender)
{
    Property<int>* p = (Property<int>*)property;
    Health -= p->getValue();
}
