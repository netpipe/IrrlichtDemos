
// Author: Greg Santucci, 2008 and 2009
// This is part of a port of the CSG project
// originally written in java by Danilo Balby
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#ifndef VERTEXSET_H
#define VERTEXSET_H

#include <vector>

class Vertex;

class VertexPointerSet
{
public:
	VertexPointerSet();
	VertexPointerSet(int nMaxSize);
	virtual ~VertexPointerSet();

	Vertex * GetVertexPtr(int nIndex);
	int GetNumVertexPointers();
	//int GetMaxVertexPointers();

	void AddVertexPointer(Vertex * pPointer);

	void add(Vertex * pPointer);
	Vertex * get(int nIndex);
	int length();

	Vertex* operator[](int index);


private:

	typedef std::vector<Vertex *> Vertices;

	Vertices m_pPointers;

	//Vertex ** m_pPointers;
	//int m_nMaxPointers;
	//int m_nNumPointers;
};

class VertexSet
{
public:
	VertexSet();
	VertexSet(int nMaxSize);
	virtual ~VertexSet();

	Vertex * GetVertex(int nIndex);
	int GetNumVertices();
	//int GetMaxVertices();

	Vertex * AddVertex(const Vertex & vertex);

	Vertex & operator[](int index);

	bool contains(Vertex * pVertex);
	int indexOf(Vertex * pVertex);

	int length();

private:

	typedef std::vector<Vertex *> Vertices;
	Vertices m_pVertices;

	//Vertex * m_pVertices;
	//int m_nMaxVertices;
	//int m_nNumVertices;
};

#endif // VERTEXSET_H
