// Copyright (C) 2014- Danyal Zia
// Copyright (C) 2009-2013 Josiah Hartzell (Skyreign Software)
// This file is part of the "irrBullet" Bullet physics wrapper.
// For conditions of distribution and use, see copyright notice in irrBullet.h
// The above copyright notice and its accompanying information must remain here.

#include <ISceneNode.h>
#include "btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"
#include "irrBulletBoxShape.h"

using namespace irr;
using namespace core;
using namespace scene;

IBoxShape::IBoxShape(ISceneNode* const n, f32 m, bool overrideMargin)
{
    node = n;
    mass = m;

	type = ECollisionShapeType::ECST_BOX;

    createShape(overrideMargin);
}

void IBoxShape::createShape(bool overrideMargin)
{
    node->updateAbsolutePosition();

    const vector3df& extent = node->getBoundingBox().getExtent() + f32((overrideMargin) ? 0.04:0.0);

    if(node->getType() == ESNT_CUBE)
    {
        shape = new btBoxShape(irrlichtToBulletVector(extent));
    }

    else
    {
        shape = new btBoxShape(irrlichtToBulletVector(extent * 0.5));
    }

	setLocalScaling(node->getScale(), EScalingPair::ESP_COLLISIONSHAPE);
	calculateLocalInertia(getMass(), vector3df(0.0f,0.0f,0.0f));
}

IBoxShape::~IBoxShape()
{
}
