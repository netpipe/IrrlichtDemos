
#ifndef __ODEMESH_H
#define __ODEMESH_H

#include <irrlicht.h>
#include <ode/ode.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

struct odeMesh
{
    odeMesh(dSpaceID, IAnimatedMesh *, vector3df, vector3df = vector3df(1,1,1));
    ~odeMesh();
    void setPosition(vector3df);
    void setRotation(vector3df);
private:
    int i, j;
	array <dTriMeshDataID> dTriMeshData;
	array <dGeomID> dGeom;
	array <dVector3 *> dVertices;
	array <int *> dIndices;
};

#endif
