/**
 * Duo Tao
 * primitives.c
 */
#include "primitives.h"

#define M_PI 3.14159265359

/*** Creating and destroying ***/
void meshTranslate(meshMesh *mesh, GLdouble translation[3]) {
	for (int i = 0; i < mesh->vertNum; i++) {
		mesh->vert[i * mesh->attrDim] += translation[0]; 
		mesh->vert[i * mesh->attrDim + 1] += translation[1]; 
		mesh->vert[i * mesh->attrDim + 2] += translation[2]; 
	}
}

/* Initializes a mesh with enough memory to hold its triangles and vertices. 
Does not actually fill in those triangles or vertices with useful data. When 
you are finished with the mesh, you must call meshDestroy to deallocate its 
backing resources. */
int meshInitialize(meshMesh *mesh, GLuint triNum, GLuint vertNum, 
		GLuint attrDim) {
	mesh->tri = (GLuint *)malloc(triNum * 3 * sizeof(GLuint) +
		vertNum * attrDim * sizeof(GLdouble));
	if (mesh->tri != NULL) {
		mesh->vert = (GLdouble *)&(mesh->tri[triNum * 3]);
		mesh->triNum = triNum;
		mesh->vertNum = vertNum;
		mesh->attrDim = attrDim;
	}
	return (mesh->tri == NULL);
}

/* Sets the trith triangle to have vertex indices i, j, k. */
void meshSetTriangle(meshMesh *mesh, GLuint tri, GLuint i, GLuint j, GLuint k) {
	if (tri < mesh->triNum) {
		mesh->tri[3 * tri] = i;
		mesh->tri[3 * tri + 1] = j;
		mesh->tri[3 * tri + 2] = k;
	}
}

/* Returns a pointer to the trith triangle. For example:
	GLuint *triangle13 = meshGetTrianglePointer(&mesh, 13);
	printf("%d, %d, %d\n", triangle13[0], triangle13[1], triangle13[2]); */
GLuint *meshGetTrianglePointer(meshMesh *mesh, GLuint tri) {
	if (tri < mesh->triNum)
		return &mesh->tri[tri * 3];
	else
		return NULL;
}

/* Sets the vertth vertex to have attributes attr. */
void meshSetVertex(meshMesh *mesh, GLuint vert, GLdouble attr[]) {
	GLuint k;
	if (vert < mesh->vertNum)
		for (k = 0; k < mesh->attrDim; k += 1)
			mesh->vert[mesh->attrDim * vert + k] = attr[k];
}

/* Returns a pointer to the vertth vertex. For example:
	GLdouble *vertex13 = meshGetVertexPointer(&mesh, 13);
	printf("x = %f, y = %f\n", vertex13[0], vertex13[1]); */
GLdouble *meshGetVertexPointer(meshMesh *mesh, GLuint vert) {
	if (vert < mesh->vertNum)
		return &mesh->vert[vert * mesh->attrDim];
	else
		return NULL;
}

/* Deallocates the resources backing the mesh. This function must be called 
when you are finished using a mesh. */
void meshDestroy(meshMesh *mesh) {
	free(mesh->tri);
}



/*** OpenGL ***/
/* Initializes an OpenGL mesh from a non-OpenGL mesh. */
void meshGLInitialize(meshGLMesh *meshGL, meshMesh *mesh) {
/*	glGenBuffers(2, meshGL->buffers);
	glBindBuffer(GL_ARRAY_BUFFER, meshGL->buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, mesh->vertNum * mesh->attrDim * sizeof(GLdouble),
		(GLvoid *)mesh->vert, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshGL->buffers[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->triNum * 3 * sizeof(GLuint),
		(GLvoid *)mesh->tri, GL_STATIC_DRAW);
	meshGL->triNum = mesh->triNum;
	meshGL->vertNum = mesh->vertNum;
	meshGL->attrDim = mesh->attrDim;
	*/
}

#define BUFFER_OFFSET(bytes) ((GLubyte*) NULL + (bytes))

/* Renders the already-initialized OpenGL mesh. attrDims is an array of length 
attrNum. For each i, its ith entry is the dimension of the ith attribute 
vector. Similarly, attrLocs is an array of length attrNum, giving the location 
of the ith attribute in the active OpenGL shader program. */
void meshGLRender(meshGLMesh *meshGL, GLuint attrNum, GLuint attrDims[], 
		GLint attrLocs[]) {
/*	// total dims
	GLuint attrDim = 0;
	for (int i = 0; i < attrNum; i++) {
		attrDim += attrDims[i];
	}
	// bind the buffers
	glBindBuffer(GL_ARRAY_BUFFER, meshGL->buffers[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshGL->buffers[1]);
	// get the attributes
	GLuint offset = 0;
	for (int i = 0; i < attrNum; i++) {
		glEnableVertexAttribArray(attrLocs[i]);
		glVertexAttribPointer(attrLocs[i], attrDims[i], GL_DOUBLE, GL_FALSE, 
			attrDim * sizeof(GLdouble), BUFFER_OFFSET(offset * sizeof(GLdouble)));
		offset += attrDims[i];
	}
	// draw it
	glDrawElements(GL_TRIANGLES, meshGL->triNum * 3, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
	// Disable the attributes when finished with them.
	for (int i = 0; i < attrNum; i++) {
		glDisableVertexAttribArray(attrLocs[i]);
	}
	*/
}

/* Deallocates the resources backing the initialized OpenGL mesh. */
void meshGLDestroy(meshGLMesh *meshGL) {
    //glDeleteBuffers(2, meshGL->buffers);
}


/*** Convenience initializers: 3D ***/

/* Assumes that attributes 0, 1, 2 are XYZ. Assumes that the vertices of the 
triangle are in counter-clockwise order when viewed from 'outside' the 
triangle. Computes the outward-pointing unit normal vector for the triangle. */
void meshTrueNormal(GLdouble a[], GLdouble b[], GLdouble c[], 
		GLdouble normal[3]) {
	GLdouble bMinusA[3], cMinusA[3];
	vecSubtract(3, b, a, bMinusA);
	vecSubtract(3, c, a, cMinusA);
	vec3Cross(bMinusA, cMinusA, normal);
	vecUnit(3, normal, normal);
}

/* Assumes that attributes 0, 1, 2 are XYZ. Sets attributes n, n + 1, n + 2 to 
flat-shaded normals. If a vertex belongs to more than triangle, then some 
unspecified triangle's normal wins. */
void meshFlatNormals(meshMesh *mesh, GLuint n) {
	GLuint i, *tri;
	GLdouble *a, *b, *c, normal[3];
	for (i = 0; i < mesh->triNum; i += 1) {
		tri = meshGetTrianglePointer(mesh, i);
		a = meshGetVertexPointer(mesh, tri[0]);
		b = meshGetVertexPointer(mesh, tri[1]);
		c = meshGetVertexPointer(mesh, tri[2]);
		meshTrueNormal(a, b, c, normal);
		vecCopy(3, normal, &a[n]);
		vecCopy(3, normal, &b[n]);
		vecCopy(3, normal, &c[n]);
	}
}

/* Assumes that attributes 0, 1, 2 are XYZ. Sets attributes n, n + 1, n + 2 to 
smooth-shaded normals. Does not do anything special to handle multiple vertices 
with the same coordinates. */
void meshSmoothNormals(meshMesh *mesh, GLuint n) {
	GLuint i, *tri;
	GLdouble *a, *b, *c, normal[3] = {0.0, 0.0, 0.0};
	/* Zero the normals. */
	for (i = 0; i < mesh->vertNum; i += 1) {
		a = meshGetVertexPointer(mesh, i);
		vecCopy(3, normal, &a[n]);
	}
	/* For each triangle, add onto the normal at each of its vertices. */
	for (i = 0; i < mesh->triNum; i += 1) {
		tri = meshGetTrianglePointer(mesh, i);
		a = meshGetVertexPointer(mesh, tri[0]);
		b = meshGetVertexPointer(mesh, tri[1]);
		c = meshGetVertexPointer(mesh, tri[2]);
		meshTrueNormal(a, b, c, normal);
		vecAdd(3, normal, &a[n], &a[n]);
		vecAdd(3, normal, &b[n], &b[n]);
		vecAdd(3, normal, &c[n], &c[n]);
	}
	/* Normalize the normals. */
	for (i = 0; i < mesh->vertNum; i += 1) {
		a = meshGetVertexPointer(mesh, i);
		vecUnit(3, &a[n], &a[n]);
	}
}

/* Builds a mesh for a parallelepiped (box) of the given size. The attributes 
are XYZ position, ST texture, and NOP unit normal vector. The normals are 
discontinuous at the edges (flat shading, not smooth). To facilitate this, some 
vertices have equal XYZ but different NOP, for 24 vertices in all. Don't forget 
to meshDestroy when finished. */
int meshInitializeBox(meshMesh *mesh, GLdouble left, GLdouble right, 
		GLdouble bottom, GLdouble top, GLdouble base, GLdouble lid) {
	GLuint error = meshInitialize(mesh, 12, 24, 3 + 2 + 3);
	if (error == 0) {
		/* Make the triangles. */
		meshSetTriangle(mesh, 0, 0, 2, 1);
		meshSetTriangle(mesh, 1, 0, 3, 2);
		meshSetTriangle(mesh, 2, 4, 5, 6);
		meshSetTriangle(mesh, 3, 4, 6, 7);
		meshSetTriangle(mesh, 4, 8, 10, 9);
		meshSetTriangle(mesh, 5, 8, 11, 10);
		meshSetTriangle(mesh, 6, 12, 13, 14);
		meshSetTriangle(mesh, 7, 12, 14, 15);
		meshSetTriangle(mesh, 8, 16, 18, 17);
		meshSetTriangle(mesh, 9, 16, 19, 18);
		meshSetTriangle(mesh, 10, 20, 21, 22);
		meshSetTriangle(mesh, 11, 20, 22, 23);
		/* Make the vertices after 0, using vertex 0 as temporary storage. */
		GLdouble *v = mesh->vert;
		vecSet(8, v, right, bottom, base, 1.0, 0.0, 0.0, 0.0, -1.0);
		meshSetVertex(mesh, 1, v);
		vecSet(8, v, right, top, base, 1.0, 1.0, 0.0, 0.0, -1.0);
		meshSetVertex(mesh, 2, v);
		vecSet(8, v, left, top, base, 0.0, 1.0, 0.0, 0.0, -1.0);
		meshSetVertex(mesh, 3, v);
		vecSet(8, v, left, bottom, lid, 0.0, 0.0, 0.0, 0.0, 1.0);
		meshSetVertex(mesh, 4, v);
		vecSet(8, v, right, bottom, lid, 1.0, 0.0, 0.0, 0.0, 1.0);
		meshSetVertex(mesh, 5, v);
		vecSet(8, v, right, top, lid, 1.0, 1.0, 0.0, 0.0, 1.0);
		meshSetVertex(mesh, 6, v);
		vecSet(8, v, left, top, lid, 0.0, 1.0, 0.0, 0.0, 1.0);
		meshSetVertex(mesh, 7, v);
		vecSet(8, v, left, top, base, 0.0, 1.0, 0.0, 1.0, 0.0);
		meshSetVertex(mesh, 8, v);
		vecSet(8, v, right, top, base, 1.0, 1.0, 0.0, 1.0, 0.0);
		meshSetVertex(mesh, 9, v);
		vecSet(8, v, right, top, lid, 1.0, 1.0, 0.0, 1.0, 0.0);
		meshSetVertex(mesh, 10, v);
		vecSet(8, v, left, top, lid, 0.0, 1.0, 0.0, 1.0, 0.0);
		meshSetVertex(mesh, 11, v);
		vecSet(8, v, left, bottom, base, 0.0, 0.0, 0.0, -1.0, 0.0);
		meshSetVertex(mesh, 12, v);
		vecSet(8, v, right, bottom, base, 1.0, 0.0, 0.0, -1.0, 0.0);
		meshSetVertex(mesh, 13, v);
		vecSet(8, v, right, bottom, lid, 1.0, 0.0, 0.0, -1.0, 0.0);
		meshSetVertex(mesh, 14, v);
		vecSet(8, v, left, bottom, lid, 0.0, 0.0, 0.0, -1.0, 0.0);
		meshSetVertex(mesh, 15, v);
		vecSet(8, v, right, top, base, 1.0, 1.0, 1.0, 0.0, 0.0);
		meshSetVertex(mesh, 16, v);
		vecSet(8, v, right, bottom, base, 1.0, 0.0, 1.0, 0.0, 0.0);
		meshSetVertex(mesh, 17, v);
		vecSet(8, v, right, bottom, lid, 1.0, 0.0, 1.0, 0.0, 0.0);
		meshSetVertex(mesh, 18, v);
		vecSet(8, v, right, top, lid, 1.0, 1.0, 1.0, 0.0, 0.0);
		meshSetVertex(mesh, 19, v);
		vecSet(8, v, left, top, base, 0.0, 1.0, -1.0, 0.0, 0.0);
		meshSetVertex(mesh, 20, v);
		vecSet(8, v, left, bottom, base, 0.0, 0.0, -1.0, 0.0, 0.0);
		meshSetVertex(mesh, 21, v);
		vecSet(8, v, left, bottom, lid, 0.0, 0.0, -1.0, 0.0, 0.0);
		meshSetVertex(mesh, 22, v);
		vecSet(8, v, left, top, lid, 0.0, 1.0, -1.0, 0.0, 0.0);
		meshSetVertex(mesh, 23, v);
		/* Now make vertex 0 for realsies. */
		vecSet(8, v, left, bottom, base, 0.0, 0.0, 0.0, 0.0, -1.0);
	}
	return error;
}

/* Rotates a 2-dimensional vector through an angle. The input can safely alias 
the output. */
void meshRotateVector(GLdouble theta, GLdouble v[2], GLdouble vRot[2]) {
	GLdouble cosTheta = cos(theta);
	GLdouble sinTheta = sin(theta);
	GLdouble vRot0 = cosTheta * v[0] - sinTheta * v[1];
	vRot[1] = sinTheta * v[0] + cosTheta * v[1];
	vRot[0] = vRot0;
}

/* Rotate a curve about the Z-axis. Can be used to make a sphere, spheroid, 
capsule, circular cone, circular cylinder, box, etc. The z-values should be in 
ascending order --- or at least the first z should be less than the last. The 
first and last r-values should be 0.0, and no others. Probably the t-values 
should be in ascending or descending order. The sideNum parameter controls the 
fineness of the mesh. The attributes are XYZ position, ST texture, and NOP unit 
normal vector. The normals are smooth. Don't forget to meshDestroy when 
finished. */
int meshInitializeRevolution(meshMesh *mesh, GLuint zNum, GLdouble z[], 
		GLdouble r[], GLdouble t[], GLuint sideNum) {
	GLuint i, j, error;
	error = meshInitialize(mesh, (zNum - 2) * sideNum * 2, 
		(zNum - 2) * (sideNum + 1) + 2, 3 + 2 + 3);
	if (error == 0) {
		/* Make the bottom triangles. */
		for (i = 0; i < sideNum; i += 1)
			meshSetTriangle(mesh, i, 0, i + 2, i + 1);
		/* Make the top triangles. */
		for (i = 0; i < sideNum; i += 1)
			meshSetTriangle(mesh, sideNum + i, mesh->vertNum - 1, 
				mesh->vertNum - 1 - (sideNum + 1) + i, 
				mesh->vertNum - 1 - (sideNum + 1) + i + 1);
		/* Make the middle triangles. */
		for (j = 1; j <= zNum - 3; j += 1)
			for (i = 0; i < sideNum; i += 1) {
				meshSetTriangle(mesh, 2 * sideNum * j + 2 * i,
					(j - 1) * (sideNum + 1) + 1 + i, 
					j * (sideNum + 1) + 1 + i + 1, 
					j * (sideNum + 1) + 1 + i);
				meshSetTriangle(mesh, 2 * sideNum * j + 2 * i + 1,
					(j - 1) * (sideNum + 1) + 1 + i, 
					(j - 1) * (sideNum + 1) + 1 + i + 1, 
					j * (sideNum + 1) + 1 + i + 1);
			}
		/* Make the vertices, using vertex 0 as temporary storage. */
		GLdouble *v = mesh->vert;
		GLdouble p[3], q[3], o[3];
		for (j = 1; j <= zNum - 2; j += 1) {
			// Form the sideNum + 1 vertices in the jth layer.
			vecSet(3, p, z[j + 1] - z[j], 0.0, r[j] - r[j + 1]);
			vecUnit(3, p, p);
			vecSet(3, q, z[j] - z[j - 1], 0.0, r[j - 1] - r[j]);
			vecUnit(3, q, q);
			vecAdd(3, p, q, o);
			vecUnit(3, o, o);
			vecSet(8, v, r[j], 0.0, z[j], 1.0, t[j], o[0], o[1], o[2]);
			meshSetVertex(mesh, j * (sideNum + 1), v);
			v[3] = 0.0;
			meshSetVertex(mesh, (j - 1) * (sideNum + 1) + 1, v);
			for (i = 1; i < sideNum; i += 1) {
				meshRotateVector(2 * M_PI / sideNum, v, v);
				v[3] += 1.0 / sideNum;
				meshRotateVector(2 * M_PI / sideNum, &v[5], &v[5]);
				meshSetVertex(mesh, (j - 1) * (sideNum + 1) + 1 + i, v);
			}
		}
		/* Form the top vertex. */
		vecSet(8, v, 0.0, 0.0, z[zNum - 1], 0.0, 0.0, 0.0, 0.0, 1.0);
		meshSetVertex(mesh, mesh->vertNum - 1, v);
		/* Finally form the bottom vertex. */
		vecSet(8, v, 0.0, 0.0, z[0], 0.0, 0.0, 0.0, 0.0, -1.0);
	}
	return error;
}

/* Builds a mesh for a sphere, centered at the origin, of radius r. The sideNum 
and layerNum parameters control the fineness of the mesh. The attributes are 
XYZ position, ST texture, and NOP unit normal vector. The normals are smooth. 
Don't forget to meshDestroy when finished. */
int meshInitializeSphere(meshMesh *mesh, GLdouble r, GLuint layerNum, 
		GLuint sideNum) {
	GLuint error, i;
	GLdouble *ts = (GLdouble *)malloc((layerNum + 1) * 3 * sizeof(GLdouble));
	if (ts == NULL)
		return 1;
	else {
		GLdouble *zs = &ts[layerNum + 1];
		GLdouble *rs = &ts[2 * layerNum + 2];
		for (i = 0; i <= layerNum; i += 1) {
			ts[i] = (GLdouble)i / layerNum;
			zs[i] = -r * cos(ts[i] * M_PI);
			rs[i] = r * sin(ts[i] * M_PI);
		}
		error = meshInitializeRevolution(mesh, layerNum + 1, zs, rs, ts, 
			sideNum);
		free(ts);
		return error;
	}
}

/* Builds a mesh for a circular cylinder with spherical caps, centered at the 
origin, of radius r and length l > 2 * r. The sideNum and layerNum parameters 
control the fineness of the mesh. The attributes are XYZ position, ST texture, 
and NOP unit normal vector. The normals are smooth. Don't forget to meshDestroy 
when finished. */
int meshInitializeCapsule(meshMesh *mesh, GLdouble r, GLdouble l, 
		GLuint layerNum, GLuint sideNum) {
	GLuint error, i;
	GLdouble theta;
	GLdouble *ts = (GLdouble *)malloc((6 * layerNum + 6) * sizeof(GLdouble));
	if (ts == NULL)
		return 1;
	else {
		GLdouble *zs = &ts[2 * layerNum + 2];
		GLdouble *rs = &ts[4 * layerNum + 4];
		zs[0] = -l / 2.0;
		rs[0] = 0.0;
		ts[0] = 0.0;
		for (i = 1; i <= layerNum; i += 1) {
			theta = M_PI / 2.0 * (3 + i / (GLdouble)layerNum);
			zs[i] = -l / 2.0 + r + r * sin(theta);
			rs[i] = r * cos(theta);
			ts[i] = (zs[i] + l / 2.0) / l;
		}
		for (i = 0; i < layerNum; i += 1) {
			theta = M_PI / 2.0 * i / (GLdouble)layerNum;
			zs[layerNum + 1 + i] = l / 2.0 - r + r * sin(theta);
			rs[layerNum + 1 + i] = r * cos(theta);
			ts[layerNum + 1 + i] = (zs[layerNum + 1 + i] + l / 2.0) / l;
		}
		zs[2 * layerNum + 1] = l / 2.0;
		rs[2 * layerNum + 1] = 0.0;
		ts[2 * layerNum + 1] = 1.0;
		error = meshInitializeRevolution(mesh, 2 * layerNum + 2, zs, rs, ts, 
			sideNum);
		free(ts);
		return error;
	}
}