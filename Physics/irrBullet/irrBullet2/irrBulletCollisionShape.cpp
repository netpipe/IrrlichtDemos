// Copyright (C) 2014- Danyal Zia
// Copyright (C) 2009-2013 Josiah Hartzell (Skyreign Software)
// This file is part of the "irrBullet" Bullet physics wrapper.
// For conditions of distribution and use, see copyright notice in irrBullet.h
// The above copyright notice and its accompanying information must remain here.

#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>
#include <BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>
#include "irrBulletCollisionShape.h"

using namespace irr;
using namespace core;
using namespace scene;

ICollisionShape::ICollisionShape()
{
}



void ICollisionShape::setLocalScaling(const vector3df &scaling, const EScalingPair esp)
{
    // TODO: Fix scaling for IGImpactMeshShapes. The bounding box does not update.
    // A call to updateBound() will make it work for what is inside its bounding
    // box, but it does not actually change to fit the real scale.

    vector3df comp = compensateForNodeType(scaling, node->getType());

	if (esp == EScalingPair::ESP_BOTH || esp == EScalingPair::ESP_COLLISIONSHAPE)
    {
        shape->setLocalScaling(btVector3(comp.X,comp.Y,comp.Z));
		if (getShapeType() == ECollisionShapeType::ECST_GIMPACT)
        {
            btGImpactMeshShape *gshape = static_cast<btGImpactMeshShape *>(shape);
            //gshape->setLocalScaling(irrlichtToBulletVector(comp));
            gshape->updateBound();

            shape = gshape;
        }
    }

	if (esp == EScalingPair::ESP_BOTH || esp == EScalingPair::ESP_VISUAL)
        node->setScale(scaling);


    calculateLocalInertia(getMass(), vector3df(0.0f,0.0f,0.0f));
}


void ICollisionShape::calculateLocalInertia(irr::f32 Mass, const vector3df &inertia)
{
    // Keeping this order is important. Strange things happen otherwise.
    btVector3 in(inertia.X,inertia.Y,inertia.Z);
    shape->calculateLocalInertia(Mass, in);

    localInertia = vector3df(in.getX(),in.getY(),in.getZ());
}


void ICollisionShape::createShape()
{
}



const vector3df ICollisionShape::getLocalScaling() const
{
    const btVector3 &scale = shape->getLocalScaling();
    return vector3df(scale.getX(),scale.getY(),scale.getZ());
}


ICollisionShape::~ICollisionShape()
{
    if(shape)
        delete shape;
}
