
// Author: Greg Santucci, 2008 and 2009
// This is part of a port of the CSG project
// originally written in java by Danilo Balby
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#include "DiagnosticEvent.h"

#include "GX_Model.h"
#include "GX_ModelUtil.h"

#include "Object3D.h"
#include "FaceSet.h"
#include "Face.h"

#include <stdlib.h>
#include <stdarg.h>

// This contains the model that will be shown when the event is to be displayed.
// We can just start with a single triangle.

// Was thinking could also add a line, but then I thought fuck it, why not generalise it to a model.

DiagnosticEvent::DiagnosticEvent()
{
	m_pModel = 0;
	m_pObject1 = 0;
	m_pObject2 = 0;

	m_sText[0] = '\0';

	// Lets see if this speeds things up
	//return;

	mlVector3D vOffset(100,50,150);
	float fScale = 20.0f;

	m_pModel = new gxModel();
	m_pModel->m_correctionalTransform.SetTranslation(vOffset);
	m_pModel->m_correctionalTransform.SetScale(fScale);
	//m_pModel = 0;

	// Instead of just holding the reference, it should hold entire models here, so that we can see how it changes.
	// This reference is displaying what it looks like after the processing - not as useful.

	m_pObject1 = new gxModel();
	m_pObject2 = new gxModel();

	m_pObject1->m_correctionalTransform.SetTranslation(vOffset);
	m_pObject1->m_correctionalTransform.SetScale(fScale);
	m_pObject2->m_correctionalTransform.SetTranslation(vOffset);
	m_pObject2->m_correctionalTransform.SetScale(fScale);
}

DiagnosticEvent::~DiagnosticEvent()
{
	delete m_pModel;

	delete m_pObject1;
	delete m_pObject2;
}

void DiagnosticEvent::Update()
{
	// TODO: Add update code
}

void DiagnosticEvent::Render()
{
	if(m_pModel)
	{
		m_pModel->Render();
	}

	if(m_pObject1)
	{
		//m_pObject1->m_bRenderTriangleIndices = true;

		m_pObject1->Render();
		m_pObject1->RenderAsWireframe(gxColorWhite, 1.0f);
	}

	if(m_pObject2)
	{
		//m_pObject1->m_bRenderTriangleIndices = true;

		m_pObject2->Render();
		m_pObject2->RenderAsWireframe(gxColorBlack, 1.0f);
	}

	extern mlTransform * g_pTheCamera;

	static float x = -13,y = -9.5,z = -10;

	//mlVector3D vTextPosition = g_pTheCamera->TransformPoint(mlVector3D(2, -2, -10));
	mlVector3D vTextPosition = g_pTheCamera->TransformPoint(mlVector3D(x,y,z));

	gxRenderText(gxColorWhite, vTextPosition, m_sText);
}

void DiagnosticEvent::Clear()
{
	m_pModel->Clear();
	m_pObject1->Clear();
	m_pObject2->Clear();
}

void DiagnosticEvent::AddTriangle(const mlVector3D & v1, const mlVector3D & v2, const mlVector3D & v3, const gxColor & col)
{
	// Lets see if this speeds things up
	//return;

	m_pModel->m_triangles.push_back(gxRenderingTriangle(col, v1,v2,v3));
}

void DiagnosticEvent::AddLine(const mlVector3D & v1, const mlVector3D & v2, const gxColor & col)
{
	// Lets see if this speeds things up
	//return;

	m_pModel->m_lines.push_back(gxRenderingLine(col, v1,v2));
}

void DiagnosticEvent::SetText(const char format[], ...)
{
	// Lets see if this speeds things up
	//return;

	const unsigned int utFormatStringMax = 4000;
	char text[utFormatStringMax];

	{
		assert(format);

		va_list argList;
		va_start(argList, format);

		U32 charCount = vsprintf(text, format, argList);

		text[utFormatStringMax - 1] = '\0';

		assert(charCount <= utFormatStringMax - 1);

		va_end(argList);
	}

	sprintf(m_sText, "%s", text);
}

void DiagnosticEvent::SetObjects(Object3D * pObj1, Object3D * pObj2)
{
	// Lets see if this speeds things up
	//return;

	// delete and re-"new" them
	// Then iterate through the faces in the objects, so the wireframe lines can be added.

	if(m_pObject1 && pObj1)
	{
		m_pObject1->Clear();

		for(int i = 0; i < pObj1->faces->GetSize(); i++)
		{
			Face * pFace = pObj1->faces->GetFace(i);
			gxRenderingTriangle tri(gxColorRed, pFace->v1->getPosition(), pFace->v2->getPosition(), pFace->v3->getPosition());
			m_pObject1->m_triangles.push_back(tri);
		}
	}

	if(m_pObject2 && pObj2)
	{
		m_pObject2->Clear();

		for(int i = 0; i < pObj2->faces->GetSize(); i++)
		{
			Face * pFace = pObj2->faces->GetFace(i);
			gxRenderingTriangle tri(gxColorYellow, pFace->v1->getPosition(), pFace->v2->getPosition(), pFace->v3->getPosition());
			m_pObject2->m_triangles.push_back(tri);
		}
	}
}

void DiagnosticEvent::SetObjectWithHighlightedTriangle(Object3D * pObj, int nTriIndex)
{
	if(m_pObject1 && pObj)
	{
		m_pObject1->Clear();

		for(int i = 0; i < pObj->faces->GetSize(); i++)
		{
			Face * pFace = pObj->faces->GetFace(i);
			if(i <= nTriIndex)
			{
				gxRenderingTriangle tri(gxColorRed, gxColorGreen, gxColorBlue, pFace->v1->getPosition(), pFace->v2->getPosition(), pFace->v3->getPosition());
				m_pObject1->m_triangles.push_back(tri);
			}
			else
			{
				//gxRenderingTriangle tri(gxColor(255,0,0,150), pFace->v1->getPosition(), pFace->v2->getPosition(), pFace->v3->getPosition());
				//m_pObject1->m_triangles.push_back(tri);
			}
		}
	}
}
