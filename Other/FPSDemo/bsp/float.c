#include "engine.h"

/*
  Find plane equation (A,B,C,D) from three points
*/
void FindPlane(float *v0, float *v1, float *v2, float *p)
{
	float w1[3], w2[3];

	FVec_SubFVec(v1,v0,w1);
	FVec_SubFVec(v2,v0,w2);
	FVec_CrossProduct(w1,w2,p);
	FVec_Normalize(p);
	p[3]=-FVec_DotProduct(p,v0);
}

/*
  Copy vector src to dest
*/
void FVec_Copy(float *src,float *dest)
{
	dest[0]=src[0];
	dest[1]=src[1];
	dest[2]=src[2];
}

/*
  Add two vectors
*/
void FVec_AddFVec(float *v1, float *v2, float *dest)
{
	dest[0]=v1[0]+v2[0];
	dest[1]=v1[1]+v2[1];
	dest[2]=v1[2]+v2[2];
}

/*
  Substract two vectors
*/
void FVec_SubFVec(float *v1, float *v2, float *dest)
{
	dest[0]=v1[0]-v2[0];
	dest[1]=v1[1]-v2[1];
	dest[2]=v1[2]-v2[2];
}

/*
  Calculate dot product of two vectors
*/
float FVec_DotProduct(float *v1,float *v2)
{
	float result;
	int i;

	result=0;
	for(i=0;i<3;i++,v1++,v2++)
		result+=(*v1)*(*v2);
	return(result);
}

/*
  Calculate cross product of two vectors
*/
void FVec_CrossProduct(float *v1,float *v2,float *dest)
{
	dest[0]=v1[1]*v2[2]-v1[2]*v2[1];
	dest[1]=v1[2]*v2[0]-v1[0]*v2[2];
	dest[2]=v1[0]*v2[1]-v1[1]*v2[0];
}

/*
  Multiply vector by a scalar
*/
void FVec_MulScalar(float *v,float s,float *dest)
{
	dest[0]=v[0]*s;
	dest[1]=v[1]*s;
	dest[2]=v[2]*s;
}

/*
  Normalize vector
  Return: length of the original vector
*/
float FVec_Normalize(float *v)
{
	float len;

	len=sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
	if (len!=0.0) {
		v[0]/=len;
		v[1]/=len;
		v[2]/=len;
	}
	return(len);
}

/*
  Multiply matrix by a vector
*/
void FMatrix_MulFVec(FMatrix A, float *v, float *dest)
{
	int i;

	for(i=0;i<3;i++)
		dest[i]=A[i][0]*v[0]+A[i][1]*v[1]+A[i][2]*v[2];
}

/*
  Multiply inverse (transposed) matrix by a vector
*/
void FMatrix_InvMulFVec(FMatrix A, float *v, float *dest)
{
	int i;

	for(i=0;i<3;i++)
		dest[i]=A[0][i]*v[0]+A[1][i]*v[1]+A[2][i]*v[2];
}

/*
  Multiply matrix with a matrix
*/
void FMatrix_MulFMatrix(FMatrix A, FMatrix B, FMatrix Dest)
{
	int i, j;

	for(i=0;i<3;i++)
		for(j=0;j<3;j++)
			Dest[i][j] =
				A[i][0]*B[0][j] +
				A[i][1]*B[1][j] +
				A[i][2]*B[2][j];
}

/*
  Multiply inv matrix with a matrix (dest=inv_a*b)
*/
void FMatrix_InvMulFMatrix(FMatrix A, FMatrix B, FMatrix Dest)
{
	int i, j;

	for(i=0;i<3;i++)
		for(j=0;j<3;j++)
			Dest[i][j] =
				A[0][i]*B[0][j] +
				A[1][i]*B[1][j] +
				A[2][i]*B[2][j];
}

static int IdentityMatrix[3][3]={ {1,0,0},{0,1,0},{0,0,1} };

/*
  Make an identity matrix (1's on main diagonal, 0's elsewhere)
*/
void FMatrix_Identity(FMatrix A)
{
	int i,j;

	for(j=0;j<3;j++)
		for(i=0;i<3;i++)
			A[j][i]=(float)IdentityMatrix[j][i];
}

/*
  Copy matrix src to dest
*/
void FMatrix_Copy(FMatrix src,FMatrix dest)
{
	int i,j;

	for(j=0;j<3;j++)
		for(i=0;i<3;i++)
			dest[j][i]=src[j][i];
}

/*
  Make rotation matrix from given angles
*/
void FMatrix_MakeRotation(FMatrix M,float *angle)
{
	float cos_a,sin_a,cos_b,sin_b,cos_c,sin_c;

	cos_b=cos(angle[0]);
	sin_b=sin(angle[0]);
	cos_c=cos(angle[1]);
	sin_c=sin(angle[1]);
	cos_a=cos(angle[2]);
	sin_a=sin(angle[2]);

// First matrix row
	M[0][0]=cos_a*cos_c-sin_a*sin_b*sin_c;
	M[0][1]=sin_a*cos_c+cos_a*sin_b*sin_c;
	M[0][2]=cos_b*sin_c;

// Second matrix row
	M[1][0]=-sin_a*cos_b;
	M[1][1]=cos_a*cos_b;
	M[1][2]=-sin_b;

// Third matrix row
	M[2][0]=-cos_a*sin_c-sin_a*sin_b*cos_c;
	M[2][1]=-sin_a*sin_c+cos_a*sin_b*cos_c;
	M[2][2]=cos_b*cos_c;
}


void FMatrix_PreXRot(FMatrix A, float Angle)
{
	float c,s;
	float Temp01, Temp11, Temp21, Temp02, Temp12, Temp22;

// Get the sine and cosine of the rotation angle
	s=sin(Angle);
	c=cos(Angle);

// Calculate the new values of the six affected matrix entries
	Temp01=c*A[0][1]+s*A[0][2];
	Temp11=c*A[1][1]+s*A[1][2];
	Temp21=c*A[2][1]+s*A[2][2];
	Temp02=c*A[0][2]-s*A[0][1];
	Temp12=c*A[1][2]-s*A[1][1];
	Temp22=c*A[2][2]-s*A[2][1];

// Put the results back into A
	A[0][1]=Temp01; A[0][2]=Temp02;
	A[1][1]=Temp11; A[1][2]=Temp12;
	A[2][1]=Temp21; A[2][2]=Temp22;
}

void FMatrix_PreYRot(FMatrix A, float Angle)
{
	float c,s;
	float Temp00, Temp10, Temp20, Temp02, Temp12, Temp22;

// Get the sine and cosine of the rotation angle
	s=sin(Angle);
	c=cos(Angle);

// Calculate the new values of the six affected matrix entries
	Temp00=c*A[0][0]+s*A[0][2];
	Temp10=c*A[1][0]+s*A[1][2];
	Temp20=c*A[2][0]+s*A[2][2];
	Temp02=c*A[0][2]-s*A[0][0];
	Temp12=c*A[1][2]-s*A[1][0];
	Temp22=c*A[2][2]-s*A[2][0];

// Put the results back into XformToChange
	A[0][0]=Temp00; A[0][2]=Temp02;
	A[1][0]=Temp10; A[1][2]=Temp12;
	A[2][0]=Temp20; A[2][2]=Temp22;
}

void FMatrix_PreZRot(FMatrix A, float Angle)
{
	float c,s;
	float Temp00, Temp01, Temp10, Temp11, Temp20, Temp21;

// Get the sine and cosine of the rotation angle
	s=sin(Angle);
	c=cos(Angle);

// Calculate the new values of the six affected matrix entries
	Temp00=c*A[0][0]+s*A[0][1];
	Temp10=c*A[1][0]+s*A[1][1];
	Temp20=c*A[2][0]+s*A[2][1];
	Temp01=c*A[0][1]-s*A[0][0];
	Temp11=c*A[1][1]-s*A[1][0];
	Temp21=c*A[2][1]-s*A[2][0];

// Put the results back into XformToChange
	A[0][0]=Temp00; A[0][1]=Temp01;
	A[1][0]=Temp10; A[1][1]=Temp11;
	A[2][0]=Temp20; A[2][1]=Temp21;
}

