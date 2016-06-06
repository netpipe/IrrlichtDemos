#include <irrlicht.h>

//Author : Abraxas)

//HOWTOUSE
//#include "Circle.h"
//cCircle circle = createCircle(core::vector3df(0,0,0),20,40,core::vector3df(0,1,0));
//drawCircle(circle, driver);
//HOWTOUSE

using namespace irr;
using namespace core;

class cCircle
{
public:
   core::vector3df vertex[10000];
   core::vector3df normal;
   core::vector3df center;
   double radius;
   int vertcount;
   cCircle();
};

cCircle::cCircle()
{
   vertcount = -1;
}


cCircle createCircle(core::vector3df center, double radius, double vertexcount, core::vector3df normal)
{

cCircle temp; //our temporary cCircle holder
int x=0;

   // We want the vertex count to reflect a polygon
if (vertexcount < 3) vertexcount = 3;
if (vertexcount > 10000) vertexcount = 10000;

normal.normalize();

double pi = 3.14159265;

   //Here we find the intersection of a plane whos normal is our normal vector,
   //with a sphere with radius 'radius'.

   //The equation of this circle is P + sV1 + tV2
   //where v1 is (0, c, −b) and v2 is (c, 0, −a), we need at least one nonzero one.
vector3df v1 = vector3df(0,normal.Z,-normal.Y);
vector3df v2 = vector3df(normal.Z,0,-normal.X);

   //scale vector to unit length (dividing by (1 − x2)1/2) where x is the 0 component of the vector, in the normal.
if (v1 != vector3df(0,0,0))
{   v1 = v1/ sqrt(1-(normal.X*normal.X));   v2 = v1.crossProduct(normal);   }
else //in the case that v1 = (0,0,0)
{v2 = v2/ sqrt(1-(normal.Y*normal.Y));   v1 = v2.crossProduct(normal);   }

for(x;x<=vertexcount;x++)
{

   //Now setting:   t = Rcos(angle)
   //            s = Rsin(angle)

   //Where R = sqrt(radius2 - d2)
   //Since we have the midpoint, we consider d = 0, R = r

   temp.vertex[x] = center      +   radius*cos(2*pi/vertexcount*x)*v1   +   radius*sin(2*pi/vertexcount*x)*v2;

   temp.vertcount++;

}




temp.vertex[temp.vertcount + 1] = temp.vertex[0];
temp.radius = radius;
temp.center = center;
temp.normal = normal;

return temp;
}

void drawCircle(cCircle circle, video::IVideoDriver* driver)
{
int x=0;

for (x;x<=circle.vertcount;x++)
driver->draw3DLine(circle.vertex[x],circle.vertex[x+1],video::SColor(255,255,255,255));


}

void drawCircleNormal(cCircle circle, video::IVideoDriver* driver)
{

driver->draw3DLine(circle.center, circle.center + (circle.normal*10) );

}
