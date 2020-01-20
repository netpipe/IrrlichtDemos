#include "PlayerCar.h"

void PlayerCar::ChangeCameraMode()
	{
		isCameraLocked = !isCameraLocked;
	}
	void PlayerCar::setCameraToFollowing()
	{
		
		vector3df lookFrom;
		f32 x1 = -40;
		f32 y1 = -80;
		lookFrom.X = sceneNode->getPosition().X + x1*cos(( -rotation.Z)/ 57.295785) - y1*sin(( -rotation.Z)/ 57.295785);
		lookFrom.Z = sceneNode->getPosition().Z + x1*sin(( -rotation.Z)/ 57.295785) + y1*cos(( -rotation.Z)/ 57.295785);		
		lookFrom.Y = 30;

		smgr->getActiveCamera()->setPosition(lookFrom);
		smgr->getActiveCamera()->setTarget(sceneNode->getPosition());
	}
	void PlayerCar::PRESSED_KEY_W()
	{
		
		
		if( maxSpeed > speed)
		speed += acceleration*SLOW_COEF;//нужна более правдоподобная формула ускорения

		lastActionTime = GT();
		Update();
	}
	void PlayerCar::PRESSED_KEY_S()
	{
		
		if( speed > -10)// задняя скорость пусть будет 36 кмч
		speed -= acceleration*SLOW_COEF;

		lastActionTime = GT();
		Update();
	}
	void PlayerCar::PRESSED_KEY_A()
	{
		
		f32 S = 0.08;
		//if(collisionCoef*S*10 < speed;
		rotation.Z -= steeringCoef*SLOW_COEF*REAL_COEF;// не учитывается физика поворота

		lastActionTime = GT();
		Update();
	}
	void PlayerCar::PRESSED_KEY_D()
	{
		
		rotation.Z += steeringCoef*SLOW_COEF*REAL_COEF;
		lastActionTime = GT();
		Update();
	}
	void PlayerCar::PRESSED_KEY_SPACE()
	{
		
		if(abs(speed) < 0.1)
			speed = 0;
		else
		speed = (speed/abs(speed))*abs((abs(speed)-brakeDeceleration*SLOW_COEF));
		
	//	lastActionTime = GT();
		Update();
	}

	void PlayerCar::Update()
	{

		Car::Update();
		if(isCameraLocked)
				setCameraToFollowing();
	}