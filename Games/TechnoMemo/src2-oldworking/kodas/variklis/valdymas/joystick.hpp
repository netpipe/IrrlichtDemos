/***********************************************************
	Joystick

	© 2009 A-E Software ®
	Arvydas Burdulis
	http://ae.gamedev.lt  aab@auto.lt


	Joystick valdymas

	Pradeta:
	    2009-01-10
	Paskutini karta keista:
	    2009-01-10
************************************************************/


#ifndef JOYSTICK_HPP
#define JOYSTICK_HPP
#include <defines.hpp>
#ifdef DEBUG
#include <variklis/logas.hpp>
#endif
#include <windows.h>


class KJoystick
{
	public:
	int		num;		//Joystick irenginio numeris
	bool	sukurta;	//Ar sukurtas?
	//Informacija
	JOYCAPS	inf;		//Irenginio informacija
	char	*infPav,	//Irenginio pavadinimas
			*infReg,	//Irenginio irasas registre
			*infDrv;	//Irenginio tvarkykle
	int		infSpSk;	//Spauduku skaicius
	int		infAsiuSk;	//Asiu skaicius
	//Parametrai
	JOYINFOEX	params;	//Irenginio busena (gauta is Windows)
	int		asiuRibos[8][3];	//Kiekvienos asies maziausia, didziausia ir vidutine reiksmes
	char	sp[32];		//Spauduku busenos
	float	asis[8];	//Asiu busenos (nuo -1.0 iki 1.0)
	
	KJoystick();
	~KJoystick();
	void Sukurti(int);
	void Kalibruoti();
	void Atnaujinti();
	void Naikinti();
};


#endif
