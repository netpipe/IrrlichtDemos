/**
 * Duo Tao
 * CSG.c
 */
#include "CSG.h"
#include "CSGNode.h"

void CSGToMesh(CSGTriangleList *triangles, meshMesh *mesh) {
	GLuint attrDim = 8;
	if (meshInitialize(mesh, triangles->triNum, triangles->triNum * 3, attrDim)) {
		externalError();
	}
	for (int i = 0; i < triangles->triNum; i++) {
		meshSetTriangle(mesh, i, i * 3, i * 3 + 1, i * 3 + 2);
		CSGTriangle *tri = CSGTriangleListGetTriangle(triangles, i);

		GLdouble vert[8];
		CSGVertex *v = CSGTriangleGetVertex(tri, 0);
		vecSet(8, vert, v->position[0], v->position[1], v->position[2], 0.0, 0.0, 
			v->normal[0], v->normal[1], v->normal[2]);
		meshSetVertex(mesh, i * 3, vert);

		v = CSGTriangleGetVertex(tri, 1);
		vecSet(8, vert, v->position[0], v->position[1], v->position[2], 0.0, 0.0, 
			v->normal[0], v->normal[1], v->normal[2]);
		meshSetVertex(mesh, i * 3 + 1, vert);

		v = CSGTriangleGetVertex(tri, 2);
		vecSet(8, vert, v->position[0], v->position[1], v->position[2], 0.0, 0.0, 
			v->normal[0], v->normal[1], v->normal[2]);
		meshSetVertex(mesh, i * 3 + 2, vert);
	}
}

void CSGUnion(meshMesh *meshA, meshMesh *meshB, meshMesh *meshUnion) {
	CSGTriangleList *la = CSGTriangleListFromMesh(meshA);
	CSGTriangleList *lb = CSGTriangleListFromMesh(meshB);
	CSGNode *a = CSGNodeInitialize(la);
	CSGNode *b = CSGNodeInitialize(lb);
    CSGNodeClipTo(a, b);
    CSGNodeClipTo(b, a);
    CSGNodeInvert(b);
    CSGNodeClipTo(b, a);
    CSGNodeInvert(b);
    CSGNodeBuild(a, CSGNodeAllTriangles(b));
    CSGToMesh(CSGNodeAllTriangles(a), meshUnion);
    CSGTriangleListDestroy(la);
    CSGTriangleListDestroy(lb);
    // CSGNodeDestroy(a);
    // CSGNodeDestroy(b); // commented because does not work
}

void CSGIntersection(meshMesh *meshA, meshMesh *meshB, meshMesh *meshIntersection) {
	CSGTriangleList *la = CSGTriangleListFromMesh(meshA);
	CSGTriangleList *lb = CSGTriangleListFromMesh(meshB);
	CSGNode *a = CSGNodeInitialize(la);
	CSGNode *b = CSGNodeInitialize(lb);
    CSGNodeInvert(a);
    CSGNodeClipTo(b, a);
    CSGNodeInvert(b);
    CSGNodeClipTo(a, b);
    CSGNodeClipTo(b, a);
    CSGNodeBuild(a, CSGNodeAllTriangles(b));
    CSGNodeInvert(a);
    CSGToMesh(CSGNodeAllTriangles(a), meshIntersection);
    CSGTriangleListDestroy(la);
    CSGTriangleListDestroy(lb);
}

void CSGSubtraction(meshMesh *meshA, meshMesh *meshB, meshMesh *meshSubtraction) {
	CSGTriangleList *la = CSGTriangleListFromMesh(meshA);
	CSGTriangleList *lb = CSGTriangleListFromMesh(meshB);
	CSGNode *a = CSGNodeInitialize(la);
	CSGNode *b = CSGNodeInitialize(lb);
    CSGNodeInvert(a);
    CSGNodeClipTo(a, b);
    CSGNodeClipTo(b, a);
    CSGNodeInvert(b);
    CSGNodeClipTo(b, a);
    CSGNodeInvert(b);
    CSGNodeBuild(a, CSGNodeAllTriangles(b));
    CSGNodeInvert(a);
    CSGToMesh(CSGNodeAllTriangles(a), meshSubtraction);
    CSGTriangleListDestroy(la);
    CSGTriangleListDestroy(lb);
}