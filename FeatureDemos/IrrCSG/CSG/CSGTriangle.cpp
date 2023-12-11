/**
 * Duo Tao
 * CSGTriangle.c
 */
#include "CSGTriangle.h"

CSGTriangle *CSGTriangleInitialize(CSGVertexList *vertices, GLdouble *shared, GLuint sharedDim, CSGPlane *plane) {
	CSGTriangle *triangle = (CSGTriangle *) malloc(sizeof(CSGTriangle));
	if (triangle == NULL) {
		heapAllocationFailure();
	}
	triangle->vertices = vertices;
	triangle->shared = (GLdouble *)malloc(sharedDim * sizeof(GLdouble));
	for (int i = 0; i < sharedDim; i++) {
		triangle->shared[i] = shared[i];
	}
	triangle->sharedDim = sharedDim;
	triangle->plane = plane;
	return triangle;
}

CSGTriangle *CSGTriangleClone(CSGTriangle *triangle) {
	CSGTriangle *target = (CSGTriangle *) malloc(sizeof(CSGTriangle));
	if (target == NULL) {
		heapAllocationFailure();
	}
	target->vertices = CSGVertexListClone(triangle->vertices);
	target->shared = (GLdouble *)malloc(triangle->sharedDim * sizeof(GLdouble));
	for (int i = 0; i < triangle->sharedDim; i++) {
		target->shared[i] = triangle->shared[i];
	}
	target->sharedDim = triangle->sharedDim;
	target->plane = CSGPlaneClone(triangle->plane);
	return target;
}

void CSGTriangleDestroy(CSGTriangle *triangle) {
	if (triangle == NULL) {
		return;
	}
	CSGVertexListDestroy(triangle->vertices);
	CSGPlaneDestroy(triangle->plane);
	free(triangle->shared);
	free(triangle);
}

void CSGTriangleFlip(CSGTriangle *triangle) {
	for (int i = 0; i < 3; i++) {
		CSGVertexFlip(CSGTriangleGetVertex(triangle, i));
	}
	CSGVertex *temp = CSGVertexListGetVertex(triangle->vertices, 0);
	CSGVertexListSetVertex(triangle->vertices, 0, CSGVertexListGetVertex(triangle->vertices, 2));
	CSGVertexListSetVertex(triangle->vertices, 2, temp);
    CSGPlaneFlip(triangle->plane);
}

CSGVertex *CSGTriangleGetVertex(CSGTriangle *triangle, GLuint i) {
	return triangle->vertices->vertices[i];
}

void CSGTriangleSetVertex(CSGTriangle *triangle, GLuint i, CSGVertex *vertex) {
	CSGVertexListSetVertex(triangle->vertices, i, vertex);
}

void CSGTriangleDump(CSGTriangle *triangle) {
	printf("---");
	printVector(3, triangle->plane->normal);
	for (int i = 0; i < 3; i++) {
		printVector(3, CSGTriangleGetVertex(triangle, i)->position);
	}
	fflush(stdout);
}