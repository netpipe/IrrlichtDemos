#ifndef _ENTITY_H_
#define _ENTITY_H_

#include "../IEntityType.h"
//! the licence is on EntityManager.h

namespace GameEntity
{
    class IEntityManager;
    class IEntity
    {
        public:
            IEntity(irr::IrrlichtDevice *Device, GamePhysique::Physique *World);

            void setType(GamePhysique::OBJECT_TYPE type)   { Obj.Type = type; }
            GamePhysique::OBJECT  getObjectStruct()        { return Obj; }
            virtual const char *getTypeName() const { return "Unknown"; }

            void setID(int id) { ID = id;  }
            unsigned int getID() const { return ID; }

            void        addAttribute   (const std::string,const std::string);
            std::string getAttribute   (const std::string);
            void        removeAttribute(const std::string);
            void        clearAttribute ();

            bool operator==(IEntity*);
            bool operator!=(IEntity*entity){ return (!operator==(entity)); }

            void setDebug(bool gdb){ debug = gdb; }
            bool isDebug(){ return debug; }

            /** for dérived class used physique  **/
            irr::f32 getDistance(IEntity*ent)  { return getPosition().getDistanceFrom(ent->getPosition()); }
            virtual void setVisible(bool){}
            virtual void setTexture(char*,int i = 0){}
            virtual void setScale   (irr::core::vector3df sc){  obj->setScale(sc);    }
            virtual void setPosition(irr::core::vector3df ps){  obj->setPosition(ps); }
            virtual void setRotation(irr::core::vector3df rt){  obj->setRotation(rt); }
            virtual irr::core::vector3df getScale()          {  return scale;  }
            virtual irr::core::vector3df getPosition()       {  return pos;    }
            virtual irr::core::vector3df getRotation()       {  return rot;    }

            irr::IrrlichtDevice*getDevice()                  {  return device; }
            GamePhysique::PhysiqueObjet *getPhysiqueObjet()  {  return obj;    }

            //virtual void addForce(irr::core::vector3df ang){}
            virtual void setMass (float){}
            /** /////////////////////////////////////////////////////////////////////// **/

            virtual void draw(){ }
        protected:
            IEntity(){}
            virtual ~IEntity();
            bool debug;
            GamePhysique::Physique *dynamicsWorld;
            irr::IrrlichtDevice *device;
            GamePhysique::PhysiqueObjet *obj;
            irr::core::vector3df scale;
            GamePhysique::OBJECT Obj;
        private:
            int ID;
            friend class IEntityManager;
            irr::core::vector3df pos;
            irr::core::vector3df rot;
            std::list<EntityAttribute> Attribute;
    };
}

#endif // ENTITY_H
