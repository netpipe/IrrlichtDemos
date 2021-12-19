// Copyright (C) 2014- Danyal Zia
// Copyright (C) 2009-2013 Josiah Hartzell (Skyreign Software)
// This file is part of the "irrBullet" Bullet physics wrapper.
// For conditions of distribution and use, see copyright notice in irrBullet.h
// The above copyright notice and its accompanying information must remain here.

#pragma once

#include "irrBulletCommon.h"

class ICollisionShape;

namespace irr
{
    namespace scene
    {
        class IMesh;
    }
}

/// Convex hull shapes are for objects that need fast simulation with approximated shapes.
/*!
    These could be used for objects such as dynamic game level props.
*/
class IConvexHullShape : public ICollisionShape
{
    public:
        IConvexHullShape(irr::scene::ISceneNode *n, irr::scene::IMesh *collMesh, irr::f32 m);

        virtual ~IConvexHullShape();


    private:
        void createShape(irr::scene::IMesh *collMesh);

        void getConvexHull(irr::scene::IMesh *collMesh, btConvexHullShape *hullShape);
};
