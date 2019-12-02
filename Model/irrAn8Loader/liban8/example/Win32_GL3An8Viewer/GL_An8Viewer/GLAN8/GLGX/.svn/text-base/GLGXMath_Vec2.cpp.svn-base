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


GLGXVECTOR2 GLGXVECTOR2::operator+ (const GLGXVECTOR2 &B)
{ 
	return GLGXVECTOR2(this->x+B.x,this->y+B.y);
}

GLGXVECTOR2 GLGXVECTOR2::operator- (const GLGXVECTOR2 &B)
{ 
	return GLGXVECTOR2(this->x-B.x,this-> y-B.y);
}

GLfloat GLGXVECTOR2::operator* (const GLGXVECTOR2 &B)
{ 
	return ( this->x * B.x ) + ( this->y * B.y );
}

GLGXVECTOR2 GLGXVECTOR2::operator* (GLfloat factor)
{ 
	return GLGXVECTOR2(this->x*factor,this->y*factor); 
}

GLGXVECTOR2 GLGXVECTOR2::operator/ (GLfloat factor)
{ 
	return GLGXVECTOR2(this->x/factor,this->y/factor);
}


GLfloat GLGXVec2Length(const GLGXVECTOR2 *vector)
{
	return sqrt( vector->x*vector->x + vector->y*vector->y);
}

GLfloat GLGXVec2Length2(const GLGXVECTOR2 *vector)
{
	return ( vector->x*vector->x + vector->y*vector->y);
}


void GLGXVec2Lerp(GLGXVECTOR2 *dest,GLGXVECTOR2 *v1,GLGXVECTOR2 *v2,GLfloat l)
{
	GLGXVECTOR2 v = *v2 - *v1;

	*dest = *v1 + v*l;
}

GLfloat GLGXVec2Dot(const GLGXVECTOR2 *srcA,const GLGXVECTOR2 *srcB)
{
	return (srcA->x*srcB->x) + (srcA->y*srcB->y);
}

GLGXVECTOR2 *GLGXVec2Scale(GLGXVECTOR2 *out,GLGXVECTOR2 *vector,GLfloat s) 
{
	out->x = vector->x / s;
	out->y = vector->y / s;
	return out;
}


float GLGXAngleBetweenVec2(GLGXVECTOR2 *srcA,GLGXVECTOR2 *srcB)
{
	GLGXVECTOR2 vecA;
	GLGXVec2Normalize(&vecA,srcA);
	GLGXVECTOR2 vecB;
	GLGXVec2Normalize(&vecB,srcB);

	float cosAngle = GLGXVec2Dot(&vecA,&vecB);
	return acosf(cosAngle);
}

GLGXVECTOR2 * GLGXVec2Normalize(GLGXVECTOR2 *pOut,const GLGXVECTOR2 *pV)
{
	GLfloat magnitude =(GLfloat) sqrt( (pV->x * pV->x) + (pV->y * pV->y) );
	
	pOut->x = pV->x / magnitude;
	pOut->y = pV->y / magnitude;
	return pOut;
}

GLGXVECTOR4 * GLGXVec2Transform(GLGXVECTOR4 * o_Out,const GLGXVECTOR2 * i_Src,const GLGXMATRIX * i_Matrix)
{
	GLGXVECTOR4 result;

	result.x = i_Src->x * i_Matrix->_11 + i_Src->y * i_Matrix->_21 + i_Matrix->_41;
	result.y = i_Src->x * i_Matrix->_12 + i_Src->y * i_Matrix->_22 + i_Matrix->_42;
	result.z = i_Src->x * i_Matrix->_13 + i_Src->y * i_Matrix->_23 + i_Matrix->_43;
	result.w = i_Src->x * i_Matrix->_14 + i_Src->y * i_Matrix->_24 + i_Matrix->_44;

	o_Out->x = result.x;
	o_Out->y = result.y;
	o_Out->z = result.z;
	o_Out->w = result.w;
	return o_Out;
}


GLGXVECTOR2 * GLGXVec2TransformCoord(GLGXVECTOR2 * dest,const GLGXVECTOR2 * src,const GLGXMATRIX * matrix)
{
	GLGXVECTOR2 result;

	float w = src->x * matrix->_14 + src->y * matrix->_24  + matrix->_44;
	result.x = src->x * matrix->_11 + src->y * matrix->_21 + matrix->_41;
	result.y = src->x * matrix->_12 + src->y * matrix->_22 + matrix->_42;
	
	if( w != 0.0f)
	{
		dest->x = result.x / w;
		dest->y = result.y / w;
	}
	else
	{
		dest->x = 0.0f;
		dest->y = 0.0f;
	}
	return dest;
}


GLGXVECTOR2 * GLGXVec2TransformNormal(GLGXVECTOR2 * dest,const GLGXVECTOR2 * src,const GLGXMATRIX * matrix)
{
	GLGXVECTOR3 result;

	result.x = src->x * matrix->_11 + src->y * matrix->_21 ;
	result.y = src->x * matrix->_12 + src->y * matrix->_22 ;

	dest->x = result.x;
	dest->y = result.y;

	return dest;
}



GLGXVECTOR2 * GLGXVec2BaryCentric(GLGXVECTOR2 *o_Result,GLGXVECTOR2 *i_src1,GLGXVECTOR2 *i_src2,GLGXVECTOR2 *i_src3,float i_f,float i_g)
{
	*o_Result = *i_src1 + (*i_src2 - *i_src1)*i_f + (*i_src3 - *i_src1)*i_g;
	return o_Result;
}

GLGXVECTOR2 * GLGXVec2CatmullRom(GLGXVECTOR2 *i_Out,GLGXVECTOR2 *i_Src0,GLGXVECTOR2 *i_Src1,GLGXVECTOR2 *i_Src2,GLGXVECTOR2 *i_Src3,float i_t)
{
	float a = -(i_t*i_t*i_t) + 2*i_t*i_t -i_t;
	float b = 3*i_t*i_t*i_t - 5*i_t*i_t + 2;
	float c = -3*i_t*i_t*i_t + 4*i_t*i_t + i_t;
	float d = i_t*i_t*i_t - i_t*i_t;

	i_Out->x = 0.5f * (a*i_Src0->x + b*i_Src1->x + c*i_Src2->x + d*i_Src3->x);
	i_Out->y = 0.5f * (a*i_Src0->y + b*i_Src1->y + c*i_Src2->y + d*i_Src3->y);
	return i_Out;
}

GLGXVECTOR2* GLGXVec2Hermite(GLGXVECTOR2 *o_Result, GLGXVECTOR2 *i_Pos0, GLGXVECTOR2 *i_Tan0, GLGXVECTOR2 *i_Pos1, GLGXVECTOR2 *i_Tan1, float i_t)
{
	float a = 2.0f*(i_t*i_t*i_t) - 3.0f*(i_t*i_t) + 1.0f;
	float b = (i_t*i_t*i_t) - 2.0f * (i_t*i_t) + i_t;
	float c = -2.0f *(i_t*i_t*i_t) + 3.0f*(i_t*i_t);
	float d = (i_t*i_t*i_t) - (i_t*i_t);

	o_Result->x = a * (i_Pos0->x) + b * (i_Tan0->x) + c * (i_Pos1->x) + d * (i_Tan1->x);
	o_Result->y = a * (i_Pos0->y) + b * (i_Tan0->y) + c * (i_Pos1->y) + d * (i_Tan1->y);

	return o_Result;
}


GLGXVECTOR2* GLGXVec2BezierDeg2(GLGXVECTOR2 *o_Result, GLGXVECTOR2 *i_P0, GLGXVECTOR2 *i_P1, GLGXVECTOR2 *i_P2, float i_t)
{
	float a = ( 1.0f - i_t )*( 1.0f - i_t );
	float b = 2.0f*i_t*( 1.0f - i_t );
	float c = (i_t*i_t);

	o_Result->x = a * (i_P0->x) + b * (i_P1->x) + c * (i_P2->x);
	o_Result->y = a * (i_P0->y) + b * (i_P1->y) + c * (i_P2->y);

	return o_Result;
}

GLGXVECTOR2* GLGXVec2BezierDeg3(GLGXVECTOR2 *o_Result, GLGXVECTOR2 *i_P0, GLGXVECTOR2 *i_P1, GLGXVECTOR2 *i_P2, GLGXVECTOR2 *i_P3, float i_t)
{
	float a = ( 1.0f - i_t )*( 1.0f - i_t )*( 1.0f - i_t );
	float b = 3.0f*i_t*( 1.0f - i_t )*( 1.0f - i_t );
	float c = 3.0f*i_t*i_t*( 1.0f - i_t );
	float d = (i_t*i_t*i_t);

	o_Result->x = a * (i_P0->x) + b * (i_P1->x) + c * (i_P2->x) + d * (i_P3->x);
	o_Result->y = a * (i_P0->y) + b * (i_P1->y) + c * (i_P2->y) + d * (i_P3->y);

	return o_Result;
}

