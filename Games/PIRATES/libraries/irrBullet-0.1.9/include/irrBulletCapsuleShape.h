// Copyright (C) 2014- Danyal Zia
// Copyright (C) 2009-2013 Josiah Hartzell (Skyreign Software)
// This file is part of the "irrBullet" Bullet physics wrapper.
// For conditions of distribution and use, see copyright notice in irrBullet.h
// The above copyright notice and its accompanying information must remain here.

#pragma once

#include "irrBulletCollisionShape.h"

class ICapsuleShape : public ICollisionShape
{
public:
	ICapsuleShape(irr::scene::ISceneNode* const n, irr::f32 m, bool overrideMargin = false);

	ICapsuleShape(const ICapsuleShape& other) = default;

	ICapsuleShape& operator=(const ICapsuleShape& other) = default;

	// move constructor
	ICapsuleShape(const ICapsuleShape&& other)
	{
		*this = std::move(other);
	}

	// Move assignment operator
	ICapsuleShape& operator=(ICapsuleShape&& other)
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

	virtual ~ICapsuleShape();

protected:
	void createShape(bool overrideMargin);
};
