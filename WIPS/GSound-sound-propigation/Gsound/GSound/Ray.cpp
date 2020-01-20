#include "Ray.h"
#include"Scene.h"
#include"MyMath.h"
Ray::Ray(void)
{
}

Ray::Ray(Position* start,Position* end,int weigtht,int length)
{
	this->start = start;
	this->end = end;
	this->weigtht = weigtht;
	this->length = length;
}


Sel_Intersect * Ray::Intersect(Scene * scene)
{
	bool first = true;
	//Sel_Intersect  * Vec_Intersect [4];
	Sel_Intersect  * detectIntersect = NULL;
	for (int i = 0; i < 6; i++)
	{
		Position * position = Chk_Intersect(scene->walls[i]);
		if(position != NULL  && ( position->x > max(scene->walls[i]->start->x,scene->walls[i]->end->x) || position->x < min(scene->walls[i]->start->x,scene->walls[i]->end->x) ) )
			continue;
		else if(scene->walls[i]->start->x == scene->walls[i]->end->x)
				if(position != NULL  && ( position->y > max(scene->walls[i]->start->y,scene->walls[i]->end->y) || position->y < min(scene->walls[i]->start->y,scene->walls[i]->end->y) ) )
					continue;
		if (position != NULL ){
			double distance = this->start->distanceTo(position);
			if(first){
				detectIntersect = new Sel_Intersect(scene->walls[i],position,distance);
				first = !first;
			}
			else {				
				if( distance < detectIntersect->distance){
					detectIntersect->position = position ;
					detectIntersect->distance = distance;
					detectIntersect->wall = scene->walls[i];
				}

			}
		}
	}
	return detectIntersect;
}

Position* Ray:: Chk_Intersect(Wall * wall){
	Position* intersetionPoint = NULL;
	double m1 = MyMath::calculateM(this->start->x,this->start->y,this->end->x,this->end->y);
	double m2 = MyMath::calculateM(wall->start->x,wall->start->y,wall->end->x,wall->end->y);
	float x = 0 , y = 0;
	if( m1 == m2 )
		return NULL;
	else if (abs(m1) == std::numeric_limits<float>::infinity())
	{
		x = this->start->x ;
		y = (m2 * x) + (wall->start->y) -(m2*wall->start->x);
	}
	else if (abs(m2) == std::numeric_limits<float>::infinity())
	{
		x = wall->start->x ;
		y = (m1 * x) + (this->start->y) -(m1*this->start->x);
	}
	else 
		MyMath::commonSolution(m1,-1,this->start->y-(m1*this->start->x),m2,-1,wall->start->y-(m2*wall->start->x),x,y);
	if(x <= max(wall->start->x,wall->end->x) && x >= min(wall->start->x,wall->end->x) )
	{
		if (this->end->x > this->start->x && x > this->start->x )
			return new Position(x,y,0);
		else if (this->end->x < this->start->x && x < this->start->x )
			return new Position(x,y,0);
		else if (this->end->y > this->start->y && y > this->start->y )
			return new Position(x,y,0);
		else if (this->end->y < this->start->y && y < this->start->y )
			return new Position(x,y,0);
		else
			return NULL;
	}
	else
		return NULL;
}

Position* Ray::reflectRay(Wall* wall,Position* position)
{
	Vector2f L(this->end->x - this->start->x, this->end->y - this->start->y);
	Vector2f N(wall->end->x - wall->start->x, wall->end->y - wall->start->y);
	Vector2f C = L.projectOn(N);
	Vector2f R = (float)2*C - L ; 
	return new Position(R.x + position->x, R.y + position->y, 0);
}

void Ray::Propagate (Scene* scene,int depth){
	if ((depth<3)&&(this->weigtht > 0)){
		Sel_Intersect * sel_intersect = this->Intersect(scene);		
		Position * endPoint = reflectRay(sel_intersect->wall,sel_intersect->position);
		this->end = sel_intersect->position ;
		this->length += (new Vector2f(this->end->x - this->start->x , this->end->y - this->start->y))->getMagnitude();
		Ray * ray = new Ray ( new Position(sel_intersect->position),endPoint,this->weigtht-sel_intersect->wall->attenuation,this->length);		
		scene->rays[scene->i] = ray;
		scene->i++;
		scene->rays[scene->i-1]->Propagate(scene,++depth);
	}
	else 
	{
		Sel_Intersect * sel_intersect = this->Intersect(scene);
		this->end = sel_intersect->position;
		//this->length += (new Vector2f(this->end->x - this->start->x , this->end->y - this->start->y))->getMagnitude();
	}
}

float Ray::getDistanceToPoint(Position* listnerPos, Position* Projected )
{
	Vector2f thisVec(this->end->x - this->start->x,this->end->y - this->start->y);
	Vector2f listnerPosVec(listnerPos->x - this->start->x,listnerPos->y - this->start->y);
	Vector2f projectedPoint = listnerPosVec.projectOn(thisVec);
	Projected->x = projectedPoint.x;
	Projected->y = projectedPoint.y;
	return projectedPoint.getDistanceTo(listnerPosVec);
}

Ray::~Ray(void)
{
}
