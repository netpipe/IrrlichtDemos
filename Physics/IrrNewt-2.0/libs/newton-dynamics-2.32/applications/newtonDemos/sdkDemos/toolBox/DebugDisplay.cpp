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

// dGeometry.cpp: implementation of the dGeometry class.
//
//////////////////////////////////////////////////////////////////////
#include <toolbox_stdafx.h>
#include <DebugDisplay.h>

#define MAX_DEBUG_CONTACTS	(1024 * 16)

struct dDebugLine
{
	dVector m_p0;
	dVector m_p1;
	dVector m_color;
	int m_isContact;
};


static int debugPointsCount;
static dDebugLine debugDisplayList[MAX_DEBUG_CONTACTS];


void DebugDrawContact (const dVector& point, const dVector& normal, dVector color, float lenght)
{
	if (debugPointsCount < MAX_DEBUG_CONTACTS) {
		debugDisplayList[debugPointsCount].m_isContact = 1;
		debugDisplayList[debugPointsCount].m_color = color;
		debugDisplayList[debugPointsCount].m_p0 = point + normal.Scale (lenght);
		debugDisplayList[debugPointsCount].m_p1 = point - normal.Scale (lenght);
		debugPointsCount ++;
	}
}

void DebugDrawLine (const dVector& p0, const dVector& p1, dVector color)
{
	if (debugPointsCount < MAX_DEBUG_CONTACTS) {
		debugDisplayList[debugPointsCount].m_isContact = 0;
		debugDisplayList[debugPointsCount].m_color = color;
		debugDisplayList[debugPointsCount].m_p0 = p0;
		debugDisplayList[debugPointsCount].m_p1 = p1;
		debugPointsCount ++;
	}
}


void DebugDrawPolygon (int count,  dVector* points, dVector color)
{
	int i0;

	i0 = count- 1;
	for (int i = 0; (i < count) && (debugPointsCount < MAX_DEBUG_CONTACTS); i ++) {
		debugDisplayList[debugPointsCount].m_isContact = 0;
		debugDisplayList[debugPointsCount].m_color = color;
		debugDisplayList[debugPointsCount].m_p0 = points[i0];
		debugDisplayList[debugPointsCount].m_p1 = points[i];
		debugPointsCount ++;
		i0 = i;
	}
}


void DebugDrawAABB (const dVector& p0, const dVector& p1, dVector color)
{
	if ((debugPointsCount + 42) < MAX_DEBUG_CONTACTS) {

		debugDisplayList[debugPointsCount].m_isContact = 0;
		debugDisplayList[debugPointsCount].m_color = color;
		debugDisplayList[debugPointsCount].m_p0 = dVector (p0.m_x, p0.m_y, p0.m_z);
		debugDisplayList[debugPointsCount].m_p1 = dVector (p1.m_x, p0.m_y, p0.m_z);
		debugPointsCount ++;

		debugDisplayList[debugPointsCount].m_isContact = 0;
		debugDisplayList[debugPointsCount].m_color = color;
		debugDisplayList[debugPointsCount].m_p0 = dVector (p0.m_x, p1.m_y, p0.m_z);
		debugDisplayList[debugPointsCount].m_p1 = dVector (p1.m_x, p1.m_y, p0.m_z);
		debugPointsCount ++;

		debugDisplayList[debugPointsCount].m_isContact = 0;
		debugDisplayList[debugPointsCount].m_color = color;
		debugDisplayList[debugPointsCount].m_p0 = dVector (p0.m_x, p1.m_y, p1.m_z);
		debugDisplayList[debugPointsCount].m_p1 = dVector (p1.m_x, p1.m_y, p1.m_z);
		debugPointsCount ++;

		debugDisplayList[debugPointsCount].m_isContact = 0;
		debugDisplayList[debugPointsCount].m_color = color;
		debugDisplayList[debugPointsCount].m_p0 = dVector (p0.m_x, p0.m_y, p1.m_z);
		debugDisplayList[debugPointsCount].m_p1 = dVector (p1.m_x, p0.m_y, p1.m_z);
		debugPointsCount ++;


		debugDisplayList[debugPointsCount].m_isContact = 0;
		debugDisplayList[debugPointsCount].m_color = color;
		debugDisplayList[debugPointsCount].m_p0 = dVector (p0.m_x, p0.m_y, p0.m_z);
		debugDisplayList[debugPointsCount].m_p1 = dVector (p0.m_x, p1.m_y, p0.m_z);
		debugPointsCount ++;

		debugDisplayList[debugPointsCount].m_isContact = 0;
		debugDisplayList[debugPointsCount].m_color = color;
		debugDisplayList[debugPointsCount].m_p0 = dVector (p1.m_x, p0.m_y, p0.m_z);
		debugDisplayList[debugPointsCount].m_p1 = dVector (p1.m_x, p1.m_y, p0.m_z);
		debugPointsCount ++;

		debugDisplayList[debugPointsCount].m_isContact = 0;
		debugDisplayList[debugPointsCount].m_color = color;
		debugDisplayList[debugPointsCount].m_p0 = dVector (p0.m_x, p0.m_y, p1.m_z);
		debugDisplayList[debugPointsCount].m_p1 = dVector (p0.m_x, p1.m_y, p1.m_z);
		debugPointsCount ++;

		debugDisplayList[debugPointsCount].m_isContact = 0;
		debugDisplayList[debugPointsCount].m_color = color;
		debugDisplayList[debugPointsCount].m_p0 = dVector (p1.m_x, p0.m_y, p1.m_z);
		debugDisplayList[debugPointsCount].m_p1 = dVector (p1.m_x, p1.m_y, p1.m_z);
		debugPointsCount ++;


		debugDisplayList[debugPointsCount].m_isContact = 0;
		debugDisplayList[debugPointsCount].m_color = color;
		debugDisplayList[debugPointsCount].m_p0 = dVector (p0.m_x, p0.m_y, p0.m_z);
		debugDisplayList[debugPointsCount].m_p1 = dVector (p0.m_x, p0.m_y, p1.m_z);
		debugPointsCount ++;

		debugDisplayList[debugPointsCount].m_isContact = 0;
		debugDisplayList[debugPointsCount].m_color = color;
		debugDisplayList[debugPointsCount].m_p0 = dVector (p0.m_x, p1.m_y, p0.m_z);
		debugDisplayList[debugPointsCount].m_p1 = dVector (p0.m_x, p1.m_y, p1.m_z);
		debugPointsCount ++;

		debugDisplayList[debugPointsCount].m_isContact = 0;
		debugDisplayList[debugPointsCount].m_color = color;
		debugDisplayList[debugPointsCount].m_p0 = dVector (p1.m_x, p1.m_y, p0.m_z);
		debugDisplayList[debugPointsCount].m_p1 = dVector (p1.m_x, p1.m_y, p1.m_z);
		debugPointsCount ++;

		debugDisplayList[debugPointsCount].m_isContact = 0;
		debugDisplayList[debugPointsCount].m_color = color;
		debugDisplayList[debugPointsCount].m_p0 = dVector (p1.m_x, p0.m_y, p0.m_z);
		debugDisplayList[debugPointsCount].m_p1 = dVector (p1.m_x, p0.m_y, p1.m_z);
		debugPointsCount ++;
	}

}

void DebugShowGeometryCollision (void* userData, int vertexCount, const dFloat* faceVertec, int id)
{
	id;
	faceVertec;
	userData;

	int i = vertexCount - 1;
	dVector p0 (faceVertec[i * 3 + 0], faceVertec[i * 3 + 1], faceVertec[i * 3 + 2]);
	for (int i = 0; i < vertexCount; i ++) {
		dVector p1 (faceVertec[i * 3 + 0], faceVertec[i * 3 + 1], faceVertec[i * 3 + 2]);
		glVertex3f (p0.m_x, p0.m_y, p0.m_z);
		glVertex3f (p1.m_x, p1.m_y, p1.m_z);
		p0 = p1;
	}
}



static void DebugSubmitGeometryCollision (void* userData, int vertexCount, const dFloat* faceVertec, int id)
{
	id;
	int i = vertexCount - 1;
	dVector p0 (faceVertec[i * 3 + 0], faceVertec[i * 3 + 1], faceVertec[i * 3 + 2]);
	dVector color(1, 1, 1, 1);
	if (userData) {
		color = *((dVector*)userData);
	}

	for (int i = 0; (i < vertexCount) && (debugPointsCount < MAX_DEBUG_CONTACTS); i ++) {
		dVector p1 (faceVertec[i * 3 + 0], faceVertec[i * 3 + 1], faceVertec[i * 3 + 2]);
		debugDisplayList[debugPointsCount].m_color = color;
		debugDisplayList[debugPointsCount].m_p0 = p0;
		debugDisplayList[debugPointsCount].m_p1 = p1;
		debugDisplayList[debugPointsCount].m_isContact = false;
		debugPointsCount ++;
		p0 = p1;
	}
}


void DebugDrawCollision (const NewtonCollision* collision, dMatrix& matrix, dVector color)
{
	NewtonCollisionForEachPolygonDo (collision, &matrix[0][0], DebugSubmitGeometryCollision, &color);
}


void DebugShowBodyCollision (const NewtonBody* body, void* userData)
{
	dFloat mass;
	dFloat Ixx;
	dFloat Iyy;
	dFloat Izz;

	userData;

	int sleepState = NewtonBodyGetSleepState(body);
	if (sleepState == 1) {
		// indicate when body is sleeping 
		glColor3f(0.42f, 0.73f, 0.98f);
	} else {
		// body is active
		glColor3f(1.0f, 1.0f, 1.0f);
	}



	NewtonBodyGetMassMatrix (body, &mass, &Ixx, &Iyy, &Izz);
	if (mass > 0.0f) {
		dMatrix matrix;
		NewtonBodyGetMatrix(body, &matrix[0][0]);
		NewtonCollisionForEachPolygonDo (NewtonBodyGetCollision(body), &matrix[0][0], DebugShowGeometryCollision, NULL);
	} else {
		NewtonCollision* collision;
		NewtonCollisionInfoRecord info;

		collision = NewtonBodyGetCollision (body);
		NewtonCollisionGetInfo (collision, &info);
		switch (info.m_collisionType) 
		{
//			case SERIALIZE_ID_TREE:
			case SERIALIZE_ID_SCENE:
			case SERIALIZE_ID_USERMESH:
			case SERIALIZE_ID_HEIGHTFIELD:
			{
				break;
			}

			default: 
			{
				dMatrix matrix;
				NewtonBodyGetMatrix(body, &matrix[0][0]);
				NewtonCollisionForEachPolygonDo (NewtonBodyGetCollision(body), &matrix[0][0], DebugShowGeometryCollision, NULL);
				break;
			}
		}
	}
}


void DebugRenderWorldCollision (const NewtonWorld* world)
{
	glDisable (GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);

	dVector aabbP0 (-1000.0f, -500.0f, -1000.0f);
	dVector aabbP1 (1000.0f, 500.0f, 1000.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_LINES);
	NewtonWorldForEachBodyInAABBDo (world, &aabbP0[0], &aabbP1[0], DebugShowBodyCollision, NULL);
	glEnd();
}

//void DebugRenderWorld (const NewtonWorld* world)
void DebugRenderDebugInfo ()
{
	glDisable (GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);

//	glLineWidth(4.0f);
	glLineWidth(1.0f);
	glBegin(GL_LINES);
	for (int i = 0; i < debugPointsCount; i ++) {
		glColor3f (debugDisplayList[i].m_color.m_x, debugDisplayList[i].m_color.m_y, debugDisplayList[i].m_color.m_z);
		glVertex3f (debugDisplayList[i].m_p0.m_x, debugDisplayList[i].m_p0.m_y, debugDisplayList[i].m_p0.m_z);
		glVertex3f (debugDisplayList[i].m_p1.m_x, debugDisplayList[i].m_p1.m_y, debugDisplayList[i].m_p1.m_z);
	}
	glEnd();
	glLineWidth(1.0f);


	glPointSize(8.0f);
	glBegin(GL_POINTS);
	glColor3f(1.0f, 0.0f, 0.0f);
	for (int i = 0; i < debugPointsCount; i ++) {
		if (debugDisplayList[i].m_isContact) {
			dVector center ((debugDisplayList[i].m_p0 + debugDisplayList[i].m_p1).Scale (0.5f));
			glVertex3f(center.m_x, center.m_y, center.m_z);
		}
	}
	glEnd();
	glPointSize(1.0f);

	debugPointsCount = 0;	
}
