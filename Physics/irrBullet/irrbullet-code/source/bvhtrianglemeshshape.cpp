// Copyright (C) 2009-2011 Josiah Hartzell (Skyreign Software)
// This file is part of the "irrBullet" Bullet physics extension library and wrapper.
// For conditions of distribution and use, see license in irrbullet.h
// The above copyright notice and its accompanying information must remain here.

#include "btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"
#include "bvhtrianglemeshshape.h"

using namespace irr;
using namespace core;
using namespace scene;

IBvhTriangleMeshShape::IBvhTriangleMeshShape(ISceneNode *n, IMesh *collMesh, f32 m)
{
    node = n;
    mass = m;

    type = ECST_BVHTRIMESH;

    createShape(collMesh);
}


void IBvhTriangleMeshShape::createShape(IMesh *mesh)
{
    CollisionMesh = createTriangleMesh(mesh);
    shape =  new btBvhTriangleMeshShape(CollisionMesh, false, true);

    calculateLocalInertia(getMass(), vector3df(0.0f,0.0f,0.0f));
}

IBvhTriangleMeshShape::~IBvhTriangleMeshShape()
{
    //dtor
}
