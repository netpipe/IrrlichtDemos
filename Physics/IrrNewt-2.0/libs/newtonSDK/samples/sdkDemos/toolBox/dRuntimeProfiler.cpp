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

#include <toolbox_stdafx.h>
#include "OpenGlUtil.h"
#include "dRuntimeProfiler.h"

struct GLViewPort
{
	GLint x;
	GLint y;
	GLsizei width;
	GLsizei height;
};

dRuntimeProfiler::dRuntimeProfiler(int origin_x, int origin_y)
{
	m_oringin_x = origin_x;
	m_oringin_y = origin_y;
	m_frameIndex = 0;
	memset (m_perfomanceTracks, 0, sizeof (m_perfomanceTracks));
}

dRuntimeProfiler::~dRuntimeProfiler(void)
{
}



void dRuntimeProfiler::DrawLabel (dFloat x, dFloat y, const char* label)
{
	int witdh;
	int height;
	GLint viewport[4]; 
	//Retrieves the viewport and stores it in the variable
	glGetIntegerv(GL_VIEWPORT, viewport); 
	witdh = viewport[2];
	height = viewport[3];

	dVector color (1.0f, 1.0f, 1.0f, 0.0f);
	Print (color, x, height - y, label);
}


void dRuntimeProfiler::DrawTrack (dFloat x0, dFloat y0, const dVector& color, int start, unsigned* track)
{
	int i;	 
	int index;	 
	glColor3f(color.m_x, color.m_y, color.m_z);

	unsigned buffer[2048];

	index = 0;
	for (i = start + 1; i < MAX_FRAMES; i ++) {
		buffer[index] = track[i];
		index ++;
	}

	for (i = 0; i < start; i ++) {
		buffer[index] = track[i];
		index ++;
	}
	buffer[index] = track[start];

	glBegin(GL_LINE_STRIP);
	for (i = 0; i < MAX_FRAMES; i ++) {
		dFloat y1 = dFloat (buffer[i]) * 1.0e-3f * (CHART_HIEGHT / 16.666f);
		glVertex3f (x0 + i * MAX_FRAMES_STEP, y0 + y1, 0.0f);
	}

	glEnd();
}


void dRuntimeProfiler::Render (NewtonWorld* nWorld, int mask)
{
	int i;
	GLViewPort viewport;

	//Retrieves the viewport and stores it in the variable
	glGetIntegerv(GL_VIEWPORT, &viewport.x); 

	for (i = 0; i < MAX_TRACKS; i ++) {
		m_perfomanceTracks[i][m_frameIndex] = NewtonReadPerformanceTicks (nWorld, i);
	}

	glColor3f(1.0, 1.0, 1.0);
	glDisable (GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);

	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, viewport.width, 0, viewport.height );

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);


	dFloat x0 = dFloat (m_oringin_x);
	dFloat y0 = dFloat (m_oringin_y);
	dFloat x1 = x0 + MAX_FRAMES * MAX_FRAMES_STEP;
	dFloat y1 = y0 + CHART_HIEGHT;


	glBegin(GL_LINES);

	glVertex3f (x0, y0, 0.0f);
	glVertex3f (x0, y1, 0.0f);

	glVertex3f (x0, y0, 0.0f);
	glVertex3f (x1, y0, 0.0f);


	for (i = 1; i < 4; i ++) {
		dFloat y = y0 + (y1 - y0) * i / 4;
		glVertex3f (x0 - 5, y, 0.0f);
		glVertex3f (x0 + 5, y, 0.0f);
	}


	for (i = 1; i < MAX_FRAMES; i += 16) {
		dFloat x = x0 + (x1 - x0) * i / MAX_FRAMES;
		glVertex3f (x , y0 - 5, 0.0f);
		glVertex3f (x , y0 + 5, 0.0f);
	}
	glEnd();

	// total engine time
	if (mask & 1) {
		DrawTrack (x0, y0, dVector (1.0f, 1.0f, 1.0f), m_frameIndex, &m_perfomanceTracks[NEWTON_PROFILER_WORLD_UPDATE][0]);
	}


	// draw collision performance
	if (mask & 2) { 
		DrawTrack (x0, y0, dVector (0.0f, 1.0f, 0.0f), m_frameIndex, &m_perfomanceTracks[NEWTON_PROFILER_COLLISION_UPDATE][0]);
	}


	// draw force Update performance
	if (mask & 4) { 
		DrawTrack (x0, y0, dVector (0.5f, 1.0f, 0.5f), m_frameIndex, &m_perfomanceTracks[NEWTON_PROFILER_FORCE_CALLBACK_UPDATE][0]);
	}

	if (mask & 8) { 
		DrawTrack (x0, y0, dVector (0.0f, 0.0f, 1.0f), m_frameIndex, &m_perfomanceTracks[NEWTON_PROFILER_COLLISION_UPDATE_BROAD_PHASE][0]);
	}

	if (mask & 16) { 
		DrawTrack (x0, y0, dVector (1.0f, 0.0f, 0.0f), m_frameIndex, &m_perfomanceTracks[NEWTON_PROFILER_COLLISION_UPDATE_NARROW_PHASE][0]);
	}


	// draw dynamics performance
	if (mask & 32) { 
		DrawTrack (x0, y0, dVector (0.0f, 1.0f, 1.0f), m_frameIndex, &m_perfomanceTracks[NEWTON_PROFILER_DYNAMICS_UPDATE][0]);
	}

	if (mask & 64) { 
		DrawTrack (x0, y0, dVector (1.0f, 1.0f, 0.0f), m_frameIndex, &m_perfomanceTracks[NEWTON_PROFILER_DYNAMICS_SOLVE_CONSTRAINT_GRAPH][0]);
	}

	if (mask & 128) { 
		DrawTrack (x0, y0, dVector (1.0f, 0.0f, 1.0f), m_frameIndex, &m_perfomanceTracks[NEWTON_PROFILER_DYNAMICS_CONSTRAINT_GRAPH][0]);
	}


//	if (m_thread == 0) {
	{
		glColor3f(1.0, 1.0, 1.0);
	//	DrawLabel (x0 + 4, y1 - 0, "time in");
	//	DrawLabel (x0 + 4, y1 - 10, "milliseconds");

		DrawLabel (x0 - 20, y0 + (y1 - y0) * 0 / 4, "0");

		for (i = 1; i < 5; i ++) {
			char label[32];
			sprintf (label, "%4.2f", (1000.0f / 60.0f) * (float)i / 4.0f );
			DrawLabel (x0 - 42, y0 + (y1 - y0) * i / 4, label);
		}

//		char label[64];
//		sprintf (label, "thread %d", m_thread);
//		DrawLabel (x0 - 4, y0 - 15, label);
//		DrawLabel (x1 - 40, y0 - 15, "frames");
	}

	glMatrixMode(GL_TEXTURE);
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glEnable( GL_DEPTH_TEST );


	m_frameIndex = (m_frameIndex + 1) % MAX_FRAMES;

	glColor3f(1.0, 1.0, 1.0);
}




