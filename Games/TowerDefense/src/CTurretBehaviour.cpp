#include "CTurretBehaviour.h"

CTurretBehaviour::CTurretBehaviour(CEntity* entity, CEntityManager* manager, CSpatialHash<>* grid) : CComponent(entity, manager, ECT_LOGIC, "TurretBehaviour"), Range(getPropertyValue<int>("Range")), Position(getPropertyValue<irr::core::vector3df>("Position")), Dir(getPropertyValue<irr::core::vector3df>("Dir")), Damage(getPropertyValue<int>("Damage"))
{
    UnitGrid = grid;
    Range = 1;
    Enemy = NULL;
    UnitGrid->getData(Position)->push_back(getOwner());
    MyIt = UnitGrid->getData(Position)->getLast();
    ReloadTime = 0.6f;
    Reload = 0.f;
}

CTurretBehaviour::~CTurretBehaviour()
{
    printf("kill turrent\n");
    if (Enemy)
        Enemy->drop();
    Enemy = NULL;
    UnitGrid->getData(Position)->erase(MyIt);
}

void CTurretBehaviour::fillEnemyList(void)
{
    EnemyList.clear();

    int X_Start = Position.X/tile_size-Range;
    int Y_Start = Position.Y/tile_size-Range;

    int X_End = Position.X/tile_size+Range+1;
    int Y_End = Position.Y/tile_size+Range+1;

    if (X_Start < 0)
        X_Start = 0;
    if (Y_Start < 0)
        Y_Start = 0;
    if (X_End >= game_width)
        X_End = game_width;
    if (Y_End >= game_height)
        Y_End = game_height;

    for (int x=X_Start;x<X_End;++x)
    {
        for (int y=Y_Start;y<Y_End;++y)
        {
            if (UnitGrid->getData(x,y) != UnitGrid->getData(Position))
                EnemyList.push_back(UnitGrid->getData(x,y));
        }
    }
}

void CTurretBehaviour::update(irr::f32 timeDiff)
{
    //reloading
    if (Reload > 0.f)
        Reload -= timeDiff;

    if (EnemyList.getSize() == 0)
        fillEnemyList();

    //search for new enemy
    bool alive = false;
    if (Enemy)
        alive = Enemy->getPropertyValue<bool>("Alive");

    if (!Enemy || !alive)
    {
        CEntity* tmp = Enemy;

        irr::core::list<irr::core::list<CEntity*>*>::Iterator it = EnemyList.begin();
        while (it != EnemyList.end())
        {
            irr::core::list<CEntity*>::Iterator at = (*it)->begin();
            while (at != (*it)->end())
            {
                if (*at != Enemy && *at != getOwner() && (*at)->getPropertyValue<E_ENTITY_TYPE>("EntityType") != EET_TOWER && Enemy && Enemy->getPropertyValue<bool>("Alive") && Position.getDistanceFromSQ((*at)->getPropertyValue<irr::core::vector3df>("Position")) < Position.getDistanceFromSQ(Enemy->getPropertyValue<irr::core::vector3df>("Position")))
                {
                    Enemy->drop();
                    Enemy = *at;
                    Enemy->grab();
                }
                else if (*at != Enemy && *at != getOwner() && (*at)->getPropertyValue<E_ENTITY_TYPE>("EntityType") != EET_TOWER)
                {
                    if (Enemy)
                    {
                        Enemy->drop();
                    }
                    Enemy = *at;
                    Enemy->grab();
                }
                ++at;
            }
            ++it;
        }
        if (Enemy == tmp)
        {
            if (Enemy)
            {
                Enemy->drop();
                Enemy = NULL;
            }
        }
    }
    else
    {
        //check if still in range
        irr::core::vector3df enemyPos = Enemy->getPropertyValue<irr::core::vector3df>("Position");
        if ((enemyPos-Position).getLength() > Range*tile_size+tile_size)
        {
            Enemy->drop();
            Enemy = NULL;
        }
        else
        {
            Dir = enemyPos-Position;
            Dir.normalize();

            //shot if possible
            if (Reload <= 0.f)
            {
                Enemy->FireEvent("receiveDamage", Damage, getOwner());
                Reload = ReloadTime;
            }
        }
    }
}
