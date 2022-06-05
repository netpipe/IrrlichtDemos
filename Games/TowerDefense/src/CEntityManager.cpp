#include "CEntityManager.h"
#include "CEntity.h"
#include "CComponentFactory.h"
#include "CComponent.h"

CEntityManager::CEntityManager(CStateManager* manager)
{
    StateManager = manager;
}

CEntityManager::~CEntityManager()
{
    //dtor
    irr::core::list<CEntity*>::Iterator it = Entities.begin();
    while (it != Entities.end())
    {
        CEntity* e = *it;
        e->drop();
        ++it;
    }
    Entities.clear();
}

CStateManager* CEntityManager::getManager(void)
{
    return StateManager;
}

CEntity* CEntityManager::createEntity(const irr::c8* name)
{
    CEntity* entity = new CEntity(this, name);
    Entities.push_back(entity);
    return entity;
}

void CEntityManager::update(irr::f32 timeDiff)
{
    irr::core::list<CEntity*>::Iterator it;

    //delete entities
    it = Entities_to_Delete.begin();
    while (it != Entities_to_Delete.end())
    {
        irr::core::list<CEntity*>::Iterator at = Entities.begin();
        while (at != Entities.end())
        {
            if (*it == *at)
            {
                //printf("Deleting Entity...");
                (*at)->drop();
                //printf("done\n");
                Entities.erase(at);
                break;
            }
            ++at;
        }
        ++it;
    }
    Entities_to_Delete.clear();

    //update entities
    it = Entities.begin();
    while (it != Entities.end())
    {
        (*it)->update(timeDiff);
        ++it;
    }


}

void CEntityManager::addEntityToDeletenList(CEntity* entity)
{
    irr::core::list<CEntity*>::Iterator it = Entities_to_Delete.begin();
    while (it != Entities_to_Delete.end())
    {
        if (*it == entity)
            return;
        ++it;
    }
    Entities_to_Delete.push_back(entity);
}

void CEntityManager::registerComponent(CComponentFactory* factory)
{
    irr::core::map<irr::core::stringc, CComponentFactory*>::Node* node = ComponentFactories.find(factory->getName());
    if (node)
    {
        CComponentFactory* f = node->getValue();
        delete f;
    }
    ComponentFactories[factory->getName()] = factory;
}

CComponent* CEntityManager::addComponentToEntity(const irr::core::stringc& name, CEntity* entity)
{
    irr::core::map<irr::core::stringc, CComponentFactory*>::Node* node = ComponentFactories.find(name);
    if (node)
    {
        CComponentFactory* f = node->getValue();
        CComponent* c = f->createComponent(entity, this);
        if (entity->Components[c->getType()])
            delete entity->Components[c->getType()];
        entity->Components[c->getType()] = c;
        return c;
    }
    return NULL;
}
