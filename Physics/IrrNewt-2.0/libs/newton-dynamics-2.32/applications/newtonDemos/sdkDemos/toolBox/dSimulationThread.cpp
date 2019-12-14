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


#include "toolbox_stdafx.h"
#include "dSimulationThread.h"
#include "../DemoEntityManager.h"


dSimulationThread::dSimulationThread()
	:dThread(), m_asycronousUpdate(true), m_physicsTime(0.0f)
{
}

dSimulationThread::~dSimulationThread()
{
}


unsigned dSimulationThread::RunMyTask ()
{
m_asycronousUpdate = false;
	if (m_asycronousUpdate) {
		DemoEntityManager& me = *((DemoEntityManager*)this);
		me.UpdatePhysics();
	}
	return 1;
}
