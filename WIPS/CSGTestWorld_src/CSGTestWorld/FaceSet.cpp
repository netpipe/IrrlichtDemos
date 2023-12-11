
// Author: Greg Santucci, 2008 and 2009
// This is part of a port of the CSG project
// originally written in java by Danilo Balby
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#include "FaceSet.h"

#include "Face.h"

#include "DiagnosticTool.h"
#include "DiagnosticEvent.h"

extern DiagnosticTool * g_pTool;

FaceSet::FaceSet()
{
	m_nMaxSize = 20000;
	m_nSize = 0;
	m_pFaces = new Face[m_nMaxSize];
}

//FaceSet::FaceSet(int nMaxSize)
//{
//	m_nMaxSize = nMaxSize;
//	m_nSize = 0;
//	m_pFaces = new Face[m_nMaxSize];
//
//	length = m_nSize;
//}

FaceSet::~FaceSet()
{
	delete [] m_pFaces;
}

//int FaceSet::GetMaxSize()
//{
//	return m_nMaxSize;
//}

int FaceSet::GetSize()
{
	return m_nSize;
}

int FaceSet::length()
{
	return m_nSize;
}

Face * FaceSet::GetFace(int i)
{
	if(i < 0) return 0;
	if(i >= m_nSize) return 0;

	return &m_pFaces[i];
}

void FaceSet::SetFace(int i, Face & vFace)
{
	if(i < 0) return;
	if(i >= m_nSize) return;

	m_pFaces[i] = vFace;
}

Face * FaceSet::AddFace(Face & vFace)
{
	if(m_nSize >= m_nMaxSize)
	{
		return 0;
	}

	if(false)
	{
		DiagnosticEvent * pEvent = g_pTool->AddEvent();
		if(pEvent)
		{
			pEvent->AddTriangle(vFace.v1->getPosition(), vFace.v2->getPosition(), vFace.v3->getPosition(), gxColorWhite);
			pEvent->SetText("AddFace %d", m_nSize+1);
		}
	}

	m_pFaces[m_nSize] = vFace;
	m_nSize++;

	return &m_pFaces[m_nSize - 1];
}

Face & FaceSet::operator[](int index)
{
	Face * pFace = GetFace(index);

	// If its null, we're in trouble...

	return *pFace;
}

Face * FaceSet::InsertFace(int i, Face & vFace)
{
	if(m_nSize >= m_nMaxSize)
	{
		return 0;
	}

	if(false)
	{
		DiagnosticEvent * pEvent = g_pTool->AddEvent();
		if(pEvent)
		{
			pEvent->AddTriangle(vFace.v1->getPosition(), vFace.v2->getPosition(), vFace.v3->getPosition(), gxColorWhite);
			pEvent->SetText("InsertFace %d", i);
		}
	}

	// Shift everything along
	for(int j = m_nSize; j >= i+1; j--)
	{
		m_pFaces[j] = m_pFaces[j-1];
	}

	m_pFaces[i] = vFace;
	m_nSize++;

	return &m_pFaces[i];
}

void FaceSet::RemoveFace(int i)
{
	if(m_nSize <= 0)
	{
		return;
	}

	if(false)
	{
		DiagnosticEvent * pEvent = g_pTool->AddEvent();
		if(pEvent)
		{
			pEvent->AddTriangle(m_pFaces[i].v1->getPosition(), m_pFaces[i].v2->getPosition(), m_pFaces[i].v3->getPosition(), gxColorWhite);
			pEvent->SetText("RemoveFace %d", i);
		}
	}

	for(int j = i; j < m_nSize-1; j++)
	{
		m_pFaces[j] = m_pFaces[j+1];
	}

	m_nSize--;
}

void FaceSet::remove(int i)
{
	RemoveFace(i);
}
