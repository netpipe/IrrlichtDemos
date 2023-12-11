/**
 * Duo Tao
 * CSGNode.c
 * BSP Tree Node
 */
#include "CSGNode.h"

CSGNode *CSGNodeInitialize(CSGTriangleList *triangles) {
	CSGNode *node = (CSGNode *) malloc(sizeof(CSGNode));
	if (node == NULL) {
		heapAllocationFailure();
	}
	node->plane = NULL;
	node->front = NULL;
	node->back = NULL;
	node->triangles = CSGTriangleListInitialize(2);
	if (node->triangles == NULL) {
		heapAllocationFailure();
	}
	if (triangles != NULL && triangles->triNum != 0) {
		CSGNodeBuild(node, triangles);
	}
	return node;
}

CSGNode *CSGNodeClone(CSGNode *node) {
	CSGNode *target = (CSGNode *) malloc(sizeof(CSGNode));
	if (target == NULL) {
		heapAllocationFailure();
	}
	target->plane = node->plane;
	target->front = node->front;
	target->back = node->back;
	target->triangles = CSGTriangleListClone(node->triangles);
	return target;
}

void CSGNodeDestroy(CSGNode *node) {
	if (node == NULL) {
		return;
	}
	CSGPlaneDestroy(node->plane);
	CSGNodeDestroy(node->front);
	CSGNodeDestroy(node->back);
	CSGTriangleListDestroy(node->triangles);
	free(node);
}

void CSGNodeInvert(CSGNode *node) {
	for (int i = 0; i < node->triangles->triNum; i++) {
    	CSGTriangleFlip(CSGTriangleListGetTriangle(node->triangles, i));
    }
    CSGPlaneFlip(node->plane);
    if (node->front != NULL) CSGNodeInvert(node->front);
    if (node->back != NULL) CSGNodeInvert(node->back);
    CSGNode *temp = node->front;
    node->front = node->back;
    node->back = temp;
}

CSGTriangleList *CSGNodeClipTriangles(CSGNode *node, CSGTriangleList *triangles) {
	if (node->plane == NULL) {
		return CSGTriangleListClone(triangles);
	}
    CSGTriangleList *front = CSGTriangleListInitialize(10);
    CSGTriangleList *back = CSGTriangleListInitialize(10);
    for (int i = 0; i < triangles->triNum; i++) {
    	CSGPlaneSplitTriangle(node->plane, triangles->triangles[i], front, back, front, back);
    }
    if (node->front) {
    	front = CSGNodeClipTriangles(node->front, front);
    }
    // This line is very profound! It eliminates all the triangles inside, 
    // even for concave meshes. Here is how it works. Concave meshes can be 
    // split into multiple convex meshes. Something inside a concave mesh must 
    // be inside a convex mesh it splits into.
    if (node->back) {
    	back = CSGNodeClipTriangles(node->back, back);
    } else {
    	CSGTriangleListClear(back);
    }
    return CSGTriangleListConcat(front, back);
}

void CSGNodeClipTo(CSGNode *node, CSGNode *bsp) {
	node->triangles = CSGNodeClipTriangles(bsp, node->triangles);
    if (node->front) {
    	CSGNodeClipTo(node->front, bsp);
    }
    if (node->back) {
    	CSGNodeClipTo(node->back, bsp);
    }
}

CSGTriangleList *CSGNodeAllTriangles(CSGNode *node) {
	CSGTriangleList *triangles = CSGTriangleListClone(node->triangles);
    if (node->front) {
    	triangles = CSGTriangleListConcat(triangles, CSGNodeAllTriangles(node->front));
    }
    if (node->back) {
    	triangles = CSGTriangleListConcat(triangles, CSGNodeAllTriangles(node->back));
    }
    return triangles;
}

void CSGNodeBuild(CSGNode *node, CSGTriangleList *triangles) {
	if (triangles->triNum == 0) {
		return;
	}
    if (node->plane == NULL) {
    	node->plane = CSGPlaneClone(triangles->triangles[0]->plane);
 	}
    CSGTriangleList *front = CSGTriangleListInitialize(5);
    CSGTriangleList *back = CSGTriangleListInitialize(5);
    for (int i = 0; i < triangles->triNum; i++) {
    	CSGPlaneSplitTriangle(node->plane, triangles->triangles[i], node->triangles, node->triangles, front, back);
    }
    if (front->triNum != 0) {
		if (node->front == NULL) {
			node->front = CSGNodeInitialize(front);
		} else {
			CSGNodeBuild(node->front, front);
		}
    }
    if (back->triNum != 0) {
		if (node->back == NULL) {
			node->back = CSGNodeInitialize(back);
		} else {
			CSGNodeBuild(node->back, back);
		}
    }
}