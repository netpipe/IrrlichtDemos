#ifndef __G_IMPACT_MESH_SHAPE_H_INCLUDED__
#define __G_IMPACT_MESH_SHAPE_H_INCLUDED__

#include "common.h"
#include <BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>
#include "collisionshape.h"


class IGImpactMeshShape : public ICollisionShape
{
    public:
        IGImpactMeshShape(irr::scene::ISceneNode *n, irr::scene::IMesh* collMesh, irr::f64 m);

        virtual void updateBound();

        ~IGImpactMeshShape();

    protected:
        btGImpactMeshShape* gimpactShape;
        virtual void createShape(irr::scene::IMesh *collMesh);
};

#endif // __G_IMPACT_MESH_SHAPE_H_INCLUDED__
