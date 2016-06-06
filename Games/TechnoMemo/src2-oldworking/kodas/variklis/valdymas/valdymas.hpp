/***********************************************************
	Valdymas

	© 2009 A-E Software ®
	Arvydas Burdulis
	http://ae.gamedev.lt  aab@auto.lt


	Valdymas - klaviaturos, peles, JoyStick'o klasiu
	apjungimas. Sukuriama vienas ar daugiau valdymo
	rinkiniu (vienas rinkinys atitinka viena zaideja).
	//_TODO

	Pradeta:
	    2009-01-24
	Paskutini karta redaguota:
	    2009-01-24
************************************************************/


#ifndef VALDYMAS_HPP
#define VALDYMAS_HPP
#include <defines.hpp>
#include <globals.hpp>
#ifdef DEBUG
#include <variklis/logas.hpp>
#endif
#include <variklis/valdymas/joystick_devs.hpp>
#include <windows.h>


enum{
	SP_KAIREN,
	SP_DESINEN,
	SP_ZEMYN,
	SP_AUKSTYN,
	SP_ENTER,
	SP_DYDIS
};


class KValdymas{
	public:
	int		iSp[SP_DYDIS];
	float	fSp[SP_DYDIS];
	int		mapKlav[SP_DYDIS];
	int		mapJoy[SP_DYDIS];
	bool	sukurta;
	
	KValdymas();
	~KValdymas();
	void Sukurti();
	void Atnaujinti();
	void Naikinti();
};


#endif
