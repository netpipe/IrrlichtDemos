
// Author: Greg Santucci, 2008 and 2009
// This is part of a port of the CSG project
// originally written in java by Danilo Balby
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#ifndef INTSET_H
#define INTSET_H

#include <vector>

class IntSet
{
public:
	IntSet();
	IntSet(int nMaxSize);
	virtual ~IntSet();

	//int GetMaxSize();
	int GetSize();
	int GetInt(int i);

	void SetInt(int i, int nInt);
	void AddInt(int nInt);

	int & operator[](int index);

	int length();

	typedef std::vector<int> IntVector;

	IntVector m_pInts;

//private:
//
//	int * m_pInts;
//	int m_nMaxSize;
//	int m_nSize;
};

#endif // INTSET_H

