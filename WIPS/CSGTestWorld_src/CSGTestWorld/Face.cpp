
// Author: Greg Santucci, 2008 and 2009
// This is part of a port of the CSG project
// originally written in java by Danilo Balby
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#include "Face.h"

#include "Vertex.h"
#include "Bound.h"

#include "ML_Vector.h"

#include "Line.h"

#include <math.h>

int Face::UNKNOWN     = 1;
int Face::INSIDE      = 2;
int Face::OUTSIDE     = 3;
int Face::SAME        = 4;
int Face::OPPOSITE    = 5;

int Face::UP          = 6;
int Face::DOWN        = 7;
int Face::ON          = 8;
int Face::NONE        = 9;

//double Face::TOL = 1e-10f;
double Face::TOL = 1e-5f;

Face::Face()
{
	v1 = 0;
	v2 = 0;
	v3 = 0;
	
	status = UNKNOWN;
}

Face::Face(Vertex * v1i, Vertex * v2i, Vertex * v3i)
{
	v1 = v1i;
	v2 = v2i;
	v3 = v3i;
	
	status = UNKNOWN;
}

Face::~Face()
{
}

void Face::CloneFrom(Face * pFace)
{
	v1 = pFace->v1;
	v2 = pFace->v2;
	v3 = pFace->v3;
	status = pFace->status;
}

bool Face::equals_pointersmatch(Face * pFace)
{
	Face & face = *pFace;
	bool cond = v1 == face.v1 && v2 == face.v2 && v3 == face.v3;
	
	return cond;
}

bool Face::equals(Face * pFace)
{
	Face & face = *pFace;
	bool cond1 = v1->equals(face.v1) && v2->equals(face.v2) && v3->equals(face.v3);
	bool cond2 = v1->equals(face.v2) && v2->equals(face.v3) && v3->equals(face.v1);
	bool cond3 = v1->equals(face.v3) && v2->equals(face.v1) && v3->equals(face.v2);
	
	return cond1 || cond2 || cond3;  	 			
}

Bound Face::getBound()
{
	return Bound(v1->getPosition(),v2->getPosition(),v3->getPosition());
}

mlVector3D Face::getNormal()
{
	mlVector3D p1 = v1->getPosition();
	mlVector3D p2 = v2->getPosition();
	mlVector3D p3 = v3->getPosition();
	
	mlVector3D xy = p2 - p1;
	mlVector3D xz = p3 - p1;

	//xy.Normalise();
	//xz.Normalise();
	
	mlVector3D normal = mlVectorCross(xy, xz);
	normal.Normalise();
	
	return normal;
}

int Face::getStatus()
{
	return status;
}

double Face::getArea()
{
	//area = (a * c * sen(B))/2

	mlVector3D p1 = v1->getPosition();
	mlVector3D p2 = v2->getPosition();
	mlVector3D p3 = v3->getPosition();
	mlVector3D xy = p2 - p1;
	mlVector3D xz = p3 - p1;
	
	double a = xy.Magnitude();
	double c = xz.Magnitude();
	//double B = xy.angle(xz);
	double B = 0.0f;
	{
		mlVector3D xyNorm = xy.Normalised();
		mlVector3D xzNorm = xz.Normalised();
		double fDot = xyNorm * xzNorm;
		double fAngle = acos(fDot);
		B = fAngle;
	}
	
	return (a * c * sin(B))/2.0f;
}

void Face::invert()
{
	Vertex * vertexTemp = v2;
	v2 = v1;
	v1 = vertexTemp;
}

bool Face::simpleClassify()
{
	int status1 = v1->getStatus();
	int status2 = v2->getStatus();
	int status3 = v3->getStatus();
		
	if(status1==Vertex::INSIDE)
	{
		status = INSIDE;
		return true; 
	}

	if(status1==Vertex::OUTSIDE)
	{
		status = OUTSIDE;
		return true; 
	}

	if(status2==Vertex::INSIDE)
	{
		status = INSIDE;
		return true;
	}

	if(status2==Vertex::OUTSIDE)
	{
		status = OUTSIDE;
		return true;
	}

	if(status3==Vertex::INSIDE)
	{
		status = INSIDE;
		return true;
	}

	if(status3==Vertex::OUTSIDE)
	{
		status = OUTSIDE;
		return true;
	}

	return false;
}

void Face::rayTraceClassify(Object3D & object)
{
	//creating a ray starting starting at the face baricenter going to the normal direction
	mlVector3D p0;
	p0.x = (v1->x + v2->x + v3->x)/3.0f;
	p0.y = (v1->y + v2->y + v3->y)/3.0f;
	p0.z = (v1->z + v2->z + v3->z)/3.0f;
	Line ray(getNormal(),p0);
	
	bool success;
	double dotProduct, distance; 
	mlVector3D intersectionPoint;
	Face * closestFace = 0;
	double closestDistance;

	//double TOL = 0.0001f;
								
	do
	{
		success = true;
		closestDistance = 99999.9f;
		//for each face from the other solid...
		for(int i=0;i<object.getNumFaces();i++)
		{
			Face & face = *(object.getFace(i));
			dotProduct = face.getNormal() * ray.getDirection(); // dot product
			bool bIntersectResult = false;
			intersectionPoint = ray.computePlaneIntersection(face.getNormal(), face.v1->getPosition(), bIntersectResult);
			// Need to return whether was successful.
							
			//if ray intersects the plane...  
			//if(intersectionPoint!=0)
			if(bIntersectResult)
			{
				distance = ray.computePointToPointDistance(intersectionPoint);
				
				//if ray lies in plane...
				if(fabs(distance)<TOL && fabs(dotProduct)<TOL)
				{
					//disturb the ray in order to not lie into another plane 
					ray.perturbDirection();
					success = false;
					break;
				}
				
				//if ray starts in plane...
				if(fabs(distance)<TOL && fabs(dotProduct)>TOL)
				{
					//if ray intersects the face...
					if(face.hasPoint(intersectionPoint))
					{
						//faces coincide
						closestFace = &face;
						closestDistance = 0;
						break;
					}
				}
				
				//if ray intersects plane... 
				else if(fabs(dotProduct)>TOL && distance>TOL)
				{
					if(distance<closestDistance)
					{
						//if ray intersects the face;
						if(face.hasPoint(intersectionPoint))
						{
							//this face is the closest face untill now
							closestDistance = distance;
							closestFace = &face;
						}
					}
				}
			}
		}
	}while(success==false);
	
	//none face found: outside face
	if(closestFace==0)
	{
		status = OUTSIDE;
	}
	//face found: test dot product
	else
	{
		dotProduct = closestFace->getNormal() * ray.getDirection();
		
		//distance = 0: coplanar faces
		if(fabs(closestDistance)<TOL)
		{
			if(dotProduct>TOL)
			{
				status = SAME;
			}
			else if(dotProduct<-TOL)
			{
				status = OPPOSITE;
			}
		}
		
		//dot product > 0 (same direction): inside face
		else if(dotProduct>TOL)
		{
			status = INSIDE;
		}
		
		//dot product < 0 (opposite direction): outside face
		else if(dotProduct<-TOL)
		{
			status = OUTSIDE;
		}
	}
}

//------------------------------------PRIVATES----------------------------------//

/**
 * Checks if the the face contains a point
 * 
 * @param point to be tested
 * @param true if the face contains the point, false otherwise 
 */	
bool Face::hasPoint(const mlVector3D &  point)
{
	int result1, result2, result3;
	mlVector3D normal = getNormal(); 

	//if x is constant...	
	if(fabs(normal.x)>TOL) 
	{
		//tests on the x plane
		result1 = linePositionInX(point, v1->getPosition(), v2->getPosition());
		result2 = linePositionInX(point, v2->getPosition(), v3->getPosition());
		result3 = linePositionInX(point, v3->getPosition(), v1->getPosition());
	}
	
	//if y is constant...
	else if(fabs(normal.y)>TOL)
	{
		//tests on the y plane
		result1 = linePositionInY(point, v1->getPosition(), v2->getPosition());
		result2 = linePositionInY(point, v2->getPosition(), v3->getPosition());
		result3 = linePositionInY(point, v3->getPosition(), v1->getPosition());
	}
	else
	{
		//tests on the z plane
		result1 = linePositionInZ(point, v1->getPosition(), v2->getPosition());
		result2 = linePositionInZ(point, v2->getPosition(), v3->getPosition());
		result3 = linePositionInZ(point, v3->getPosition(), v1->getPosition());
	}
	
	//if the point is up and down two lines...		
	if(((result1==UP)||(result2==UP)||(result3==UP))&&((result1==DOWN)||(result2==DOWN)||(result3==DOWN)))
	{
		return true;
	}
	//if the point is on of the lines...
	else if ((result1==ON)||(result2==ON)||(result3==ON))
	{
		return true;
	}
	else
	{
		return false;
	}
}

/** 
 * Gets the position of a point relative to a line in the x plane
 * 
 * @param point point to be tested
 * @param pointLine1 one of the line ends
 * @param pointLine2 one of the line ends
 * @return position of the point relative to the line - UP, DOWN, ON, NONE 
 */
int Face::linePositionInX(const mlVector3D & point, const mlVector3D & pointLine1, const mlVector3D & pointLine2)
{
	double a, b, z;
	if((fabs(pointLine1.y-pointLine2.y)>TOL)&&(((point.y>=pointLine1.y)&&(point.y<=pointLine2.y))||((point.y<=pointLine1.y)&&(point.y>=pointLine2.y))))
	{
		a = (pointLine2.z-pointLine1.z)/(pointLine2.y-pointLine1.y);
		b = pointLine1.z - a*pointLine1.y;
		z = a*point.y + b;
		if(z>point.z+TOL)
		{
			return UP;			
		}
		else if(z<point.z-TOL)
		{
			return DOWN;
		}
		else
		{
			return ON;
		}
	}
	else
	{
		return NONE;
	}
}

/** 
 * Gets the position of a point relative to a line in the y plane
 * 
 * @param point point to be tested
 * @param pointLine1 one of the line ends
 * @param pointLine2 one of the line ends
 * @return position of the point relative to the line - UP, DOWN, ON, NONE 
 */

int Face::linePositionInY(const mlVector3D & point, const mlVector3D & pointLine1, const mlVector3D & pointLine2)
{
	double a, b, z;
	if((fabs(pointLine1.x-pointLine2.x)>TOL)&&(((point.x>=pointLine1.x)&&(point.x<=pointLine2.x))||((point.x<=pointLine1.x)&&(point.x>=pointLine2.x))))
	{
		a = (pointLine2.z-pointLine1.z)/(pointLine2.x-pointLine1.x);
		b = pointLine1.z - a*pointLine1.x;
		z = a*point.x + b;
		if(z>point.z+TOL)
		{
			return UP;			
		}
		else if(z<point.z-TOL)
		{
			return DOWN;
		}
		else
		{
			return ON;
		}
	}
	else
	{
		return NONE;
	}
}

/** 
 * Gets the position of a point relative to a line in the z plane
 * 
 * @param point point to be tested
 * @param pointLine1 one of the line ends
 * @param pointLine2 one of the line ends
 * @return position of the point relative to the line - UP, DOWN, ON, NONE 
 */

int Face::linePositionInZ(const mlVector3D & point, const mlVector3D & pointLine1, const mlVector3D & pointLine2)
{
	double a, b, y;
	if((fabs(pointLine1.x-pointLine2.x)>TOL)&&(((point.x>=pointLine1.x)&&(point.x<=pointLine2.x))||((point.x<=pointLine1.x)&&(point.x>=pointLine2.x))))
	{
		a = (pointLine2.y-pointLine1.y)/(pointLine2.x-pointLine1.x);
		b = pointLine1.y - a*pointLine1.x;
		y = a*point.x + b;
		if(y>point.y+TOL)
		{
			return UP;			
		}
		else if(y<point.y-TOL)
		{
			return DOWN;
		}
		else
		{
			return ON;
		}
	}
	else
	{
		return NONE;
	}
}
