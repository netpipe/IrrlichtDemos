#ifndef PROPERTY_H
#define PROPERTY_H

#include <irrlicht.h>

class Base_Property
{
public:

    Base_Property(const irr::core::stringc& name)
    {
        Name = name;
    }
    virtual ~Base_Property(void)
    {
    }
    const irr::core::stringc& getName(void)
    {
        return Name;
    }
    virtual void* getData(void) = 0;
protected:
    irr::core::stringc Name;
};
template<typename T>
class Property : public Base_Property
{
public:
    Property(const irr::core::stringc& name) : Base_Property(name)
    {
    }
    virtual ~Property(void)
    {
    }
    virtual void* getData(void)
    {
        return &Data;
    }
    T& getValue(void)
    {
        return Data;
    }
protected:
    T Data;
};

#endif // PROPERTY_H
