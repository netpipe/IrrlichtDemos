#include "CEnemyMovement.h"

CEnemyMovement::CEnemyMovement(CEntity* entity, CEntityManager* manager, CSpatialHash<>* grid) : CComponent(entity, manager, ECT_MOVE, "EnemyMovement"), Position(entity->getPropertyValue<irr::core::vector3df>("Position")), TargetPosition(entity->getPropertyValue<irr::core::vector3df>("TargetPosition")), Dir(entity->getPropertyValue<irr::core::vector3df>("Dir")), Speed(entity->getPropertyValue<irr::f32>("Speed"))
{
    //ctor
    MyList = NULL;
    UnitGrid = grid;
    if (Speed == 0.f)
        Speed = tile_size*2;
}

CEnemyMovement::~CEnemyMovement()
{
    if (MyList)
    {
        MyList->erase(MyIt);
    }
}

void CEnemyMovement::update(irr::f32 timeDiff)
{
    Dir = (TargetPosition-Position);
    if (Dir.getLengthSQ() < 100)
    {
        Position = TargetPosition;
    }
    else
    {
        Dir.normalize();
        Position += Dir*timeDiff*Speed;
    }
    if (MyList != UnitGrid->getData(Position))
    {
        if (MyList)
        {
            MyList->erase(MyIt);
        }
        MyList = UnitGrid->getData(Position);
        MyList->push_back(Owner);
        MyIt = MyList->getLast();
    }
}
