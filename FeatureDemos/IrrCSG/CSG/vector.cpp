/**
 * Duo Tao
 * vector.c
 */
#include "vector.h"

/*** OpenGL ***/
void vecOpenGL(GLuint dim, GLdouble v[], GLfloat openGL[]) {
	for (int i = 0; i < dim; i += 1)
		openGL[i] = v[i];
}

/* Returns the dot product of the dim-dimensional vectors v and w. */
GLdouble vecDot(GLuint dim, GLdouble v[], GLdouble w[]) {
	GLdouble sum = 0;
	for (int i = 0; i < dim; i++) {
		sum += v[i] * w[i];
	}
	return sum;
}

/* Returns the length of the dim-dimensional vector v. */
GLdouble vecLength(GLuint dim, GLdouble v[]) {
	GLdouble sum = 0;
	for (int i = 0; i < dim; i++) {
		sum += v[i] * v[i];
	}
	return sqrt(sum);
}

/* Returns the length of the dim-dimensional vector v. If the length is 
non-zero, then also places a scaled version of v into the dim-dimensional 
vector unit, so that unit has length 1. */
GLdouble vecUnit(GLuint dim, GLdouble v[], GLdouble unit[]) {
	GLdouble len = vecLength(dim, v);
	if (len != 0) {
		for (int i = 0; i < dim; i++) {
			unit[i] = v[i] / len;
		}
	}
	return len;
}

/* Computes the cross product of the 3-dimensional vectors v and w, and places 
it into vCrossW. */
void vec3Cross(GLdouble v[3], GLdouble w[3], GLdouble vCrossW[3]) {
	vCrossW[0] = v[1] * w[2] - v[2] * w[1];
	vCrossW[1] = v[2] * w[0] - v[0] * w[2];
	vCrossW[2] = v[0] * w[1] - v[1] * w[0];
}

/* Computes the 3-dimensional vector v from its spherical coordinates. 
rho >= 0.0 is the radius. 0 <= phi <= pi is the co-latitude. -pi <= theta <= pi 
is the longitude or azimuth. */
void vec3Spherical(GLdouble rho, GLdouble phi, GLdouble theta, GLdouble v[3]) {
	// phi = phi / 180.0 * PI;
	// theta = theta / 180.0 * PI;
	v[0] = rho * sin(phi) * cos(theta);
	v[1] = rho * sin(phi) * sin(theta);
	v[2] = rho * cos(phi);
}

/* Copies the dim-dimensional vector v to the dim-dimensional vector copy. */
void vecCopy(GLuint dim, GLdouble v[], GLdouble copy[]) {
	for (int i = 0; i < dim; i++) {
		copy[i] = v[i];
	}
}

/* Adds the dim-dimensional vectors v and w. */
void vecAdd(GLuint dim, GLdouble v[], GLdouble w[], GLdouble vPlusW[]) {
	for (int i = 0; i < dim; i++) {
		vPlusW[i] = v[i] + w[i];
	}
}

/* Subtracts the dim-dimensional vectors v and w. */
void vecSubtract(GLuint dim, GLdouble v[], GLdouble w[], GLdouble vMinusW[]) {
	for (int i = 0; i < dim; i++) {
		vMinusW[i] = v[i] - w[i];
	}
}

/* Scales the dim-dimensional vector w by the number c. */
void vecScale(GLuint dim, GLdouble c, GLdouble w[], GLdouble cTimesW[]) {
	for (int i = 0; i < dim; i++) {
		cTimesW[i] = c * w[i];
	}
}

void printVector(GLuint dim, GLdouble w[]) {
	for (int i = 0; i < dim; i++) {
		printf("%f ", w[i]);
	}
	printf("\n");
}

/* Assumes that there are dim + 2 arguments, the last dim of which are GLdoubles. 
Sets the dim-dimensional vector v to those GLdoubles. */
void vecSet(GLuint dim, GLdouble v[], ...) {
	va_list argumentPointer;
	va_start(argumentPointer, v);
	for (int i = 0 ; i < dim; i++) {
		v[i] = va_arg(argumentPointer, GLdouble);
	}
	va_end(argumentPointer);
}

void vecLerp(GLuint dim, GLdouble v[], GLdouble w[], GLdouble lerp[], GLdouble t) {
	for (int i = 0; i < dim; i++) {
		lerp[i] = v[i] + (w[i] - v[i]) * t;
	}
}