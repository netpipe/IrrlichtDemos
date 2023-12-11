#include "IEntity.h"
//! the licence is on EntityManager.h

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;

namespace GameEntity
{
    IEntity::IEntity(irr::IrrlichtDevice *Device, GamePhysique::Physique *World) : device(Device), dynamicsWorld(World)
    {
        setID(-1);
        setType(GamePhysique::OPT_NULL);
        setPosition(irr::core::vector3df(0,0,0));
        setRotation(irr::core::vector3df(0,0,0));
        obj = NULL;
    }
    IEntity::~IEntity()
    {
        clearAttribute();
    }
    bool IEntity::operator==(IEntity*entity)
    {
//        if(debug == entity->isDebug() && Obj == entity->getObjectStruct() && scale == entity->getScale() && pos == entity->getPosition() && rot == entity->getRotation() && ((char)this) == ((char)entity))
 //          return true;
        return false;
    }
    void IEntity::addAttribute(const std::string Ent, const std::string Att)
    {
        Attribute.push_back(EntityAttribute(Ent,Att));
    }
    std::string IEntity::getAttribute(const std::string Ent)
    {
        for (std::list<EntityAttribute>::iterator It = Attribute.begin() ; It != Attribute.end() ; ++It)
        {
            std::string cmp = It->ent;
            if(Ent == cmp)
                return It->att;
        }
        return "";//for delete warning "control reaches and of non-void function"
    }
    void IEntity::removeAttribute(const std::string Ent)
    {
        if(Ent != "")
        {
            for (std::list<EntityAttribute>::iterator It = Attribute.begin() ; It != Attribute.end() ; ++It)
                if((*It).ent == Ent)
                    It = Attribute.erase(It);
        }
    }
    void IEntity::clearAttribute()
    {
        Attribute.clear();
    }
}
