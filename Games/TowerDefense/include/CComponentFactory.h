#ifndef CCOMPONENTFACTORY_H
#define CCOMPONENTFACTORY_H

#include <irrlicht.h>

class CComponent;
class CEntityManager;
class CComponentFactory
{
    public:
        CComponentFactory(void) {}
        CComponentFactory(const irr::core::stringc& name)
        {
            Name = name;
        }
        CComponentFactory(const CComponentFactory& factory)
        {
            Name = factory.Name;
        }

        virtual ~CComponentFactory() {}

        virtual CComponent* createComponent(CEntity* entity, CEntityManager* manager)
        {
            return NULL;
        }

        const irr::core::stringc& getName(void)
        {
            return Name;
        }
    protected:
        irr::core::stringc Name;
    private:
};
template<class T>
class CSimpleComponentFactory : public CComponentFactory
{
    public:
        CSimpleComponentFactory(const irr::core::stringc& name) : CComponentFactory(name)
        {}

        virtual ~CSimpleComponentFactory() {}

        virtual CComponent* createComponent(CEntity* entity, CEntityManager* manager)
        {
            return new T(entity, manager);
        }
    protected:
    private:
};

#endif // CCOMPONENTFACTORY_H
