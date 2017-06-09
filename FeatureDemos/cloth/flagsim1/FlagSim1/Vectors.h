#ifndef CC_VECTORS_INCLUDED

#define CC_VECTORS_INCLUDED

#include <GL\glut.h>

#define PI 3.1415926535897932384626433832795
#define PIdiv180 PI/180.0


struct SF3dVector  //Float 3d-vect, normally used
{
	GLfloat x,y,z;
};
struct SF2dVector
{
	GLfloat x,y;
};


SF3dVector F3dVector ( GLfloat x, GLfloat y, GLfloat z );
SF3dVector AddF3dVectors ( SF3dVector * u, SF3dVector * v);
void AddF3dVectorToVector ( SF3dVector * Dst, SF3dVector * V2);


GLfloat GetF3dVectorLength( SF3dVector * v);

SF3dVector CrossProduct (SF3dVector * u, SF3dVector * v);

SF3dVector Normalize3dVector( SF3dVector v);

SF3dVector operator+ (SF3dVector v, SF3dVector u);

SF3dVector operator- (SF3dVector v, SF3dVector u);

SF3dVector operator* (SF3dVector v, float r);
SF3dVector operator/ (SF3dVector v, float r);

float operator* (SF3dVector v, SF3dVector u);	//dot product


#endif