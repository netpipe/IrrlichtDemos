#include "OdeObject.h"

OdeObject::OdeObject(const dReal d, const dReal weight) :
	body(0), geom(0), density(d), weight(weight)
{
	memset(&surface, 0, sizeof(surface));
	this->surface.mu = 1;
	//surface.soft_erp = 0.8;
	//surface.soft_cfm = 0.01;
}

OdeObject::~OdeObject()
{
	if (this->geom)
		dGeomDestroy(this->geom);

	if (this->body)
		dBodyDestroy(this->body);

	dGeomTriMeshDataDestroy(this->data);

    delete[] this->vertices;
    delete[] this->indices;
}
