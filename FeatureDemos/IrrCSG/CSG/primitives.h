#ifndef PRIMITIVES_H
#define PRIMITIVES_H
/**
 * Duo Tao
 * primitives.h
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdarg.h>
#include "vector.h"

/*** Creating and destroying ***/

/* Feel free to read the struct's members, but don't write them, except through 
the accessors below such as meshSetTriangle, meshSetVertex. */
typedef struct meshMesh meshMesh;
struct meshMesh {
	GLuint triNum, vertNum, attrDim;
	GLuint *tri;						/* triNum * 3 GLuints */
	GLdouble *vert;					/* vertNum * attrDim GLdoubles */
};

void meshTranslate(meshMesh *mesh, GLdouble translation[3]);

/* Initializes a mesh with enough memory to hold its triangles and vertices. 
Does not actually fill in those triangles or vertices with useful data. When 
you are finished with the mesh, you must call meshDestroy to deallocate its 
backing resources. */
int meshInitialize(meshMesh *mesh, GLuint triNum, GLuint vertNum, 
		GLuint attrDim);

/* Sets the trith triangle to have vertex indices i, j, k. */
void meshSetTriangle(meshMesh *mesh, GLuint tri, GLuint i, GLuint j, GLuint k);

/* Returns a pointer to the trith triangle. For example:
	GLuint *triangle13 = meshGetTrianglePointer(&mesh, 13);
	printf("%d, %d, %d\n", triangle13[0], triangle13[1], triangle13[2]); */
GLuint *meshGetTrianglePointer(meshMesh *mesh, GLuint tri);

/* Sets the vertth vertex to have attributes attr. */
void meshSetVertex(meshMesh *mesh, GLuint vert, GLdouble attr[]);

/* Returns a pointer to the vertth vertex. For example:
	GLdouble *vertex13 = meshGetVertexPointer(&mesh, 13);
	printf("x = %f, y = %f\n", vertex13[0], vertex13[1]); */
GLdouble *meshGetVertexPointer(meshMesh *mesh, GLuint vert);

/* Deallocates the resources backing the mesh. This function must be called 
when you are finished using a mesh. */
void meshDestroy(meshMesh *mesh);



/*** OpenGL ***/

/* Feel free to read from this struct's members, but don't write to them, 
except through accessor functions. */
typedef struct meshGLMesh meshGLMesh;
struct meshGLMesh {
	GLuint triNum, vertNum, attrDim;
	GLuint buffers[2];
};

/* Initializes an OpenGL mesh from a non-OpenGL mesh. */
void meshGLInitialize(meshGLMesh *meshGL, meshMesh *mesh);

#define BUFFER_OFFSET(bytes) ((GLubyte*) NULL + (bytes))

/* Renders the already-initialized OpenGL mesh. attrDims is an array of length 
attrNum. For each i, its ith entry is the dimension of the ith attribute 
vector. Similarly, attrLocs is an array of length attrNum, giving the location 
of the ith attribute in the active OpenGL shader program. */
void meshGLRender(meshGLMesh *meshGL, GLuint attrNum, GLuint attrDims[], 
		GLint attrLocs[]);

/* Deallocates the resources backing the initialized OpenGL mesh. */
void meshGLDestroy(meshGLMesh *meshGL);

/*** Convenience initializers: 3D ***/

/* Assumes that attributes 0, 1, 2 are XYZ. Assumes that the vertices of the 
triangle are in counter-clockwise order when viewed from 'outside' the 
triangle. Computes the outward-pointing unit normal vector for the triangle. */
void meshTrueNormal(GLdouble a[], GLdouble b[], GLdouble c[], 
		GLdouble normal[3]);

/* Assumes that attributes 0, 1, 2 are XYZ. Sets attributes n, n + 1, n + 2 to 
flat-shaded normals. If a vertex belongs to more than triangle, then some 
unspecified triangle's normal wins. */
void meshFlatNormals(meshMesh *mesh, GLuint n);

/* Assumes that attributes 0, 1, 2 are XYZ. Sets attributes n, n + 1, n + 2 to 
smooth-shaded normals. Does not do anything special to handle multiple vertices 
with the same coordinates. */
void meshSmoothNormals(meshMesh *mesh, GLuint n);

/* Builds a mesh for a parallelepiped (box) of the given size. The attributes 
are XYZ position, ST texture, and NOP unit normal vector. The normals are 
discontinuous at the edges (flat shading, not smooth). To facilitate this, some 
vertices have equal XYZ but different NOP, for 24 vertices in all. Don't forget 
to meshDestroy when finished. */
int meshInitializeBox(meshMesh *mesh, GLdouble left, GLdouble right, 
		GLdouble bottom, GLdouble top, GLdouble base, GLdouble lid);

/* Rotates a 2-dimensional vector through an angle. The input can safely alias 
the output. */
void meshRotateVector(GLdouble theta, GLdouble v[2], GLdouble vRot[2]);

/* Rotate a curve about the Z-axis. Can be used to make a sphere, spheroid, 
capsule, circular cone, circular cylinder, box, etc. The z-values should be in 
ascending order --- or at least the first z should be less than the last. The 
first and last r-values should be 0.0, and no others. Probably the t-values 
should be in ascending or descending order. The sideNum parameter controls the 
fineness of the mesh. The attributes are XYZ position, ST texture, and NOP unit 
normal vector. The normals are smooth. Don't forget to meshDestroy when 
finished. */
int meshInitializeRevolution(meshMesh *mesh, GLuint zNum, GLdouble z[], 
		GLdouble r[], GLdouble t[], GLuint sideNum);

/* Builds a mesh for a sphere, centered at the origin, of radius r. The sideNum 
and layerNum parameters control the fineness of the mesh. The attributes are 
XYZ position, ST texture, and NOP unit normal vector. The normals are smooth. 
Don't forget to meshDestroy when finished. */
int meshInitializeSphere(meshMesh *mesh, GLdouble r, GLuint layerNum, 
		GLuint sideNum);

/* Builds a mesh for a circular cylinder with spherical caps, centered at the 
origin, of radius r and length l > 2 * r. The sideNum and layerNum parameters 
control the fineness of the mesh. The attributes are XYZ position, ST texture, 
and NOP unit normal vector. The normals are smooth. Don't forget to meshDestroy 
when finished. */
int meshInitializeCapsule(meshMesh *mesh, GLdouble r, GLdouble l, 
		GLuint layerNum, GLuint sideNum);
#endif