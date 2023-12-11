#include "joystick_devs.hpp"


int JoystickDevs::sk = 0;
bool JoystickDevs::sukurta = false;
KJoystick * JoystickDevs::devs = NULL;


JoystickDevs::JoystickDevs()
{
}


JoystickDevs::~JoystickDevs()
{
}


void JoystickDevs::Sukurti()
{
	JOYINFO inf;
	if(sukurta)
		Naikinti();
	int maks = joyGetNumDevs();
	for(int i = 0; i < maks; i++){
		if(joyGetPos(i, &inf) == JOYERR_NOERROR)
			++sk;
		else
			break;
	}
	
	if(sk > 0){
		devs = new KJoystick[sk];
		for(int i = 0; i < sk; i++)
			devs[i].Sukurti(i);
	}
	sukurta = true;
}


void JoystickDevs::Kalibruoti()
{
	for(int i = 0; i < sk; i++){
		devs[i].Atnaujinti();
		devs[i].Kalibruoti();
	}
}


KJoystick & JoystickDevs::Gauti(int n)
{
	if(n > 0 && n < sk)
		return devs[n];
	else
		return devs[0];
}


KJoystick & JoystickDevs::Gauti(const char *pav)
{
	//_TODO: gaunamas JoyStick'as pagal kazkoki (?) parametra
	//if(n > 0 && n < num)
	//	return devs[n];
	//else
		return devs[0];
}


void JoystickDevs::Atnaujinti()
{
	for(int i = 0; i < sk; i++)
		devs[i].Atnaujinti();
}


void JoystickDevs::Naikinti()
{
	if(!sukurta)
		return;
	for(int i = 0; i < sk; i++)
		devs[i].Naikinti();
	delete[] devs;
	devs = NULL;
	sk = 0;
	sukurta = false;
}
