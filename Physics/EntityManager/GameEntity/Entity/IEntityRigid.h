#ifndef IENTITYMOVABLE_H
#define IENTITYMOVABLE_H

#include "IEntity.h"
//! the licence is on EntityManager.h

namespace GameEntity
{
    class IEntityRigid : public IEntity
    {
        public:
            IEntityRigid(irr::IrrlichtDevice*Device,GamePhysique::Physique*World,GamePhysique::OBJECT obj,irr::scene::IMeshSceneNode*mesh,float mass=0.1f,int id=-1,bool gdb=true);
            IEntityRigid(irr::IrrlichtDevice*Device,GamePhysique::Physique*World,GamePhysique::OBJECT obj,irr::scene::IMesh*mesh,float mass=0.1f,int id=-1,bool gdb=true);
            IEntityRigid(irr::IrrlichtDevice*Device,GamePhysique::Physique*World,GamePhysique::OBJECT obj,float mass=0.1f,int id=-1,bool gdb=true);
            virtual ~IEntityRigid();

            void setTexture(char*texture,int layer = 0){ Node->setMaterialTexture(layer, device->getVideoDriver()->getTexture(texture)); }
            irr::core::vector3df getScale()   { return Node->getScale();    }
            irr::core::vector3df getPosition(){ return Node->getPosition(); }
            irr::core::vector3df getRotation(){ return Node->getRotation(); }
            void setVisible(bool vs){ Node->setVisible(vs); }

            virtual const char *getTypeName() const { return "Rigid Entity"; }
            virtual void draw();
        protected:
            float mass;
            irr::scene::IMeshSceneNode *Node;
        private:
            irr::scene::ISceneManager *smgr;
            irr::core::vector3df begin;//for debug
            irr::core::vector3df end;
    };
}

#endif // IENTITYMOVABLE_H
