
// Author: Greg Santucci, 2008 and 2009
// This is part of a port of the CSG project
// originally written in java by Danilo Balby
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#include "CSGPlayWorld.h"

#include "GX_TextureArchive.h"
#include "GX_RenderingTriangle.h"

#include "DiagnosticTool.h"

DiagnosticTool * g_pTool = 0;

CSGPlayWorld::CSGPlayWorld() : EnhancedCameraWorld()
{
	m_pModeller = 0;
	m_pSolid1 = 0;
	m_pSolid2 = 0;
	m_pUnionResult = 0;
	m_pIntersectionResult = 0;
	m_pDifferenceResult = 0;

	m_bRenderDiagnostics = false;
	m_bDiagnosticKeyWentDown = false;

	m_pTool = new DiagnosticTool();
	g_pTool = m_pTool;

	//RunIt_Simple();
	RunIt_Complex();
}

CSGPlayWorld::~CSGPlayWorld()
{
	printf("Deleting solids\n");
	delete m_pSolid1;
	delete m_pSolid2;
	printf("Deleting modeller\n");
	delete m_pModeller;
	printf("Deleting result\n");
	delete m_pUnionResult;
	delete m_pIntersectionResult;
	delete m_pDifferenceResult;

	printf("Deleting diagnostic tool\n");
	delete m_pTool;
	g_pTool = 0;
}

void CSGPlayWorld::RunIt_Simple()
{
	m_pSolid1 = new Solid("box.txt", gxColorBlue, 0, 150, -120);
	m_pSolid2 = new Solid("cone.txt", gxColorRed, -120, 150, 0);

	m_pSolid1->Translate(mlVector3D(0.2, -0.2, 0));
	m_pSolid1->Scale(mlVector3D(0.4, 0.3, 2.8));

	m_pModeller = new BooleanModeller(m_pSolid2, m_pSolid1);

	m_pUnionResult = m_pModeller->getUnion();
	m_pIntersectionResult = m_pModeller->getIntersection();
	m_pDifferenceResult = m_pModeller->getDifference();

	printf("              %10s %10s %10s\n", "Union", "Intersection", "Difference");
	printf("Num vertices  %10d %10d %10d\n", m_pUnionResult->vertices.GetSize(), m_pIntersectionResult->vertices.GetSize(), m_pDifferenceResult->vertices.GetSize());
	printf("Num indices   %10d %10d %10d\n", m_pUnionResult->indices.GetSize(), m_pIntersectionResult->indices.GetSize(), m_pDifferenceResult->indices.GetSize());
	printf("Num colors    %10d %10d %10d\n", m_pUnionResult->colors.GetSize(), m_pIntersectionResult->colors.GetSize(), m_pDifferenceResult->colors.GetSize());
}

void CSGPlayWorld::RunIt_Complex()
{
	m_pSolid1 = new Solid("box.txt", gxColorBlue, 0, 150, -120);
	m_pSolid2 = new Solid("cone.txt", gxColorRed, -120, 150, 0);

	m_pSolid1->Translate(mlVector3D(0.2, -0.2, 0));
	m_pSolid1->Scale(mlVector3D(0.4, 2.3, 2.8));

	BooleanModeller * pMidModeller = new BooleanModeller(m_pSolid2, m_pSolid1);

	Solid * pMidResult = pMidModeller->getDifference();

	Solid * pSphere = new Solid("sphere.txt", gxColorGreen, 150, -120, 0);

	pSphere->Translate(mlVector3D(0.5, 0, 0));
	pSphere->Scale(mlVector3D(1.0, 1.0, 0.4));

	//m_pModeller = new BooleanModeller(m_pSolid1, m_pSolid2);
	//m_pModeller = new BooleanModeller(m_pSolid2, m_pSolid1);
	m_pModeller = new BooleanModeller(pMidResult, pSphere);

	m_pUnionResult = m_pModeller->getUnion();
	m_pIntersectionResult = m_pModeller->getIntersection();
	m_pDifferenceResult = m_pModeller->getDifference();

	printf("              %10s %10s %10s\n", "Union", "Intersection", "Difference");
	printf("Num vertices  %10d %10d %10d\n", m_pUnionResult->vertices.GetSize(), m_pIntersectionResult->vertices.GetSize(), m_pDifferenceResult->vertices.GetSize());
	printf("Num indices   %10d %10d %10d\n", m_pUnionResult->indices.GetSize(), m_pIntersectionResult->indices.GetSize(), m_pDifferenceResult->indices.GetSize());
	printf("Num colors    %10d %10d %10d\n", m_pUnionResult->colors.GetSize(), m_pIntersectionResult->colors.GetSize(), m_pDifferenceResult->colors.GetSize());

	// Cleaning up sub objects.

	delete pSphere;
	delete pMidResult;
	delete pMidModeller;

	// A save function may be handy as well for the Solid
}

void CSGPlayWorld::UpdateSimulation(float fDelta)
{
	if(m_bUpArrowKeyDown || m_bRightArrowKeyWentDown)
	{
		m_pTool->StepForward();
	}

	if(m_bDownArrowKeyDown || m_bLeftArrowKeyWentDown)
	{
		m_pTool->StepBackward();
	}

	//m_pTool->Update();

	if(m_bDiagnosticKeyWentDown) m_bRenderDiagnostics = !m_bRenderDiagnostics;

	m_bDiagnosticKeyWentDown = false;
}

void CSGPlayWorld::RenderScene()
{
	m_pUnionResult->m_correctionalTransform.SetTranslation(mlVector3D(0,40,0));
	m_pIntersectionResult->m_correctionalTransform.SetTranslation(mlVector3D(50,40,0));
	m_pDifferenceResult->m_correctionalTransform.SetTranslation(mlVector3D(100,40,0));

	m_pUnionResult->Render();
	m_pIntersectionResult->Render();
	m_pDifferenceResult->Render();

	if(m_bRenderDiagnostics)
	{
		m_pTool->Render();
	}
}

void CSGPlayWorld::OnKeyDown(unsigned char nKey, int nX, int nY)
{
	if(!IsPointInViewport(nX, nY))
	{
		return;
	}
	
	if(nKey == 'd')
	{
		m_bDiagnosticKeyWentDown = true;
	}

	EnhancedCameraWorld::OnKeyDown(nKey, nX, nY);
}
