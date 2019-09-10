#ifndef VECTOR_H
#define VECTOR_H
/**
 * Duo Tao
 * vector.h
 */
#ifdef WINDOWS
//#include <Windows.h>
//#include <gl\GL.h>
#else
#include <GL/gl.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdarg.h>

/*** OpenGL ***/
void vecOpenGL(GLuint dim, GLdouble v[], GLfloat openGL[]);

/* Returns the dot product of the dim-dimensional vectors v and w. */
GLdouble vecDot(GLuint dim, GLdouble v[], GLdouble w[]);

/* Returns the length of the dim-dimensional vector v. */
GLdouble vecLength(GLuint dim, GLdouble v[]);

/* Returns the length of the dim-dimensional vector v. If the length is
non-zero, then also places a scaled version of v into the dim-dimensional
vector unit, so that unit has length 1. */
GLdouble vecUnit(GLuint dim, GLdouble v[], GLdouble unit[]);

/* Computes the cross product of the 3-dimensional vectors v and w, and places
it into vCrossW. */
void vec3Cross(GLdouble v[3], GLdouble w[3], GLdouble vCrossW[3]);

/* Computes the 3-dimensional vector v from its spherical coordinates.
rho >= 0.0 is the radius. 0 <= phi <= pi is the co-latitude. -pi <= theta <= pi
is the longitude or azimuth. */
void vec3Spherical(GLdouble rho, GLdouble phi, GLdouble theta, GLdouble v[3]);

/* Copies the dim-dimensional vector v to the dim-dimensional vector copy. */
void vecCopy(GLuint dim, GLdouble v[], GLdouble copy[]);

/* Adds the dim-dimensional vectors v and w. */
void vecAdd(GLuint dim, GLdouble v[], GLdouble w[], GLdouble vPlusW[]);

/* Subtracts the dim-dimensional vectors v and w. */
void vecSubtract(GLuint dim, GLdouble v[], GLdouble w[], GLdouble vMinusW[]);

/* Scales the dim-dimensional vector w by the number c. */
void vecScale(GLuint dim, GLdouble c, GLdouble w[], GLdouble cTimesW[]);

void printVector(GLuint dim, GLdouble w[]);

/* Assumes that there are dim + 2 arguments, the last dim of which are GLdoubles.
Sets the dim-dimensional vector v to those GLdoubles. */
void vecSet(GLuint dim, GLdouble v[], ...);

void vecLerp(GLuint dim, GLdouble v[], GLdouble w[], GLdouble lerp[], GLdouble t);
#endif
