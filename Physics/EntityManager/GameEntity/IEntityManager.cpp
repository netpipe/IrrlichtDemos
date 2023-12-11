#include "IEntityManager.h"
//! the licence is on EntityManager.h

namespace GameEntity
{
    IEntityManager::IEntityManager(irr::IrrlichtDevice *Device) : device(Device)
    {
        TimeStamp = device->getTimer()->getTime(), DeltaTime = 0;
        debug = false;
        phy = new GamePhysique::Physique(device);
    }
    IEntityManager::~IEntityManager()
    {
        clearEntity();
        device = NULL;
        TimeStamp = NULL;
        DeltaTime = NULL;
        delete phy;
    }

    IEntity *IEntityManager::addEntity(OBJECT obj,unsigned int id,float mass)
    {
        removeEntity(getEntityByID(id));
        IEntity *newEnt = NULL;
        if(obj.Material == GamePhysique::OMT_GHOST)      newEnt = new IEntityUnknown();
        else if(obj.Material == GamePhysique::OMT_NORMAL)newEnt = new IEntityRigid(device,phy,obj,mass,id,debug);
        //else if(obj.Material == GamePhysique::OMT_SOFT)  newEnt = new IEntitySoft(device, phy, debug);
        else    return NULL;
        ent.push_back(newEnt);
        return newEnt;
    }
    IEntity *IEntityManager::addEntity(GamePhysique::OBJECT obj,irr::scene::IMesh*mesh,unsigned int id,float mass)
    {
        removeEntity(getEntityByID(id));
        IEntity *newEnt = NULL;
        if(obj.Material == GamePhysique::OMT_GHOST)      newEnt = new IEntityUnknown();
        else if(obj.Material == GamePhysique::OMT_NORMAL)newEnt = new IEntityRigid(device,phy,obj,mesh,mass,id,debug);
        //else if(obj.Material == GamePhysique::OMT_SOFT)  newEnt = new IEntitySoft(device, phy, debug);
        else    return NULL;
        ent.push_back(newEnt);
        return newEnt;
    }
    IEntity *IEntityManager::addEntity(GamePhysique::OBJECT obj,irr::scene::IMeshSceneNode*mesh,unsigned int id,float mass)
    {
        removeEntity(getEntityByID(id));
        IEntity *newEnt = NULL;
        if(obj.Material == GamePhysique::OMT_GHOST)      newEnt = new IEntityUnknown();
        else if(obj.Material == GamePhysique::OMT_NORMAL)newEnt = new IEntityRigid(device,phy,obj,mesh,mass,id,debug);
        //else if(obj.Material == GamePhysique::OMT_SOFT)  newEnt = new IEntitySoft(device, phy, debug);
        else    return NULL;
        ent.push_back(newEnt);
        return newEnt;
    }
    IEntity *IEntityManager::addEmptyEntity(IEntity*entity,GamePhysique::OBJECT obj,unsigned int ID)
    {
        entity->setType(obj.Type);
        entity->setID(ID);
        ent.push_back(entity);
        return entity;
    }
    void IEntityManager::setDebugMode(bool gdb)
    {
        debug = gdb;
        for (int i=0;i<getNomberEntity();++i)
            getEntity(i)->setDebug(gdb);
    }
    void IEntityManager::removeEntity(IEntity*entity)
    {
        if(entity != ENTITY_INVALIDE)
        {
            for (std::list<IEntity*>::iterator It = ent.begin() ; It != ent.end() ; ++It)
                if((*It) == entity)
                {
                    delete *It;
                    It = ent.erase(It);
                }
            entity = ENTITY_INVALIDE;
        }
    }

    IEntity *IEntityManager::getEntity(unsigned int loc)
    {
        if(loc <= ent.size())
        {
            unsigned int n = 0;
            if(loc < 0)  loc = 0^loc;
            for (std::list<IEntity*>::iterator It = ent.begin() ; It != ent.end() ; ++It)
                if(n++ == loc)
                    return *It;
        }
        return ENTITY_INVALIDE;
    }
    IEntity *IEntityManager::getEntityByID(unsigned int id)
    {
        for (std::list<IEntity*>::iterator It = ent.begin() ; It != ent.end() ; ++It)
           if(id   == (*It)->getID()  ) return *It;
        return ENTITY_INVALIDE;
    }

    unsigned int IEntityManager::getNomberEntity() const
    {
        return ent.size();
    }

    void IEntityManager::clearEntity()
    {
        while(getNomberEntity() > 0)
            for(int i=0;i<getNomberEntity();++i)
                removeEntity(getEntity(i));
    }
    void IEntityManager::drawAll()
    {
        phy->drawAll(GamePhysique::Physique::RTG_FAST);
    }
}
