#pragma once
#include "Sel_Intersect.h"

class Scene;
class Ray
{
public:
	Ray(void);
	Ray(Position *,Position *,int,int);
	~Ray(void);
	Position * start;
	Position * end;
	int weigtht;
	double length;
	//bool Detect_Object();
	Position* reflectRay(Wall *,Position*);
	Sel_Intersect* Intersect(Scene*);
	void Propagate (Scene*, int);
	Position* Chk_Intersect(Wall*);
	float getDistanceToPoint(Position*, Position*);
};