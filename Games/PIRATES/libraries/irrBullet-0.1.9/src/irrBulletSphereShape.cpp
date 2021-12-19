// Copyright (C) 2014- Danyal Zia
// Copyright (C) 2009-2013 Josiah Hartzell (Skyreign Software)
// This file is part of the "irrBullet" Bullet physics wrapper.
// For conditions of distribution and use, see copyright notice in irrBullet.h
// The above copyright notice and its accompanying information must remain here.

#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>
#include "irrBulletSphereShape.h"

using namespace irr;
using namespace core;
using namespace scene;

ISphereShape::ISphereShape(ISceneNode *n, f32 m, bool overrideMargin)
{
    node = n;
    mass = m;

	type = ECollisionShapeType::ECST_SPHERE;

    createShape(overrideMargin);
}

void ISphereShape::createShape(bool overrideMargin)
{
    node->updateAbsolutePosition();
    /*const aabbox3df& box = node->getTransformedBoundingBox();
    const vector3df& diag = (box.MaxEdge - box.getCenter());
    const f32 radius = diag.getLength() * 0.53;*/
    const aabbox3df& box = node->getTransformedBoundingBox();
    const vector3df& diag = (box.MaxEdge - box.getCenter()) + f32((overrideMargin) ? 0.04:0.0);
    const f32 radius = f32(diag.getLength() * 0.5f);


	shape = new btSphereShape(radius);
	//setLocalScaling(node->getScale(), ESP_COLLISIONSHAPE);
	calculateLocalInertia(getMass(), vector3df(0.0f,0.0f,0.0f));
}

ISphereShape::~ISphereShape()
{
}

