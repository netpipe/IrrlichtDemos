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


GLGXVECTOR3 GLGXVECTOR3::operator+ (const GLGXVECTOR3 &B)
{ 
	return GLGXVECTOR3( this->x + B.x, this->y + B.y, this->z + B.z);
}

GLGXVECTOR3 GLGXVECTOR3::operator- (const GLGXVECTOR3 &B)
{ 
	return GLGXVECTOR3( this->x - B.x, this->y - B.y, this->z - B.z);
}

GLfloat GLGXVECTOR3::operator* (const GLGXVECTOR3 &B)
{ 
	return ( this->x * B.x ) + ( this->y * B.y ) + ( this->z * B.z );
}

GLGXVECTOR3 GLGXVECTOR3::operator* (GLfloat factor)
{ 
	return GLGXVECTOR3( this->x * factor, this->y * factor, this->z * factor); 
}

GLGXVECTOR3 GLGXVECTOR3::operator/ (GLfloat factor)
{ 
	return GLGXVECTOR3( this->x / factor, this->y / factor, this->z / factor);
}


/////////////////////////////////////////////////////////////

GLGXVECTOR3* GLGXVec3Cross(GLGXVECTOR3* result,GLGXVECTOR3* A,GLGXVECTOR3* B)
{
	GLfloat x = A->y*B->z - A->z*B->y;
	GLfloat y = A->z*B->x - A->x*B->z;
	GLfloat z = A->x*B->y - A->y*B->x;
	result->x = x;
	result->y = y;
	result->z = z;

	return result;
}

GLfloat GLGXVec3Dot(const GLGXVECTOR3 *srcA,const GLGXVECTOR3 *srcB)
{
	return (srcA->x*srcB->x) + (srcA->y*srcB->y) + (srcA->z*srcB->z);
}


GLGXVECTOR3 *GLGXVec3Normalize(GLGXVECTOR3 *pOut,const GLGXVECTOR3 *pV)
{
	GLfloat magnitude =(GLfloat) sqrt( (pV->x * pV->x) + (pV->y * pV->y) + (pV->z * pV->z) );
	
	pOut->x = pV->x / magnitude;
	pOut->y = pV->y / magnitude;
	pOut->z = pV->z / magnitude;

	return pOut;
}


GLGXVECTOR3 * GLGXVec3PackTo01(GLGXVECTOR3 *pOut,const GLGXVECTOR3 *pV)
{	// Vector is supposed to be normalized
	pOut->x = pV->x*0.5f + 0.5f;
	pOut->y = pV->y*0.5f + 0.5f;
	pOut->z = pV->z*0.5f + 0.5f;

	return pOut;
}

GLfloat GLGXVec3Length(const GLGXVECTOR3 *vector)
{
	return sqrt( vector->x*vector->x + vector->y*vector->y + vector->z*vector->z);
}

GLfloat GLGXVec3Length2(const GLGXVECTOR3 *vector)
{
	return ( vector->x*vector->x + vector->y*vector->y + vector->z*vector->z);
}


void GLGXVec3Lerp(GLGXVECTOR3 *dest,GLGXVECTOR3 *v1,GLGXVECTOR3 *v2,GLfloat l)
{
	GLGXVECTOR3 v = *v2 - *v1;

	*dest = *v1 + v*l;
}

GLGXVECTOR4 * GLGXVec3Transform(GLGXVECTOR4 * dest,const GLGXVECTOR3 * src,const GLGXMATRIX * matrix)
{
	GLGXVECTOR4 result;

	result.x = src->x * matrix->_11 + src->y * matrix->_21 + src->z * matrix->_31 + matrix->_41;
	result.y = src->x * matrix->_12 + src->y * matrix->_22 + src->z * matrix->_32 + matrix->_42;
	result.z = src->x * matrix->_13 + src->y * matrix->_23 + src->z * matrix->_33 + matrix->_43;
	result.w = src->x * matrix->_14 + src->y * matrix->_24 + src->z * matrix->_34 + matrix->_44;

	dest->x = result.x;
	dest->y = result.y;
	dest->z = result.z;
	dest->w = result.w;
	return dest;
}

GLGXVECTOR3 * GLGXVec3TransformCoord(GLGXVECTOR3 * dest,const GLGXVECTOR3 * src,const GLGXMATRIX * matrix)
{
	GLGXVECTOR3 result;

	float w = src->x * matrix->_14 + src->y * matrix->_24 + src->z * matrix->_34 + matrix->_44;
	result.x = src->x * matrix->_11 + src->y * matrix->_21 + src->z * matrix->_31 + matrix->_41;
	result.y = src->x * matrix->_12 + src->y * matrix->_22 + src->z * matrix->_32 + matrix->_42;
	result.z = src->x * matrix->_13 + src->y * matrix->_23 + src->z * matrix->_33 + matrix->_43;
	
	if( w != 0.0f)
	{
		dest->x = result.x / w;
		dest->y = result.y / w;
		dest->z = result.z / w;
	}
	else
	{
		dest->x = 0.0f;
		dest->y = 0.0f;
		dest->z = 0.0f;
	}
	return dest;
}



GLGXVECTOR3 * GLGXVec3TransformNormal(GLGXVECTOR3 * dest,const GLGXVECTOR3 * src,const GLGXMATRIX * matrix)
{
	GLGXVECTOR3 result;

	result.x = src->x * matrix->_11 + src->y * matrix->_21 + src->z * matrix->_31;
	result.y = src->x * matrix->_12 + src->y * matrix->_22 + src->z * matrix->_32;
	result.z = src->x * matrix->_13 + src->y * matrix->_23 + src->z * matrix->_33;
	//result.w = src->x * matrix->_14 + src->y * matrix->_24 + src->z * matrix->_34;

	dest->x = result.x;
	dest->y = result.y;
	dest->z = result.z;
	//dest->w = result.w;
	return dest;
}




GLGXVECTOR3 *GLGXVec3Scale(GLGXVECTOR3 *out,GLGXVECTOR3 *vector,GLfloat s) 
{
	out->x = vector->x / s;
	out->y = vector->y / s;
	out->z = vector->z / s;

	return out;
}


float GLGXAngleBetweenVec3(GLGXVECTOR3 *srcA,GLGXVECTOR3 *srcB)
{
	GLGXVECTOR3 vecA;
	GLGXVec3Normalize(&vecA,srcA);
	GLGXVECTOR3 vecB;
	GLGXVec3Normalize(&vecB,srcB);

	float cosAngle = GLGXVec3Dot(&vecA,&vecB);
	return acosf(cosAngle);
}

GLGXVECTOR3 * GLGXVec3BaryCentric(GLGXVECTOR3 *o_Result,GLGXVECTOR3 *i_src1,GLGXVECTOR3 *i_src2,GLGXVECTOR3 *i_src3,float i_f,float i_g)
{
	*o_Result = *i_src1 + (*i_src2 - *i_src1)*i_f + (*i_src3 - *i_src1)*i_g;
	return o_Result;
}

GLGXVECTOR3 * GLGXVec3CatmullRom(GLGXVECTOR3 *o_Result,GLGXVECTOR3 *i_Src0,GLGXVECTOR3 *i_Src1,GLGXVECTOR3 *i_Src2,GLGXVECTOR3 *i_Src3,float i_t)
{
	float a = -(i_t*i_t*i_t) + 2*(i_t*i_t) - i_t;
	float b = 3*i_t*i_t*i_t - 5*(i_t*i_t) + 2;
	float c = -3*(i_t*i_t*i_t) + 4*(i_t*i_t) + i_t;
	float d = (i_t*i_t*i_t) - (i_t*i_t);

	o_Result->x = 0.5f * (a*i_Src0->x + b*i_Src1->x + c*i_Src2->x + d*i_Src3->x);
	o_Result->y = 0.5f * (a*i_Src0->y + b*i_Src1->y + c*i_Src2->y + d*i_Src3->y);
	o_Result->z = 0.5f * (a*i_Src0->z + b*i_Src1->z + c*i_Src2->z + d*i_Src3->z);
	return o_Result;
}

GLGXVECTOR3* GLGXVec3Hermite(GLGXVECTOR3 *o_Result, GLGXVECTOR3 *i_Pos0, GLGXVECTOR3 *i_Tan0, GLGXVECTOR3 *i_Pos1, GLGXVECTOR3 *i_Tan1, float i_t)
{
	float a = 2.0f*(i_t*i_t*i_t) - 3.0f*(i_t*i_t) + 1.0f;
	float b = (i_t*i_t*i_t) - 2.0f * (i_t*i_t) + i_t;
	float c = -2.0f *(i_t*i_t*i_t) + 3.0f*(i_t*i_t);
	float d = (i_t*i_t*i_t) - (i_t*i_t);

	o_Result->x = a * (i_Pos0->x) + b * (i_Tan0->x) + c * (i_Pos1->x) + d * (i_Tan1->x);
	o_Result->y = a * (i_Pos0->y) + b * (i_Tan0->y) + c * (i_Pos1->y) + d * (i_Tan1->y);
	o_Result->z = a * (i_Pos0->z) + b * (i_Tan0->z) + c * (i_Pos1->z) + d * (i_Tan1->z);

	return o_Result;
}

GLGXVECTOR3* GLGXVec3BezierDeg2(GLGXVECTOR3 *o_Result, GLGXVECTOR3 *i_P0, GLGXVECTOR3 *i_P1, GLGXVECTOR3 *i_P2, float i_t)
{
	float a = ( 1.0f - i_t )*( 1.0f - i_t );
	float b = 2.0f*i_t*( 1.0f - i_t );
	float c = (i_t*i_t);

	o_Result->x = a * (i_P0->x) + b * (i_P1->x) + c * (i_P2->x);
	o_Result->y = a * (i_P0->y) + b * (i_P1->y) + c * (i_P2->y);
	o_Result->z = a * (i_P0->z) + b * (i_P1->z) + c * (i_P2->z);

	return o_Result;
}

GLGXVECTOR3* GLGXVec3BezierDeg3(GLGXVECTOR3 *o_Result, GLGXVECTOR3 *i_P0, GLGXVECTOR3 *i_P1, GLGXVECTOR3 *i_P2, GLGXVECTOR3 *i_P3, float i_t)
{
	float a = ( 1.0f - i_t )*( 1.0f - i_t )*( 1.0f - i_t );
	float b = 3.0f*i_t*( 1.0f - i_t )*( 1.0f - i_t );
	float c = 3.0f*i_t*i_t*( 1.0f - i_t );
	float d = (i_t*i_t*i_t);

	o_Result->x = a * (i_P0->x) + b * (i_P1->x) + c * (i_P2->x) + d * (i_P3->x);
	o_Result->y = a * (i_P0->y) + b * (i_P1->y) + c * (i_P2->y) + d * (i_P3->y);
	o_Result->z = a * (i_P0->z) + b * (i_P1->z) + c * (i_P2->z) + d * (i_P3->z);

	return o_Result;
}
/*
GLGXVECTOR3* GLGXVec3BSpline(GLGXVECTOR3 *o_Result, GLGXVECTOR3 *i_P0, GLGXVECTOR3 *i_P1, GLGXVECTOR3 *i_P2, GLGXVECTOR3 *i_P3, float i_t)
{
	//TMS = 1/6 [(-t^3 + 3t^2 - 3t +1)(+3t^3 - 6t^2 + 4)(-3t^3 + 3t^2 + 3t +1)(t^3)]
	//float a = (-t^3 + 3t^2 - 3t +1)/6.0f;
	//float b = (+3t^3 - 6t^2 + 4)/6.0f;
	//float c = (-3t^3 + 3t^2 + 3t +1)/6.0f;
	//float d = (t^3)/6.0f;
}
*/
