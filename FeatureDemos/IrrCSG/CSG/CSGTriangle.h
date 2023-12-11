#ifndef CSGTRIANGLE_H
#define CSGTRIANGLE_H
/**
 * Duo Tao
 * CSGTriangle.h
 */
#ifdef WINDOWS
#include <Windows.h>
#endif

#include <stdio.h>
#include <stdlib.h>

#include "CSGVertexList.h"
#include "CSGPlane.h"
typedef struct CSGTriangle CSGTriangle;
struct CSGTriangle {
	CSGVertexList *vertices;
  	GLdouble *shared;
  	GLuint sharedDim;
 	CSGPlane *plane;
};

#include "CSGVertex.h"

CSGTriangle *CSGTriangleInitialize(CSGVertexList *vertices, GLdouble *shared, GLuint sharedDim, CSGPlane *plane);

CSGTriangle *CSGTriangleClone(CSGTriangle *triangle);

void CSGTriangleDestroy(CSGTriangle *triangle);

void CSGTriangleFlip(CSGTriangle *triangle);

CSGVertex *CSGTriangleGetVertex(CSGTriangle *triangle, GLuint i);

void CSGTriangleSetVertex(CSGTriangle *triangle, GLuint i, CSGVertex *vertex);

void CSGTriangleDump(CSGTriangle *triangle);

#endif
