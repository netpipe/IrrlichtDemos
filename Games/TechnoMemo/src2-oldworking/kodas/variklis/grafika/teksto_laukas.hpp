/***********************************************************
	Teksto laukas

	© 2008 A-E Software ®
	Arvydas Burdulis
	http://ae.gamedev.lt  aab@auto.lt


	Teksto isvedimo laukas.

	Pradeta:
	    2008-06-22
	Paskutini karta redaguota:
	    2008-06-23
************************************************************/


#ifndef TEKSTO_LAUKAS_HPP
#define TEKSTO_LAUKAS_HPP
#include <defines.hpp>
#ifdef OS_UNIX
#include <X11/Xlib.h>
#include <GL/glx.h>
#endif


class KTekstoLaukas
{
	public:
	int		x,			//Pozicija
			y;
	int		dydisX,		//Dydis (eilutes ilgis simboliais)
			dydisY;		//Dydis (aukstis eilutemis)
	float	spalva[4];	//Teksto spalva
	float	trukme;		//Teksto eilutes gyvavimo trukme
	float	alfaKoef;	//Nykimo greicio koeficientas
			
	char	**eil;		//Rodykles i kiekviena eilute
	float	*alfa;		//Kiekvienos eilutes alfa
	bool	ij;			//Ar ijungtas teksto laukas?
	
	KTekstoLaukas();
	~KTekstoLaukas();
	void Sukurti();				//Sukurti standartini teksto lauka
	void Sukurti(int, int);		//Sukurti pasirinkto dydzio teksto lauka
	void Prideti(const char*);	//Pridedama eilute(s)
	void Atnaujinti();			//Atnaujinamos alfa reiksmes
	void Piesti();				//Piesiamas tekstas
	void Valyti();				//Valomas tekstas
	void Naikinti();			//Naikinamas
};


#endif
