/***********************************************************
	Joystick Devices

	© 2009 A-E Software ®
	Arvydas Burdulis
	http://ae.gamedev.lt  aab@auto.lt


	Joystick irenginiu valdymas

	Pradeta:
	    2009-01-10
	Paskutini karta keista:
	    2009-01-10
************************************************************/


#ifndef JOYSTICK_DEVS_HPP
#define JOYSTICK_DEVS_HPP
#include <defines.hpp>
#ifdef DEBUG
#include <variklis/logas.hpp>
#endif
#include <windows.h>
#include "joystick.hpp"


class JoystickDevs
{
	public:
	static int	sk;			//Joystick irenginiu skaicius
	static bool sukurta;	//Ar sukurta?
	static KJoystick *devs;	//Joystick irenginiai
	
	JoystickDevs();
	~JoystickDevs();
	static void Sukurti();
	static void Kalibruoti();
	static KJoystick & Gauti(int);
	static KJoystick & Gauti(const char*);
	static void Atnaujinti();
	static void Naikinti();
};


#endif
