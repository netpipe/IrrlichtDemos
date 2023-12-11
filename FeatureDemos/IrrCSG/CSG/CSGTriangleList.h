#ifndef CSGTRIANGLELIST_H
#define CSGTRIANGLELIST_H
/**
 * Duo Tao
 * CSGTriangleList.h
 */
#include <stdio.h>
#include <stdlib.h>
#include "primitives.h"

struct CSGTriangle;
typedef struct CSGTriangle CSGTriangle;

#include "CSGTriangle.h"


typedef struct CSGTriangleList {
	GLuint triNum;
	GLuint size;
	CSGTriangle **triangles;
} CSGTriangleList;

CSGTriangleList *CSGTriangleListInitialize(GLuint size);

CSGTriangleList *CSGTriangleListClone(CSGTriangleList *list);

void CSGTriangleListPush(CSGTriangleList *list, CSGTriangle *triangle);

void CSGTriangleListDestroy(CSGTriangleList *list);

void CSGTriangleListClear(CSGTriangleList *list);

CSGTriangleList *CSGTriangleListConcat(CSGTriangleList *A, CSGTriangleList *B);

/**
 * Converts a mesh to a triangle list
 */
CSGTriangleList *CSGTriangleListFromMesh(meshMesh *mesh);

CSGTriangle *CSGTriangleListGetTriangle(CSGTriangleList *list, GLuint i);

void CSGTriangleListDump(CSGTriangleList *list);
#endif