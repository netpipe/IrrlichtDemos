
// Author: Greg Santucci, 2008 and 2009
// This is part of a port of the CSG project
// originally written in java by Danilo Balby
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#ifndef VECTORSET_H
#define VECTORSET_H

#include <vector>

#include "ML_Vector.h"

class VectorSet
{
public:
	VectorSet();
	VectorSet(int nMaxSize);
	virtual ~VectorSet();

	//int GetMaxSize();
	int GetSize();
	mlVector3D GetVector(int i);

	void SetVector(int i, const mlVector3D & vVector);
	void AddVector(const mlVector3D & vVector);

	void RemoveVector(int i);

	mlVector3D & operator[](int index);

	int length();

	typedef std::vector<mlVector3D> VectorVector;

	VectorVector m_pVectors;

//private:
//
//	mlVector3D * m_pVectors;
//	int m_nMaxSize;
//	int m_nSize;
};

#endif // VECTORSET_H

