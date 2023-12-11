
// Author: Greg Santucci, 2008 and 2009
// This is part of a port of the CSG project
// originally written in java by Danilo Balby
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#include "DiagnosticTool.h"

#include "DiagnosticEvent.h"

// Instances of this will be kept in the main viewer.
// This thing will reveal all the 3D goings on of the algorithm.

// Repeated calls to this thing's AddEvent function will add 3D events.
// This thing will merely store and playback the events.

DiagnosticTool::DiagnosticTool()
{
	m_nNumEvents = 0;
	m_nMaxEvents = 3000;
	m_nMaxEvents = 10000;
	m_pEvents = new DiagnosticEvent[m_nMaxEvents];

	m_nCurrentEvent = 0;
	m_nStepDelayReset = 20;
	m_nStepDelay = m_nStepDelayReset;
}

DiagnosticTool::~DiagnosticTool()
{
	// TODO: Implement destructor
}

void DiagnosticTool::Update()
{
	m_nStepDelay--;

	if(m_nStepDelay > 0) return;

	m_nStepDelay = m_nStepDelayReset;

	StepForward();
}

void DiagnosticTool::Render()
{
	if(m_nCurrentEvent < m_nNumEvents && m_nCurrentEvent >= 0)
	{
		m_pEvents[m_nCurrentEvent].Render();
	}
}

int g_CSGDiagToolNumEventAdds = 0;

DiagnosticEvent * DiagnosticTool::AddEvent()
{
	g_CSGDiagToolNumEventAdds++;

	if(g_CSGDiagToolNumEventAdds % 1000 == 0)
	{
		//printf("Num event adds = %d\n", g_CSGDiagToolNumEventAdds);
	}

	//return &m_pEvents[0];

	if(m_nNumEvents >= m_nMaxEvents)
	{
		return 0;
	}

	DiagnosticEvent * pEvent = &m_pEvents[m_nNumEvents];

	pEvent->Clear();

	m_nNumEvents++;

	return pEvent;
}

int DiagnosticTool::GetNumEvents()
{
	return m_nNumEvents;
}

void DiagnosticTool::ResetPlayback()
{
	m_nCurrentEvent = 0;
}

void DiagnosticTool::StepForward()
{
	m_nCurrentEvent++;

	if(m_nCurrentEvent >= m_nNumEvents)
	{
		m_nCurrentEvent = m_nNumEvents - 1;
	}
}

void DiagnosticTool::StepBackward()
{
	m_nCurrentEvent--;

	if(m_nCurrentEvent < 0)
	{
		m_nCurrentEvent = 0;
	}
}
