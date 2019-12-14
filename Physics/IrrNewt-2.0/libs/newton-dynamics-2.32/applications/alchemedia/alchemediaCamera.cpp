/////////////////////////////////////////////////////////////////////////////
// Name:        alchemediaCamera.cpp
// Purpose:     
// Author:      Julio Jerez
// Modified by: 
// Created:     22/05/2010 08:02:08
// RCS-ID:      
// Copyright:   Copyright (c) <2010> <Newton Game Dynamics>
// License:     
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
// 
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely
/////////////////////////////////////////////////////////////////////////////

#include "alchemedia.h"
#include "alchemediaCamera.h"
#include "alchemediaScene.h"



D_ALCHEMDIA_REGISTER_CLASS(alchemediaCamera);


alchemediaCamera::alchemediaCamera()
	:dCameraNodeInfo(), m_projectionMode(m_perspective), m_2dViewportSize (20.0f)
{
	//m_pointOfInterest.m_x = -30.0f;
}

alchemediaCamera::alchemediaCamera(dScene* world)
	:dCameraNodeInfo(world), m_projectionMode(m_perspective), m_2dViewportSize (20.0f)
{
	//m_pointOfInterest.m_x = -30.0f;
}

alchemediaCamera::alchemediaCamera(const alchemediaCamera& me)
	:dCameraNodeInfo(me), m_projectionMode(me.m_projectionMode), m_2dViewportSize (me.m_2dViewportSize)
{
}


alchemediaCamera::~alchemediaCamera(void)
{
}

alchemediaCamera::ProjectionModes alchemediaCamera::GetViewMode() const
{
	return m_projectionMode;
}

// Set the View  mode for this camera
void alchemediaCamera::SetViewMode(ProjectionModes viewMode)
{
	m_projectionMode = viewMode;
}


// set the current camera mode
void alchemediaCamera::SetProjectionMode(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	switch (m_projectionMode)
	{
		case m_perspective:
		{
			gluPerspective(60.0f, GLfloat (width) /GLfloat(height), 0.1f, 1000.0f);
			break;
		}

		case m_orthographic:
		{
			//glOrtho(-1.0, 1.0, -1.0, 1.0, 0.1, 1000.0f);
			dFloat aspectRatio = GLfloat(height) / GLfloat (width);
			glOrtho(-m_2dViewportSize * 0.5f, m_2dViewportSize * 0.5f, -m_2dViewportSize * 0.5f * aspectRatio, m_2dViewportSize * 0.5f * aspectRatio, 0.1, 1000.0f);
			break;
		}

		case m_front:
		{
			m_yaw = 180.0f * 3.141592f / 180.0f;
			m_roll =  0.0f * 3.141592f / 180.0f;

			dFloat aspectRatio = GLfloat(height) / GLfloat (width);
			glOrtho(-m_2dViewportSize * 0.5f, m_2dViewportSize * 0.5f, -m_2dViewportSize * 0.5f * aspectRatio, m_2dViewportSize * 0.5f * aspectRatio, 0.1, 1000.0f);
			break;
		}

		case m_back:
		{
			m_yaw  = 0.0f * 3.141592f / 180.0f;
			m_roll = 0.0f * 3.141592f / 180.0f;

			dFloat aspectRatio = GLfloat(height) / GLfloat (width);
			glOrtho(-m_2dViewportSize * 0.5f, m_2dViewportSize * 0.5f, -m_2dViewportSize * 0.5f * aspectRatio, m_2dViewportSize * 0.5f * aspectRatio, 0.1, 1000.0f);
			break;
		}


		case m_left:
		{
			m_yaw = 90.0f * 3.141592f / 180.0f;
			m_roll = 0.0f * 3.141592f / 180.0f;

			dFloat aspectRatio = GLfloat(height) / GLfloat (width);
			glOrtho(-m_2dViewportSize * 0.5f, m_2dViewportSize * 0.5f, -m_2dViewportSize * 0.5f * aspectRatio, m_2dViewportSize * 0.5f * aspectRatio, 0.1, 1000.0f);
			break;
		}


		case m_right:
		{
			m_yaw = 270.0f * 3.141592f / 180.0f;
			m_roll = 0.0f * 3.141592f / 180.0f;

			dFloat aspectRatio = GLfloat(height) / GLfloat (width);
			glOrtho(-m_2dViewportSize * 0.5f, m_2dViewportSize * 0.5f, -m_2dViewportSize * 0.5f * aspectRatio, m_2dViewportSize * 0.5f * aspectRatio, 0.1, 1000.0f);
			break;
		}

		case m_top:
		{
			m_yaw = 0.0f * 3.141592f / 180.0f;
			m_roll = 90.0f * 3.141592f / 180.0f;

			dFloat aspectRatio = GLfloat(height) / GLfloat (width);
			glOrtho(-m_2dViewportSize * 0.5f, m_2dViewportSize * 0.5f, -m_2dViewportSize * 0.5f * aspectRatio, m_2dViewportSize * 0.5f * aspectRatio, 0.1, 1000.0f);
			break;
		}

		case m_bottom:
		{
			m_yaw = 0.0f * 3.141592f / 180.0f;
			m_roll = -90.0f * 3.141592f / 180.0f;

			dFloat aspectRatio = GLfloat(height) / GLfloat (width);
			glOrtho(-m_2dViewportSize * 0.5f, m_2dViewportSize * 0.5f, -m_2dViewportSize * 0.5f * aspectRatio, m_2dViewportSize * 0.5f * aspectRatio, 0.1, 1000.0f);
			break;
		}

	}

	//	GLfloat projection[16]; 
	//	glGetFloatv(GL_PROJECTION_MATRIX, projection);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}


// Set the Camera Matrix
void alchemediaCamera::SetCameraMatrix(void)
{
	switch (m_projectionMode)
	{
		case m_perspective:
		case m_orthographic:
		{
			dMatrix matrix (dRollMatrix (-m_roll) * dYawMatrix (m_yaw));
			matrix.m_posit = m_origin;

			dVector up (0.0f, 1.0f, 0.0f, 0.0f);
			dVector pointOfInterest  (matrix.TransformVector (dVector(-1, 0.0, 0.0f, 1.0f)));

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			gluLookAt(m_origin.m_x, m_origin.m_y, m_origin.m_z, pointOfInterest.m_x, pointOfInterest.m_y, pointOfInterest.m_z, up.m_x, up.m_y, up.m_z);	

			//glGetFloatv(GL_MODELVIEW_MATRIX, &matrix[0][0]);
			//glGetFloatv(GL_MODELVIEW_MATRIX, &matrix[0][0]);
			break;
		}

		case m_front:
		case m_back:
		case m_left:
		case m_right:
		{

			dMatrix matrix (dRollMatrix (-m_roll) * dYawMatrix (m_yaw));
			matrix.m_posit = m_origin;

			dVector up (0.0f, 1.0f, 0.0f, 0.0f);
			dVector pointOfInterest  (matrix.TransformVector (dVector(-1, 0.0, 0.0f, 1.0f)));

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			gluLookAt(m_origin.m_x, m_origin.m_y, m_origin.m_z, pointOfInterest.m_x, pointOfInterest.m_y, pointOfInterest.m_z, up.m_x, up.m_y, up.m_z);	

			break;
		}

		case m_top:
		case m_bottom:
		{
			dMatrix matrix (dRollMatrix (-m_roll) * dYawMatrix (m_yaw));
			matrix.m_posit = m_origin;

			dVector up (1.0f, 0.0f, 0.0f, 0.0f);
			dVector pointOfInterest  (matrix.TransformVector (dVector(-1, 0.0, 0.0f, 1.0f)));

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			gluLookAt(m_origin.m_x, m_origin.m_y, m_origin.m_z, pointOfInterest.m_x, pointOfInterest.m_y, pointOfInterest.m_z, up.m_x, up.m_y, up.m_z);	

			//glGetFloatv(GL_MODELVIEW, &matrix[0][0]);
			//glGetFloatv(GL_PROJECTION_MATRIX, &matrix[0][0]);
			//glGetFloatv(GL_MODELVIEW, &matrix[0][0]);
			break;
		}
	}
}


/*
void alchemediaCamera::Serialize (TiXmlElement* rootNode) const
{
	SerialiseBase(dCameraNodeInfo, rootNode);

	TiXmlElement* position = new TiXmlElement ("position");
	rootNode->LinkEndChild(position);
	position->SetDoubleAttribute("x", double (m_pointOfInterest.m_x));
	position->SetDoubleAttribute("y", double (m_pointOfInterest.m_y));
	position->SetDoubleAttribute("z", double (m_pointOfInterest.m_z));

	TiXmlElement* rotation = new TiXmlElement ("rotation");
	rootNode->LinkEndChild(rotation);
	rotation->SetDoubleAttribute("pitch", double (m_roll * 180.0f / 3.141592f));
	rotation->SetDoubleAttribute("yaw", double (m_yaw * 180.0f / 3.141592f));

	TiXmlElement* miscelaneus = new TiXmlElement ("miscelaneus");
	rootNode->LinkEndChild(miscelaneus);
	miscelaneus->SetDoubleAttribute("dist", double (m_distance));
	miscelaneus->SetDoubleAttribute("viewportOrthoSize", double (m_2dViewportSize));

	switch (m_projectionMode) {
		case m_perspective:
			miscelaneus->SetAttribute("projectionMode", "perspective");
			break;

		case m_orthographic:
			miscelaneus->SetAttribute("projectionMode", "orthographic");
			break;

		case m_front:
			miscelaneus->SetAttribute("projectionMode", "front");
			break;

		case m_back:
			miscelaneus->SetAttribute("projectionMode", "back");
			break;

		case m_left:
			miscelaneus->SetAttribute("projectionMode", "left");
			break;

		case m_right:
			miscelaneus->SetAttribute("projectionMode", "right");
			break;

		case m_top:
			miscelaneus->SetAttribute("projectionMode", "top");
			break;

		case m_bottom:
			miscelaneus->SetAttribute("projectionMode", "bottom");
			break;
	}
}


void alchemediaCamera::SerializeBinary (FILE* file) 
{
	fprintf (file, "%s\n%s\n", GetClassName(), GetName());

	fwrite (&m_pointOfInterest.m_x, 3, sizeof (dFloat), file);
	fwrite (&m_roll, 1, sizeof (dFloat), file);
	fwrite (&m_yaw, 1, sizeof (dFloat), file);
	fwrite (&m_distance, 1, sizeof (dFloat), file);
	fwrite (&m_2dViewportSize, 1, sizeof (dFloat), file);

	int projection = m_projectionMode;
	fwrite (&projection, 1, sizeof (int), file);
}
*/


bool alchemediaCamera::Deserialize (TiXmlElement* rootNode, int revisionNumber) 
{
	dCameraNodeInfo::Deserialize (rootNode, revisionNumber);

	TiXmlElement* miscelaneus = (TiXmlElement*) rootNode->FirstChild ("miscelaneus");
	if (miscelaneus) {
		//double x;
		double y;
		//miscelaneus->Attribute("dist", &x);
		miscelaneus->Attribute("viewportOrthoSize", &y);
		const char* projection = miscelaneus->Attribute("projectionMode");

		//m_distance = dFloat (x);
		m_2dViewportSize = dFloat (y);

		if (!strcmp (projection, "perspective")) {
			m_projectionMode = m_perspective; 
		} else if (!strcmp (projection, "orthographic")) { 
			m_projectionMode = m_orthographic; 
		} else if (!strcmp (projection, "front")) { 
			m_projectionMode = m_front; 
		} else if (!strcmp (projection, "back")) { 
			m_projectionMode = m_back; 
		} else if (!strcmp (projection, "left")) { 
			m_projectionMode = m_left; 
		} else if (!strcmp (projection, "right")) { 
			m_projectionMode = m_right; 
		} else if (!strcmp (projection, "top")) { 
			m_projectionMode = m_top; 
		} else if (!strcmp (projection, "bottom")) { 
			m_projectionMode = m_bottom; 
		}
	}
	return true;

}


// draw the 2d construction grid with origin at point (0, 0, 0)
void alchemediaCamera::DrawConstructionGrid() const
{
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable (GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);

	glPushMatrix();
	switch (m_projectionMode)
	{
		case m_perspective:
		case m_orthographic:
		{
			glBegin(GL_LINES);

			dFloat z = -10.0f;
			dFloat step = 1.0f;
			int count = 21;

			glColor3f (0.65f, 0.65f, 0.65f);
			for (int j = 0; j < count; j ++) {
				glVertex3f(- step * (count / 2), 0.0f, z); // origin of the line
				glVertex3f(  step * (count / 2), 0.0f, z); // ending point of the line

				glVertex3f(z, 0.0f, - step * (count / 2)); // origin of the line
				glVertex3f(z, 0.0f,   step * (count / 2)); // ending point of the line

				z += step;
			}

			glColor3f (0, 0, 0);

			glVertex3f(- step * (count / 2), 0.0f, 0.0f); // origin of the line
			glVertex3f(  step * (count / 2), 0.0f, 0.0f); // ending point of the line

			glVertex3f(0.0f, 0.0f, - step * (count / 2)); // origin of the line
			glVertex3f(0.0f, 0.0f,   step * (count / 2)); // ending point of the line

			glEnd( );
			break;
		}

		case m_front:
		case m_back:
		case m_left:
		case m_right:
		case m_top:
		case m_bottom:
		{
			GLint viewport[4]; 
			glGetIntegerv(GL_VIEWPORT, viewport); 

			dVector p0 (ScreenToWorld(dVector (0.0f, 0.0f, 0.0f, 0.0f)));
			dVector p1 (ScreenToWorld(dVector (dFloat (viewport[2]), 0.0f, 0.0f, 0.0f)));
			dVector p3 (ScreenToWorld(dVector (0.0f, dFloat(viewport[3]), 0.0f, 0.0f)));

			glBegin(GL_LINES);

			glColor3f (0.65f, 0.65f, 0.65f);

			dVector vStep (p3 - p0) ;
			dFloat vDist = dSqrt (vStep % vStep);
			vStep = vStep.Scale (1.0f / vDist);
			dVector q0 (p0);
			dVector q1 (p1);
			for (dFloat acc = 0.0f; acc < vDist; acc += 1.0f) {

				glVertex3f(q0.m_x, q0.m_y, q0.m_z); // origin of the line
				glVertex3f(q1.m_x, q1.m_y, q1.m_z); // origin of the line

				q0 += vStep.Scale (1.0f) ;
				q1 += vStep.Scale (1.0f) ;
			}

			dVector hStep (p1 - p0) ;
			dFloat hDist = dSqrt (hStep % hStep);
			hStep = hStep.Scale (1.0f / hDist);
			dVector r0 (p0);
			dVector r1 (p3);
			for (dFloat acc = 0.0f; acc < hDist; acc += 1.0f) {

				glVertex3f(r0.m_x, r0.m_y, r0.m_z); // origin of the line
				glVertex3f(r1.m_x, r1.m_y, r1.m_z); // origin of the line

				r0 += hStep.Scale (1.0f) ;
				r1 += hStep.Scale (1.0f) ;
			}
			glEnd();



			p0 = ScreenToWorld(dVector (dFloat(viewport[2] >> 1), 0.0f, 0.0f));
			p1 = ScreenToWorld (dVector (dFloat(viewport[2] >> 1), dFloat(viewport[3]), 0.0f, 0.0f));
			glBegin(GL_LINES);
			glColor3f (0.f, 0.f, 0.f);
			glVertex3f(p0.m_x, p0.m_y, p0.m_z); // origin of the line
			glVertex3f(p1.m_x, p1.m_y, p1.m_z); // origin of the line
			glEnd();

			p0 = ScreenToWorld(dVector (0.0f, dFloat(viewport[3] >> 1), 0.0f, 0.0f));
			p1 = ScreenToWorld(dVector (dFloat(viewport[2]), dFloat(viewport[3] >> 1), 0.0f, 0.0f));
			glBegin(GL_LINES);
			glVertex3f(p0.m_x, p0.m_y, p0.m_z); // origin of the line
			glVertex3f(p1.m_x, p1.m_y, p1.m_z); // origin of the line
			glEnd();


			break;
		}

		default:
		{

		}
	}
	glPopMatrix();
}







// draw scene in wire frame mode
//void alchemediaCamera::DrawWireFrame(dScene* const world, dScene::dTreeNode* const myNode, const dVector& color)
void alchemediaCamera::DrawWireFrame(dScene* const world, dScene::dTreeNode* const myNode, const dVector& color) const
{
	_ASSERTE (world->GetInfoFromNode(myNode) == this);


	glEnable (GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDisable(GL_CULL_FACE);
	glDisable (GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);

	for (dScene::dTreeNode* sceneNode = world->GetFirstNode (); sceneNode; sceneNode = world->GetNextNode (sceneNode)) {
		alchemediaCamera* info = (alchemediaCamera*) world->GetInfoFromNode (sceneNode);
		if (info->IsType(dSceneNodeInfo::GetRttiType())) {
			dSceneNodeInfo* const sceneInfo = (dSceneNodeInfo*) info;
			info->DrawWireFrame(world, sceneNode, sceneInfo->GetColor());
		}
	}
}




// draw scene in solid wire frame mode
//void alchemediaCamera::DrawSolidWireFrame(dScene* const world, dScene::dTreeNode* const myNode, const dVector& color)
void alchemediaCamera::DrawSolidWireFrame(dScene* const world, dScene::dTreeNode* const myNode, const dVector& color) const
{
	_ASSERTE (world->GetInfoFromNode(myNode) == this);

	//	glDisable (GL_DEPTH_TEST);
	glEnable (GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDisable(GL_CULL_FACE);
	glDisable (GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);

	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset (1.0f, 1.0f);

	//	glColor3f (0.5f, 0.5f, 0.5f);
	for (dScene::dTreeNode* sceneNode = world->GetFirstNode (); sceneNode; sceneNode = world->GetNextNode (sceneNode)) {
		alchemediaCamera* info = (alchemediaCamera*) world->GetInfoFromNode (sceneNode);
		if (info->IsType(dSceneNodeInfo::GetRttiType())) {
			dSceneNodeInfo* const sceneInfo = (dSceneNodeInfo*) info;
			info->DrawSolidWireFrame(world, sceneNode, sceneInfo->GetColor());
		}
	}

	glDisable(GL_POLYGON_OFFSET_FILL);

}

// draw scene in Gouraud shaded normal textured mode 
void alchemediaCamera::DrawGouraudShaded(dScene* const world, dScene::dTreeNode* const myNode, const dVector& color) const
{
	_ASSERTE (world->GetInfoFromNode(myNode) == this);


	glEnable (GL_DEPTH_TEST);
	glEnable (GL_CULL_FACE);
	glEnable (GL_BLEND);
	glShadeModel (GL_SMOOTH);
	glDepthFunc(GL_LEQUAL);

	// Culling. 
	glCullFace (GL_BACK);
	glFrontFace (GL_CCW);

	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset (1.0f, 1.0f);

	//	glColor3f (1.0f, 1.0f, 1.0f);
	for (dScene::dTreeNode* sceneNode = world->GetFirstNode (); sceneNode; sceneNode = world->GetNextNode (sceneNode)) {
		alchemediaCamera* info = (alchemediaCamera*) world->GetInfoFromNode (sceneNode);
		if (info->IsType(dSceneNodeInfo::GetRttiType())) {
			dSceneNodeInfo* sceneInfo = (dSceneNodeInfo*) info;
			info->DrawGouraudShaded(world, sceneNode, sceneInfo->GetColor());
		}
	}

	glDisable(GL_POLYGON_OFFSET_FILL);

}


void alchemediaCamera::DrawGizmo(dScene* const world, dScene::dTreeNode* const myNode, const dMatrix& coordinaSystem, const dVector& color, dGizmoMode mode, dFloat size) const
{

	// GetLower left corner in screen space
	GLint viewport[4]; 
	glGetIntegerv(GL_VIEWPORT, viewport); 

	// draw the coordinate axis on the lower left corner of the screen
	dFloat bolder = size;
	dVector screenPoint (dFloat(bolder * 1.5f), dFloat(viewport[3]) - bolder * 1.5f, 0.5f, 0.0f);
	dVector p0 (ScreenToWorld(screenPoint));
	screenPoint.m_x += bolder;
	dVector p1 (ScreenToWorld(screenPoint));
	dVector p1p0 (p1 - p0);
	dFloat dist = dSqrt (p1p0 % p1p0);

	glDisable (GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable (GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);

	glBegin(GL_LINES);
	for (int i = 0; i < 3; i ++) {
		dVector p1 (p0);
		dVector color (0.0f, 0.0f, 0.0f, 0.0f) ;

		p1[i] += dist;
		color[i] = 1.0f;
		glColor3f (color[0], color[1], color[2]);
		glVertex3f(p0.m_x, p0.m_y, p0.m_z); // origin of the line
		glVertex3f(p1.m_x, p1.m_y, p1.m_z); // origin of the line
	}
	glEnd();
}



dVector alchemediaCamera::ScreenToWorld(const dVector& screenPoint) const
{
	//Retrieves the viewport and stores it in the variable
	// get a point on the display array of the windows
	GLint viewport[4]; 
	glGetIntegerv(GL_VIEWPORT, viewport); 


	dMatrix modelView;
	dMatrix projection;
	glGetFloatv(GL_PROJECTION_MATRIX, &projection[0][0]);
	glGetFloatv(GL_MODELVIEW_MATRIX, &modelView[0][0]);
	modelView = modelView * projection;
	modelView = modelView.Inverse4x4();

	dVector p (2.0f * (screenPoint.m_x - (dFloat)viewport[0]) / (dFloat)viewport[2] - 1.0f,
		2.0f * (((dFloat)viewport[3] - screenPoint.m_y) - (dFloat)viewport[1]) / (dFloat)viewport[3] - 1.0f,
		2.0f * screenPoint.m_z - 1.0f,
		1.0f);

	p = modelView.RotateVector4x4(p);

	dFloat w = 1.0f / p.m_w;;
	p.m_w = 1.0f;
	p.m_x *= w;
	p.m_y *= w;
	p.m_z *= w;
	return p;
}
