
// Author: Greg Santucci, 2008 and 2009
// This is part of a port of the CSG project
// originally written in java by Danilo Balby
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#include "BooleanModeller.h"

#include "Object3D.h"
#include "Solid.h"
#include "Face.h"

#include "VertexSet.h"
#include "VectorSet.h"
#include "IntSet.h"
#include "ColorSet.h"

BooleanModeller::BooleanModeller(Solid * solid1, Solid * solid2)
{
	printf("\nConstruction Object3D from Solid\n");

	//representation to apply boolean operations
	m_pObject1 = new Object3D(solid1);
	m_pObject2 = new Object3D(solid2);
	
	printf("\nObject1 - Split against Object2\n");
	//split the faces so that none of them intercepts each other
	m_pObject1->splitFaces(m_pObject2);
	printf("\nObject2 - Split against Object1\n");
	m_pObject2->splitFaces(m_pObject1);

	printf("\nObject1 - Classify faces of Object2\n");
			
	//classify faces as being inside or outside the other solid
	m_pObject1->classifyFaces(*m_pObject2);

	printf("\nObject2 - Classify faces of Object1\n");

	m_pObject2->classifyFaces(*m_pObject1);
}

BooleanModeller::~BooleanModeller()
{
	printf("Deleting object 1\n");
	delete m_pObject1;
	printf("Deleting object 2\n");
	delete m_pObject2;
}

Solid * BooleanModeller::getUnion()
{
	return composeSolid(Face::OUTSIDE, Face::SAME, Face::OUTSIDE);
}

Solid * BooleanModeller::getIntersection()
{
	return composeSolid(Face::INSIDE, Face::SAME, Face::INSIDE);
}

Solid * BooleanModeller::getDifference()
{
	m_pObject2->invertInsideFaces();
	Solid * result = composeSolid(Face::OUTSIDE, Face::OPPOSITE, Face::INSIDE);
	m_pObject2->invertInsideFaces();

	return result;
}

Solid * BooleanModeller::composeSolid(int faceStatus1, int faceStatus2, int faceStatus3) 
{
	VertexSet * vertices = new VertexSet();
	IntSet * indices = new IntSet();
	ColorSet * colors = new ColorSet();
	
	//group the elements of the two solids whose faces fit with the desired status  
	groupObjectComponents(*m_pObject1, *vertices, *indices, *colors, faceStatus1, faceStatus2);
	groupObjectComponents(*m_pObject2, *vertices, *indices, *colors, faceStatus3, faceStatus3);
	
	VectorSet * vectors = new VectorSet();

	for(int i = 0; i < vertices->GetNumVertices(); i++)
	{
		Vertex * pVertex = vertices->GetVertex(i);
		
		vectors->AddVector(pVertex->getPosition());
	}
	
	Solid *result = new Solid(vectors, indices, colors);

	delete colors;
	delete indices;
	delete vertices;
	delete vectors;

	return result;
}

void BooleanModeller::groupObjectComponents(Object3D & object, VertexSet & vertices, IntSet & indices, ColorSet & colors, int faceStatus1, int faceStatus2)
{
	//for each face..
	for(int i=0;i<object.getNumFaces();i++)
	{
		Face & face = *(object.getFace(i));
		//if the face status fits with the desired status...
		if(face.getStatus()==faceStatus1 || face.getStatus()==faceStatus2)
		{
			//adds the face elements into the arrays 

			VertexPointerSet faceVerts;
			faceVerts.add(face.v1);
			faceVerts.add(face.v2);
			faceVerts.add(face.v3);

			for(int j=0;j<faceVerts.length();j++)
			{
				if(vertices.contains(faceVerts[j]))
				{
					indices.AddInt(vertices.indexOf(faceVerts[j]));
				}
				else
				{
					indices.AddInt(vertices.length());
					vertices.AddVertex(*faceVerts[j]);
					colors.AddColor(faceVerts[j]->getColor());
				}
			}
		}
	}
}
