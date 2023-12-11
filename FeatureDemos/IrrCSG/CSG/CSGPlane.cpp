/**
 * Duo Tao
 * CSGPlane.c
 */
#include "CSGPlane.h"

CSGPlane *CSGPlaneInitialize( GLdouble normal[3], GLdouble w) {
	CSGPlane *plane = (CSGPlane *) malloc(sizeof(CSGPlane));
	if (plane == NULL) {
		heapAllocationFailure();
	}
	vecCopy(3, normal, plane->normal);
	plane->w = w;
	return plane;
}

CSGPlane *CSGPlaneClone(CSGPlane *plane) {
	CSGPlane *target = (CSGPlane *) malloc(sizeof(CSGPlane));
	if (plane == NULL) {
		heapAllocationFailure();
	}
	vecCopy(3, plane->normal, target->normal);
	target->w = plane->w;
	return target;
}

CSGPlane *CSGPlaneFromPoints(CSGVertex *A, CSGVertex *B, CSGVertex *C) {
	CSGPlane *plane = (CSGPlane *) malloc(sizeof(CSGPlane));
	if (plane == NULL) {
		heapAllocationFailure();
	}
	// cross: normal vector n finally
	GLdouble ab[3], ac[3], cross[3];
	vecSubtract(3, B->position, A->position, ab);
	vecSubtract(3, C->position, A->position, ac);
	vec3Cross(ab, ac, cross);
	vecUnit(3, cross, cross);
	vecCopy(3, cross, plane->normal);
	// w = a \dot n
	plane->w = vecDot(3, cross, A->position);
	return plane;
}

void CSGPlaneDestroy(CSGPlane *plane) {
	free(plane);
}

void CSGPlaneFlip(CSGPlane *plane) {
	vecScale(3, -1, plane->normal, plane->normal);
	plane->w = -plane->w;
}

void CSGPlaneSplitTriangle(CSGPlane *plane, CSGTriangle *triangle, CSGTriangleList *coplanarFront, 
	CSGTriangleList *coplanarBack, CSGTriangleList *front, CSGTriangleList *back) {
	static const unsigned char COPLANAR = 0;
    static const unsigned char FRONT = 1;
    static const unsigned char BACK = 2;
    static const unsigned char SPANNING = 3;

    // Classify each point as well as the entire triangle into one of the above
    // four classes.
    unsigned char triangleType = 0;
    unsigned char types[3];
    for (int i = 0; i < 3; i++) {
		GLdouble t = vecDot(3, plane->normal, CSGTriangleGetVertex(triangle, i)->position) - plane->w;
		unsigned char type;
		if (t < -EPSILON) {
			type = BACK;
		} else if (t > EPSILON) {
			type = FRONT;
		} else {
			type = COPLANAR;
		}
		triangleType |= type;
		types[i] =type;
    }
    // Put the triangle in the correct list, splitting it when necessary.
    CSGVertexList *f;
    CSGVertexList *b;
    switch (triangleType) {
		case COPLANAR:
			CSGTriangleListPush((vecDot(3, plane->normal, triangle->plane->normal) > 0 ? coplanarFront : coplanarBack), triangle);
			break;
		case FRONT:
			CSGTriangleListPush(front, triangle);
			break;
		case BACK:
			CSGTriangleListPush(back, triangle);
			break;
		case SPANNING:
			f = CSGVertexListInitialize(4);
			b = CSGVertexListInitialize(4);
			for (int i = 0; i < 3; i++) {
				int j = (i + 1) % 3;
				unsigned char ti = types[i], tj = types[j];
				CSGVertex *vi = CSGTriangleGetVertex(triangle, i);
				CSGVertex *vj = CSGTriangleGetVertex(triangle, j);
				if (ti != BACK) {
					CSGVertexListPush(f, vi);
				}
				if (ti != FRONT) {
					if (ti != BACK) {
						CSGVertexListPush(b, CSGVertexClone(vi));
					} else {
						CSGVertexListPush(b, vi);
					}
				}
				if ((ti | tj) == SPANNING) {
					GLdouble ij[3];
					vecSubtract(3, vj->position, vi->position, ij);
					GLdouble t = (plane->w - vecDot(3, plane->normal, vi->position)) / vecDot(3, plane->normal, ij);
					CSGVertex *v = CSGVertexLerp(vi, vj, t);
					CSGVertexListPush(f, v);
					CSGVertexListPush(b, CSGVertexClone(v));
				}
			}
			if (f->vertNum == 3) {
				CSGTriangleListPush(front, CSGTriangleInitialize(f, triangle->shared, triangle->sharedDim, triangle->plane));
			} else if (f->vertNum == 4) {
				CSGVertexList *triOne = CSGVertexListInitialize(3);
				CSGVertexListPush(triOne, f->vertices[0]);
				CSGVertexListPush(triOne, f->vertices[1]);
				CSGVertexListPush(triOne, f->vertices[2]);
				CSGVertexList *triTwo = CSGVertexListInitialize(3);
				CSGVertexListPush(triTwo, CSGVertexClone(f->vertices[0]));
				CSGVertexListPush(triTwo, CSGVertexClone(f->vertices[2]));
				CSGVertexListPush(triTwo, f->vertices[3]);
				CSGTriangleListPush(front, CSGTriangleInitialize(triOne, triangle->shared, triangle->sharedDim, triangle->plane));
				CSGTriangleListPush(front, CSGTriangleInitialize(triTwo, triangle->shared, triangle->sharedDim, triangle->plane));
				CSGVertexListShallowDestroy(f); // destroy it because it is split into two lists
			} else {
				triangleSplitError();
			}
			if (b->vertNum == 3) {
				CSGTriangleListPush(back, CSGTriangleInitialize(b, triangle->shared, triangle->sharedDim, triangle->plane));
			} else if (b->vertNum == 4) {
				CSGVertexList *triOne = CSGVertexListInitialize(3);
				CSGVertexListPush(triOne, b->vertices[0]);
				CSGVertexListPush(triOne, b->vertices[1]);
				CSGVertexListPush(triOne, b->vertices[2]);
				CSGVertexList *triTwo = CSGVertexListInitialize(3);
				CSGVertexListPush(triTwo, CSGVertexClone(b->vertices[0]));
				CSGVertexListPush(triTwo, CSGVertexClone(b->vertices[2]));
				CSGVertexListPush(triTwo, b->vertices[3]);
				CSGTriangleListPush(back, CSGTriangleInitialize(triOne, triangle->shared, triangle->sharedDim, triangle->plane));
				CSGTriangleListPush(back, CSGTriangleInitialize(triTwo, triangle->shared, triangle->sharedDim, triangle->plane));
				CSGVertexListShallowDestroy(b); // destroy it because it is split into two lists
			} else {
				triangleSplitError();
			}
			break;
    }
}

void CSGPlaneDump(CSGPlane *plane) {
	printf("normal:");
	printVector(3, plane->normal);
	printf("w:%f\n", plane->w);
	fflush(stdout);
}


