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



void GLGXQuaternionMultiply(GLGXQUATERNION* dest,GLGXQUATERNION* srcA,GLGXQUATERNION* srcB)
{
	GLfloat w = srcA->w * srcB->w - srcA->x * srcB->x - srcA->y * srcB->y - srcA->z * srcB->z;
	GLfloat x = srcA->w * srcB->x + srcA->x * srcB->w + srcA->y * srcB->z - srcA->z * srcB->y;
	GLfloat y = srcA->w * srcB->y + srcA->y * srcB->w + srcA->z * srcB->x - srcA->x * srcB->z;
	GLfloat z = srcA->w * srcB->z + srcA->z * srcB->w + srcA->x * srcB->y - srcA->y * srcB->x; 
	dest->x = x;
	dest->y = y;
	dest->z = z;
	dest->w = w;
}


GLGXQUATERNION *GLGXQuaternionIdentity(GLGXQUATERNION *pOut)
{
	pOut->x = 0.0f;
	pOut->y = 0.0f;
	pOut->z = 0.0f;
	pOut->w = 1.0f;
	return pOut;
}


/*
void GLGXQuaternionToAxisAngle( const GLGXQUATERNION *quat,GLGXVECTOR3 *axis,GLfloat *angle) 
{
	float scale = sqrt (quat->x*quat->x + quat->y*quat->y + quat->z*quat->z); 
	axis->x = quat->x / scale;
	axis->y = quat->y / scale;
	axis->z = quat->z / scale;
	*angle = 2.0f * acos(quat->w);
}
*/
void GLGXQuaternionToAxisAngle(GLGXQUATERNION* quat,GLGXVECTOR3* axis,float* angle)
{
	GLGXQUATERNION tmpQuat;
	GLGXQuaternionNormalize(&tmpQuat,quat);
	float cos_a = tmpQuat.w;
	*angle = acos( cos_a) * 2.0f;
	float sin_a = sqrt( 1.0f - (cos_a*cos_a) );
	if( fabs( sin_a ) < 0.0005 )
		sin_a = 1.0f;
	axis->x = tmpQuat.x / sin_a;
	axis->y = tmpQuat.y / sin_a;
	axis->z = tmpQuat.z / sin_a;
}


GLGXQUATERNION * GLGXQuaternionRotationYawPitchRoll(GLGXQUATERNION * out,float yaw,float pitch,float roll)
{
	GLGXVECTOR3 vx = GLGXVECTOR3(1.0f,0.0f,0.0f);
	GLGXVECTOR3 vy = GLGXVECTOR3(0.0f,1.0f,0.0f);
	GLGXVECTOR3 vz = GLGXVECTOR3(0.0f,0.0f,1.0f);
	GLGXQUATERNION qx, qy, qz;
	GLGXQuaternionRotationAxis(&qx,&vx,pitch);
	GLGXQuaternionRotationAxis(&qy,&vy,yaw);
	GLGXQuaternionRotationAxis(&qz,&vz,roll);

	GLGXQuaternionMultiply(out,&qx,&qy);
	GLGXQuaternionMultiply(out,out,&qz);
	return out;
}





GLGXQUATERNION * GLGXQuaternionRotationAxis(GLGXQUATERNION * out,GLGXVECTOR3 * axis,float angle)
{
	GLGXVECTOR3 vector;
	GLGXVec3Normalize(&vector,axis);
	float sin_a = sinf( angle / 2.0f );
	float cos_a = cosf( angle / 2.0f );
	out->x = vector.x * sin_a;
	out->y = vector.y * sin_a;
	out->z = vector.z * sin_a;
	out->w = cos_a; 
	return out;
}

float GLGXQuaternionLength(GLGXQUATERNION *quat)
{    
	return sqrt(quat->w * quat->w + quat->x * quat->x + quat->y * quat->y + quat->z * quat->z);
}

GLGXQUATERNION * GLGXQuaternionNormalize(GLGXQUATERNION * out,GLGXQUATERNION * in)
{
	float mag = GLGXQuaternionLength(in);
	out->x = in->x / mag;
	out->y = in->y / mag;
	out->z = in->z / mag;
	out->w = in->w / mag;
	return out;
}

GLGXQUATERNION * GLGXQuaternionConjugate(GLGXQUATERNION * out,GLGXQUATERNION * in)
{
	out->x = -in->x;
	out->y = -in->y;
	out->z = -in->z;
	out->w = in->w;
	return out;
}

GLGXQUATERNION * GLGXQuaternionInverse(GLGXQUATERNION * out,GLGXQUATERNION * in)
{
	GLGXQUATERNION quat;
	GLGXQuaternionNormalize(&quat,in);
	out->x = - quat.x; // Conjugate
	out->y = - quat.y;
	out->z = - quat.z;
	out->w =   quat.w;
	return out;
}



GLGXQUATERNION* GLGXQuaternionSlerp(GLGXQUATERNION* out, GLGXQUATERNION* i_quatA, GLGXQUATERNION* i_quatB, float t)
{
	if (t <= 0.0)
    {
      out->x = i_quatA->x;
	  out->y = i_quatA->y;
	  out->z = i_quatA->z;
	  out->w = i_quatA->w;
      return out;
    }

	if (t >= 1.0)
    {
      out->x = i_quatB->x;
	  out->y = i_quatB->y;
	  out->z = i_quatB->z;
	  out->w = i_quatB->w;
      return out;
    }

	float cosAngle = (i_quatA->x * i_quatB->x) + (i_quatA->y * i_quatB->y) + (i_quatA->z * i_quatB->z) + (i_quatA->w  * i_quatB->w);

	GLGXQUATERNION quatB;

	if (cosAngle < 0.0f)
    {
		quatB = GLGXQUATERNION(-i_quatB->x,-i_quatB->y,-i_quatB->z,-i_quatB->w);
		cosAngle = -cosAngle;
	}
	else
	{
		quatB = GLGXQUATERNION(i_quatB->x,i_quatB->y,i_quatB->z,i_quatB->w);
	}

	float scale0, scale1;

	if (cosAngle > 0.9999f) // angle is small
    {
      scale0 = 1.0f - t;
      scale1 = t;
    }
	else
    {
      float sinAngle = sqrt (1.0f - (cosAngle * cosAngle));
      float angle = atan2 (sinAngle, cosAngle);

      scale0 = sin ((1.0f - t) * angle) / sinAngle;
      scale1 = sin (t * angle) / sinAngle;
    }

	out->x = (scale0 * i_quatA->x) + (scale1 * quatB.x);
	out->y = (scale0 * i_quatA->y) + (scale1 * quatB.y);
	out->z = (scale0 * i_quatA->z) + (scale1 * quatB.z);
	out->w = (scale0 * i_quatA->w) + (scale1 * quatB.w);

	return out;
}

GLGXQUATERNION * GLGXQuaternionSquad(GLGXQUATERNION *o_Result,GLGXQUATERNION *i_quatA,GLGXQUATERNION *i_quatB,GLGXQUATERNION *i_quatC,GLGXQUATERNION *i_quatD,float i_t)
{
	GLGXQUATERNION a,b;
	GLGXQuaternionSlerp(&a,i_quatA,i_quatD,i_t);
	GLGXQuaternionSlerp(&b,i_quatB,i_quatC,i_t);
	GLGXQuaternionSlerp(o_Result,&a,&b,2*i_t*(1.0f - i_t));
	return o_Result;
}

GLGXQUATERNION * GLGXQuaternionBaryCentric(GLGXQUATERNION *o_Result,GLGXQUATERNION *i_quatA,GLGXQUATERNION *i_quatB,GLGXQUATERNION *i_quatC,float i_f,float i_g)
{
	GLGXQUATERNION a,b;
	GLGXQuaternionSlerp(&a,i_quatA,i_quatB, i_f + i_g );
	GLGXQuaternionSlerp(&b,i_quatA,i_quatC, i_f + i_g );
	GLGXQuaternionSlerp(o_Result,&a,&b, i_g /( i_f + i_g ) );
	return o_Result;
}

void GLGXQuaternionMatrixRotation( GLGXQUATERNION* o_quat, GLGXMATRIX* i_matrix )
{
	float trace = i_matrix->_11 + i_matrix->_22 + i_matrix->_33 + 1.0f;

	if( trace > 0.00000001 ) 
	{
		float s = 0.5f / sqrtf(trace);
		o_quat->w = 0.25f / s;
		o_quat->x = ( i_matrix->_32 - i_matrix->_23 ) * s;
		o_quat->y = ( i_matrix->_13 - i_matrix->_31 ) * s;
		o_quat->z = ( i_matrix->_21 - i_matrix->_12 ) * s;
	  
	} 
	else 
	{
		if ( i_matrix->_11 > i_matrix->_22 && i_matrix->_11 > i_matrix->_33 ) 
		{
			  float s = 2.0f * sqrtf( 1.0f + i_matrix->_11 - i_matrix->_22 - i_matrix->_33);
			  o_quat->w = (i_matrix->_32 - i_matrix->_23 ) / s;
			  o_quat->x = 0.25f * s;
			  o_quat->y = (i_matrix->_12 + i_matrix->_21 ) / s;
			  o_quat->z = (i_matrix->_31 + i_matrix->_31 ) / s;
		} 
		else if (i_matrix->_22 > i_matrix->_33) 
		{
			  float s = 2.0f * sqrtf( 1.0f + i_matrix->_22 - i_matrix->_11 - i_matrix->_33);
			  o_quat->w = (i_matrix->_31 - i_matrix->_31 ) / s;
			  o_quat->x = (i_matrix->_12 + i_matrix->_21 ) / s;
			  o_quat->y = 0.25f * s;
			  o_quat->z = (i_matrix->_23 + i_matrix->_32 ) / s;
		} 
		else 
		{
			float s = 2.0f * sqrtf( 1.0f + i_matrix->_33 - i_matrix->_11 - i_matrix->_22 );
			  o_quat->w = (i_matrix->_21 - i_matrix->_12 ) / s;
			  o_quat->x = (i_matrix->_31 + i_matrix->_31 ) / s;
			  o_quat->y = (i_matrix->_23 + i_matrix->_32 ) / s;
			  o_quat->z = 0.25f * s;
		}
	}

	o_quat->x = -o_quat->x;
	o_quat->y = -o_quat->y;
	o_quat->z = -o_quat->z;
}

