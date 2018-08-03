
// Author: Greg Santucci, 2008 and 2009
// This is part of a port of the CSG project
// originally written in java by Danilo Balby
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#include "IntSet.h"

IntSet::IntSet()
{
	//m_nMaxSize = 10000;
	//m_nSize = 0;
	//m_pInts = new int[m_nMaxSize];

	//length = m_nSize;
}

IntSet::IntSet(int nMaxSize)
{
	//m_nMaxSize = nMaxSize;
	//m_nSize = 0;
	//m_pInts = new int[m_nMaxSize];

	//length = m_nSize;
}

IntSet::~IntSet()
{
	//delete [] m_pInts;
}

//int IntSet::GetMaxSize()
//{
//	return m_nMaxSize;
//}

int IntSet::GetSize()
{
	return m_pInts.size();
	//return m_nSize;
}

int IntSet::length()
{
	return m_pInts.size();
}

int IntSet::GetInt(int i)
{
	if(i < 0) return 0;
	if(i >= m_pInts.size()) return 0;

	return m_pInts[i];
}

void IntSet::SetInt(int i, int nInt)
{
	if(i < 0) return;
	if(i >= m_pInts.size()) return;

	m_pInts[i] = nInt;
}

void IntSet::AddInt(const int nInt)
{
	//if(m_nSize >= m_nMaxSize) return;

	m_pInts.push_back(nInt);

	//m_pInts[m_nSize] = nInt;
	//m_nSize++;

	//length = m_nSize;
}

int & IntSet::operator[](int index)
{
	int & pInt = m_pInts[index];

	// If its null, we're in trouble...

	return pInt;
}
