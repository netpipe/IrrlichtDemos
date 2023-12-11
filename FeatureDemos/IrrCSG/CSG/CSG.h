#ifndef CSG_H
#define CSG_H

#include "CSGTriangleList.h"
#include "primitives.h"

void CSGToMesh(CSGTriangleList *triangles, meshMesh *mesh);

void CSGUnion(meshMesh *meshA, meshMesh *meshB, meshMesh *meshUnion);

void CSGIntersection(meshMesh *meshA, meshMesh *meshB, meshMesh *meshIntersection);

void CSGSubtraction(meshMesh *meshA, meshMesh *meshB, meshMesh *meshSubtraction);

#endif