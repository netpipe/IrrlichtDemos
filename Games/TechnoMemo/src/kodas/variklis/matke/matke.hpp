/***********************************************************
	Matke

	© 2009 A-E Software ®
	Arvydas Burdulis
	http://ae.gamedev.lt  aab@auto.lt


	Ivairios matematines funkcijos.
	
	Pradeta:
	    2007-10-04
	Paskutini karta redaguota:
	    2009-01-31
************************************************************/


#ifndef MATKE_HPP
#define MATKE_HPP
#include <defines.hpp>
#include <math.h>


float abs(float);

namespace Matke{
	//Skaiciuojami atstumai tarp dvieju tasku (2D)
	/*	static float Atstumas2D(float x1, float y1, float x2, float y2);	//Skaiciuojamas atstumas tarp tasku (x1, y1) ir (x2, y2)
		static float Atstumas2D(float *t1, float *t2);				//Skaiciuojamas atstumas tarp tasku t1 ir t2
		static float Atstumas2D(STaskas &t1, STaskas &t2);			//Skaiciuojamas atstumas tarp tasku t1 ir t2
		static float Atstumas2D(float *sk);						//Skaiciuojamas atstumas tarp tasku (sk, sk + 1) ir (sk + 2, sk + 3)
		static float Atstumas2D(STaskas &t1);					//Skaiciuojamas vektoriaus t1 ilgis
		
	//Skaiciuojami atstumu tarp dvieju tasku (2D) kvadratai
		static float Atstumas2D2(float x1, float y1, float x2, float y2);	//Skaiciuojamas atstumo tarp tasku (x1, y1) ir (x2, y2) kvadratas
		const float Atstumas2D2(float *t1, float *t2);			//Skaiciuojamas atstumo tarp tasku t1 ir t2 kvadratas
		const float Atstumas2D2(STaskas &t, float x, float y);	//Skaiciuojamas atstumo tarp tasku t1 ir (x, y) kvadratas
		const float Atstumas2D2(STaskas &t1, STaskas &t2);		//Skaiciuojamas atstumo tarp tasku t1 ir t2 kvadratas
		const float Atstumas2D2(float *sk);						//Skaiciuojamas atstumo tarp tasku (sk, sk + 1) ir (sk + 2, sk + 3) kvadratas
	
	//Skaiciuojami atstumai tarp dvieju tasku (3D)
		const float Atstumas3D(float x1, float y1, float z1, float x2, float y2, float z2);
		const float Atstumas3D(float *t1, float *t2);
		const float Atstumas3D(STaskas &t1, STaskas &t2);
		const float Atstumas3D(float *sk);
		
	//Skaiciuojami atstumu tarp dvieju tasku (3D) kvadratai
		const float Atstumas3D2(float x1, float y1, float z1, float x2, float y2, float z2);
		const float Atstumas3D2(float *t1, float *t2);
		const float Atstumas3D2(STaskas &t, float x, float y, float z);
		const float Atstumas3D2(STaskas &t1, STaskas &t2);
		const float Atstumas3D2(float *sk);
	
	//Skaiciuojamas kampas tarp duotos linijos ir Y+ asies
		const float Kampas(float x1, float y1, float x2, float y2);	//Skaiciuojamas kampas tarp duotos tieses ir Y+ asies
		const float Kampas(float *t1, float *t2);
		const float Kampas(STaskas &t1, STaskas &t2);
		const float Kampas(STaskas &t1);
		
	//Skaiciuojamas dvieju tiesiu susikirtimo taskas
		STaskas	KoefKB(float *t1, float *t2);			//Skaiciuojami tieses k ir b koeficientai; t1 - pradzia, t2 - pabaiga
		STaskas	KoefKB(STaskas &t1, STaskas &t2);
		STaskas Susikirtimas(float *t1, float *t2);		//Skaiciuojamas dvieju tiesiu susikirtimas, kai duoti k ir b koeficientai
		STaskas Susikirtimas(float *t1, STaskas &t2);
		STaskas Centras(STaskas*, float*, float*, float*, float*);	//Skaiciuojamas apskritimas, einantis tarp dvieju liestiniu
	*/
	//Kampu palyginimas
		static float KampaiMod(float k1, float k2);		//Kampu palyginimas; grazinamas skirtumo modulis
		float KampaiSkirt(float k1, float k2);			//Kampu skirtumas
		static float KampaiSkirtMod(float k1, float k2);			//Kampu skirtumas, grazinamas rezultato modulis
		void KampasMod(float *k1);								//Kampo modulis (nuo -180 iki 180 laipsniu)
		float KampasMod(float k1);								//Kampo modulis (nuo -180 iki 180 laipsniu)
		
	//Skaiciu palyginimas
		float SkirtMod(float k1, float k2);				//Grazinamas skaiciu atimties modulis
		int	  SkirtMod(int k1, int k2);
		float Mod(float sk, float mod);					//Skaiciaus 'sk' modulis pagrindu 'mod'
		float Zenklas(float sk);						//Patikrinamas skaiciaus zenklas, grazinama 1.0 arba -1.0
};


#endif
