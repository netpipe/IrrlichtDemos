
// Author: Greg Santucci, 2008 and 2009
// This is part of a port of the CSG project
// originally written in java by Danilo Balby
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#include "Solid.h"

#include "ML_Vector.h"

#include "VectorSet.h"
#include "ColorSet.h"
#include "IntSet.h"

#include <fstream>

#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>
#include <tchar.h>

#include <GL/gl.h>
#include <GL/glu.h>

Solid::Solid(const std::string & sFileName, const gxColor & colBase, int dRed, int dGreen, int dBlue)
{
	loadCoordinateFile(sFileName, colBase, dRed, dGreen, dBlue);
}

Solid::Solid(VectorSet * vertices, IntSet * indices, ColorSet * colors)
{
	setData(*vertices, *indices, *colors);		
}

Solid::~Solid()
{
}

//---------------------------------------GETS-----------------------------------//

VectorSet * Solid::getVertices()
{
	// This thing makes a fresh copy and hands the requestor the copy.

	VectorSet * newVertices = new VectorSet();

	for(int i = 0; i < vertices.length(); i++)
	{
		newVertices->AddVector(vertices[i]);
	}

	return newVertices;
}

IntSet * Solid::getIndices()
{
	IntSet * newIndices = new IntSet();

	for(int i = 0; i < indices.length(); i++)
	{
		newIndices->AddInt(indices[i]);
	}

	return newIndices;
}

ColorSet * Solid::getColors()
{
	ColorSet * newColors = new ColorSet();

	for(int i = 0; i < colors.length(); i++)
	{
		newColors->AddColor(colors[i]);
	}

	return newColors;
}

bool Solid::isEmpty()
{
	if(indices.length() <= 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//---------------------------------------SETS-----------------------------------//

/**
 * Sets the solid data. An exception may occur in the case of abnormal arrays 
 * (indices making references to inexistent vertices, there are less colors 
 * than vertices...)
 * 
 * @param vertices array of points defining the solid vertices
 * @param indices array of indices for a array of vertices
 * @param colors array of colors defining the vertices colors 
 */
void Solid::setData(VectorSet & vertices, IntSet & indices, ColorSet & colors)
{
	// Clear them...
	//this->vertices = new VectorSet();
	//this->colors = new ColorSet();
	//this->indices = new IntSet();

	//this->vertices.Clear();
	//this->colors.Clear();
	//this->indices.Clear();

	if(indices.length() > 0)
	{
		for(int i=0; i<vertices.length(); i++)
		{
			// Should add cloning to vectorset, and other 2 sets

			this->vertices.AddVector(vertices[i]);
			this->colors.AddColor(colors[i]);
			//this->indices.AddInt(indices[i]);
		}
	
		for(int i=0; i<indices.length(); i++)
		{
			this->indices.AddInt(indices[i]);
		}
	}
}

/**
 * Sets the solid color (all the vertices with the same color)
 * 
 * @param color solid color
 */
void Solid::setColor(const gxColor & color)
{
	for(int i=0; i < vertices.length(); i++)
	{
		colors.SetColor(i, color);
	}
}

//-------------------------GEOMETRICAL_TRANSFORMATIONS-------------------------//

void Solid::Translate(const mlVector3D & t)
{
	for(int i = 0; i < vertices.GetSize(); i++)
	{
		mlVector3D v = vertices.GetVector(i);

		v = v + t;

		vertices.SetVector(i, v);
	}
}

void Solid::Rotate(const mlVector3D & a, const mlVector3D & b, float angle)
{
	for(int i = 0; i < vertices.GetSize(); i++)
	{
		mlVector3D v = vertices.GetVector(i);

		v = mlVectorRotate(v, a, b, angle);

		vertices.SetVector(i, v);
	}
}

void Solid::Scale(const mlVector3D & s)
{
	for(int i = 0; i < vertices.GetSize(); i++)
	{
		mlVector3D v = vertices.GetVector(i);

		v = mlVectorScale(v, s);

		vertices.SetVector(i,v);
	}
}

//-----------------------------------PRIVATES--------------------------------//

void Solid::Render()
{
	glPushMatrix();
	
	mlMatrix4x4 mMatrix = mlMatrix4x4(m_correctionalTransform.GetMatrix());
	mlMatrix4x4 * pMatrix = &mMatrix;
	mlFloat * pafMatrix = reinterpret_cast<mlFloat*>(pMatrix);

	glMultMatrixf(pafMatrix);

	glEnable (GL_POLYGON_OFFSET_FILL);
	glPolygonOffset (1., 1.);

	glLineWidth(1);

	glBegin(GL_TRIANGLES);

	int nNumTriangles = indices.GetSize() / 3;

	for(int i = 0; i < nNumTriangles; i++)
	{
		int nIndex1 = indices.GetInt(i * 3 + 0);
		int nIndex2 = indices.GetInt(i * 3 + 1);
		int nIndex3 = indices.GetInt(i * 3 + 2);

		mlVector3D vP1 = vertices.GetVector(nIndex1);
		mlVector3D vP2 = vertices.GetVector(nIndex2);
		mlVector3D vP3 = vertices.GetVector(nIndex3);

		float fScale = 40.0f;
		vP1 *= fScale;
		vP2 *= fScale;
		vP3 *= fScale;

		mlVector3D vTranslation(50,20,50);
		vP1 += vTranslation;
		vP2 += vTranslation;
		vP3 += vTranslation;

		gxColor col1 = colors.GetColor(nIndex1);
		gxColor col2 = colors.GetColor(nIndex2);
		gxColor col3 = colors.GetColor(nIndex3);

		glColor4ub(col1.red, col1.green, col1.blue, 255);
		glVertex3d(vP1.x, vP1.y, vP1.z);

		glColor4ub(col2.red, col2.green, col2.blue, 255);
		glVertex3d(vP2.x, vP2.y, vP2.z);

		glColor4ub(col3.red, col3.green, col3.blue, 255);
		glVertex3d(vP3.x, vP3.y, vP3.z);
	}

	glEnd();

	glLineWidth(1);

	glBegin(GL_LINES);

	for(int i = 0; i < nNumTriangles; i++)
	{
		int nIndex1 = indices.GetInt(i * 3 + 0);
		int nIndex2 = indices.GetInt(i * 3 + 1);
		int nIndex3 = indices.GetInt(i * 3 + 2);

		mlVector3D vP1 = vertices.GetVector(nIndex1);
		mlVector3D vP2 = vertices.GetVector(nIndex2);
		mlVector3D vP3 = vertices.GetVector(nIndex3);

		float fScale = 40.0f;
		vP1 *= fScale;
		vP2 *= fScale;
		vP3 *= fScale;

		mlVector3D vTranslation(50,20,50);
		vP1 += vTranslation;
		vP2 += vTranslation;
		vP3 += vTranslation;

		gxColor col1 = colors.GetColor(nIndex1);
		gxColor col2 = colors.GetColor(nIndex2);
		gxColor col3 = colors.GetColor(nIndex3);

		//glColor4ub(255,255,255, 255);
		//glColor4ub(127,127,127, 255);

		glColor4ub(200,200,200, 255);
		//glColor4ub(0,0,0, 255);

		glVertex3d(vP1.x, vP1.y, vP1.z);
		glVertex3d(vP2.x, vP2.y, vP2.z);

		glVertex3d(vP2.x, vP2.y, vP2.z);
		glVertex3d(vP3.x, vP3.y, vP3.z);

		glVertex3d(vP3.x, vP3.y, vP3.z);
		glVertex3d(vP1.x, vP1.y, vP1.z);
	}

	glEnd();

	glLineWidth(1);
	glColor4ub(255,255,255, 255);

	glPopMatrix();
}

/**
 * Gets the solid mean
 * 
 * @return point representing the mean
 */
mlVector3D Solid::getMean()
{
	mlVector3D mean;
	for(int i=0;i<vertices.length();i++)
	{
		mean.x += vertices[i].x;
		mean.y += vertices[i].y;
		mean.z += vertices[i].z;
	}
	mean.x /= vertices.length();
	mean.y /= vertices.length();
	mean.z /= vertices.length();
	
	return mean;
}

/**
 * Loads a coordinates file, setting vertices and indices 
 * 
 * @param solidFile file used to create the solid
 * @param color solid color
 */
void Solid::loadCoordinateFile(const std::string & sFileName, const gxColor & colBase, int dRed, int dGreen, int dBlue)
{
	std::ifstream modelFile;

	modelFile.open(sFileName.c_str());

	int nNumVertices = 0;
	modelFile >> nNumVertices;

	for(int i = 0; i < nNumVertices; i++)
	{
		// Read in a vector.
		int nVertexID = 0;

		mlVector3D vPosition;

		modelFile >> nVertexID;

		modelFile >> vPosition.x;
		modelFile >> vPosition.y;
		modelFile >> vPosition.z;

		vPosition = mlVectorScale(vPosition, mlVector3D(0.5f, 0.5f, 0.5f));

		vertices.AddVector(vPosition);
	}

	// Now load up the indices.

	int nNumTriangles = 0;
	modelFile >> nNumTriangles;

	for(int i = 0; i < nNumTriangles; i++)
	{
		int nTriangleID = 0;

		int nTriIndex1 = 0;
		int nTriIndex2 = 0;
		int nTriIndex3 = 0;

		modelFile >> nTriangleID;

		modelFile >> nTriIndex1;
		modelFile >> nTriIndex2;
		modelFile >> nTriIndex3;

		indices.AddInt(nTriIndex1);
		indices.AddInt(nTriIndex2);
		indices.AddInt(nTriIndex3);
	}

	modelFile.close();

	for(int i = 0; i < nNumVertices; i++)
	{
		gxColor col = colBase;

		if(dRed < 0) col.red -= rand() % (-1*dRed); else if(dRed > 0) col.red += rand() % dRed;
		if(dGreen < 0) col.green -= rand() % (-1*dGreen); else if(dGreen > 0) col.green += rand() % dGreen;
		if(dBlue < 0) col.blue -= rand() % (-1*dBlue); else if(dBlue > 0) col.blue += rand() % dBlue;

		colors.AddColor(col);
	}
}
