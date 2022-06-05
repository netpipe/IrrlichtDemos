#include "CEntity.h"
#include "CEntityManager.h"
#include "CComponent.h"

CEntity::CEntity(CEntityManager* manager, const irr::c8* name)
{
    Manager = manager;
    Name = name;

    for (int i=0;i<ECT_COUNT;++i)
    {
        Components[i] = NULL;
    }
}

CEntity::~CEntity()
{
    for (int i=0;i<ECT_COUNT;++i)
    {
        if (Components[i])
            delete Components[i];
    }

    irr::core::list<Base_Property*>::Iterator it = Properties.begin();
    while (it != Properties.end())
    {
        Base_Property* p = (*it);
        delete p;
        ++it;
    }
    Properties.clear();
}

void CEntity::remove(void)
{
    Manager->addEntityToDeletenList(this);
}

void CEntity::update(float timediff)
{
    for (int i=0;i<ECT_COUNT;++i)
    {
        if (Components[i])
            Components[i]->update(timediff);
    }
}

void CEntity::addComponent(const irr::core::stringc& name)
{
    CComponent* c = Manager->addComponentToEntity(name, this);
}
