#ifndef IENTITYOBJET_H
#define IENTITYOBJET_H

#include "IEntity.h"
namespace GameEntity
{
    class IEntitySoft : public IEntity
    {
        public:
            IEntitySoft(irr::IrrlichtDevice*Device, btDiscreteDynamicsWorld*World, bool gdb = true);
            virtual ~IEntitySoft();

            virtual void setScale   (irr::core::vector3df sc);
            virtual void setPosition(irr::core::vector3df ps);
            virtual void setRotation(irr::core::vector3df rt);
            virtual irr::core::vector3df getScale()   { return Node->getScale();    }
            virtual irr::core::vector3df getPosition(){ return Node->getPosition(); }
            virtual irr::core::vector3df getRotation(){ return Node->getRotation(); }

            virtual const char *getTypeName() const { return "Static Item"; }
            virtual void draw();
        protected:
            irr::scene::IMesh *mesh;
            irr::scene::ISceneNode *Node;
            // physique
            btSoftBody *TObject;
            btCollisionShape *Shape;
            btTriangleMesh *CollisionMesh;
            btSoftBodyWorldInfo Info;
            virtual void creatShape();
            void physique();
        private:
            irr::scene::ISceneManager *smgr;
    };
}
#endif // IENTITYOBJET_H
