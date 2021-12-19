// Copyright (C) 2014- Danyal Zia
// Copyright (C) 2009-2013 Josiah Hartzell (Skyreign Software)
// This file is part of the "irrBullet" Bullet physics wrapper.
// For conditions of distribution and use, see copyright notice in irrBullet.h
// The above copyright notice and its accompanying information must remain here.

#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>
#include <BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>
#include "irrBulletGImpactMeshShape.h"


using namespace irr;
using namespace core;
using namespace scene;

IGImpactMeshShape::IGImpactMeshShape(ISceneNode *n, IMesh *collMesh, f32 m)
{
    node = n;
    mass = m;

	type = ECollisionShapeType::ECST_GIMPACT;

    createShape(collMesh);
}


void IGImpactMeshShape::createShape(IMesh *mesh)
{
    CollisionMesh = createTriangleMesh(mesh);
	shape = new btGImpactMeshShape(CollisionMesh);

    updateBound();

    calculateLocalInertia(getMass(), vector3df(0.0f,0.0f,0.0f));
}


void IGImpactMeshShape::updateBound()
{
    static_cast<btGImpactMeshShape*>(shape)->updateBound();
}


IGImpactMeshShape::~IGImpactMeshShape()
{
}
