#ifndef PHYSICOBJECT_H_
#define PHYSICOBJECT_H_

#include <ode/ode.h>
#include <ode/common.h>

typedef uint32 dTriIndex;


class game;

/**
 * This class is one of the base classes of GameObject.
 */
class OdeObject
{

protected:

	///A handle to the body data of the object, the main handle for physical interactions.
	dBodyID body;

	///A handle to the geometry data of the object, the main handle for collision interactions.
	dGeomID geom;

	///The mass of the object.
    dMass mass;

    ///The density of the object.
    dReal density;

	///Total weight parameter.
	dReal weight;

	///ODE surface parameters.
	dSurfaceParameters surface; //not used yet.

	///Vertex array for trimesh geom.
	dVector3* vertices;

	///Index array for trimesh geom.
	dTriIndex* indices;

	dTriMeshDataID data;

	virtual const void internalSetUpMass() = 0;

    virtual const void internalCreateGeom() = 0;

public:

	OdeObject(const dReal density, const dReal weight);

	virtual ~OdeObject();

	virtual inline const dBodyID getBody() const { return this->body; }

	virtual inline const dGeomID getGeometry() const { return this->geom; }
};

#endif /*PHYSICOBJECT_H_*/
