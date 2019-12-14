/* Copyright (c) <2009> <Newton Game Dynamics>
* 
* This software is provided 'as-is', without any express or implied
* warranty. In no event will the authors be held liable for any damages
* arising from the use of this software.
* 
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely
*/
	


// OpenGlUtil.h: interface for the OpenGlUtil class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OPENGLUTIL_H__INCLUDED_)
#define AFX_OPENGLUTIL_H__INCLUDED_

#include <toolbox_stdafx.h>


//#define RECORD_LOG
//#define READ_LOG



//#define USE_TEST_SERIALIZATION
//#define USE_STATIC_MESHES_DEBUG_COLLISION
//#define USE_TEST_ALL_FACE_USER_RAYCAST_CALLBACK


enum KEY_asciiCode
{
/*
	KeyCode_UNKNOWN		= 0,

//	KeyCode_F1	= GLUT_KEY_F1,
	KeyCode_F1	= 0,
	KeyCode_F2,
	KeyCode_F3,
	KeyCode_F4,
	KeyCode_F5,
	KeyCode_F6,
	KeyCode_F7,
	KeyCode_F8,
	KeyCode_F9,
	KeyCode_F10,
	KeyCode_F11,
	KeyCode_F12,
	KeyCode_lastSpecialKey,

	KeyCode_ESCAPE	= 27,
	KeyCode_SPACE	= 32,
	KeyCode_PLUS	= 43,
	KeyCode_MINUS	= 45,

	KeyCode_a = 97,
	KeyCode_b,
	KeyCode_c,
	KeyCode_d,
	KeyCode_e,
	KeyCode_f,
	KeyCode_g,
	KeyCode_h,
	KeyCode_i,
	KeyCode_j,
	KeyCode_k,
	KeyCode_l,
	KeyCode_m,
	KeyCode_n,
	KeyCode_o,
	KeyCode_p,
	KeyCode_q,
	KeyCode_r,
	KeyCode_s,
	KeyCode_t,
	KeyCode_u,
	KeyCode_v,
	KeyCode_w,
	KeyCode_x,
	KeyCode_y,
	KeyCode_z,
*/
	KeyCode_L_BUTTON,
	KeyCode_R_BUTTON
} ;



//typedef void (*GlutCallBack) ();
//class dVector;
//class DGCamera;

struct dMOUSE_POINT
{
	int x;
	int y;
};


#define dRAND_MAX		0x0fffffff

unsigned dRand ();
dMatrix GetModelViewMatrix ();

void HandleResize(int width, int height);
int dGetKeyState(int key);
void GetCursorPos(dMOUSE_POINT& point);
dVector ScreenToWorld (const dVector& screen);
dVector WorldToScreen (const dVector& world);
void ShowMousePicking (const dVector& p0, const dVector& p1); 

void KeyUp(int key);
void KeyDown (int key);



#endif 

