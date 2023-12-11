#pragma once
#include"Position.h"
class MyMath
{
public:
	inline  static void  commonSolution(float a1,float b1,float c1,float a2 , float b2,float c2,float& x,float& y){
		if ((MyMath::calculateM(a1,b1))==(MyMath::calculateM(a2,b2))){
			cout<<"error ! same m :3 ";
		}
		else		
		{
			/*x = (c*e-b*f)/(a*e-b*d);
			y = (a*f-c*d)/(a*e-b*d); */	
			float Ta1 =a1;
			float Tc1 = c1;
				a1= a1* -b2 ;
				c1= c1 * -b2;
				a2 = a2 * b1;
				c2 = c2 * b1 ;
				x = (-(c1+c2))/(a1+a2);
				y = ((-(Ta1*x))+(-Tc1))/b1;
		}
}
inline  static double  calculateM(double x1, double y1, double x2, double y2)
{
		return  (y1-y2)/double(x1-x2);
}

inline  static double  calculateM(float a, float b ) 
{
	return  -b/a;
}
};