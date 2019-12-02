/*
* GLGX 
* An OpenGL Utility Library
*
* Copyright (c) 2002-2010, Grégory Smialek
*
* All rights reserved.
* 
* Redistribution and use in source and binary forms, with or without modification, are permitted 
* provided that the following conditions are met:
* 
*   * Redistributions of source code must retain the above copyright notice, 
*     this list of conditions and the following disclaimer.
*   * Redistributions in binary form must reproduce the above copyright notice,
*     this list of conditions and the following disclaimer in the documentation 
*     and/or other materials provided with the distribution.
*   * Neither the name of the Grégory Smialek nor the names of its contributors
*     may be used to endorse or promote products derived from this software 
*     without specific prior written permission.
* 
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
* A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
* CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
* EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
* PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
* PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
* LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include"GLGXMath.h"
#include<math.h>



GLGXMATRIX GLGXMATRIX::operator* (const GLGXMATRIX &B)
{
	GLGXMATRIX temp;

	temp.Array[0] = this->Array[0]*B.Array[0] + this->Array[4]*B.Array[1] +  this->Array[8]*B.Array[2] + this->Array[12]*B.Array[3];
	temp.Array[1] = this->Array[1]*B.Array[0] + this->Array[5]*B.Array[1] +  this->Array[9]*B.Array[2] + this->Array[13]*B.Array[3];
	temp.Array[2] = this->Array[2]*B.Array[0] + this->Array[6]*B.Array[1] + this->Array[10]*B.Array[2] + this->Array[14]*B.Array[3];
	temp.Array[3] = this->Array[3]*B.Array[0] + this->Array[7]*B.Array[1] + this->Array[11]*B.Array[2] + this->Array[15]*B.Array[3];

	temp.Array[4] = this->Array[0]*B.Array[4] + this->Array[4]*B.Array[5] +  this->Array[8]*B.Array[6] + this->Array[12]*B.Array[7];
	temp.Array[5] = this->Array[1]*B.Array[4] + this->Array[5]*B.Array[5] +  this->Array[9]*B.Array[6] + this->Array[13]*B.Array[7];
	temp.Array[6] = this->Array[2]*B.Array[4] + this->Array[6]*B.Array[5] + this->Array[10]*B.Array[6] + this->Array[14]*B.Array[7];
	temp.Array[7] = this->Array[3]*B.Array[4] + this->Array[7]*B.Array[5] + this->Array[11]*B.Array[6] + this->Array[15]*B.Array[7];

	temp.Array[8]  = this->Array[0]*B.Array[8] + this->Array[4]*B.Array[9] +  this->Array[8]*B.Array[10] + this->Array[12]*B.Array[11];
	temp.Array[9]  = this->Array[1]*B.Array[8] + this->Array[5]*B.Array[9] +  this->Array[9]*B.Array[10] + this->Array[13]*B.Array[11];
	temp.Array[10] = this->Array[2]*B.Array[8] + this->Array[6]*B.Array[9] + this->Array[10]*B.Array[10] + this->Array[14]*B.Array[11];
	temp.Array[11] = this->Array[3]*B.Array[8] + this->Array[7]*B.Array[9] + this->Array[11]*B.Array[10] + this->Array[15]*B.Array[11];

	temp.Array[12] = this->Array[0]*B.Array[12] + this->Array[4]*B.Array[13] +  this->Array[8]*B.Array[14] + this->Array[12]*B.Array[15];
	temp.Array[13] = this->Array[1]*B.Array[12] + this->Array[5]*B.Array[13] +  this->Array[9]*B.Array[14] + this->Array[13]*B.Array[15];
	temp.Array[14] = this->Array[2]*B.Array[12] + this->Array[6]*B.Array[13] + this->Array[10]*B.Array[14] + this->Array[14]*B.Array[15];
	temp.Array[15] = this->Array[3]*B.Array[12] + this->Array[7]*B.Array[13] + this->Array[11]*B.Array[14] + this->Array[15]*B.Array[15];

	return temp;
}



void GLGXMatrixIdentity(GLGXMATRIX* matrix)
{
		matrix->_12 = matrix->_13 = matrix->_14 = matrix->_21 = matrix->_23 = matrix->_24 = matrix->_31 =matrix->_32 = matrix->_34 = matrix->_41 = matrix->_42 = matrix->_43 = 0.0f;
		matrix->_11 = matrix->_22 = matrix->_33 = matrix->_44 = 1.0f;
}

void GLGXMatrixMultiply( GLGXMATRIX* Result, GLGXMATRIX* A, GLGXMATRIX* B )
{
	GLGXMATRIX temp;

	temp.Array[0] = A->Array[0]*B->Array[0] + A->Array[4]*B->Array[1] +  A->Array[8]*B->Array[2] + A->Array[12]*B->Array[3];
	temp.Array[1] = A->Array[1]*B->Array[0] + A->Array[5]*B->Array[1] +  A->Array[9]*B->Array[2] + A->Array[13]*B->Array[3];
	temp.Array[2] = A->Array[2]*B->Array[0] + A->Array[6]*B->Array[1] + A->Array[10]*B->Array[2] + A->Array[14]*B->Array[3];
	temp.Array[3] = A->Array[3]*B->Array[0] + A->Array[7]*B->Array[1] + A->Array[11]*B->Array[2] + A->Array[15]*B->Array[3];

	temp.Array[4] = A->Array[0]*B->Array[4] + A->Array[4]*B->Array[5] +  A->Array[8]*B->Array[6] + A->Array[12]*B->Array[7];
	temp.Array[5] = A->Array[1]*B->Array[4] + A->Array[5]*B->Array[5] +  A->Array[9]*B->Array[6] + A->Array[13]*B->Array[7];
	temp.Array[6] = A->Array[2]*B->Array[4] + A->Array[6]*B->Array[5] + A->Array[10]*B->Array[6] + A->Array[14]*B->Array[7];
	temp.Array[7] = A->Array[3]*B->Array[4] + A->Array[7]*B->Array[5] + A->Array[11]*B->Array[6] + A->Array[15]*B->Array[7];

	temp.Array[8]  = A->Array[0]*B->Array[8] + A->Array[4]*B->Array[9] +  A->Array[8]*B->Array[10] + A->Array[12]*B->Array[11];
	temp.Array[9]  = A->Array[1]*B->Array[8] + A->Array[5]*B->Array[9] +  A->Array[9]*B->Array[10] + A->Array[13]*B->Array[11];
	temp.Array[10] = A->Array[2]*B->Array[8] + A->Array[6]*B->Array[9] + A->Array[10]*B->Array[10] + A->Array[14]*B->Array[11];
	temp.Array[11] = A->Array[3]*B->Array[8] + A->Array[7]*B->Array[9] + A->Array[11]*B->Array[10] + A->Array[15]*B->Array[11];

	temp.Array[12] = A->Array[0]*B->Array[12] + A->Array[4]*B->Array[13] +  A->Array[8]*B->Array[14] + A->Array[12]*B->Array[15];
	temp.Array[13] = A->Array[1]*B->Array[12] + A->Array[5]*B->Array[13] +  A->Array[9]*B->Array[14] + A->Array[13]*B->Array[15];
	temp.Array[14] = A->Array[2]*B->Array[12] + A->Array[6]*B->Array[13] + A->Array[10]*B->Array[14] + A->Array[14]*B->Array[15];
	temp.Array[15] = A->Array[3]*B->Array[12] + A->Array[7]*B->Array[13] + A->Array[11]*B->Array[14] + A->Array[15]*B->Array[15];

	for( int i = 0; i < 16 ; ++i)
	{
		Result->Array[i] = temp.Array[i];
	}
}

void GLGXMatrixTranspose( GLGXMATRIX* Result, GLGXMATRIX* matrix)
{
	GLGXMATRIX temp;

	temp.Array[0] = matrix->Array[0];  temp.Array[4] = matrix->Array[1];
	temp.Array[1] = matrix->Array[4];  temp.Array[5] = matrix->Array[5];
	temp.Array[2] = matrix->Array[8];  temp.Array[6] = matrix->Array[9];
	temp.Array[3] = matrix->Array[12]; temp.Array[7] = matrix->Array[13];
			   
	temp.Array[8] = matrix->Array[2];  temp.Array[12] = matrix->Array[3];
	temp.Array[9] = matrix->Array[6];  temp.Array[13] = matrix->Array[7];
	temp.Array[10]= matrix->Array[10]; temp.Array[14] = matrix->Array[11];
	temp.Array[11]= matrix->Array[14]; temp.Array[15] = matrix->Array[15];

	for( int i = 0; i < 16 ; ++i)
	{
		Result->Array[i] = temp.Array[i];
	}
}

void GLGXMatrixCopy( GLGXMATRIX* Result, GLGXMATRIX* matrix)
{
	for( int i = 0; i < 16 ; ++i)
	{
		Result->Array[i] = matrix->Array[i];
	}
}




GLGXMATRIX* GLGXMatrixLookAt( GLGXMATRIX* pResult,GLGXVECTOR3 *pPosition,GLGXVECTOR3 *pTarget,GLGXVECTOR3 *pUp)
{

	GLGXMATRIX orient;

	GLGXVECTOR3 forward = *pPosition - *pTarget; // OK
	GLGXVec3Normalize(&forward,&forward);

	GLGXVECTOR3  up;
	GLGXVec3Normalize(&up,pUp);
	GLGXVECTOR3 side;
	GLGXVec3Cross(&side, &up, &forward);
	GLGXVec3Normalize(&side,&side);

	GLGXVec3Cross(&up, &forward, &side);
	GLGXVec3Normalize(&up,&up);

	orient._11 = side.x;
	orient._12 = side.y;
	orient._13 = side.z;
	orient._14 = 0.0f;

	orient._21 = up.x;
	orient._22 = up.y;
	orient._23 = up.z;
	orient._24 = 0.0f;
	
	orient._31 = forward.x;
	orient._32 = forward.y;
	orient._33 = forward.z;
	orient._34 = 0.0f;

	orient._41 =  0.0f;
	orient._42 =  0.0f;
	orient._43 =  0.0f;
	orient._44 =  1.0f;

	GLGXMatrixTranspose(&orient,&orient);

	GLGXMATRIX correction;

	correction._41 = -pPosition->x;
	correction._42 = -pPosition->y;
	correction._43 = -pPosition->z;
	correction._44 = 1.0f;

	GLGXMatrixMultiply(pResult,&orient,&correction);

	return pResult;
}


void GLGXMatrixRotationQuaternion( GLGXMATRIX* matrix, GLGXQUATERNION* quat )
{
	// Quaternion -> matrix
	matrix->Array[0]= 1-(2*quat->y*quat->y)
						-(2*quat->z*quat->z);
	matrix->Array[4]= (2*quat->x*quat->y)
						-(2*quat->w*quat->z);
	matrix->Array[8]= (2*quat->x*quat->z)
						+(2*quat->w*quat->y);
	matrix->Array[12]= 0.0f;

	matrix->Array[1]= (2*quat->x*quat->y)
						+(2*quat->w*quat->z);
	matrix->Array[5]= 1-(2*quat->x*quat->x)
						-(2*quat->z*quat->z);
	matrix->Array[9]= (2*quat->y*quat->z)
						-(2*quat->w*quat->x);
	matrix->Array[13]= 0.0f;
   			      
	matrix->Array[2]= (2*quat->x*quat->z)
						-(2*quat->w*quat->y);
	matrix->Array[6]= (2*quat->y*quat->z)
						+(2*quat->w*quat->x);
	matrix->Array[10]= 1-(2*quat->x*quat->x)
						-(2*quat->y*quat->y);
	matrix->Array[14]= 0.0f;
			
	matrix->Array[3]= 0.0f;
	matrix->Array[7]= 0.0f;
	matrix->Array[11]= 0.0f;
	matrix->Array[15]= 1.0f;
}


void GLGXMatrixTranslation(GLGXMATRIX* dest,GLfloat x, GLfloat y, GLfloat z)
{
	dest->Array[0]  = 1.0f; dest->Array[1]  = 0.0f; dest->Array[2]  = 0.0f; dest->Array[3]  = 0.0f;
	dest->Array[4]  = 0.0f; dest->Array[5]  = 1.0f; dest->Array[6]  = 0.0f; dest->Array[7]  = 0.0f;
	dest->Array[8]  = 0.0f; dest->Array[9]  = 0.0f; dest->Array[10] = 1.0f; dest->Array[11] = 0.0f;
	dest->Array[12] = x;    dest->Array[13] = y;    dest->Array[14] = z; dest->Array[15] = 1.0f;
}


void GLGXMatrixTranslation(GLGXMATRIX* dest,GLGXVECTOR3* vector)
{
	dest->Array[0]  = 1.0f; dest->Array[1]  = 0.0f; dest->Array[2]  = 0.0f; dest->Array[3]  = 0.0f;
	dest->Array[4]  = 0.0f; dest->Array[5]  = 1.0f; dest->Array[6]  = 0.0f; dest->Array[7]  = 0.0f;
	dest->Array[8]  = 0.0f; dest->Array[9]  = 0.0f; dest->Array[10] = 1.0f; dest->Array[11] = 0.0f;
	dest->Array[12] = vector->x; dest->Array[13] = vector->y; dest->Array[14] = vector->z; dest->Array[15] = 1.0f;
}

void GLGXMatrixScaling(GLGXMATRIX* dest,GLfloat x,GLfloat y,GLfloat z)
{
	dest->Array[0]  = x; dest->Array[1]  = 0.0f; dest->Array[2]  = 0.0f; dest->Array[3]  = 0.0f;
	dest->Array[4]  = 0.0f; dest->Array[5]  = y; dest->Array[6]  = 0.0f; dest->Array[7]  = 0.0f;
	dest->Array[8]  = 0.0f; dest->Array[9]  = 0.0f; dest->Array[10] = z; dest->Array[11] = 0.0f;
	dest->Array[12] = 0.0f; dest->Array[13] = 0.0f; dest->Array[14] = 0.0f; dest->Array[15] = 1.0f;
}

void GLGXMatrixScaling(GLGXMATRIX* dest,GLGXVECTOR3* vector)
{
	dest->Array[0]  = vector->x; dest->Array[1]  = 0.0f; dest->Array[2]  = 0.0f; dest->Array[3]  = 0.0f;
	dest->Array[4]  = 0.0f; dest->Array[5]  = vector->y; dest->Array[6]  = 0.0f; dest->Array[7]  = 0.0f;
	dest->Array[8]  = 0.0f; dest->Array[9]  = 0.0f; dest->Array[10] = vector->z; dest->Array[11] = 0.0f;
	dest->Array[12] = 0.0f; dest->Array[13] = 0.0f; dest->Array[14] = 0.0f; dest->Array[15] = 1.0f;
}

float GLGXMatrixDeterminant( GLGXMATRIX* i_matrix)
{
	return
   i_matrix->_41 * i_matrix->_32 * i_matrix->_23 * i_matrix->_14 - i_matrix->_31 * i_matrix->_42 * i_matrix->_23 * i_matrix->_14 - i_matrix->_41 * i_matrix->_22 * i_matrix->_33 * i_matrix->_14 + i_matrix->_21 * i_matrix->_42 * i_matrix->_33 * i_matrix->_14 +
   i_matrix->_31 * i_matrix->_22 * i_matrix->_43 * i_matrix->_14 - i_matrix->_21 * i_matrix->_32 * i_matrix->_43 * i_matrix->_14 - i_matrix->_41 * i_matrix->_32 * i_matrix->_13 * i_matrix->_24 + i_matrix->_31 * i_matrix->_42 * i_matrix->_13 * i_matrix->_24 +
   i_matrix->_41 * i_matrix->_12 * i_matrix->_33 * i_matrix->_24 - i_matrix->_11 * i_matrix->_42 * i_matrix->_33 * i_matrix->_24 - i_matrix->_31 * i_matrix->_12 * i_matrix->_43 * i_matrix->_24 + i_matrix->_11 * i_matrix->_32 * i_matrix->_43 * i_matrix->_24 +
   i_matrix->_41 * i_matrix->_22 * i_matrix->_13 * i_matrix->_34 - i_matrix->_21 * i_matrix->_42 * i_matrix->_13 * i_matrix->_34 - i_matrix->_41 * i_matrix->_12 * i_matrix->_23 * i_matrix->_34 + i_matrix->_11 * i_matrix->_42 * i_matrix->_23 * i_matrix->_34 +
   i_matrix->_21 * i_matrix->_12 * i_matrix->_43 * i_matrix->_34 - i_matrix->_11 * i_matrix->_22 * i_matrix->_43 * i_matrix->_34 - i_matrix->_31 * i_matrix->_22 * i_matrix->_13 * i_matrix->_44 + i_matrix->_21 * i_matrix->_32 * i_matrix->_13 * i_matrix->_44 +
   i_matrix->_31 * i_matrix->_12 * i_matrix->_23 * i_matrix->_44 - i_matrix->_11 * i_matrix->_32 * i_matrix->_23 * i_matrix->_44 - i_matrix->_21 * i_matrix->_12 * i_matrix->_33 * i_matrix->_44 + i_matrix->_11 * i_matrix->_22 * i_matrix->_33 * i_matrix->_44 ;
}

GLGXMATRIX * GLGXMatrixInverse(GLGXMATRIX * o_Matrix,float* o_Determinant,GLGXMATRIX * i_Matrix)
{
	float determinant = GLGXMatrixDeterminant(i_Matrix);

	if( determinant > -0.0001f && determinant < 0.0001f)
	{
		o_Matrix = NULL;
		return NULL;
	}

	float scale = 1.0f/ determinant;

	GLGXMATRIX tempMatrix;
    tempMatrix._11 = (i_Matrix->_32*i_Matrix->_43*i_Matrix->_24 - i_Matrix->_42*i_Matrix->_33*i_Matrix->_24 + i_Matrix->_42*i_Matrix->_23*i_Matrix->_34 - i_Matrix->_22*i_Matrix->_43*i_Matrix->_34 - i_Matrix->_32*i_Matrix->_23*i_Matrix->_44 + i_Matrix->_22*i_Matrix->_33*i_Matrix->_44)*scale;
    tempMatrix._21 = (i_Matrix->_41*i_Matrix->_33*i_Matrix->_24 - i_Matrix->_31*i_Matrix->_43*i_Matrix->_24 - i_Matrix->_41*i_Matrix->_23*i_Matrix->_34 + i_Matrix->_21*i_Matrix->_43*i_Matrix->_34 + i_Matrix->_31*i_Matrix->_23*i_Matrix->_44 - i_Matrix->_21*i_Matrix->_33*i_Matrix->_44)*scale;
    tempMatrix._31 = (i_Matrix->_31*i_Matrix->_42*i_Matrix->_24 - i_Matrix->_41*i_Matrix->_32*i_Matrix->_24 + i_Matrix->_41*i_Matrix->_22*i_Matrix->_34 - i_Matrix->_21*i_Matrix->_42*i_Matrix->_34 - i_Matrix->_31*i_Matrix->_22*i_Matrix->_44 + i_Matrix->_21*i_Matrix->_32*i_Matrix->_44)*scale;
    tempMatrix._41 = (i_Matrix->_41*i_Matrix->_32*i_Matrix->_23 - i_Matrix->_31*i_Matrix->_42*i_Matrix->_23 - i_Matrix->_41*i_Matrix->_22*i_Matrix->_33 + i_Matrix->_21*i_Matrix->_42*i_Matrix->_33 + i_Matrix->_31*i_Matrix->_22*i_Matrix->_43 - i_Matrix->_21*i_Matrix->_32*i_Matrix->_43)*scale;
   
	tempMatrix._12 = (i_Matrix->_42*i_Matrix->_33*i_Matrix->_14 - i_Matrix->_32*i_Matrix->_43*i_Matrix->_14 - i_Matrix->_42*i_Matrix->_13*i_Matrix->_34 + i_Matrix->_12*i_Matrix->_43*i_Matrix->_34 + i_Matrix->_32*i_Matrix->_13*i_Matrix->_44 - i_Matrix->_12*i_Matrix->_33*i_Matrix->_44)*scale;
    tempMatrix._22 = (i_Matrix->_31*i_Matrix->_43*i_Matrix->_14 - i_Matrix->_41*i_Matrix->_33*i_Matrix->_14 + i_Matrix->_41*i_Matrix->_13*i_Matrix->_34 - i_Matrix->_11*i_Matrix->_43*i_Matrix->_34 - i_Matrix->_31*i_Matrix->_13*i_Matrix->_44 + i_Matrix->_11*i_Matrix->_33*i_Matrix->_44)*scale;
    tempMatrix._32 = (i_Matrix->_41*i_Matrix->_32*i_Matrix->_14 - i_Matrix->_31*i_Matrix->_42*i_Matrix->_14 - i_Matrix->_41*i_Matrix->_12*i_Matrix->_34 + i_Matrix->_11*i_Matrix->_42*i_Matrix->_34 + i_Matrix->_31*i_Matrix->_12*i_Matrix->_44 - i_Matrix->_11*i_Matrix->_32*i_Matrix->_44)*scale;
    tempMatrix._42 = (i_Matrix->_31*i_Matrix->_42*i_Matrix->_13 - i_Matrix->_41*i_Matrix->_32*i_Matrix->_13 + i_Matrix->_41*i_Matrix->_12*i_Matrix->_33 - i_Matrix->_11*i_Matrix->_42*i_Matrix->_33 - i_Matrix->_31*i_Matrix->_12*i_Matrix->_43 + i_Matrix->_11*i_Matrix->_32*i_Matrix->_43)*scale;
   
	tempMatrix._13 = (i_Matrix->_22*i_Matrix->_43*i_Matrix->_14 - i_Matrix->_42*i_Matrix->_23*i_Matrix->_14 + i_Matrix->_42*i_Matrix->_13*i_Matrix->_24 - i_Matrix->_12*i_Matrix->_43*i_Matrix->_24 - i_Matrix->_22*i_Matrix->_13*i_Matrix->_44 + i_Matrix->_12*i_Matrix->_23*i_Matrix->_44)*scale;
    tempMatrix._23 = (i_Matrix->_41*i_Matrix->_23*i_Matrix->_14 - i_Matrix->_21*i_Matrix->_43*i_Matrix->_14 - i_Matrix->_41*i_Matrix->_13*i_Matrix->_24 + i_Matrix->_11*i_Matrix->_43*i_Matrix->_24 + i_Matrix->_21*i_Matrix->_13*i_Matrix->_44 - i_Matrix->_11*i_Matrix->_23*i_Matrix->_44)*scale;
    tempMatrix._33 = (i_Matrix->_21*i_Matrix->_42*i_Matrix->_14 - i_Matrix->_41*i_Matrix->_22*i_Matrix->_14 + i_Matrix->_41*i_Matrix->_12*i_Matrix->_24 - i_Matrix->_11*i_Matrix->_42*i_Matrix->_24 - i_Matrix->_21*i_Matrix->_12*i_Matrix->_44 + i_Matrix->_11*i_Matrix->_22*i_Matrix->_44)*scale;
    tempMatrix._43 = (i_Matrix->_41*i_Matrix->_22*i_Matrix->_13 - i_Matrix->_21*i_Matrix->_42*i_Matrix->_13 - i_Matrix->_41*i_Matrix->_12*i_Matrix->_23 + i_Matrix->_11*i_Matrix->_42*i_Matrix->_23 + i_Matrix->_21*i_Matrix->_12*i_Matrix->_43 - i_Matrix->_11*i_Matrix->_22*i_Matrix->_43)*scale;
   
	tempMatrix._14 = (i_Matrix->_32*i_Matrix->_23*i_Matrix->_14 - i_Matrix->_22*i_Matrix->_33*i_Matrix->_14 - i_Matrix->_32*i_Matrix->_13*i_Matrix->_24 + i_Matrix->_12*i_Matrix->_33*i_Matrix->_24 + i_Matrix->_22*i_Matrix->_13*i_Matrix->_34 - i_Matrix->_12*i_Matrix->_23*i_Matrix->_34)*scale;
    tempMatrix._24 = (i_Matrix->_21*i_Matrix->_33*i_Matrix->_14 - i_Matrix->_31*i_Matrix->_23*i_Matrix->_14 + i_Matrix->_31*i_Matrix->_13*i_Matrix->_24 - i_Matrix->_11*i_Matrix->_33*i_Matrix->_24 - i_Matrix->_21*i_Matrix->_13*i_Matrix->_34 + i_Matrix->_11*i_Matrix->_23*i_Matrix->_34)*scale;
    tempMatrix._34 = (i_Matrix->_31*i_Matrix->_22*i_Matrix->_14 - i_Matrix->_21*i_Matrix->_32*i_Matrix->_14 - i_Matrix->_31*i_Matrix->_12*i_Matrix->_24 + i_Matrix->_11*i_Matrix->_32*i_Matrix->_24 + i_Matrix->_21*i_Matrix->_12*i_Matrix->_34 - i_Matrix->_11*i_Matrix->_22*i_Matrix->_34)*scale;
    tempMatrix._44 = (i_Matrix->_21*i_Matrix->_32*i_Matrix->_13 - i_Matrix->_31*i_Matrix->_22*i_Matrix->_13 + i_Matrix->_31*i_Matrix->_12*i_Matrix->_23 - i_Matrix->_11*i_Matrix->_32*i_Matrix->_23 - i_Matrix->_21*i_Matrix->_12*i_Matrix->_33 + i_Matrix->_11*i_Matrix->_22*i_Matrix->_33)*scale;
    
	*o_Matrix = tempMatrix;

	if( o_Determinant != NULL )
		*o_Determinant = determinant;

	return o_Matrix;
}

GLGXMATRIX* GLGXMatrixRotationAxis( GLGXMATRIX* o_Matrix,GLGXVECTOR3* i_Vector, GLfloat i_Angle)
{
	GLGXQUATERNION quat;
	GLGXQuaternionRotationAxis(&quat,i_Vector,i_Angle);

	GLGXMatrixRotationQuaternion( o_Matrix, &quat);
	return o_Matrix;
}

GLGXMATRIX* GLGXMatrixRotationX( GLGXMATRIX* o_Matrix, GLfloat i_Angle)
{
	GLGXQUATERNION quat;
	GLGXVECTOR3 vector(1.0f,0.0f,0.0f);
	GLGXQuaternionRotationAxis(&quat,&vector,i_Angle);

	GLGXMatrixRotationQuaternion( o_Matrix, &quat);
	return o_Matrix;
}

GLGXMATRIX* GLGXMatrixRotationY( GLGXMATRIX* o_Matrix, GLfloat i_Angle)
{
	GLGXQUATERNION quat;
	GLGXVECTOR3 vector(0.0f,1.0f,0.0f);
	GLGXQuaternionRotationAxis(&quat,&vector,i_Angle);

	GLGXMatrixRotationQuaternion( o_Matrix, &quat);
	return o_Matrix;
}

GLGXMATRIX* GLGXMatrixRotationZ( GLGXMATRIX* o_Matrix, GLfloat i_Angle)
{
	GLGXQUATERNION quat;
	GLGXVECTOR3 vector(0.0f,0.0f,1.0f);
	GLGXQuaternionRotationAxis(&quat,&vector,i_Angle);

	GLGXMatrixRotationQuaternion( o_Matrix, &quat);
	return o_Matrix;
}

GLGXMATRIX * GLGXMatrixMultiplyTranspose(GLGXMATRIX *o_Matrix,GLGXMATRIX *i_Matrix1,GLGXMATRIX *i_Matrix2)
{
	GLGXMatrixMultiply(o_Matrix,i_Matrix1,i_Matrix2);
	GLGXMatrixTranspose(o_Matrix,o_Matrix);
	return o_Matrix;
}

GLGXMATRIX * GLGXMatrixPerspectiveFov(
  GLGXMATRIX * o_Matrix,
  double i_fovy,
  double i_Aspect,
  double i_zNear,
  double i_zFar
)
{
	double radians = i_fovy / 2.0 * GLGX_PI / 180.0;
	double sine    = sin(radians);

	double f =  cos(radians) / sine;

	GLGXMatrixIdentity(o_Matrix);

	o_Matrix->_11 = float(f / i_Aspect);
	o_Matrix->_22 = f;
	o_Matrix->_33 = (i_zFar + i_zNear) / (i_zNear - i_zFar);
	o_Matrix->_34 = -1.0f;
	o_Matrix->_43 = (2 * i_zFar * i_zNear) / (i_zNear - i_zFar);
	o_Matrix->_44 = 0.0f;

	return o_Matrix;
}

GLGXMATRIX * GLGXMatrixOrthoOffCenter(
  GLGXMATRIX * o_Matrix,
	double 	i_Left, 
	double 	i_Right, 
	double 	i_Bottom, 
	double 	i_Top, 
	double 	i_zNear, 
	double 	i_zFar
)
{
	GLGXMatrixIdentity(o_Matrix);

	float tx = -(i_Right + i_Left   ) / ( i_Right - i_Left   );
	float ty = -(i_Top   + i_Bottom ) / ( i_Top   - i_Bottom );
	float tz = -(i_zFar  + i_zNear  ) / ( i_zFar  - i_zNear  );

	o_Matrix->_11 = 2.0 / (i_Right - i_Left);
	o_Matrix->_22 = 2.0 / (i_Top - i_Bottom);
	o_Matrix->_33 = -2.0 / (i_zFar - i_zNear);

	o_Matrix->_41 = tx;
	o_Matrix->_42 = ty;
	o_Matrix->_43 = tz;

	return o_Matrix;
}

GLGXMATRIX * GLGXMatrixOrthoOffCenter2D(
  GLGXMATRIX * o_Matrix,
	double 	i_Left, 
	double 	i_Right, 
	double 	i_Bottom, 
	double 	i_Top
)
{
	return GLGXMatrixOrthoOffCenter(o_Matrix,i_Left,i_Right,i_Bottom, i_Top, -1.0 , 1.0 );
}

GLGXMATRIX * GLGXMatrixFrustum(	
	GLGXMATRIX * o_Matrix,
	double  	i_Left, 
 	double  	i_Right, 
 	double  	i_Bottom, 
 	double  	i_Top, 
 	double  	i_zNear, 
 	double  	i_zFar
	)
{
	GLGXMatrixIdentity(o_Matrix);

	o_Matrix->_11 = (2.0*i_zNear) / (i_Right - i_Left);
	o_Matrix->_22 = (2.0*i_zNear) / (i_Top - i_Bottom);
	o_Matrix->_33 = -(i_zFar + i_zNear) / (i_zFar - i_zNear);
	o_Matrix->_44 = 0.0f;

	o_Matrix->_31 =   (i_Right + i_Left) / (i_Right - i_Left);
	o_Matrix->_32 =   (i_Top + i_Bottom) / (i_Top - i_Bottom);
	o_Matrix->_43 =  -(2*i_zFar*i_zNear) / (i_zFar - i_zNear);

	return o_Matrix;
}


