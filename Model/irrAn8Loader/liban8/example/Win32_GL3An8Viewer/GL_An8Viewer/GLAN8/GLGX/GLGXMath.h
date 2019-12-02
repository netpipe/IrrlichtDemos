/*
* GLGX 
* An OpenGL Utility Library
*
* Copyright (c) 2002-2009, Grégory Smialek
*
* All rights reserved.
* 
* Redistribution and use in source and binary forms, with or without modification, are permitted 
* provided that the following conditions are met:
* 
*  * Redistributions of source code must retain the above copyright notice, this list of conditions
*    and the following disclaimer in the documentation and/or other materials provided with the distribution. 
*  * Redistributions in binary form must reproduce the above copyright notice, this list of
*    conditions and the following disclaimer in the documentation and/or other materials provided with the distribution. 
*  * Neither the name of the Grégory Smialek nor the names of its contributors may be used
*    to endorse or promote products derived from this software without specific prior written
*    permission. 
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

#ifndef _GLGXMATH_H
#define _GLGXMATH_H

#include "GLGXSetup.h"


#define GLGX_PI 3.14159265f

#define GLGXToDegree(i_radianAngle) ((i_radianAngle) * (180.0f / GLGX_PI))
#define GLGXToRadian(i_DegreeAngle) ((i_DegreeAngle) * (GLGX_PI / 180.0f))

struct GLGXCOLOR {
	union {   
		GLfloat Array[4];
		struct {            
			GLfloat r,g,b,a;        
		};
	};
	explicit GLGXCOLOR(){r=g=b=0.0f;a=1.0f;}
	explicit GLGXCOLOR( GLfloat r , GLfloat g , GLfloat b, GLfloat a )
	{ 
		this->r = r; this->g = g; this->b = b;this->a = a;
	}
	inline operator GLfloat*() 	
	{ 		
		return Array; 	
	}
	GLGXCOLOR operator= (const GLGXCOLOR &B)	
	{		
		if (this == &B) return B;		
		this->r = B.r;		
		this->g = B.g;		
		this->b = B.b;	
		this->a = B.a;	
		return B;	
	}

	~GLGXCOLOR(){};
};



class GLGXPLANE{
public:
		GLfloat a;
		GLfloat b;
		GLfloat c;
		GLfloat d;

	GLGXPLANE(){a=0.0f;b=0.0f;c=0.0f;d=0.0f;};
	GLGXPLANE( GLfloat a , GLfloat b , GLfloat c, GLfloat d ){ this->a = a; this->b = b; this->c = c;this->d = d;}
	~GLGXPLANE(){};
};

class GLGXFRUSTUM{
public:
		GLGXPLANE Left;
		GLGXPLANE Right;
		GLGXPLANE Top;
		GLGXPLANE Bottom;
		GLGXPLANE Front;
		GLGXPLANE Back;
	void Compute(GLfloat* pProjectionMatrix, GLfloat* pModelViewMatrix);
	bool IsPointInside( GLfloat x, GLfloat y, GLfloat z );
	bool IsSphereInside( GLfloat x, GLfloat y, GLfloat z, GLfloat radius );
	GLGXFRUSTUM(){};
	~GLGXFRUSTUM(){};
};




class GLGXQUATERNION{
public:
		GLfloat x;
		GLfloat y;
		GLfloat z;
		GLfloat w;

	GLGXQUATERNION(){x=0.0f;y=0.0f;z=0.0f;w=1.0f;};
	GLGXQUATERNION( GLfloat x , GLfloat y , GLfloat z, GLfloat w ){ this->x = x; this->y = y; this->z = z;this->w = w;}
	~GLGXQUATERNION(){};
};

class GLGXVECTOR4{
public:
	union {   

		GLfloat Array[4];
		struct {            
			GLfloat x,y,z,w;        
		};
		struct 
		{            
			GLfloat s,t,r,q;        
		};   
	};

	explicit GLGXVECTOR4()
	{
		x=0.0f;y=0.0f;z=0.0f;w=0.0f;
	};
	explicit GLGXVECTOR4( GLfloat x , GLfloat y , GLfloat z, GLfloat w )
	{ 
		this->x = x; this->y = y; this->z = z;this->w = w;
	}
	inline operator GLfloat*() 	
	{ 		
		return Array; 	
	}

	GLGXVECTOR4 operator= (const GLGXVECTOR4 &B)	
	{		
		if (this == &B) return B;		
		this->x = B.x;		
		this->y = B.y;		
		this->z = B.z;	
		this->w = B.w;	
		return B;	
	}

	~GLGXVECTOR4(){};



	GLGXVECTOR4 operator+ (const GLGXVECTOR4 &B);
	GLGXVECTOR4 operator- (const GLGXVECTOR4 &B);
	GLfloat	  operator* (const GLGXVECTOR4 &B);
	GLGXVECTOR4 operator* (GLfloat factor);
	GLGXVECTOR4 operator/ (GLfloat factor);
};


class GLGXVECTOR3{
public:
	union {  
		GLfloat Array[3];
		struct {            
			GLfloat x,y,z;        
		};
		struct 
		{            
			GLfloat s,t,r;        
		};          
	};

	explicit GLGXVECTOR3()
	{
		x = 0.0f; y = 0.0f; z = 0.0f;
	};
	explicit GLGXVECTOR3( GLfloat x , GLfloat y , GLfloat z)
	{ 
		this->x = x; this->y = y; this->z = z;
	}
	inline operator GLfloat*() 	
	{ 		
		return Array; 	
	}

	GLGXVECTOR3 operator= (const GLGXVECTOR3 &B)	
	{		
		if (this == &B) return B;		
		this->x = B.x;		
		this->y = B.y;		
		this->z = B.z;			
		return B;	
	}
	
	~GLGXVECTOR3(){};

	GLGXVECTOR3 operator+ (const GLGXVECTOR3 &B);
	GLGXVECTOR3 operator- (const GLGXVECTOR3 &B);
	GLfloat	  operator* (const GLGXVECTOR3 &B);
	GLGXVECTOR3 operator* (GLfloat factor);
	GLGXVECTOR3 operator/ (GLfloat factor);
};

class GLGXVECTOR2{
public:
	union {   
		GLfloat Array[2];
		struct {            
			GLfloat x,y;        
		};
		struct 
		{            
			GLfloat u,v;        
		};
		struct 
		{            
			GLfloat s,t;        
		};        
	};

	explicit GLGXVECTOR2()
	{
		x=0.0f;y=0.0f;
	};
	explicit GLGXVECTOR2( GLfloat x , GLfloat y)
	{ 
		this->x = x; this->y = y;
	}
	inline operator GLfloat*() 	
	{ 		
		return Array; 	
	}

	GLGXVECTOR2 operator= (const GLGXVECTOR2 &B)	
	{		
		if (this == &B) return B;		
		this->x = B.x;		
		this->y = B.y;				
		return B;	
	}

	~GLGXVECTOR2(){};

	GLGXVECTOR2 operator+ (const GLGXVECTOR2 &B);
	GLGXVECTOR2 operator- (const GLGXVECTOR2 &B);
	GLfloat	  operator* (const GLGXVECTOR2 &B);
	GLGXVECTOR2 operator* (GLfloat factor);
	GLGXVECTOR2 operator/ (GLfloat factor);
};



class GLGXMATRIX{
public:
	union{
		struct {            
			GLfloat _11,_12,_13,_14,_21,_22,_23,_24,_31,_32,_33,_34,_41,_42,_43,_44;
		};
		struct {            
			GLfloat Array[16];
		};
	};
		
	explicit GLGXMATRIX()
	{
		_12 = _13 = _14 = _21 = _23 = _24 = _31 = _32 = _34 = _41 = _42 = _43 = 0.0f;
		_11 = _22 = _33 = _44 = 1.0f;
		
	};

	inline operator GLfloat*() 	
	{ 		
		return Array; 	
	}

	GLGXMATRIX operator= (const GLGXMATRIX &B)	
	{		
		if (this == &B) return B;		
		this->_11 = B._11;this->_12 = B._12;this->_13 = B._13;this->_14 = B._14;		
		this->_21 = B._21;this->_22 = B._22;this->_23 = B._23;this->_24 = B._24;
		this->_31 = B._31;this->_32 = B._32;this->_33 = B._33;this->_34 = B._34;	
		this->_41 = B._41;this->_42 = B._42;this->_43 = B._43;this->_44 = B._44;	
		return B;	
	}

	~GLGXMATRIX(){};

	//GLfloat	  operator* (const GLGXMATRIX &B);
	//GLGXMATRIX operator* (GLfloat factor);
	GLGXMATRIX operator* (const GLGXMATRIX &B);

};


// Matrix ///////////////////////////////////////////////////

void  GLGXMatrixMultiply( GLGXMATRIX* Result, GLGXMATRIX* A, GLGXMATRIX* B );
void  GLGXMatrixIdentity(GLGXMATRIX* matrix);
void  GLGXMatrixMultVect( GLGXVECTOR3* result, GLGXMATRIX* matrix, GLGXVECTOR3* vector );
//void  GLGXMatrixRotateVect( GLGXVECTOR3* result, GLGXMATRIX* matrix, GLGXVECTOR3* vector );
void  GLGXMatrixTranspose( GLGXMATRIX* Result, GLGXMATRIX* matrix);
GLGXMATRIX * GLGXMatrixMultiplyTranspose(GLGXMATRIX *o_Matrix,GLGXMATRIX *i_Matrix1,GLGXMATRIX *i_Matrix2);

void  GLGXMatrixCopy( GLGXMATRIX* Result, GLGXMATRIX* matrix);
void  GLGXMatrixScaling(GLGXMATRIX* dest,GLfloat x,GLfloat y,GLfloat z);
void  GLGXMatrixScaling(GLGXMATRIX* dest,GLGXVECTOR3* vector);
void  GLGXMatrixTranslation(GLGXMATRIX* dest,GLfloat x, GLfloat y,GLfloat z);
void  GLGXMatrixTranslation(GLGXMATRIX* dest,GLGXVECTOR3* vector);
float GLGXMatrixDeterminant( GLGXMATRIX* i_matrix);
GLGXMATRIX * GLGXMatrixInverse(GLGXMATRIX * o_Matrix,float* o_Determinant,GLGXMATRIX * i_Matrix);

GLGXMATRIX* GLGXMatrixLookAt( GLGXMATRIX* pResult,GLGXVECTOR3 *pPosition,GLGXVECTOR3 *pTarget,GLGXVECTOR3 *pUp);

GLGXMATRIX* GLGXMatrixRotationAxis( GLGXMATRIX o_Matrix,GLGXVECTOR3* i_Vector, GLfloat i_Angle);
GLGXMATRIX* GLGXMatrixRotationX( GLGXMATRIX o_Matrix, GLfloat i_Angle);
GLGXMATRIX* GLGXMatrixRotationY( GLGXMATRIX o_Matrix, GLfloat i_Angle);
GLGXMATRIX* GLGXMatrixRotationZ( GLGXMATRIX o_Matrix, GLfloat i_Angle);

GLGXMATRIX * GLGXMatrixPerspectiveFov(GLGXMATRIX * o_Matrix,double i_fovy,double i_Aspect,double i_zNear,double i_zFar);
GLGXMATRIX * GLGXMatrixOrthoOffCenter(GLGXMATRIX * o_Matrix,double 	i_Left, double 	i_Right, double i_Bottom, double i_Top, double i_zNear, double i_zFar);
GLGXMATRIX * GLGXMatrixOrthoOffCenter2D(GLGXMATRIX * o_Matrix,double i_Left, double i_Right, double i_Bottom, double i_Top);
GLGXMATRIX * GLGXMatrixFrustum(	GLGXMATRIX * o_Matrix,double  i_Left, double  i_Right, double  i_Bottom, double  i_Top, double  i_zNear, double  i_zFar);

//void GLGXMatrixDecompose( GLGXVECTOR3 *pOutScale,GLGXQUATERNIONM *pOutRotation,GLGXVECTOR3 *pOutTranslation,const GLGXMATRIX *pM);
//void GLGXMatrixReflect(GLGXMATRIX *pOut,const GLGXPLANE *pPlane);

void GLGXYawPitchRollRotationMatrix(double* o_Yaw, double* o_Pitch, double* o_Roll,GLGXMATRIX* i_Matrix);
void GLGXYawPitchRollQuaternion(double* o_Yaw, double* o_Pitch , double* o_Roll, GLGXQUATERNION* i_Quat );

void GLGXMatrixDecompose( 
		GLGXVECTOR3 *o_Scale,GLGXQUATERNION *o_Rotation,GLGXVECTOR3 *o_Translation,
		GLGXMATRIX *i_Matrix);

// Quaternion ///////////////////////////////////////////////
void            GLGXMatrixRotationQuaternion( GLGXMATRIX* o_matrix, GLGXQUATERNION* i_quat );
void			GLGXQuaternionMatrixRotation( GLGXQUATERNION* o_quat, GLGXMATRIX* i_matrix );
void            GLGXQuaternionMultiply(GLGXQUATERNION* dest,GLGXQUATERNION* srcA,GLGXQUATERNION* srcB);
GLGXQUATERNION* GLGXQuaternionIdentity(GLGXQUATERNION *pOut);
GLGXQUATERNION* GLGXQuaternionRotationYawPitchRoll(GLGXQUATERNION * out,float yaw,float pitch,float roll);
GLGXQUATERNION* GLGXQuaternionRotationAxis(GLGXQUATERNION * out,GLGXVECTOR3 * axis,float angle);
void            GLGXQuaternionToAxisAngle(GLGXQUATERNION* quat,GLGXVECTOR3* axis,float* angle);
GLGXQUATERNION* GLGXQuaternionConjugate(GLGXQUATERNION * out,GLGXQUATERNION * in);
GLGXQUATERNION* GLGXQuaternionInverse(GLGXQUATERNION * out,GLGXQUATERNION * in);
GLGXQUATERNION* GLGXQuaternionNormalize(GLGXQUATERNION * out,GLGXQUATERNION * in);
float           GLGXQuaternionLength(GLGXQUATERNION *quat);
GLGXQUATERNION* GLGXQuaternionSlerp(GLGXQUATERNION* out, GLGXQUATERNION* i_quatA, GLGXQUATERNION* i_quatB, float t);
GLGXQUATERNION * GLGXQuaternionSquad(GLGXQUATERNION *o_Result,GLGXQUATERNION *i_quatA,GLGXQUATERNION *i_quatB,GLGXQUATERNION *i_quatC,GLGXQUATERNION *i_quatD,float i_t);
GLGXQUATERNION * GLGXQuaternionBaryCentric(GLGXQUATERNION *o_Result,GLGXQUATERNION *i_quatA,GLGXQUATERNION *i_quatB,GLGXQUATERNION *i_quatC,float i_f,float i_g);

// Vector3 //////////////////////////////////////////////////
GLGXVECTOR3*  GLGXVec3Cross(GLGXVECTOR3* result,GLGXVECTOR3* A,GLGXVECTOR3* B);
GLGXVECTOR3 * GLGXVec3Normalize(GLGXVECTOR3 *pOut,const GLGXVECTOR3 *pV);
GLGXVECTOR3 * GLGXVec3PackTo01(GLGXVECTOR3 *pOut,const GLGXVECTOR3 *pV); // Vector is supposed to be normalized
GLGXVECTOR4 * GLGXVec3Transform(GLGXVECTOR4 * dest,const GLGXVECTOR3 * src,const GLGXMATRIX * matrix);
GLGXVECTOR3 * GLGXVec3TransformNormal(GLGXVECTOR3 * dest,const GLGXVECTOR3 * src,const GLGXMATRIX * matrix);
GLGXVECTOR3 * GLGXVec3TransformCoord(GLGXVECTOR3 * dest,const GLGXVECTOR3 * src,const GLGXMATRIX * matrix);

GLfloat      GLGXVec3Dot(const GLGXVECTOR3 *srcA,const GLGXVECTOR3 *srcB);
GLfloat      GLGXVec3Length(const GLGXVECTOR3 *pV);
GLfloat		 GLGXVec3Length2(const GLGXVECTOR3 *vector);
void         GLGXVec3Lerp(GLGXVECTOR3 *dest,GLGXVECTOR3 *v1,GLGXVECTOR3 *v2,GLfloat l);
GLGXVECTOR3* GLGXVec3Scale(GLGXVECTOR3 *out,GLGXVECTOR3 *vector,GLfloat s) ;
GLGXVECTOR3 * GLGXVec3BaryCentric(GLGXVECTOR3 *o_Result,GLGXVECTOR3 *i_src1,GLGXVECTOR3 *i_src2,GLGXVECTOR3 *i_src3,float i_f,float i_g);

GLGXVECTOR3 * GLGXVec3CatmullRom(GLGXVECTOR3 *o_Result,GLGXVECTOR3 *i_Src0,GLGXVECTOR3 *i_Src1,GLGXVECTOR3 *i_Src2,GLGXVECTOR3 *i_Src3,float i_t);
GLGXVECTOR3* GLGXVec3Hermite(GLGXVECTOR3 *o_Result, GLGXVECTOR3 *i_Pos0, GLGXVECTOR3 *i_Tan0, GLGXVECTOR3 *i_Pos1, GLGXVECTOR3 *i_Tan1, float i_t);
GLGXVECTOR3* GLGXVec3BezierDeg2(GLGXVECTOR3 *o_Result, GLGXVECTOR3 *i_P0, GLGXVECTOR3 *i_P1, GLGXVECTOR3 *i_P2, float i_t);
GLGXVECTOR3* GLGXVec3BezierDeg3(GLGXVECTOR3 *o_Result, GLGXVECTOR3 *i_P0, GLGXVECTOR3 *i_P1, GLGXVECTOR3 *i_P2, GLGXVECTOR3 *i_P3, float i_t);
float        GLGXAngleBetweenVec3(GLGXVECTOR3 *srcA,GLGXVECTOR3 *srcB);

// Vector2 //////////////////////////////////////////////////
GLfloat      GLGXVec2Length(const GLGXVECTOR2 *vector);
GLfloat      GLGXVec2Length2(const GLGXVECTOR2 *vector);
GLGXVECTOR2* GLGXVec2Normalize(GLGXVECTOR2 *pOut,const GLGXVECTOR2 *pV);
void         GLGXVec2Lerp(GLGXVECTOR2 *dest,GLGXVECTOR2 *v1,GLGXVECTOR2 *v2,GLfloat l);
GLfloat      GLGXVec2Dot(const GLGXVECTOR2 *srcA,const GLGXVECTOR2 *srcB);
GLGXVECTOR2* GLGXVec2Scale(GLGXVECTOR2 *out,GLGXVECTOR2 *vector,GLfloat s);
float        GLGXAngleBetweenVec2(GLGXVECTOR2 *srcA,GLGXVECTOR2 *srcB);
GLGXVECTOR4 * GLGXVec2Transform(GLGXVECTOR4 * o_Out,const GLGXVECTOR2 * i_Src,const GLGXMATRIX * i_Matrix);
GLGXVECTOR2 * GLGXVec2TransformNormal(GLGXVECTOR2 * dest,const GLGXVECTOR2 * src,const GLGXMATRIX * matrix);
GLGXVECTOR2 * GLGXVec2TransformCoord(GLGXVECTOR2 * dest,const GLGXVECTOR2 * src,const GLGXMATRIX * matrix);
GLGXVECTOR2 * GLGXVec2BaryCentric(GLGXVECTOR2 *o_Result,GLGXVECTOR2 *i_src1,GLGXVECTOR2 *i_src2,GLGXVECTOR2 *i_src3,float f,float g);
GLGXVECTOR2 * GLGXVec2CatmullRom(GLGXVECTOR2 *i_Out,GLGXVECTOR2 *i_Src0,GLGXVECTOR2 *i_Src1,GLGXVECTOR2 *i_Src2,GLGXVECTOR2 *i_Src3,float i_t);
GLGXVECTOR2* GLGXVec2Hermite(GLGXVECTOR2 *o_Result, GLGXVECTOR2 *i_Pos0, GLGXVECTOR2 *i_Tan0, GLGXVECTOR2 *i_Pos1, GLGXVECTOR2 *i_Tan1, float i_t);
GLGXVECTOR2* GLGXVec2BezierDeg2(GLGXVECTOR2 *o_Result, GLGXVECTOR2 *i_P0, GLGXVECTOR2 *i_P1, GLGXVECTOR2 *i_P2, float i_t);
GLGXVECTOR2* GLGXVec2BezierDeg3(GLGXVECTOR2 *o_Result, GLGXVECTOR2 *i_P0, GLGXVECTOR2 *i_P1, GLGXVECTOR2 *i_P2, GLGXVECTOR2 *i_P3, float i_t);

// Vector4 //////////////////////////////////////////////////
GLGXVECTOR4 * GLGXVec4BaryCentric(GLGXVECTOR4 *o_Result,GLGXVECTOR4 *i_src1,GLGXVECTOR4 *i_src2,GLGXVECTOR4 *i_src3,float i_f,float i_g);
GLGXVECTOR4 * GLGXVec4CatmullRom(GLGXVECTOR4 *i_Out,GLGXVECTOR4 *i_Src0,GLGXVECTOR4 *i_Src1,GLGXVECTOR4 *i_Src2,GLGXVECTOR4 *i_Src3,float i_t);
GLGXVECTOR4*  GLGXVec4Cross(GLGXVECTOR4 *i_Out, GLGXVECTOR4 *i_A, GLGXVECTOR4 *i_B, GLGXVECTOR4 *i_C);
GLGXVECTOR4*  GLGXVec4Hermite(GLGXVECTOR4 *o_Result, GLGXVECTOR4 *i_Pos0, GLGXVECTOR4 *i_Tan0, GLGXVECTOR4 *i_Pos1, GLGXVECTOR4 *i_Tan1, float i_t);
GLGXVECTOR4*  GLGXVec4Normalize(GLGXVECTOR4 *pOut,const GLGXVECTOR4 *pV); 
GLGXVECTOR4 * GLGXVec3Transform(GLGXVECTOR4 * dest,const GLGXVECTOR4 * src,const GLGXMATRIX * matrix);
GLfloat       GLGXVec4Length(const GLGXVECTOR4 *vector);
void          GLGXVec4Lerp(GLGXVECTOR4 *dest,GLGXVECTOR4 *v1,GLGXVECTOR2 *v2,GLfloat l);
GLGXVECTOR4* GLGXVec4BezierDeg2(GLGXVECTOR4 *o_Result, GLGXVECTOR4 *i_P0, GLGXVECTOR4 *i_P1, GLGXVECTOR4 *i_P2, float i_t);
GLGXVECTOR4* GLGXVec4BezierDeg3(GLGXVECTOR4 *o_Result, GLGXVECTOR4 *i_P0, GLGXVECTOR4 *i_P1, GLGXVECTOR4 *i_P2, GLGXVECTOR4 *i_P3, float i_t);



// Plane //////////////////////////////////////////////////
GLGXPLANE * GLGXPlaneNormalize(GLGXPLANE * pDest, GLGXPLANE * pSrc);
float GLGXVec3DistanceToPlane( GLGXPLANE * i_Plane, GLGXVECTOR3* i_Point);
//GLGXProjectPoint( GLfloat x, GLfloat y, GLfloat z, GLfloat* pModelViewMatrix,GLfloat* pProjectionMatrix,GLfloat* pViewportMatrix, int* destX,int* destY,int* destZ );


// Other ///////////////////////////////////////////////////

bool GLGXComputeBoundingBox(float* i_Vertices, int i_NbrVertex, int i_Stride, GLGXVECTOR3* o_MinAABB, GLGXVECTOR3* o_MaxAABB);
bool GLGXComputeBoundingRectangle(float* i_Vertices, int i_NbrVertex, int i_Stride, GLGXVECTOR2* o_MinAABB, GLGXVECTOR2* o_MaxAABB);

void GLGXComputeBoundingSphere( float* i_Array, int i_NbrVertex, int i_Stride, GLGXVECTOR3* o_Center, float* o_Radius);
void GLGXComputeBoundingCircle( float* i_Array, int i_NbrVertex, int i_Stride, GLGXVECTOR2* o_Center, float* o_Radius);

#endif	// _GLGXMATH_H

