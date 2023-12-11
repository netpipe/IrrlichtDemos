// Copyright (C) 2014- Danyal Zia
// Copyright (C) 2009-2013 Josiah Hartzell (Skyreign Software)
// This file is part of the "irrBullet" Bullet physics wrapper.
// For conditions of distribution and use, see copyright notice in irrBullet.h
// The above copyright notice and its accompanying information must remain here.

#pragma once

#include "irrBulletCommon.h"
#include <BulletCollision/Gimpact/btGImpactShape.h>
#include "irrBulletTriangleMeshShape.h"


/// The GImpact mesh shape is a triangle mesh shape for concave collision detection for dynamic objects.
/*!
    This is useful for accurate shapes, such as the car frame in
    a racing game, or the aircraft body in a flight simulator.
*/
class IGImpactMeshShape : public ITriangleMeshShape
{
    public:
        IGImpactMeshShape(irr::scene::ISceneNode *n, irr::scene::IMesh* collMesh, irr::f32 m);
        virtual ~IGImpactMeshShape();

        void updateBound();


    protected:
        void createShape(irr::scene::IMesh* const collMesh);
};
