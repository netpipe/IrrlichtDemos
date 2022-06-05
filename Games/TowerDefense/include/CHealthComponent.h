#ifndef CHEALTHCOMPONENT_H
#define CHEALTHCOMPONENT_H

#include <irrlicht.h>
#include <CComponent.h>

class Base_Property;
class CEntity;
class CHealthComponent : public CComponent
{
    public:
        CHealthComponent(CEntity* entity, CEntityManager* manager);
        virtual ~CHealthComponent();
        void update(irr::f32 timeDiff);
    protected:
        int& Health;

        void receiveDamage(Base_Property* property, CEntity* sender);
    private:
};

#endif // CHEALTHCOMPONENT_H
