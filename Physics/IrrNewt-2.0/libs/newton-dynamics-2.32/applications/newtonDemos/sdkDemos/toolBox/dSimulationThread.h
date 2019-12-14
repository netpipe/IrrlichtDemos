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

#ifndef __DRENDER_THREAD_H__
#define __DRENDER_THREAD_H__

class DemoEntityManager;

class dSimulationThread: public dThread
{
	public:
	dSimulationThread();
	~dSimulationThread();

	virtual unsigned RunMyTask ();
	bool m_asycronousUpdate;
	dFloat m_physicsTime;
};

#endif