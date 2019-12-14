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


#ifndef AFX_DEBUG_DISPLAY_H____INCLUDED_
#define AFX_DEBUG_DISPLAY_H____INCLUDED_

#include <toolbox_stdafx.h>
#include <dVector.h>
#include <dMatrix.h>


void DebugDrawLine (const dVector& p0, const dVector& p1, dVector color = dVector (1.0f, 1.0f, 1.0f, 1.0f));

void DebugDrawAABB (const dVector& p0, const dVector& p1, dVector color = dVector (1.0f, 1.0f, 1.0f, 1.0f));
void DebugDrawPolygon (int count, dVector* points, dVector color = dVector (1.0f, 1.0f, 1.0f, 1.0f));
void DebugDrawContact (const dVector& point, const dVector& normal, dVector color = dVector (1.0f, 1.0f, 0.0f, 1.0f), float lenght = 1.0f);

void DebugDrawCollision (const NewtonCollision* collision, dMatrix& matrix, dVector color = dVector (1.0f, 1.0f, 1.0f, 1.0f));
void DebugRenderWorldCollision (const NewtonWorld* world);

void DebugRenderDebugInfo ();


#endif
