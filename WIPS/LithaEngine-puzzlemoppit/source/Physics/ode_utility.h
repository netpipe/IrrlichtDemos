
#ifndef ODE_UTILITY_H
#define ODE_UTILITY_H

#include "litha_internal.h"
#include <ode/ode.h>

core::matrix4 ODEGetBodyTransformation( dBodyID b );
core::matrix4 ODEGetGeomTransformation( dGeomID g );

void ODESetBodyTransformation( dBodyID b, const core::matrix4 &m );
void ODESetGeomTransformation( dGeomID g, const core::matrix4 &m );

core::vector3df ODEGetBodyPosition( dBodyID b );
core::vector3df ODEGetGeomPosition( dGeomID g );

void ODESetBodyPosition( dBodyID b, const core::vector3df &pos );
void ODESetGeomPosition( dGeomID g, const core::vector3df &pos );

core::vector3df ODEGetBodyRotation( dBodyID b );
core::vector3df ODEGetGeomRotation( dGeomID g );

void ODESetBodyRotation( dBodyID b, const core::vector3df &rot );
void ODESetGeomRotation( dGeomID g, const core::vector3df &rot );

// Note: This struct must be destructed BEFORE dCloseODE() is called.
struct ODEMeshData
{
	dTriMeshDataID triMeshData;
	
	dVector3 *vertices;
	int *indices;
	
	core::vector3df size;
	
	~ODEMeshData()
	{
		dGeomTriMeshDataDestroy(triMeshData);
		delete [] vertices;
		delete [] indices;
	}
};

// Creates ODE trimesh data.
// Dynamically allocated, should delete before dCloseODE()
ODEMeshData *ODECreateMeshData(scene::IMesh *mesh, core::vector3df scale);

#endif
