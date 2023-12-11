// Copyright (C) 2009-2011 Josiah Hartzell (Skyreign Software)
// This file is part of the "irrBullet" Bullet physics extension library and wrapper.
// For conditions of distribution and use, see license in irrbullet.h
// The above copyright notice and its accompanying information must remain here.

#include "Bullet/btBulletDynamicsCommon.h"
#include "Bullet/btBulletCollisionCommon.h"
#include "Bullet/BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h"
#include "gimpactmeshshape.h"


using namespace irr;
using namespace core;
using namespace scene;

IGImpactMeshShape::IGImpactMeshShape(ISceneNode *n, IMesh *collMesh, f32 m)
{
    node = n;
    mass = m;

    type = ECST_GIMPACT;

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
