/***********************************************************
	Kamera

	© 2009 A-E Software ®
	Arvydas Burdulis
	http://ae.gamedev.lt  aab@auto.lt


	Kameros valdymas.

	Pradeta:
	    2007-06-13
	Paskutini karta redaguota:
	    2009-01-29
************************************************************/


#ifndef KAMERA_HPP
#define KAMERA_HPP
#include <defines.hpp>
#include <globals.hpp>
#include <variklis/matke/taskas.hpp>


class KKamera{
	public:
	KTaskas	k,		//Kameros pozicija
			t,		//Kameros taikinys
			up;		//UP kryptis
	float	kh,		//Kameros horizontalus kampas (nuo -PI iki +PI)
			kv,		//Kameros vertikalus kampas
			fov,	//Matymo kampas (Field of View)
			ratio,	//Ilgio ir aukscio santykis
			arti,	//Artimoji nukirpimo plokstuma
			toli;	//Tolimoji nukirpimo plokstuma
	
	float	gr,		//Kameros skridimo greitis
			gr2,	//Naujas greitis
			grInert;//Greicio keitimosi is gr i gr2 inertiskumas
	KTaskas	k2,		//Pozicija, i kuria skrenda kamera
			t2;		//Pozicija, i kuria skrenda taikinys
			
	bool	atejo;	//Ar kamera (ir taikinys) atejo i reikiama pozicija?
	
	KKamera();
	~KKamera();
	
	void Sukurti();
	void Atnaujinti();
	void KampaiMod();
	void KampasToPoz();
	void PozToKampas();
	void NustKam(float, float, float);
	void NustTaik(float, float, float);
	void Pasukti(float, float);			//Pasukti kamera apie vertikalia/horizontalia asi
	void Eiti(float, float, float);		//Eiti nurodytu atstumu (lokaliomis koordinatemis)
	void Eiti(KTaskas*, KTaskas*);
	void EitiKam(float, float, float);
	void EitiTaik(float, float, float);
};


#endif
