/***********************************************************
	Laiko skaiciavimas

	© 2009 A-E Software ®
	Arvydas Burdulis
	http://ae.gamedev.lt  aab@auto.lt


	Laiko skaiciavimas.
	_TODO: laikmaciu sukurimas/valdymas

	Pradeta:
	    2007-06-15
	Paskutini karta keista:
	    2009-01-22
************************************************************/


#ifndef LAIKAS_HPP
#define LAIKAS_HPP
#include <defines.hpp>
#ifdef DEBUG
#include <variklis/logas.hpp>
#endif

#ifdef OS_WIN32
#include <windows.h>
#include <time.h>
//#include <winnt.h>
#endif

#ifdef OS_GNU
#include <sys/time.h>
#endif


/*class KLaikas;


class KLaikmatis{
	private:
	KLaikas *kLaikas;
	public:
	float	liko;		//Kiek laiko dar liko iki laikmacio pabaigos?
	bool	baigesi;	//Ar baigesi laikas?

	KLaikmatis();
	~KLaikmatis();
	void Naikinti();
};*/


class KLaikas{
    public:
    signed int  daznis,	//Timerio daznis hercais
                    prad,	//Laikas, kada pradëjo veikti zaidimas
                    dab,	//Dabartinis laikas
                    pask;	//Paskutinio kadro laikas

    float	kadro,			//Paskutinio kadro trukme
            veikimo,		//Zaidimo veikimo trukme
            fpsLaik;		//Timeris, naudojamas FPS skaiciuoti
	int		fps,			//Kadrai per sekunde (realus)
			fpsSk;			//Kiek kadru praejo
	float	gr;				//Greicio daugiklis
	bool	stop;			//Ar sustabdytas?

	//Laikmaciai
	/*float	*laikmat;		//Laikmaciu laikai
	unsigned int *laikmat_map;		//Laikmaciu indeksai laikmat masyve
	unsigned int *laikmat_map_rev;	//Atgaliniai indeksai
	int		laikmat_akt;	//Kiek aktyviu?
	int		laikmat_kiek;	//Kiek yra laikmaciu?*/

	KLaikas();
	~KLaikas();
	void Sukurti();			//Gaunamas pradinis laikas, daznis. Funkcija vykdoma
							//viena karta zaidimo pradzioje
    void Gauti();			//Gauti dabartini laika, apskaiciuoti reiksmes
    void Stabdyti();		//Sustabdyti laika
    void Paleisti();		//Paleisti laika

    //Laikmaciai
    /*unsigned int Laikmatis(float);	//Sukuriamas laikmatis, grazinamas jo ID
    bool Baigesi(unsigned int);		//Tikrinama, ar jau baigesi laikmatis
    void LaikmatisNaikinti(unsigned int);	//Naikinamas laikmatis*/
};


#endif
