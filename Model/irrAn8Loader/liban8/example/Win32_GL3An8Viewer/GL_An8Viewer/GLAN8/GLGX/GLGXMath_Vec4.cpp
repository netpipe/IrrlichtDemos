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

GLGXVECTOR4 GLGXVECTOR4::operator+ (const GLGXVECTOR4 &B)
{ 
	return GLGXVECTOR4( this->x + B.x, this->y + B.y, this->z + B.z, this->w + B.w);
}

GLGXVECTOR4 GLGXVECTOR4::operator- (const GLGXVECTOR4 &B)
{ 
	return GLGXVECTOR4( this->x - B.x, this->y - B.y, this->z - B.z, this->w - B.w);
}


GLfloat GLGXVECTOR4::operator* (const GLGXVECTOR4 &B)
{ 
	return ( this->x * B.x ) + ( this->y * B.y ) + ( this->z * B.z ) + ( this->w * B.w );
}

GLGXVECTOR4 GLGXVECTOR4::operator* (GLfloat factor)
{ 
	return GLGXVECTOR4( this->x * factor, this->y * factor, this->z * factor, this->w * factor); 
}

GLGXVECTOR4 GLGXVECTOR4::operator/ (GLfloat factor)
{ 
	return GLGXVECTOR4( this->x / factor, this->y / factor, this->z / factor, this->w / factor);
}

GLfloat GLGXVec4Length(const GLGXVECTOR4 *i_vec)
{
	return (GLfloat) sqrt( (i_vec->x * i_vec->x) + (i_vec->y * i_vec->y) + (i_vec->z * i_vec->z) + (i_vec->w * i_vec->w) );
}

GLGXVECTOR4 * GLGXVec4Normalize(GLGXVECTOR4 *pOut,const GLGXVECTOR4 *pV)
{
	GLfloat magnitude =(GLfloat) sqrt( (pV->x * pV->x) + (pV->y * pV->y) + (pV->z * pV->z) + (pV->w * pV->w) );
	
	pOut->x = pV->x / magnitude;
	pOut->y = pV->y / magnitude;
	pOut->z = pV->z / magnitude;
	pOut->w = pV->w / magnitude;

	return pOut;
}

void GLGXVec4Lerp(GLGXVECTOR4 *dest,GLGXVECTOR4 *v1,GLGXVECTOR4 *v2,GLfloat l)
{
	GLGXVECTOR4 v = *v2 - *v1;

	*dest = *v1 + v*l;
}

GLGXVECTOR4 * GLGXVec3Transform(GLGXVECTOR4 * dest,const GLGXVECTOR4 * src,const GLGXMATRIX * matrix)
{
	GLGXVECTOR4 result;

	result.x = src->x * matrix->_11 + src->y * matrix->_21 + src->z * matrix->_31 + src->w * matrix->_41;
	result.y = src->x * matrix->_12 + src->y * matrix->_22 + src->z * matrix->_32 + src->w * matrix->_42;
	result.z = src->x * matrix->_13 + src->y * matrix->_23 + src->z * matrix->_33 + src->w * matrix->_43;
	result.w = src->x * matrix->_14 + src->y * matrix->_24 + src->z * matrix->_34 + src->w * matrix->_44;

	dest->x = result.x;
	dest->y = result.y;
	dest->z = result.z;
	dest->w = result.w;
	return dest;
}

GLGXVECTOR4 * GLGXVec4BaryCentric(GLGXVECTOR4 *o_Result,GLGXVECTOR4 *i_src1,GLGXVECTOR4 *i_src2,GLGXVECTOR4 *i_src3,float i_f,float i_g)
{
	*o_Result = *i_src1 + (*i_src2 - *i_src1)*i_f + (*i_src3 - *i_src1)*i_g;
	return o_Result;
}

GLGXVECTOR4* GLGXVec4Cross(GLGXVECTOR4 *i_Out, GLGXVECTOR4 *i_A, GLGXVECTOR4 *i_B, GLGXVECTOR4 *i_C)
{
    float x = i_A->y * (i_B->z * i_C->w - i_C->z * i_B->w) - i_A->z * (i_B->y * i_C->w - i_C->y * i_B->w) + i_A->w * (i_B->y * i_C->z - i_B->z *i_C->y);
    float y = -(i_A->x * (i_B->z * i_C->w - i_C->z * i_B->w) - i_A->z * (i_B->x * i_C->w - i_C->x * i_B->w) + i_A->w * (i_B->x * i_C->z - i_C->x * i_B->z));
    float z = i_A->x * (i_B->y * i_C->w - i_C->y * i_B->w) - i_A->y * (i_B->x *i_C->w - i_C->x * i_B->w) + i_A->w * (i_B->x * i_C->y - i_C->x * i_B->y);
    float w = -(i_A->x * (i_B->y * i_C->z - i_C->y * i_B->z) - i_A->y * (i_B->x * i_C->z - i_C->x *i_B->z) + i_A->z * (i_B->x * i_C->y - i_C->x * i_B->y));
    
	i_Out->x = x;
	i_Out->y = y;
	i_Out->z = z;
	i_Out->w = w;
	return i_Out;
}

GLGXVECTOR4 * GLGXVec4CatmullRom(GLGXVECTOR4 *i_Out,GLGXVECTOR4 *i_Src0,GLGXVECTOR4 *i_Src1,GLGXVECTOR4 *i_Src2,GLGXVECTOR4 *i_Src3,float i_t)
{
	float a = -(i_t*i_t*i_t) + 2*i_t*i_t -i_t;
	float b = 3*i_t*i_t*i_t - 5*i_t*i_t + 2;
	float c = -3*i_t*i_t*i_t + 4*i_t*i_t + i_t;
	float d = i_t*i_t*i_t - i_t*i_t;

	i_Out->x = 0.5f * (a*i_Src0->x + b*i_Src1->x + c*i_Src2->x + d*i_Src3->x);
	i_Out->y = 0.5f * (a*i_Src0->y + b*i_Src1->y + c*i_Src2->y + d*i_Src3->y);
	i_Out->z = 0.5f * (a*i_Src0->z + b*i_Src1->z + c*i_Src2->z + d*i_Src3->z);
	i_Out->w = 0.5f * (a*i_Src0->w + b*i_Src1->w + c*i_Src2->w + d*i_Src3->w);
	return i_Out;
}

GLGXVECTOR4* GLGXVec4Hermite(GLGXVECTOR4 *o_Result, GLGXVECTOR4 *i_Pos0, GLGXVECTOR4 *i_Tan0, GLGXVECTOR4 *i_Pos1, GLGXVECTOR4 *i_Tan1, float i_t)
{
	float a = 2.0f*(i_t*i_t*i_t) - 3.0f*(i_t*i_t) + 1.0f;
	float b = (i_t*i_t*i_t) - 2.0f * (i_t*i_t) + i_t;
	float c = -2.0f *(i_t*i_t*i_t) + 3.0f*(i_t*i_t);
	float d = (i_t*i_t*i_t) - (i_t*i_t);

	o_Result->x = a * (i_Pos0->x) + b * (i_Tan0->x) + c * (i_Pos1->x) + d * (i_Tan1->x);
	o_Result->y = a * (i_Pos0->y) + b * (i_Tan0->y) + c * (i_Pos1->y) + d * (i_Tan1->y);
	o_Result->z = a * (i_Pos0->z) + b * (i_Tan0->z) + c * (i_Pos1->z) + d * (i_Tan1->z);
	o_Result->w = a * (i_Pos0->w) + b * (i_Tan0->w) + c * (i_Pos1->w) + d * (i_Tan1->w);

	return o_Result;
}


GLGXVECTOR4* GLGXVec4BezierDeg2(GLGXVECTOR4 *o_Result, GLGXVECTOR4 *i_P0, GLGXVECTOR4 *i_P1, GLGXVECTOR4 *i_P2, float i_t)
{
	float a = ( 1.0f - i_t )*( 1.0f - i_t );
	float b = 2.0f*i_t*( 1.0f - i_t );
	float c = (i_t*i_t);


	o_Result->x = a * (i_P0->x) + b * (i_P1->x) + c * (i_P2->x);
	o_Result->y = a * (i_P0->y) + b * (i_P1->y) + c * (i_P2->y);
	o_Result->z = a * (i_P0->z) + b * (i_P1->z) + c * (i_P2->z);
	o_Result->w = a * (i_P0->w) + b * (i_P1->w) + c * (i_P2->w);

	return o_Result;
}

GLGXVECTOR4* GLGXVec4BezierDeg3(GLGXVECTOR4 *o_Result, GLGXVECTOR4 *i_P0, GLGXVECTOR4 *i_P1, GLGXVECTOR4 *i_P2, GLGXVECTOR4 *i_P3, float i_t)
{
	float a = ( 1.0f - i_t )*( 1.0f - i_t )*( 1.0f - i_t );
	float b = 3.0f*i_t*( 1.0f - i_t )*( 1.0f - i_t );
	float c = 3.0f*i_t*i_t*( 1.0f - i_t );
	float d = (i_t*i_t*i_t);

	o_Result->x = a * (i_P0->x) + b * (i_P1->x) + c * (i_P2->x) + d * (i_P3->x);
	o_Result->y = a * (i_P0->y) + b * (i_P1->y) + c * (i_P2->y) + d * (i_P3->y);
	o_Result->z = a * (i_P0->z) + b * (i_P1->z) + c * (i_P2->z) + d * (i_P3->z);
	o_Result->w = a * (i_P0->w) + b * (i_P1->w) + c * (i_P2->w) + d * (i_P3->w);

	return o_Result;
}


