#ifndef CSGVERTEXLIST_H
#define CSGVERTEXLIST_H
/**
 * Duo Tao
 * CSGVertexList.h
 */
#include <stdio.h>


#include "CSGVertex.h"

typedef struct CSGVertexList {
	int vertNum;
	int size;
	CSGVertex **vertices;
} CSGVertexList;

CSGVertexList *CSGVertexListInitialize(GLuint size);

CSGVertexList *CSGVertexListClone(CSGVertexList *list);

void CSGVertexListPush(CSGVertexList *list, CSGVertex *vertex);

void CSGVertexListDestroy(CSGVertexList *list);

// needed for temporary list
void CSGVertexListShallowDestroy(CSGVertexList *list);

void CSGVertexListSetVertex(CSGVertexList *list, GLuint i, CSGVertex *vertex);

CSGVertex *CSGVertexListGetVertex(CSGVertexList *list, GLuint i);

#endif