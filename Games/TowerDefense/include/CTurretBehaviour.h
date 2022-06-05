#ifndef CTURRETBEHAVIOUR_H
#define CTURRETBEHAVIOUR_H

#include <CComponent.h>
#include <CComponentFactory.h>
#include <CSpatialHash.h>


class CTurretBehaviour : public CComponent
{
    public:
        CTurretBehaviour(CEntity* entity, CEntityManager* manager, CSpatialHash<>* grid);
        virtual ~CTurretBehaviour();

        void update(irr::f32 timeDiff);
    protected:
        void fillEnemyList(void);

        CSpatialHash<>* UnitGrid;

        irr::core::vector3df& Position;
        irr::core::vector3df& Dir;

        //turret  data
        int& Damage;
        int& Range;
        float ReloadTime;

        float Reload;

         irr::core::list<irr::core::list<CEntity*>*> EnemyList;
         irr::core::list<CEntity*>::Iterator MyIt;
         CEntity* Enemy;
    private:
};

class CTurretBehaviourFactory : public CComponentFactory
{
    public:
        CTurretBehaviourFactory(CSpatialHash<>* unitGrid) : CComponentFactory("TurretBehaviour")
        {
            UnitGrid = unitGrid;
        }

        virtual ~CTurretBehaviourFactory() {}

        virtual CComponent* createComponent(CEntity* entity, CEntityManager* manager)
        {
            return new CTurretBehaviour(entity, manager, UnitGrid);
        }
    protected:
        CSpatialHash<>* UnitGrid;
    private:
};

#endif // CTURRETBEHAVIOUR_H
