/***********************************************************
	Pele

	© 2009 A-E Software ®
	Arvydas Burdulis
	http://ae.gamedev.lt  aab@auto.lt


	Peles valdymas. Peles pozicija isreiskiama lango
	koordinatemis ir poslinkiu.
	Darbo pradzioje reikia nustatyti "lang*" reiksmes.
	Jei dirbama Windowed rezimu, sias reiksmes reikia
	atnaujinti po kiekvieno lango dydzio/pozicijos
	pakeitimo.

	Pradeta:
	    2008-04-13
	Paskutini karta redaguota:
	    2009-01-29
************************************************************/


#ifndef PELE_HPP
#define PELE_HPP
#include <defines.hpp>
#ifdef DEBUG
#include <variklis/logas.hpp>
#endif
#include <variklis/nustatymai.hpp>
#ifdef OS_WIN32
#include <windows.h>
#endif
#include <irrlicht.h>

class KPele{
	private:
	bool	naujRat,	//Ar pasuktas ratukas?
			naujKair,
			naujDes,
			naujVid;
	irr::core::vector3df	poz;

	public:
	int		dydisX,		//Lango ilgis
			dydisY,		//Lango plotis
			pozXC,		//Lango centro X koordinate
			pozYC,		//Lango centro Y koordinate
			x,			//X pozicija
			y,			//Y pozicija
			xDiff,		//X pozicijos poslinkis per paskutini kadra
			yDiff,		//Y pozicijos poslinkis
			rat,		//Ratukas
			ratDiff,	//Ratuko poslinkis
			spKair,		//Kairio spauduko busena (0 - nepaspaustas, 1 - pirma karta atleistas, 2 - pirma karta paspaustas, 3 - paspaustas)
			spDes,		//Desinio spauduko busena
			spVid;		//Vidurinio spauduko busena
	bool	ij;			//Ar ijungta pele?
	//float	aktyvParam[4],	//Peles aktyvumo laiko parametrai - minimali ir maksimali ribos, isijungimo ir issijungimo laikai
	//		aktyvLaik;	//Peles aktyvumo laikas
	//bool	aktyvi;		//Ar pele aktyvi?

	KPele();
	~KPele();
	void Params(KNustatymai &);
	void Ratukas(int);	//Ratuko pasukimo apdorojimas
	void Kair(bool);	//Kairio spauduko paspaudimas (true - paspaustas, false - atleistas)
	void Des(bool);		//Desinio spauduko paspaudimas
	void Vid(bool);		//Vidurinio spauduko paspaudimas
	void NustPoz(int, int);	//Nustatoma peles zymeklio pozicija
	void Atnaujinti();
};


#endif
