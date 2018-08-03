
// Author: Greg Santucci, 2008 and 2009
// This is part of a port of the CSG project
// originally written in java by Danilo Balby
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#ifndef DIAGNOSTICTOOL_H
#define DIAGNOSTICTOOL_H

#include "ML_Vector.h"
#include "GX_Color.h"

class DiagnosticEvent;

// Perhaps make a stack, or make it possible to have several triangles in each event.

class DiagnosticTool
{
public:
	DiagnosticTool();
	virtual ~DiagnosticTool();
	
	void Update();
	void Render();

	DiagnosticEvent * AddEvent();

	void ResetPlayback();
	void StepForward();
	void StepBackward();

	int GetNumEvents();
	
private:

	DiagnosticEvent * m_pEvents;
	int m_nNumEvents;
	int m_nMaxEvents;

	int m_nCurrentEvent;

	int m_nStepDelay;
	int m_nStepDelayReset;
};

#endif // DIAGNOSTICTOOL_H

