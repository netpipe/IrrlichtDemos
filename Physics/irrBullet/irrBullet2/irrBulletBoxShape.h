// Copyright (C) 2014- Danyal Zia
// Copyright (C) 2009-2013 Josiah Hartzell (Skyreign Software)
// This file is part of the "irrBullet" Bullet physics wrapper.
// For conditions of distribution and use, see copyright notice in irrBullet.h
// The above copyright notice and its accompanying information must remain here.

#pragma once

#include "irrBulletCollisionShape.h"

class IBoxShape : public ICollisionShape
{
public:
	IBoxShape(irr::scene::ISceneNode* const n, irr::f32 m, bool overrideMargin = false);

	IBoxShape(const IBoxShape& other) = default;

	IBoxShape& operator=(const IBoxShape& other) = default;

	// move constructor
	IBoxShape(const IBoxShape&& other)
	{
		*this = std::move(other);
	}

	// Move assignment operator
	IBoxShape& operator=(IBoxShape&& other)
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

	virtual ~IBoxShape();

protected:
	void createShape(bool overrideMargin);
};

