#ifndef CENEMYBEHAVIOUR_H
#define CENEMYBEHAVIOUR_H

#include <CComponent.h>
#include <CComponentFactory.h>

class CMap;
class CTile;
class CEnemyBehaviour : public CComponent
{
    public:
        /** Default constructor */
        CEnemyBehaviour(CEntity* entity, CEntityManager* manager, CMap* map);
        /** Default destructor */
        virtual ~CEnemyBehaviour();

        void update(irr::f32 timeDiff);
    protected:
        irr::core::vector3df& Position;
        irr::core::vector3df& TargetPosition;
        CMap* Map;
        bool c;
        irr::core::list<CTile*> Path;

        void calcPath(void);
    private:
};

class CEnemyBehaviourFactory : public CComponentFactory
{
    public:
        CEnemyBehaviourFactory(CMap* map) : CComponentFactory("EnemyBehaviour")
        {
            Map = map;
        }

        virtual ~CEnemyBehaviourFactory() {}

        virtual CComponent* createComponent(CEntity* entity, CEntityManager* manager)
        {
            return new CEnemyBehaviour(entity, manager, Map);
        }
    protected:
        CMap* Map;
    private:
};

#endif // CENEMYBEHAVIOUR_H
