#ifndef CSGNODE_H
#define CSGNODE_H
/**
 * Duo Tao
 * CSGNode.h
 * BSP Tree Node
 */

#include "CSGPlane.h"
#include "CSGTriangleList.h"
#include "CSGError.h"

typedef struct CSGNode CSGNode;
struct CSGNode {
	CSGPlane *plane;
  	CSGNode *front;
  	CSGNode *back;
  	CSGTriangleList *triangles;
};

CSGNode *CSGNodeInitialize(CSGTriangleList *triangles);

CSGNode *CSGNodeClone(CSGNode *node);

void CSGNodeDestroy(CSGNode *node);

void CSGNodeInvert(CSGNode *node);

void CSGNodeCliptriangles(CSGNode *node, CSGTriangleList *triangles);

void CSGNodeClipTo(CSGNode *node, CSGNode *clipTo);

CSGTriangleList *CSGNodeAllTriangles(CSGNode *node);

void CSGNodeBuild(CSGNode *node, CSGTriangleList *triangles);

#endif