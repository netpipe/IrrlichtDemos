#ifndef CENEMYBEHAVIOURFLY_H
#define CENEMYBEHAVIOURFLY_H

#include <CComponent.h>
#include <CComponentFactory.h>

class CMap;
class CEnemyBehaviourFly : public CComponent
{
    public:
        /** Default constructor */
        CEnemyBehaviourFly(CEntity* entity, CEntityManager* manager, CMap* map);
        /** Default destructor */
        virtual ~CEnemyBehaviourFly();

        void update(irr::f32 timeDiff);
    protected:
        irr::core::vector3df& TargetPosition;
        irr::core::vector3df& Position;
        CMap* Map;
        bool c;
    private:
};

class CEnemyBehaviourFlyFactory : public CComponentFactory
{
    public:
        CEnemyBehaviourFlyFactory(CMap* map) : CComponentFactory("EnemyBehaviourFly")
        {
            Map = map;
        }

        virtual ~CEnemyBehaviourFlyFactory() {}

        virtual CComponent* createComponent(CEntity* entity, CEntityManager* manager)
        {
            return new CEnemyBehaviourFly(entity, manager, Map);
        }
    protected:
        CMap* Map;
    private:
};

#endif // CENEMYBEHAVIOURFLY_H
