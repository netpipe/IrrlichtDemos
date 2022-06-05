#ifndef CENEMYMOVEMENT_H
#define CENEMYMOVEMENT_H

#include <CComponent.h>
#include <CComponentFactory.h>
#include <CSpatialHash.h>

class CEnemyMovement : public CComponent
{
    public:
        /** Default constructor */
        CEnemyMovement(CEntity* entity, CEntityManager* manager, CSpatialHash<>* grid);
        /** Default destructor */
        virtual ~CEnemyMovement();

        void update(irr::f32 timeDiff);
    protected:
        irr::core::vector3df& Position;
        irr::core::vector3df& TargetPosition;
        irr::core::vector3df& Dir;
        irr::f32& Speed;

        CSpatialHash<>* UnitGrid;
        irr::core::list<CEntity*>* MyList;
        irr::core::list<CEntity*>::Iterator MyIt;
    private:
};

class CEnemyMovementFactory : public CComponentFactory
{
    public:
        CEnemyMovementFactory(CSpatialHash<>* unitGrid) : CComponentFactory("EnemyMovement")
        {
            UnitGrid = unitGrid;
        }

        virtual ~CEnemyMovementFactory() {}

        virtual CComponent* createComponent(CEntity* entity, CEntityManager* manager)
        {
            return new CEnemyMovement(entity, manager, UnitGrid);
        }
    protected:
        CSpatialHash<>* UnitGrid;
    private:
};

#endif // CENEMYMOVEMENT_H
