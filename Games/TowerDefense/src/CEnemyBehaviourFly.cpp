#include "CEnemyBehaviourFly.h"
#include "CTile.h"
#include "CMap.h"
#include "CEntityManager.h"
#include "CStateManager.h"

CEnemyBehaviourFly::CEnemyBehaviourFly(CEntity* entity, CEntityManager* manager, CMap* map) : CComponent(entity, manager, ECT_LOGIC, "EnemyLogicFly"), TargetPosition(entity->getPropertyValue<irr::core::vector3df>("TargetPosition")), Position(entity->getPropertyValue<irr::core::vector3df>("Position"))
{
    c = true;
    Map = map;
}

CEnemyBehaviourFly::~CEnemyBehaviourFly()
{
    //dtor
}

void CEnemyBehaviourFly::update(irr::f32 timeDiff)
{
    if (c)
    {
        c = false;
        TargetPosition = Map->getGoal()->getPos();
    }
    if (Map->getGoal() == Map->getTile(Position))
    {
        getPropertyValue<bool>("Alive") = false;
        Manager->getManager()->FireEvent("EnemyWin", 1, getOwner());
        getOwner()->remove();
    }
}
