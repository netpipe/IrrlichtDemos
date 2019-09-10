/**
 * Duo Tao
 * CSGVertexList.c
 */
#include "CSGVertexList.h"

CSGVertexList *CSGVertexListInitialize(GLuint size) {
	CSGVertexList *list = (CSGVertexList *) malloc(sizeof(CSGVertexList));
	if (list == NULL) {
		heapAllocationFailure();
	}
	list->vertNum = 0;
	list->size = size;
	list->vertices = (CSGVertex **) malloc(sizeof(CSGVertex *) * size);
	if (list->vertices == NULL) {
		heapAllocationFailure();
	}
	return list;
}

CSGVertexList *CSGVertexListClone(CSGVertexList *list) {
	CSGVertexList *target = (CSGVertexList *) malloc(sizeof(CSGVertexList));
	target->vertNum = list->vertNum;
	target->size = list->size;
	target->vertices = (CSGVertex **) malloc(sizeof(CSGVertex *) * list->size);
	for (int i = 0; i < list->vertNum; i++) {
		target->vertices[i] = CSGVertexClone(list->vertices[i]);
	}
	return target;
}

void CSGVertexListPush(CSGVertexList *list, CSGVertex *vertex) {
	if (list->vertNum == list->size) {
		list->vertices = (CSGVertex**)realloc(list->vertices, sizeof(CSGVertex *) * list->size * 2);
		if (list->vertices == NULL) {
			heapAllocationFailure();
		}
		list->size *= 2;
	}
	list->vertices[list->vertNum] = vertex;
	list->vertNum++;
}

void CSGVertexListDestroy(CSGVertexList *list) {
	if (list == NULL) {
		return;
	}
	for (int i = 0; i < list->vertNum; i++) {
		CSGVertexDestroy(list->vertices[i]);
	}
	free(list->vertices);
	free(list);
}

void CSGVertexListShallowDestroy(CSGVertexList *list) {
	free(list->vertices);
	free(list);
}

void CSGVertexListSetVertex(CSGVertexList *list, GLuint i, CSGVertex *vertex) {
	if (list == NULL) {
		return;
	}
	list->vertices[i] = vertex;
}

CSGVertex *CSGVertexListGetVertex(CSGVertexList *list, GLuint i) {
	return list->vertices[i];
}