/**
 * Duo Tao
 * CSGTriangleList.c
 */
#include "CSGTriangleList.h"

CSGTriangleList *CSGTriangleListInitialize(GLuint size) {
	CSGTriangleList *list = (CSGTriangleList *) malloc(sizeof(CSGTriangleList));
	if (list == NULL) {
		heapAllocationFailure();
	}
	list->triNum = 0;
	list->size = size;
	list->triangles = (CSGTriangle **) malloc(sizeof(CSGTriangle *) * size);
	if (list->triangles == NULL) {
		heapAllocationFailure();
	}
	return list;
}

CSGTriangleList *CSGTriangleListClone(CSGTriangleList *list) {
	CSGTriangleList *target = (CSGTriangleList *) malloc(sizeof(CSGTriangleList));
	if (target == NULL) {
		heapAllocationFailure();
	}
	target->triNum = list->triNum;
	target->size = list->size;
	target->triangles = (CSGTriangle **) malloc(sizeof(CSGTriangle *) * list->triNum);
	for (int i = 0; i < list->triNum; i++) {
		target->triangles[i] = CSGTriangleClone(list->triangles[i]);
	}
	return target;
}

void CSGTriangleListPush(CSGTriangleList *list, CSGTriangle *triangle) {
	if (list->triNum == list->size) {
		list->triangles = (CSGTriangle**)realloc(list->triangles, sizeof(CSGTriangle *) * list->size * 2);
		if (list->triangles == NULL) {
			heapAllocationFailure();
		}
		list->size = list->size * 2;
	}
	list->triangles[list->triNum] = triangle;
	list->triNum++;
}

void CSGTriangleListDestroy(CSGTriangleList *list) {
	if (list == NULL) {
		return;
	}
	for (int i = 0; i < list->triNum; i++) {
		CSGTriangleDestroy(list->triangles[i]);
	}
	free(list->triangles);
	free(list);
}

void CSGTriangleListClear(CSGTriangleList *list) {
	list->triangles = (CSGTriangle**)realloc(list->triangles, sizeof(CSGTriangle *) * 1);
	if (list->triangles == NULL) {
		heapAllocationFailure();
	}
	list->triNum = 0;
	list->size = 1;
}

CSGTriangleList *CSGTriangleListConcat(CSGTriangleList *A, CSGTriangleList *B) {
	CSGTriangleList *res = CSGTriangleListInitialize(A->triNum + B->triNum);
	for (int i = 0; i < A->triNum; i++) {
		CSGTriangleListPush(res, A->triangles[i]);
	}
	for (int i = 0; i < B->triNum; i++) {
		CSGTriangleListPush(res, B->triangles[i]);
	}
	return res;
}

CSGTriangleList *CSGTriangleListFromMesh(meshMesh *mesh) {
	CSGTriangleList *list = CSGTriangleListInitialize(mesh->triNum);
	for (int i = 0; i < mesh->triNum; i++) {
		CSGVertexList *vertices = CSGVertexListInitialize(3);
		for (int v = 0; v < 3; v++) {
			GLuint vi = mesh->tri[i * 3 + v];
			CSGVertex *vert = CSGVertexInitialize(&mesh->vert[vi * mesh->attrDim], &mesh->vert[vi * mesh->attrDim + 5]);
			CSGVertexListPush(vertices, vert);
		}
		CSGPlane *plane = CSGPlaneFromPoints(CSGVertexListGetVertex(vertices, 0), CSGVertexListGetVertex(vertices, 1), CSGVertexListGetVertex(vertices, 2));
		CSGTriangle *tri = CSGTriangleInitialize(vertices, NULL, 0, plane);
		CSGTriangleListPush(list, tri);
	}
	return list;
}

CSGTriangle *CSGTriangleListGetTriangle(CSGTriangleList *list, GLuint i) {
	return list->triangles[i];
}

void CSGTriangleListDump(CSGTriangleList *list) {
	printf("triNum:%d\n", list->triNum);
	for (int i = 0; i < list->triNum; i++) {
    	printf("normal:");
    	CSGTriangle *t = CSGTriangleListGetTriangle(list, i);
    	printVector(3, t->plane->normal);
    	for (int i = 0; i < 3; i++) {
    		printVector(3, CSGTriangleGetVertex(t, i)->position);
    	}
    }
    fflush(stdout);
}