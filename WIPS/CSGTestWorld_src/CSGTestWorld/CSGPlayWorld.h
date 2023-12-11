
// Author: Greg Santucci, 2008 and 2009
// This is part of a port of the CSG project
// originally written in java by Danilo Balby
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#ifndef CSGPLAYWORLD_H
#define CSGPLAYWORLD_H

#include "BooleanModeller.h"
#include "Solid.h"
#include "Object3D.h"

#include "EnhancedCameraWorld.h"

class DiagnosticTool;

class CSGPlayWorld : public EnhancedCameraWorld
{
public:
	CSGPlayWorld();
	virtual ~CSGPlayWorld();

	void RunIt_Simple();
	void RunIt_Complex();
	
	virtual void UpdateSimulation(float fDelta);
	virtual void RenderScene();

	virtual void OnKeyDown(unsigned char nKey, int nX, int nY);

	BooleanModeller * m_pModeller;

	Solid * m_pSolid1;
	Solid * m_pSolid2;

	Solid * m_pMainSolid;
	Solid * m_pOperatorSolid;

	Solid * m_pUnionResult;
	Solid * m_pIntersectionResult;
	Solid * m_pDifferenceResult;

	DiagnosticTool * m_pTool;

	bool m_bRenderDiagnostics;

	bool m_bDiagnosticKeyWentDown;

private:
	
};

#endif // CSGPLAYWORLD_H
