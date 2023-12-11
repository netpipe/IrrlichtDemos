// Copyright (C) 2014- Danyal Zia
// Copyright (C) 2009-2013 Josiah Hartzell (Skyreign Software)
// This file is part of the "irrBullet" Bullet physics wrapper.
// For conditions of distribution and use, see copyright notice in irrBullet.h
// The above copyright notice and its accompanying information must remain here.

#pragma once

#include "irrBulletCollisionShape.h"

class ICylinderShape : public ICollisionShape
{
public:
	ICylinderShape(irr::scene::ISceneNode* const n, irr::f32 m, bool overrideMargin = false);

	ICylinderShape(const ICylinderShape& other) = default;

	ICylinderShape& operator=(const ICylinderShape& other) = default;

	// move constructor
	ICylinderShape(const ICylinderShape&& other)
	{
		*this = std::move(other);
	}

	// Move assignment operator
	ICylinderShape& operator=(ICylinderShape&& other)
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

	virtual ~ICylinderShape();

protected:
	void createShape(bool overrideMargin);
};
