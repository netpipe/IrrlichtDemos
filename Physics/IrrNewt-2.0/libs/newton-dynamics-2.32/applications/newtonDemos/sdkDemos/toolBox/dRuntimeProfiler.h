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

#ifndef _D_RUNTINE_PROFILER_164045623__
#define _D_RUNTINE_PROFILER_164045623__

struct NewtonWorld;

#define MAX_FRAMES 256
#define MAX_FRAMES_STEP 1
#define MAX_TRACKS 10

#define CHART_HIEGHT 120

class QPainter;

class dRuntimeProfiler
{
	public:
	dRuntimeProfiler(int origin_x, int origin_y);
	~dRuntimeProfiler(void);

	void Render (NewtonWorld* nWorld, int mask, QPainter& context);
	void DrawLabel (dFloat x, dFloat y, const char* label, QPainter& context);
	void DrawTrack (dFloat x, dFloat y, const dVector& color, int start, unsigned* track);

	void ReanderThreadPerformace (NewtonWorld* nWorld, QPainter& context);



	static unsigned GetTimeInMicrosecunds();

	int m_oringin_x;
	int m_oringin_y;
	int m_frameIndex;
	unsigned m_perfomanceTracks[MAX_TRACKS][MAX_FRAMES];
};


#endif
