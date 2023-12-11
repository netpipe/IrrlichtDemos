/***********************************************************
	Valdymo nustatymai

	© 2009 A-E Software ®
	Arvydas Burdulis
	http://ae.gamedev.lt  aab@auto.lt


	//_TODO

	Pradeta:
	    2009-01-31
	Paskutini karta redaguota:
	    2009-01-31
************************************************************/


#ifndef VALD_NUST_HPP
#define VALD_NUST_HPP
#include <defines.hpp>
#include <globals.hpp>
#ifdef DEBUG
#include <variklis/logas.hpp>
#endif
#include <variklis/valdymas/klaviatura.hpp>
#include <variklis/valdymas/joystick_devs.hpp>


enum{
	SP_KAIREN,
	SP_DESINEN,
	SP_ZEMYN,
	SP_AUKSTYN,
	SP_ENTER,
	SP_DYDIS
};


class KValdNust{
	public:
	int		iSp[SP_DYDIS];
	float	fSp[SP_DYDIS];
	int		mapKlav[SP_DYDIS];
	int		mapJoy[SP_DYDIS];
	bool	sukurta;
	
	KValdNust();
	~KValdNust();
	void Sukurti();
	void Atnaujinti();
	void Naikinti();
};


#endif
