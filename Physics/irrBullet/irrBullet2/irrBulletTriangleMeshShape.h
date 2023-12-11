// Copyright (C) 2014- Danyal Zia
// Copyright (C) 2009-2013 Josiah Hartzell (Skyreign Software)
// This file is part of the "irrBullet" Bullet physics wrapper.
// For conditions of distribution and use, see copyright notice in irrBullet.h
// The above copyright notice and its accompanying information must remain here.

#pragma once

#include "irrBulletCollisionShape.h"
#include <BulletCollision/CollisionShapes/btTriangleMesh.h>
#include "irrBulletCommon.h"

namespace irr
{
    namespace scene
    {
        class IMesh;
    }
}

/// The triangle mesh shape, as the name suggests, is made up of triangles extracted from a mesh.
/*!
    @see IGImpactMeshShape
    @see IBvhTriangleMeshShape
*/
class ITriangleMeshShape : public ICollisionShape
{
    public:
		ITriangleMeshShape();
        virtual ~ITriangleMeshShape();

    protected:
        /// To be implemented by triangle geometry-related shape classes inheriting from ITriangleMeshShape
        virtual void createShape(irr::scene::IMesh* const collMesh) = 0;

        /// @return The btTriangleMesh created by extracting the geometry from an Irrlicht IMesh.
        btTriangleMesh *createTriangleMesh(irr::scene::IMesh* const mesh);

        btTriangleMesh* CollisionMesh;
};
