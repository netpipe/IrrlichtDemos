#pragma once
#include "Car.h"
#include <vector>
class NPCCar:public Car
{
	vector <vector3df> moveWay;
	vector <vector3df>::iterator iter;
	vector3df aim;
	f32 lt;
public:

	NPCCar(IrrlichtDevice* irrDevice,CIrrOdeWorld *world, vector <vector3df> way, const irr::io::path & modelFileName = "" ,const irr::io::path & textureFileName = "", MyEventReceiver* rec = 0)
		:Car(irrDevice,world, modelFileName,textureFileName,rec)
	{
		moveWay = way;		
	
		iter = moveWay.begin();
		
		sceneNode->setPosition(*iter);
		
		iter++;
		
		speed = 25;
		lt = GT();
	}
	virtual void Update()
	{
		

	
	
			aim = *(iter) - sceneNode->getPosition();
			
			f32 LG = aim.getLength();
				if(LG == 0)
					aim.Z += 0.01;			// чтобы избежать деления на нуль
				
		if(aim.getLength() < 3)
		{
			if(++iter == moveWay.end())
			iter = moveWay.begin();
		}
		
			
		int s_Z = aim.Z/abs(aim.Z);int s_X = aim.X/abs(aim.X);
		f32 ang;
		if(s_Z > 0)
			ang = 0;
		else
			ang = -180;
	
		rotation.Z =  s_Z*s_X*acos(abs(aim.Z)/LG)*57.295785 + ang;

	

		Car::Update();
		
		

	}

};