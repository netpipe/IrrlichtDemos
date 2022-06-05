#ifndef CENTITY_H
#define CENTITY_H

#include <irrlicht.h>
#include <GameVariables.h>
#include <Functor.h>
#include <Property.h>
#include <CEventDispatcher.h>

class CEntityManager;
class CComponent;

class CEntity : public irr::IReferenceCounted, public CEventDispatcher
{
public:
    /** Default constructor */
    CEntity(CEntityManager* manager, const irr::c8* name);

    /** Default destructor */
    virtual ~CEntity();

    const irr::core::stringc& getName(void)
    {
        return Name;
    }

    void remove(void);

    void update(float timediff);

    void addComponent(const irr::core::stringc& name);

    template<typename T>
    T& getPropertyValue(const irr::core::stringc& name)
    {
        irr::core::list<Base_Property*>::Iterator it = Properties.begin();
        while (it != Properties.end())
        {
            if ((*it)->getName() == name)
            {
                Property<T>* p = (Property<T>*)(*it);
                return p->getValue();
            }
            ++it;
        }
        Property<T>* p = new Property<T>(name);
        Properties.push_back(p);
        return p->getValue();
    }

    template<typename T>
    bool checkPropertyValue(const irr::core::stringc& name, T& value)
    {
        irr::core::list<Base_Property*>::Iterator it = Properties.begin();
        while (it != Properties.end())
        {
            if ((*it)->getName() == name)
            {
                Property<T>* p = (Property<T>*)(*it);
                value = p->getValue();
                return true;
            }
            ++it;
        }
        return false;
    }

    template<typename T>
    Property<T>* getProperty(const irr::core::stringc& name)
    {
        irr::core::list<Base_Property*>::Iterator it = Properties.begin();
        while (it != Properties.end())
        {
            if ((*it)->getName() == name)
            {
                Property<T>* p = (Property<T>*)(*it);
                return p;
            }
            ++it;
        }
        Property<T>* p = new Property<T>(name);
        Properties.push_back(p);
        return p;
    }
protected:
    friend class CEntityManager;

    irr::core::stringc Name;
    CEntityManager* Manager;
    CComponent* Components[ECT_COUNT];
    irr::core::list<Base_Property*> Properties;
private:
};

#endif // CENTITY_H
