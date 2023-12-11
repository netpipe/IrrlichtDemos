// Copyright (C) 2014- Danyal Zia
// Copyright (C) 2009-2013 Josiah Hartzell (Skyreign Software)
// This file is part of the "irrBullet" Bullet physics wrapper.
// For conditions of distribution and use, see copyright notice in irrBullet.h
// The above copyright notice and its accompanying information must remain here.

#pragma once

#include "irrBulletTriangleMeshShape.h"

/// The BVH triangle mesh shape is useful for static triangle mesh shapes, such as game levels.
class IBvhTriangleMeshShape : public ITriangleMeshShape
{
    public:
        IBvhTriangleMeshShape(irr::scene::ISceneNode *n, irr::scene::IMesh *collMesh, irr::f32 m);

		IBvhTriangleMeshShape(const IBvhTriangleMeshShape& other) = default;

		IBvhTriangleMeshShape& operator=(const IBvhTriangleMeshShape& other) = default;

		// Move constructor
		IBvhTriangleMeshShape(const IBvhTriangleMeshShape&& other)
		{
			*this = std::move(other);
		}

		// Move assignment operator.
		IBvhTriangleMeshShape& operator=(IBvhTriangleMeshShape&& other)
		{
			if (this != &other)
			{
				delete node;

				node = other.node;
				mass = other.mass;

				other.node = nullptr;
				other.mass = 0;
			}
			return *this;
		}

        virtual ~IBvhTriangleMeshShape();


    protected:
        void createShape(irr::scene::IMesh* const mesh);
};
