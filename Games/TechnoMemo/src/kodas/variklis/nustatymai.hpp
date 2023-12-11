/***********************************************************
	Nustatymai

	© 2009 A-E Software ®
	Arvydas Burdulis
	http://ae.gamedev.lt  aab@auto.lt


	Nustatymu ukrovimas/issaugojimas.
	Nustatymu failo formatas:
		kintamasis = reiksme
		kintamasis = reiksme1, reiksme2
	Kiekviena kintamojo-reiksmes pora yra naujoje eiluteje.
	Gali buti pateiktos ne visos eilutes, taip pat nebutinai
	eiles tvarka. Bool reiksmes - tai 0 (false) arba 1 (true).
	Jei uzkraunant ar issaugant nustatymus nenurodomas failo
	pavadinimas, naudojamas paskutini karta buves, jei ir
	jo nera - standartinis (nustatymai.cfg).

	Pradeta:
	    2009-01-17
	Paskutini karta keista:
	    2009-01-24
************************************************************/

/*
	pavad = string		- projekto pavadinimas
	kalba = string		- naudojama kalba
	gpu = string		- GPU irenginio pavadinimas
	class = string		- lango klases pavadinimas 
	poz = int, int		- lango pradine pozicija (jei Windowed)
	dydis = int, int	- lango dydis
	fps = int			- piesimo daznis
	bpp = int			- spalvu gylis
	z_bpp = int			- Z buferio spalvu gylis
	accum_bpp = int		- Accumulation buferio spalvu gylis
	stencil_bpp = int	- Stencil buferio gylis
	fullscr = bool		- ar ijungtas FullScreen?
	v-sync = bool		- ar riboti FPS?
*/


#ifndef NUSTATYMAI_HPP
#define NUSTATYMAI_HPP
#include <defines.hpp>
#ifdef DEBUG
#include <variklis/logas.hpp>
#endif
#include <iostream>
#include <fstream>
using namespace std;


class KNustatymai
{
	public:
	char	*pav;	//Nustatymu failo pavadinimas
	//Lango nustatymai
	int	pozX,		//Lango pozicija
		pozY,
		dydisX,		//Lango dydis taskais
		dydisY,
		fps,		//Piesimo daznis
		bpp,		//Spalvu gylis
		zBpp,		//Z buferio gylis
		accumBpp,	//Accum buferio gylis
		stencilBpp;	//Stencil buferio gylis
	bool	fullScr,	//Ar naudoti FullScreen?
			ribotiFps;	//Ar riboti FPS (ijungti V-Sync)?
	char	pavad[32],	//Projekto pavadinimas
			wndClass[32],	//Lango klases vardas
			*gpuStr,		//GPU pavadinimas
			kalba[3];	//Naudojama kalba
	//Vaizdo kokybe
	//_TODO
	//Garsas
	//_TODO
	//Valdymas
	//_TODO
	
	KNustatymai();
	~KNustatymai();
	void GautiPav(char*);
	void Uzkrauti(char* = NULL);
	void Issaugoti(char* = NULL);
	void Naikinti();
	#ifdef DEBUG
	void Log();
	#endif
};


#endif
