#ifndef __BVH_TRIANGLE_MESH_SHAPE_H_INCLUDED__
#define __BVH_TRIANGLE_MESH_SHAPE_H_INCLUDED__

#include "collisionshape.h"


class IBvhTriangleMeshShape : public ICollisionShape
{
    public:
        IBvhTriangleMeshShape(irr::scene::ISceneNode *n, irr::scene::IMesh *collMesh, irr::f64 m);

        ~IBvhTriangleMeshShape();
    protected:
        void createShape(irr::scene::IMesh *mesh);
};

#endif // __BVH_TRIANGLE_MESH_SHAPE_H_INCLUDED__
