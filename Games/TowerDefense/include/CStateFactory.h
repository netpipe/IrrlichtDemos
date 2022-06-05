#ifndef CSTATEFACTORY_H
#define CSTATEFACTORY_H

#include <irrlicht.h>

class CState;
class CStateManager;
class CStateFactory
{
public:
    CStateFactory(const irr::core::stringc& name)
    {
        Name = name;
    }
    CStateFactory(const CStateFactory& factory)
    {
        Name = factory.Name;
    }
    virtual CState* createState(CStateManager* manager)
    {
        return NULL;
    }
    virtual ~CStateFactory(void)
    {
    }
    irr::core::stringc Name;
};
template<class T>
class CSimpleStateFactory : public CStateFactory
{
public:
    CSimpleStateFactory(const irr::core::stringc& name) : CStateFactory(name)
    {
    }
    virtual CState* createState(CStateManager* manager)
    {
        return new T(manager);
    }
};

#endif // CSTATEFACTORY_H
