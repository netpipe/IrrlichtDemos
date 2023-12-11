
// Author: Greg Santucci, 2008 and 2009
// This is part of a port of the CSG project
// originally written in java by Danilo Balby
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#include "ColorSet.h"

#include "GX_Color.h"

ColorSet::ColorSet()
{
	//m_nMaxSize = 10000;
	//m_nSize = 0;
	//m_pColors = new gxColor[m_nMaxSize];

	//length = m_nSize;
}

ColorSet::ColorSet(int nMaxSize)
{
	//m_nMaxSize = nMaxSize;
	//m_nSize = 0;
	//m_pColors = new gxColor[m_nMaxSize];

	//length = m_nSize;
}

ColorSet::~ColorSet()
{
	//delete [] m_pColors;
}

//int ColorSet::GetMaxSize()
//{
//	return m_nMaxSize;
//}

int ColorSet::GetSize()
{
	//return m_nSize;
	return m_pColors.size();
}

int ColorSet::length()
{
	return m_pColors.size();
}

gxColor ColorSet::GetColor(int i)
{
	if(i < 0) return gxColor();
	if(i >= m_pColors.size()) return gxColor();

	return m_pColors[i];
}

void ColorSet::SetColor(int i, const gxColor & vColor)
{
	if(i < 0) return;
	if(i >= m_pColors.size()) return;

	m_pColors[i] = vColor;
}

void ColorSet::AddColor(const gxColor & vColor)
{
	m_pColors.push_back(vColor);

	//if(m_nSize >= m_nMaxSize) return;

	//m_pColors[m_nSize] = vColor;
	//m_nSize++;
	//length = m_nSize;
}

gxColor & ColorSet::operator[](int index)
{
	gxColor & pColor = m_pColors[index];

	return pColor;
}
