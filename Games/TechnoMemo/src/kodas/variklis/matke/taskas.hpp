/***********************************************************
	Taskas

	© 2009 A-E Software ®
	Arvydas Burdulis
	http://ae.gamedev.lt  aab@auto.lt


	Taskas (vektorius). Ivairus skaiciavimai - 
	atstumas iki centro, kito tasko, kampas.

	Pradeta:
	    2008-09-09
	Paskutini karta keista:
	    2009-01-26
************************************************************/


#ifndef TASKAS_HPP
#define TASKAS_HPP
#include <defines.hpp>
#include <math.h>


class KTaskas{
	public:
    float	x,
			y,
			z;
	
	void operator = (const KTaskas);
	void operator = (const KTaskas*);
	void operator += (const KTaskas);
	void operator += (const KTaskas*);
	KTaskas * operator *= (float);
	KTaskas operator - (const KTaskas);
	KTaskas operator - (const KTaskas*);
	float operator ^ (KTaskas);			//Kampo tarp vektoriu kosinusas
	float operator ^ (KTaskas*);
	
	KTaskas();
	KTaskas(float, float, float);
	~KTaskas();
	
	const float Ilgis();			//Grazinamas vektoriaus ilgis
	KTaskas * Ilgis(float ilg);			//Vekstoriaus ilgis nustatomas i ilg
	KTaskas * IlgisMaks(float maks);	//Vektoriaus ilgis ribojamas iki maks
	float Atstumas(KTaskas&);			//Atstumas iki kito tasko
	void Normale(float, float);			//Vektorius (normale) skaiciuojamas pagal plokstumos pasisukimo kampus
};


#endif
