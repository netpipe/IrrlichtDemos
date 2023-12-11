#ifndef _COLLISION_FORM_H_
#define _COLLISION_FORM_H_

#include "../Outils.h"
#define  PhyNode btRigidBody
//! the licence is on EntityManager.h

namespace GamePhysique
{
    class CollisionForm
    {
        public:
            CollisionForm(irr::IrrlichtDevice*);
            ~CollisionForm();
            void makeRigid(irr::scene::IMeshSceneNode*,float,OBJECT_FORME);
            void makeTerrain(int height, int witdh, void*data);
            void makeSoft (irr::scene::IMeshSceneNode*,float,OBJECT_FORME);

            void setOrigine(float mass,irr::core::vector3df org);

            void meshToPhysic(irr::scene::IMesh *TMesh, btTriangleMesh *TCollisionMesh, const irr::core::vector3df &TScale);
            btSoftBody*meshToSoftPhysic(irr::scene::IMesh*TMesh,Outils::MeshData*data, const irr::core::vector3df &TScale);
            void meshBufferToPhysic(irr::scene::IMeshBuffer *TMesh, btTriangleMesh *TCollisionMesh, const irr::core::vector3df &TScale);
            void meshBufferToSoftPhysic(irr::scene::IMeshBuffer *TMesh, btTriangleMesh *TCollisionMesh, const irr::core::vector3df &TScale);

            void drawDebug();
            btCollisionShape*getShape(){ return Shape; }
            btVector3 getLocalInertia(){ return LocalInertia; }
        private:
            Outils::MeshData data;
            btVector3 LocalInertia;
            irr::IrrlichtDevice *device;
            btCollisionShape *Shape;
    };
}
#endif // _COLLISION_FORM_H_
