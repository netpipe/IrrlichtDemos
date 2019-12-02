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



GLGXPLANE * GLGXPlaneNormalize(GLGXPLANE * pDest, GLGXPLANE * pSrc)
{
	GLfloat magnitude = (GLfloat)sqrt( pSrc->a * pSrc->a + 
								   pSrc->b * pSrc->b + 
								   pSrc->c * pSrc->c );

	pDest->a /= magnitude;
	pDest->b /= magnitude;
	pDest->c /= magnitude;
	pDest->d /= magnitude; 

	return pDest;
}

float GLGXVec3DistanceToPlane( GLGXPLANE * i_Plane, GLGXVECTOR3* i_Point)
{
	return i_Plane->a * i_Point->x + i_Plane->b * i_Point->y + i_Plane->c * i_Point->z + i_Plane->d ;
}


void GLGXFRUSTUM::Compute(GLfloat* pProjectionMatrix, GLfloat* pModelViewMatrix)
{
	GLfloat   fMatClip[16];		// This will hold the fMatClipping planes

	fMatClip[ 0] = pModelViewMatrix[0] * pProjectionMatrix[0] + pModelViewMatrix[1] * pProjectionMatrix[4] + pModelViewMatrix[2] * pProjectionMatrix[ 8] + pModelViewMatrix[3] * pProjectionMatrix[12];
	fMatClip[ 1] = pModelViewMatrix[0] * pProjectionMatrix[1] + pModelViewMatrix[1] * pProjectionMatrix[5] + pModelViewMatrix[2] * pProjectionMatrix[ 9] + pModelViewMatrix[3] * pProjectionMatrix[13];
	fMatClip[ 2] = pModelViewMatrix[0] * pProjectionMatrix[2] + pModelViewMatrix[1] * pProjectionMatrix[6] + pModelViewMatrix[2] * pProjectionMatrix[10] + pModelViewMatrix[3] * pProjectionMatrix[14];
	fMatClip[ 3] = pModelViewMatrix[0] * pProjectionMatrix[3] + pModelViewMatrix[1] * pProjectionMatrix[7] + pModelViewMatrix[2] * pProjectionMatrix[11] + pModelViewMatrix[3] * pProjectionMatrix[15];

	fMatClip[ 4] = pModelViewMatrix[4] * pProjectionMatrix[0] + pModelViewMatrix[5] * pProjectionMatrix[4] + pModelViewMatrix[6] * pProjectionMatrix[ 8] + pModelViewMatrix[7] * pProjectionMatrix[12];
	fMatClip[ 5] = pModelViewMatrix[4] * pProjectionMatrix[1] + pModelViewMatrix[5] * pProjectionMatrix[5] + pModelViewMatrix[6] * pProjectionMatrix[ 9] + pModelViewMatrix[7] * pProjectionMatrix[13];
	fMatClip[ 6] = pModelViewMatrix[4] * pProjectionMatrix[2] + pModelViewMatrix[5] * pProjectionMatrix[6] + pModelViewMatrix[6] * pProjectionMatrix[10] + pModelViewMatrix[7] * pProjectionMatrix[14];
	fMatClip[ 7] = pModelViewMatrix[4] * pProjectionMatrix[3] + pModelViewMatrix[5] * pProjectionMatrix[7] + pModelViewMatrix[6] * pProjectionMatrix[11] + pModelViewMatrix[7] * pProjectionMatrix[15];

	fMatClip[ 8] = pModelViewMatrix[8] * pProjectionMatrix[0] + pModelViewMatrix[9] * pProjectionMatrix[4] + pModelViewMatrix[10] * pProjectionMatrix[ 8] + pModelViewMatrix[11] * pProjectionMatrix[12];
	fMatClip[ 9] = pModelViewMatrix[8] * pProjectionMatrix[1] + pModelViewMatrix[9] * pProjectionMatrix[5] + pModelViewMatrix[10] * pProjectionMatrix[ 9] + pModelViewMatrix[11] * pProjectionMatrix[13];
	fMatClip[10] = pModelViewMatrix[8] * pProjectionMatrix[2] + pModelViewMatrix[9] * pProjectionMatrix[6] + pModelViewMatrix[10] * pProjectionMatrix[10] + pModelViewMatrix[11] * pProjectionMatrix[14];
	fMatClip[11] = pModelViewMatrix[8] * pProjectionMatrix[3] + pModelViewMatrix[9] * pProjectionMatrix[7] + pModelViewMatrix[10] * pProjectionMatrix[11] + pModelViewMatrix[11] * pProjectionMatrix[15];

	fMatClip[12] = pModelViewMatrix[12] * pProjectionMatrix[0] + pModelViewMatrix[13] * pProjectionMatrix[4] + pModelViewMatrix[14] * pProjectionMatrix[ 8] + pModelViewMatrix[15] * pProjectionMatrix[12];
	fMatClip[13] = pModelViewMatrix[12] * pProjectionMatrix[1] + pModelViewMatrix[13] * pProjectionMatrix[5] + pModelViewMatrix[14] * pProjectionMatrix[ 9] + pModelViewMatrix[15] * pProjectionMatrix[13];
	fMatClip[14] = pModelViewMatrix[12] * pProjectionMatrix[2] + pModelViewMatrix[13] * pProjectionMatrix[6] + pModelViewMatrix[14] * pProjectionMatrix[10] + pModelViewMatrix[15] * pProjectionMatrix[14];
	fMatClip[15] = pModelViewMatrix[12] * pProjectionMatrix[3] + pModelViewMatrix[13] * pProjectionMatrix[7] + pModelViewMatrix[14] * pProjectionMatrix[11] + pModelViewMatrix[15] * pProjectionMatrix[15];
	
	// RIGHT side of the frustum
	Right.a = fMatClip[ 3] - fMatClip[ 0];
	Right.b = fMatClip[ 7] - fMatClip[ 4];
	Right.c = fMatClip[11] - fMatClip[ 8];
	Right.d = fMatClip[15] - fMatClip[12];
	GLGXPlaneNormalize(&Right, &Right);

	// LEFT side of the frustum
	Left.a = fMatClip[ 3] + fMatClip[ 0];
	Left.b = fMatClip[ 7] + fMatClip[ 4];
	Left.c = fMatClip[11] + fMatClip[ 8];
	Left.d = fMatClip[15] + fMatClip[12];
	GLGXPlaneNormalize(&Left, &Left);

	// BOTTOM side of the frustum
	Bottom.a = fMatClip[ 3] + fMatClip[ 1];
	Bottom.b = fMatClip[ 7] + fMatClip[ 5];
	Bottom.c = fMatClip[11] + fMatClip[ 9];
	Bottom.d = fMatClip[15] + fMatClip[13];
	GLGXPlaneNormalize(&Bottom, &Bottom);

	// TOP side of the frustum
	Top.a = fMatClip[ 3] - fMatClip[ 1];
	Top.b = fMatClip[ 7] - fMatClip[ 5];
	Top.c = fMatClip[11] - fMatClip[ 9];
	Top.d = fMatClip[15] - fMatClip[13];
	GLGXPlaneNormalize(&Top, &Top);

	// BACK side of the frustum
	Back.a = fMatClip[ 3] - fMatClip[ 2];
	Back.b = fMatClip[ 7] - fMatClip[ 6];
	Back.c = fMatClip[11] - fMatClip[10];
	Back.d = fMatClip[15] - fMatClip[14];
	GLGXPlaneNormalize(&Back, &Back);

	// FRONT side of the frustum
	Front.a = fMatClip[ 3] + fMatClip[ 2];
	Front.b = fMatClip[ 7] + fMatClip[ 6];
	Front.c = fMatClip[11] + fMatClip[10];
	Front.d = fMatClip[15] + fMatClip[14];
	GLGXPlaneNormalize(&Front, &Front);
}



bool GLGXFRUSTUM::IsPointInside( GLfloat x, GLfloat y, GLfloat z )
{

	//  (a*x + b*y + b*z + d = 0) 
    
	if(Right.a * x + Right.b * y + Right.c * z + Right.d <= 0)
	{
      return false; 
	}
	if(Left.a * x + Left.b * y + Left.c * z + Left.d <= 0)
	{
      return false; 
	}
	if(Top.a * x + Top.b * y + Top.c * z + Top.d <= 0)
	{
      return false; 
	}
	if(Bottom.a * x + Bottom.b * y + Bottom.c * z + Bottom.d <= 0)
	{
      return false; 
	}
	if(Front.a * x + Front.b * y + Front.c * z + Front.d <= 0)
	{
      return false; 
	}
	if(Back.a * x + Back.b * y + Back.c * z + Back.d <= 0)
	{
      return false;
	}

  return true; 
}

bool GLGXFRUSTUM::IsSphereInside( GLfloat x, GLfloat y, GLfloat z, GLfloat radius )
{	
	if( Right.a * x + Right.b * y + Right.c * z + Right.d <= -radius )
	{
	  return false;
	}
	if( Left.a * x + Left.b * y + Left.c * z + Left.d <= -radius )
	{
	  return false; 
	}
	if( Top.a * x + Top.b * y + Top.c * z + Top.d <= -radius )
	{
	  return false; 
	}
	if( Bottom.a * x + Bottom.b * y + Bottom.c * z + Bottom.d <= -radius )
	{
	  return false; 
	}
	if( Front.a * x + Front.b * y + Front.c * z + Front.d <= -radius )
	{
	  return false; 
	}
	if( Back.a * x + Back.b * y + Back.c * z + Back.d <= -radius )
	{
	  return false; 
	}

	return true; 
}

bool GLGXComputeBoundingBox(float* i_Vertices, int i_NbrVertex, int i_Stride, GLGXVECTOR3* o_MinAABB, GLGXVECTOR3* o_MaxAABB)
{
	if( i_Vertices == NULL || i_NbrVertex == 0 )
		return false;

	float lcNewX,lcNewY,lcNewZ;
	lcNewX=lcNewY=lcNewZ=0.0f;

	float minX,maxX,minY,maxY,minZ,maxZ;
	minX=maxX=minY=maxY=minZ=maxZ=0.0f;

	minX = minY = minZ =  9999999.0f;
	maxX = maxY = maxZ = -9999999.0f;

	for(int p = 0 ; p < i_NbrVertex ; ++p)
	{
		int index = p*i_Stride;
		lcNewX = i_Vertices[index];   // x
		lcNewY = i_Vertices[index+1]; // y
		lcNewZ = i_Vertices[index+2]; // z

        if(lcNewX<minX)
          minX=lcNewX;
        if(lcNewX>maxX)
          maxX=lcNewX;

        if(lcNewY<minY)
          minY=lcNewY;
        if(lcNewY>maxY)
          maxY=lcNewY;

        if(lcNewZ<minZ)
          minZ=lcNewZ;
        if(lcNewZ>maxZ)
          maxZ=lcNewZ;

	} // End of point

	o_MinAABB->x = minX;
	o_MaxAABB->x = maxX;

	o_MinAABB->y = minY;
	o_MaxAABB->y = maxY;

	o_MinAABB->z = minZ;
	o_MaxAABB->z = maxZ;

	return true;
}

bool GLGXComputeBoundingRectangle(float* i_Vertices, int i_NbrVertex, int i_Stride, GLGXVECTOR2* o_MinAABB, GLGXVECTOR2* o_MaxAABB)
{
	if( i_Vertices == NULL || i_NbrVertex == 0 )
		return false;

	float lcNewX,lcNewY;
	lcNewX=lcNewY=0.0f;

	float minX,maxX,minY,maxY;
	minX=maxX=minY=maxY=0.0f;

	minX = minY =  9999999.0f;
	maxX = maxY = -9999999.0f;

	for(int p = 0 ; p < i_NbrVertex ; ++p)
	{
		int index = p*i_Stride;
		lcNewX = i_Vertices[index];   // x
		lcNewY = i_Vertices[index+1]; // y

        if(lcNewX<minX)
          minX=lcNewX;
        if(lcNewX>maxX)
          maxX=lcNewX;

        if(lcNewY<minY)
          minY=lcNewY;
        if(lcNewY>maxY)
          maxY=lcNewY;

	} // End of point

	o_MinAABB->x = minX;
	o_MaxAABB->x = maxX;

	o_MinAABB->y = minY;
	o_MaxAABB->y = maxY;

	return true;
}

void GLGXYawPitchRollRotationMatrix(double* o_Yaw, double* o_Pitch, double* o_Roll, GLGXMATRIX* i_Matrix)
{
   double threshold = 0.001; 

   *o_Pitch = asin(-i_Matrix->_32);
   double cosPitch      = cos(*o_Pitch);

   if(cosPitch > threshold) 
   {
      *o_Roll = atan2(i_Matrix->_12, i_Matrix->_22);
      *o_Yaw  = atan2(i_Matrix->_31, i_Matrix->_33);
   }
   else 
   {
      *o_Roll = atan2(-i_Matrix->_21, i_Matrix->_11);
      *o_Yaw  = 0.0;
   }
} 

void GLGXYawPitchRollQuaternion(double* o_Yaw, double* o_Pitch, double* o_Roll, GLGXQUATERNION* i_Quat )
{
	double testSingularity = i_Quat->x*i_Quat->y + i_Quat->z*i_Quat->w;
	if (testSingularity > 0.499) 
	{
		*o_Yaw = 2 * atan2(i_Quat->x,i_Quat->w);
		*o_Roll = GLGX_PI/2;
		*o_Pitch = 0;
		return;
	}
	if (testSingularity < -0.499) 
	{ 
		*o_Yaw = -2 * atan2(i_Quat->x,i_Quat->w);
		*o_Roll = - GLGX_PI/2;
		*o_Pitch = 0;
		return;
	}
    double sqx = i_Quat->x*i_Quat->x;
    double sqy = i_Quat->y*i_Quat->y;
    double sqz = i_Quat->z*i_Quat->z;

	double qx = i_Quat->x;
	double qy = i_Quat->y;
	double qz = i_Quat->z;
	double qw = i_Quat->w;

    *o_Yaw = atan2( 2.0 * qy * qw - 2.0 * qx * qz ,1.0 - 2.0*sqy - 2.0*sqz); // Y
	*o_Roll = asin(2*testSingularity); // Z
	*o_Pitch = atan2(2*qx*qw-2*qy*qz , 1 - 2*sqx - 2*sqz); // X
}


void GLGXMatrixDecompose( 
		GLGXVECTOR3 *o_Scale,GLGXQUATERNION *o_Rotation,GLGXVECTOR3 *o_Translation,
		GLGXMATRIX *i_Matrix)
{
	o_Scale->x = sqrt(i_Matrix->_11*i_Matrix->_11 + i_Matrix->_12*i_Matrix->_12 + i_Matrix->_13*i_Matrix->_13);
	o_Scale->y = sqrt(i_Matrix->_21*i_Matrix->_21 + i_Matrix->_22*i_Matrix->_22 + i_Matrix->_23*i_Matrix->_23);
	o_Scale->z = sqrt(i_Matrix->_31*i_Matrix->_31 + i_Matrix->_32*i_Matrix->_32 + i_Matrix->_33*i_Matrix->_33);

	float determinant = i_Matrix->_11 * (i_Matrix->_21 * i_Matrix->_31 - i_Matrix->_24 * i_Matrix->_22) - i_Matrix->_14 * (i_Matrix->_12 * i_Matrix->_31 - i_Matrix->_24 * i_Matrix->_13) + i_Matrix->_23 * (i_Matrix->_12 * i_Matrix->_22 - i_Matrix->_21 * i_Matrix->_13);
	float sign = determinant / fabs( determinant );
	o_Scale->x = o_Scale->x * sign;
	o_Scale->y = o_Scale->y * sign;
	o_Scale->z = o_Scale->z * sign;

	GLGXQuaternionMatrixRotation(o_Rotation,i_Matrix);

	o_Translation->x = i_Matrix->_41;
	o_Translation->y = i_Matrix->_42;
	o_Translation->z = i_Matrix->_43;
}


void GLGXComputeBoundingCircle( float* i_Array, int i_NbrVertex, int i_Stride, GLGXVECTOR2* o_Center, float* o_Radius)
{
    // Look for the index of Min point and Max point which define the BoundingBox 
	// Tempory BoundingBox values:
	GLGXVECTOR2 bbMin(i_Array[0],i_Array[1]);
	GLGXVECTOR2 bbMax(i_Array[0],i_Array[1]); 

	int   ptMinX, ptMaxX, ptMinY, ptMaxY;  // Indices of extremities of the BoundingBox
	ptMinX = ptMaxX = ptMinY = ptMaxY = 0;

    for ( int i = 1; i < i_NbrVertex; ++i) 
	{
		// Min/Max X
        if( i_Array[i*i_Stride] < bbMin.x) 
		{
            bbMin.x = i_Array[i*i_Stride];
            ptMinX  = i*i_Stride;
        }
        else if( i_Array[i*i_Stride] > bbMax.x) 
		{
            bbMax.x = i_Array[i*i_Stride];
            ptMaxX  = i*i_Stride;
        }

		// Min/Max Y
        if( i_Array[i*i_Stride+1] < bbMin.y) 
		{
            bbMin.y = i_Array[i*i_Stride+1];
            ptMinY = i*i_Stride;
        }
        else if( i_Array[i*i_Stride+1] > bbMax.y) 
		{
            bbMax.y = i_Array[i*i_Stride+1];
            ptMaxY = i*i_Stride;
        }
    }
   
    GLGXVECTOR2 maxDistanceX = GLGXVECTOR2( // Max distance bteween points
		i_Array[ptMaxX] - i_Array[ptMinX],
		i_Array[ptMaxX+1] - i_Array[ptMinX+1]
	); 
    GLGXVECTOR2 maxDistanceY = GLGXVECTOR2( // Max distance bteween points
		i_Array[ptMaxY] - i_Array[ptMinY],
		i_Array[ptMaxY+1] - i_Array[ptMinY+1]
	); 
    
	GLGXVECTOR2 ballCenter; 
	float radius;
	float radius2; // radius*radius

	if( GLGXVec2Length2(&maxDistanceX) >= GLGXVec2Length2(&maxDistanceY) ) 
	{                   
        ballCenter = GLGXVECTOR2(
			i_Array[ptMinX]   + (maxDistanceX.x / 2.0),
			i_Array[ptMinX+1] + (maxDistanceX.y / 2.0)
		);        
		GLGXVECTOR2 vec2 = GLGXVECTOR2(
			i_Array[ptMaxX]   - ballCenter.x,
			i_Array[ptMaxX+1] - ballCenter.y
		);
		radius2 = GLGXVec2Length2(&vec2);
    }
    else 
	{   
        ballCenter = GLGXVECTOR2(
			i_Array[ptMinY]   + (maxDistanceY.x / 2.0),
			i_Array[ptMinY+1] + (maxDistanceY.y / 2.0)
		); 
		GLGXVECTOR2 vec2 = GLGXVECTOR2(
			i_Array[ptMaxY]   - ballCenter.x,
			i_Array[ptMaxY+1] - ballCenter.y
		);
		radius2 = GLGXVec2Length2(&vec2);
    }
    radius = sqrt(radius2);

	// Expand the circle if vertices are not in current circle
    GLGXVECTOR2 ball2Vec2;
    float ball2Vec2Length, ball2Vec2Length2;

    for( int i = 0; i < i_NbrVertex; ++i ) 
	{
        ball2Vec2 = GLGXVECTOR2(
			i_Array[i*i_Stride]   - ballCenter.x,
			i_Array[i*i_Stride+1] - ballCenter.y
		);
		ball2Vec2Length2 = GLGXVec2Length2(&ball2Vec2);
        if (ball2Vec2Length2 <= radius2) // Vertex is inside the sphere
            continue;

		// New Radius, new center
        ball2Vec2Length = sqrt(ball2Vec2Length2);
        radius = (radius + ball2Vec2Length) / 2.0;
        radius2 = radius * radius;
		ballCenter = ballCenter + ( ball2Vec2 * (  (ball2Vec2Length-radius) / ball2Vec2Length )  );
    }
    *o_Center = ballCenter;
    *o_Radius = radius;
    return;
}


void GLGXComputeBoundingSphere( float* i_Array, int i_NbrVertex, int i_Stride, GLGXVECTOR3* o_Center, float* o_Radius)
{
    // Look for the index of Min point and Max point which define the BoundingBox 
	// Tempory BoundingBox values:
	GLGXVECTOR3 bbMin(i_Array[0],i_Array[1],i_Array[2]);
	GLGXVECTOR3 bbMax(i_Array[0],i_Array[1],i_Array[2]);

	// Indices of extremities of the BoundingBox
	int   ptMinX, ptMaxX, ptMinY, ptMaxY, ptMinZ, ptMaxZ;  
	ptMinX = ptMaxX = ptMinY = ptMaxY = ptMinZ = ptMaxZ = 0;

    for (int i = 1; i < i_NbrVertex; i++) 
	{
		// Min/Max X
        if (i_Array[i*i_Stride] < bbMin.x) 
		{
            bbMin.x = i_Array[i*i_Stride];
            ptMinX = i*i_Stride;
        }
        else if (i_Array[i*i_Stride] > bbMax.x) 
		{
            bbMax.x = i_Array[i*i_Stride];
            ptMaxX = i*i_Stride;
        }

		// Min/Max Y
        if (i_Array[i*i_Stride+1] < bbMin.y) 
		{
            bbMin.y = i_Array[i*i_Stride+1];
            ptMinY = i*i_Stride;
        }
        else if (i_Array[i*i_Stride+1] > bbMax.y) 
		{
            bbMax.y = i_Array[i*i_Stride+1];
            ptMaxY = i*i_Stride;
        }

		// Min/Max Z
		if (i_Array[i*i_Stride+2] < bbMin.z) 
		{
            bbMin.z = i_Array[i*i_Stride+2];
            ptMinZ = i*i_Stride;
        }
        else if (i_Array[i*i_Stride+2] > bbMax.z) 
		{
            bbMax.z = i_Array[i*i_Stride+2];
            ptMaxZ = i*i_Stride;
        }
    }

    GLGXVECTOR3 maxDistanceX = GLGXVECTOR3(
		i_Array[ptMaxX] - i_Array[ptMinX],
		i_Array[ptMaxX+1] - i_Array[ptMinX+1],
		i_Array[ptMaxX+2] - i_Array[ptMinX+2]
	); 
    GLGXVECTOR3 maxDistanceY = GLGXVECTOR3(
		i_Array[ptMaxY] - i_Array[ptMinY],
		i_Array[ptMaxY+1] - i_Array[ptMinY+1],
		i_Array[ptMaxY+2] - i_Array[ptMinY+2]
	); 
	GLGXVECTOR3 maxDistanceZ = GLGXVECTOR3(
		i_Array[ptMaxZ] - i_Array[ptMinZ],
		i_Array[ptMaxZ+1] - i_Array[ptMinZ+1],
		i_Array[ptMaxZ+2] - i_Array[ptMinZ+2]
	); 
    
    float fMaxDistX2 = GLGXVec3Length2(&maxDistanceX);
	float fMaxDistY2 = GLGXVec3Length2(&maxDistanceY);
	float fMaxDistZ2 = GLGXVec3Length2(&maxDistanceZ);

	GLGXVECTOR3 ballCenter; 
    float radius;
	float radius2; // radius*radius
	
	if( fMaxDistX2 >= fMaxDistY2 && fMaxDistX2 >= fMaxDistZ2) 
	{                   
        ballCenter = GLGXVECTOR3(
			i_Array[ptMinX]   + (maxDistanceX.x / 2.0),
			i_Array[ptMinX+1] + (maxDistanceX.y / 2.0),
			i_Array[ptMinX+2] + (maxDistanceX.z / 2.0)
		);        
		GLGXVECTOR3 vec3 = GLGXVECTOR3( 
			i_Array[ptMaxX]   - ballCenter.x,
			i_Array[ptMaxX+1] - ballCenter.y,
			i_Array[ptMaxX+2] - ballCenter.z
		);
		radius2 = GLGXVec3Length2(&vec3);
    }
    else if( fMaxDistY2 >= fMaxDistX2 && fMaxDistY2 >= fMaxDistZ2) 
	{   
        ballCenter = GLGXVECTOR3(
			i_Array[ptMinY]   + (maxDistanceY.x / 2.0),
			i_Array[ptMinY+1] + (maxDistanceY.y / 2.0),
			i_Array[ptMinY+2] + (maxDistanceY.z / 2.0)
		); 
		GLGXVECTOR3 vec3 = GLGXVECTOR3(
			i_Array[ptMaxY]   - ballCenter.x,
			i_Array[ptMaxY+1] - ballCenter.y,
			i_Array[ptMaxY+2] - ballCenter.z
		);
		radius2 = GLGXVec3Length2(&vec3);
    }
	else
	{   
        ballCenter = GLGXVECTOR3(
			i_Array[ptMinZ]   + (maxDistanceZ.x / 2.0),
			i_Array[ptMinZ+1] + (maxDistanceZ.y / 2.0),
			i_Array[ptMinZ+2] + (maxDistanceZ.z / 2.0)
		); 
		GLGXVECTOR3 vec3 = GLGXVECTOR3(
			i_Array[ptMaxZ]   - ballCenter.x,
			i_Array[ptMaxZ+1] - ballCenter.y,
			i_Array[ptMaxZ+2] - ballCenter.z
		);
		radius2 = GLGXVec3Length2(&vec3);
    }
    radius = sqrt(radius2);

	// Expand the Sphere if vertices are not in current sphere
    GLGXVECTOR3 ball2Vec3;
    float ball2Vec3Length, ball2Vec2Length2;
    for (int i=0; i<i_NbrVertex; i++) 
	{
        ball2Vec3 = GLGXVECTOR3(
			i_Array[i*i_Stride]   - ballCenter.x,
			i_Array[i*i_Stride+1] - ballCenter.y,
			i_Array[i*i_Stride+2] - ballCenter.z
		);
		ball2Vec2Length2 = GLGXVec3Length2(&ball2Vec3);
        if (ball2Vec2Length2 <= radius2) // Vertex is inside the sphere
            continue;

		// New Radius, new center
        ball2Vec3Length = sqrt(ball2Vec2Length2);
        radius = (radius + ball2Vec3Length) / 2.0;  
        radius2 = radius * radius;
		ballCenter = ballCenter + ( ball2Vec3 * (  (ball2Vec3Length-radius) / ball2Vec3Length )  );

    }
    *o_Center = ballCenter;
    *o_Radius = radius;
    return;
}
