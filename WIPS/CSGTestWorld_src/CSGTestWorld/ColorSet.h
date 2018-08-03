
// Author: Greg Santucci, 2008 and 2009
// This is part of a port of the CSG project
// originally written in java by Danilo Balby
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#ifndef COLORSET_H
#define COLORSET_H

#include <vector>

class gxColor;

class ColorSet
{
public:
	ColorSet();
	ColorSet(int nMaxSize);
	virtual ~ColorSet();

	//int GetMaxSize();
	int GetSize();
	gxColor GetColor(int i);

	void SetColor(int i, const gxColor & vColor);
	void AddColor(const gxColor & vColor);

	gxColor & operator[](int index);

	int length();

	typedef std::vector<gxColor> ColorVector;

	ColorVector m_pColors;

//private:
//
//	gxColor * m_pColors;
//	int m_nMaxSize;
//	int m_nSize;
};

#endif // COLORSET_H

