#ifndef MESHDATA_H
#define MESHDATA_H
#include "vector3d.h"


struct vertexData{
	vector3d position;
	vector3d normal;
	vector3d tangent;
	vector3d color;
	float U,V;
};

struct textureData{
	unsigned int id;
	unsigned int type;
};

#endif
