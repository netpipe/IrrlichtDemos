/**
 * Duo Tao
 * CSGVertex.c
 */
#include "CSGVertex.h"

CSGVertex *CSGVertexInitialize(GLdouble position[3], GLdouble normal[3]) {
	CSGVertex *vertex = (CSGVertex *) malloc(sizeof(CSGVertex));
	if (vertex == NULL) {
		heapAllocationFailure();
	}
	vecCopy(3, position, vertex->position);
	vecCopy(3, normal, vertex->normal);
	return vertex;
}

CSGVertex *CSGVertexClone(CSGVertex *vertex) {
	CSGVertex *target = (CSGVertex *) malloc(sizeof(CSGVertex));
	if (target == NULL) {
		heapAllocationFailure();
	}
	vecCopy(3, vertex->position, target->position);
	vecCopy(3, vertex->normal, target->normal);
	return target;
}

void CSGVertexDestroy(CSGVertex *vertex) {
	free(vertex);
}

void CSGVertexFlip(CSGVertex *vertex) {
	vecScale(3, -1, vertex->normal, vertex->normal);
}

CSGVertex *CSGVertexLerp(CSGVertex *A, CSGVertex *B, GLdouble t) {
	CSGVertex *res = (CSGVertex *) malloc(sizeof(CSGVertex));
	if (res == NULL) {
		heapAllocationFailure();
	}
	vecLerp(3, A->position, B->position, res->position, t);
	vecLerp(3, A->normal, B->normal, res->normal, t);
	return res;
}