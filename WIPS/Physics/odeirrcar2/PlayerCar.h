#pragma once
#include "Car.h"


class PlayerCar: public Car
{									// физика машины совсем не соответствует правде и задается програмно (например отсутствует сила тяги двигателя и трения воздуха)
									// по этой же причине машина может стоя крутится на месте
private:
	
	
	bool isCameraLocked;	

	
public:
	
	PlayerCar(IrrlichtDevice* irrDevice,CIrrOdeWorld *world, const irr::io::path & modelFileName = "" ,const irr::io::path & textureFileName = "", MyEventReceiver* rec = 0):Car(irrDevice,world, modelFileName,textureFileName,rec)
	{
		isCameraLocked = 0; // выставляет изначальное положение камеры - привязана к машине или нет
		//sceneNode->setPosition(vector3df(2155,220,600));
	};

	


	
	
	void setCameraToFollowing();	//выставляет камеру сцены на камеру следования за машиной
	void ChangeCameraMode();	// изменяет камеру на привязанную к машине или нет

	void PRESSED_KEY_W();			//реакция машины на нажатие пользователем клавиш ( газ, тормоз, поворот)
	void PRESSED_KEY_S();
	void PRESSED_KEY_A();	
	void PRESSED_KEY_D();
	void PRESSED_KEY_SPACE();
	
	void Update();
	
	
};

