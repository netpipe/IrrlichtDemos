// Copyright (C) 2007 Kevin van Leeuwen
// This file is part of the "LeeuwenGfx" Library.
// For conditions of distribution and use, see copyright notice in LeeuwenGfx.h

#ifndef _VectorGfxHeader_
#define _VectorGfxHeader_

#include <iostream>
#include <cstdlib>
#include <cmath>

namespace lew
{
	namespace gfx
	{
		//! the constant PI
		const float PI = 3.1415926535897932384626433832795f;
		
		//! convert degrees to radiants
		const float  DEG2RAD = PI/180.0f;
		
		//! convert radiants to degrees
		const float  RAD2DEG = 180.0f*PI;
		
		//! A general 2d-vector class...
		/** This class provides operators and math functions.

		\code
			Vector v1 = Vector(2, 2);
			Vector v2 = v2*2; //will be Vector(4, 4)
			
			v1 = v2.squared(); //will be Vector(16, 16)
			
			Vector v3 = v2.difference(v1); //will be Vector(14, 14)
			
			v2 += (v1+v3); //will be Vector(34, 34)
		\endcode
		*/	
		class Vector
		{
			public:	
				//! constructor
				//! \param p1: x value.
				//! \param p2: y value.
				Vector(const float p1 = 0.0f, const float p2 = 0.0f)
				{
					x = p1;
					y = p2;
				}

				Vector operator=(const Vector New)
				{x = New.x; y = New.y; return *this;}

				Vector operator=(const float Value)
				{x = Value; y = Value; return *this;}

				Vector operator+(const Vector New) const
				{return Vector(x+New.x, y+New.y);}

				Vector operator+(const float Value) const
				{return Vector(x+Value, y+Value);}

				Vector operator+=(const Vector New)
				{return Vector(x+=New.x, y+=New.y);}

				Vector operator+=(const float Value) 
				{return Vector(x+=Value, y+=Value);}
		 
				Vector operator-(const Vector New) const
				{return Vector(x-New.x, y-New.y);}

				Vector operator-(const float Value) const
				{return Vector(x-Value, y-Value);}

				Vector operator-=(const Vector New)
				{return Vector(x-=New.x, y-=New.y);}

				Vector operator-=(const float Value)
				{return Vector(x-=Value, y-=Value);}

				Vector operator*(const Vector New) const
				{return Vector(x*New.x, y*New.y);}

				Vector operator*(const float Value) const
				{return Vector(x*Value, y*Value);}

				Vector operator*=(const Vector New)
				{return Vector(x*=New.x, y*=New.y);}

				Vector operator*=(const float Value)
				{return Vector(x*=Value, y*=Value);}

				Vector operator/(const Vector New)  const
				{return Vector(x/New.x, y/New.y);}

				Vector operator/(const float Value)  const
				{return Vector(x/Value, y/Value);}

				Vector operator/=(const Vector New)
				{return Vector(x/=New.x, y/=New.y);}

				Vector operator/=(const float Value)
				{return Vector(x/=Value, y/=Value);}

				bool operator<(const Vector New)  const
				{return x<New.x && y<New.y;}

				bool operator<(const float Value)  const
				{return x<Value && y<Value;}

				bool operator<=(const Vector New)  const
				{return x<=New.x && y<=New.y;}

				bool operator<=(const float Value)  const
				{return x<=Value && y<=Value;}
				
				bool operator>(const Vector New)  const
				{return x>New.x && y>New.y;}

				bool operator>(const float Value)  const
				{return x>Value && y>Value;}

				bool operator>=(const Vector New)  const
				{return x>=New.x && y>=New.y;}

				bool operator>=(const float Value)  const
				{return x>=Value && y>=Value;}

				bool operator==(const Vector New)  const
				{return x==New.x && y==New.y;}

				bool operator==(const float Value)  const
				{return x==Value && y==Value;}

				bool operator!=(const Vector New)  const
				{return x!=New.x || y!=New.y;}

				bool operator!=(const float Value)  const
				{return x!=Value || y!=Value;}

			
				//! clears x and y to 0.
				void clear()
				{
					x = y = 0.0f;
				}
				
				//! limits x and y to a specific value.
				//! \param pMin: the min. value of x and y.
				//! \param pMax: the max. value of x and y.	
				void clamp(const float pMin, const float pMax)
				{
					if(x < pMin)
						x = pMin;
					
					if(x > pMax)
						x = pMax;
						
					if(y < pMin)
						y = pMin;
						
					if(y > pMax)
						y = pMax;
				}
				
				//! limits x and y to a specific value.
				//! \param pMin: the min. value of x and y.
				//! \param pMax: the max. value of x and y.				
				void clamp(const Vector pMin, const Vector pMax)
				{
					if(x < pMin.x)
						x = pMin.x;
					
					if(x > pMax.x)
						x = pMax.x;
						
					if(y < pMin.y)
						y = pMin.y;
						
					if(y > pMax.y)
						y = pMax.y;
				}
				
				//! round x and y down to a specific value.
				//! e.g. round the value 400.0 to 360.0 -> 40.0.
				//! \param pValue: the value x and y are rounded to.
				void round(const float pValue)
				{
					while(x > pValue)
						x -= pValue;
				
					while(y > pValue)
						y -= pValue;
				}
	
				//! round x and y down to a specific value.
				//! e.g. round the value 400.0 to 360.0 -> 40.0.
				//! \param pValue: the value x and y are rounded to.			
				void round(const Vector pValue)
				{
					while(x > pValue.x)
						x -= pValue.x;
				
					while(y > pValue.y)
						y -= pValue.y;		
				}
				
				//! calculates the distance from x and y to another Vector.
				//! \param Point: the Vector of which the distance is calculated from.
				//! \param pSquared: if true, the "real" distance will be returned.
				//! otherwise a squared distance will be returned, which is
				//! a little bit faster because no sqrt() call has to be made.
				//! \return the distance from x and y to a point.
				float distance(const Vector Point, const bool pSquared = false) const
				{
					const float nX = x - Point.x;
					const float nY = y - Point.y;

					if(!pSquared)
						return (nX*nX + nY*nY);

					return sqrt(nX*nX + nY*nY);
				}

				//! inverts x and y.
				Vector inverted() const
				{
					return Vector(x*-1.0f, y*-1.0f);
				}
				
				//! squares x and y.
				Vector squared() const
				{
					return Vector(x*x, y*y);	
				}
				
				//! normalizes the vector to 0-1
				Vector normalized()
				{
					float X = x;
					float Y = y;
					float L = lenght();
					
					if(L > 0)
					{
						X *= 1.0f/L;
						Y *= 1.0f/L;
					}
					
					return Vector(X, Y);
				}

				//! calculates the difference between x and y and another Vector.
				//! \param Point: the Vector of which the difference is calculated from.
				//! \return the difference of x and y to the other Vector.
				Vector difference(const Vector Point) const
				{
					return Vector(fabs(x-Point.x), fabs(y-Point.y));
				}

				//! \return the lenght of this vector.
				float lenght() const
				{
					return sqrt(y*y + x*x);
				}
			
				//! \return the sum of x and y.
				float sum() const
				{
					return (x+y);
				}
				
				//! \return the product of x and y.
				float product() const
				{
					return (x*y);
				}

				float x, y;
		};
	};
};

#endif

