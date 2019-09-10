#ifndef CSGVERTEX_H
#define CSGVERTEX_H
/**
 * Duo Tao
 * CSGVertex.h
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdarg.h>
#include "CSGError.h"
#include "vector.h"

typedef struct CSGVertex {
	GLdouble position[3];
  	GLdouble normal[3];
} CSGVertex;

CSGVertex *CSGVertexInitialize(GLdouble position[3], GLdouble normal[3]);

CSGVertex *CSGVertexClone(CSGVertex *vertex);

void CSGVertexDestroy(CSGVertex *vertex);

void CSGVertexFlip(CSGVertex *vertex);

CSGVertex *CSGVertexLerp(CSGVertex *A, CSGVertex *B, GLdouble t);

#endif