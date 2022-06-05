#ifndef CCOMPONENT_H
#define CCOMPONENT_H

#include <irrlicht.h>
#include <GameVariables.h>
#include <CEntity.h>

class CEntityManager;
class CComponent
{
    public:
        /** Default constructor */
        CComponent(CEntity* entity, CEntityManager* manager, E_COMPONENT_TYPE type, const irr::core::stringc& name);

        /** Default destructor */
        virtual ~CComponent(void);

        E_COMPONENT_TYPE getType(void);

        const irr::core::stringc& getName(void);

        CEntity* getOwner(void);

        virtual void update(irr::f32 timeDiff) = 0;

        template<typename T>
        T& getPropertyValue(const irr::core::stringc& name)
        {
            if (Owner)
                 return Owner->getPropertyValue<T>(name);
            exit(1);
        }
        template<typename T>
        Property<T>* getProperty(const irr::core::stringc& name)
        {
            if (Owner)
                return Owner->getProperty<T>(name);
            exit(1);
        }
    protected:
        irr::core::stringc Name;
        E_COMPONENT_TYPE Type;
        CEntity* Owner;
        CEntityManager* Manager;
    private:
};

#endif // CCOMPONENT_H
