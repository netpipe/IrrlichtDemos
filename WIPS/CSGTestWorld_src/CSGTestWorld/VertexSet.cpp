
// Author: Greg Santucci, 2008 and 2009
// This is part of a port of the CSG project
// originally written in java by Danilo Balby
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#include "VertexSet.h"

#include "VertexSet.h"
#include "Vertex.h"

#include <stdio.h>

VertexPointerSet::VertexPointerSet()
{
	//m_nMaxPointers = 10000;
	//m_nNumPointers = 0;
	//length = m_nNumPointers;

	//m_pPointers = new Vertex*[m_nMaxPointers];

	//m_pPointers.reserve(100);
}

VertexPointerSet::VertexPointerSet(int nMaxSize)
{
	//m_nMaxPointers = nMaxSize;
	//m_nNumPointers = 0;
	//length = m_nNumPointers;

	//m_pPointers = new Vertex*[m_nMaxPointers];
}

VertexPointerSet::~VertexPointerSet()
{
	//delete [] m_pPointers;
}

Vertex * VertexPointerSet::GetVertexPtr(int nIndex)
{
	if(nIndex < 0) return 0;
	if(nIndex >= m_pPointers.size()) return 0;
	//if(nIndex >= m_nNumPointers) return 0;

	Vertex * pVertex = m_pPointers[nIndex];
	return pVertex;
}

int VertexPointerSet::GetNumVertexPointers()
{
	//return m_nNumPointers;
	return m_pPointers.size();
}

//int VertexPointerSet::GetMaxVertexPointers()
//{
//	return m_nMaxPointers;
//}

void VertexPointerSet::AddVertexPointer(Vertex * pPointer)
{
	//if(m_nNumPointers >= m_nMaxPointers) return;

	m_pPointers.push_back(pPointer);

	//m_pPointers[m_nNumPointers] = pPointer;
	//m_nNumPointers++;
	//length = m_nNumPointers;
}

void VertexPointerSet::add(Vertex * pPointer)
{
	AddVertexPointer(pPointer);
}

Vertex * VertexPointerSet::operator[](int index)
{
	Vertex * pVertex = GetVertexPtr(index);

	// Do something if its null....

	return pVertex;
}

int VertexPointerSet::length()
{
	return m_pPointers.size();
}

//////////////////////////////////////////////////////////////////////

VertexSet::VertexSet()
{
	//m_nMaxVertices = 10000;
	//m_nNumVertices = 0;
	////length = m_nNumVertices;

	//m_pVertices = new Vertex[m_nMaxVertices];
}

VertexSet::VertexSet(int nMaxSize)
{
	//m_nMaxVertices = nMaxSize;
	//m_nNumVertices = 0;
	////length = m_nNumVertices;

	//m_pVertices = new Vertex[m_nMaxVertices];
}

VertexSet::~VertexSet()
{
	printf("Deleting vertex array\n");

	for(int i = 0; i < m_pVertices.size(); i++)
	{
		delete m_pVertices[i];
	}

	//delete [] m_pVertices;
}

Vertex * VertexSet::GetVertex(int nIndex)
{
	if(nIndex < 0) return 0;
	if(nIndex >= m_pVertices.size()) return 0;
	//if(nIndex >= m_nNumVertices) return 0;

	//Vertex * pVertex = &m_pVertices[nIndex];
	Vertex * pVertex = m_pVertices[nIndex];
	return pVertex;
}

int VertexSet::GetNumVertices()
{
	//return m_nNumVertices;
	return m_pVertices.size();
}

//int VertexSet::GetMaxVertices()
//{
//	return m_nMaxVertices;
//}

Vertex * VertexSet::AddVertex(const Vertex & vertex)
{
	//if(m_nNumVertices >= m_nMaxVertices) return 0;

	m_pVertices.push_back(new Vertex(vertex));

	//m_pVertices[m_nNumVertices] = vertex;
	//m_nNumVertices++;
	////length = m_nNumVertices;

	return m_pVertices[m_pVertices.size() - 1];
	//return &m_pVertices[m_nNumVertices - 1];
}

Vertex & VertexSet::operator[](int index)
{
	Vertex * pVertex = GetVertex(index);

	// Do something if its null....

	return *pVertex;
}

bool VertexSet::contains(Vertex * pVertex)
{
	// Match pointers or content??
	// Match pointers.

	for(int i = 0; i < length(); i++)
	{
		Vertex * pOurVertex = GetVertex(i);

		if(pOurVertex == pVertex)
		{
			return true;
		}
	}

	return false;
}

int VertexSet::indexOf(Vertex * pVertex)
{
	for(int i = 0; i < length(); i++)
	{
		Vertex * pOurVertex = GetVertex(i);

		if(pOurVertex == pVertex)
		{
			return i;
		}
	}

	return -1;
}

int VertexSet::length()
{
	//return m_nNumVertices;
	return m_pVertices.size();
}
