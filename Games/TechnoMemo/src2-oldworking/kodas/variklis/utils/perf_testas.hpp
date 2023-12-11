/***********************************************************
	Performance test

	© 2009 A-E Software ®
	Arvydas Burdulis
	http://ae.gamedev.lt  aab@auto.lt


	Programos veikimo greicio testavimas

	Pradeta:
	    2007-12-01
	Paskutini karta keista:
	    2009-01-18
************************************************************/


#ifndef PERFTESTAS_HPP
#define PERFTESTAS_HPP
#include <fstream>
#include <iomanip>


class KPerfTestas{
	public:
	_LARGE_INTEGER	daznis,	//Laikmacio daznis
					prad,	//Testo pradzia
					pab;	//Testo pabaiga
	float	lQuery,		//Laiko gavimo trukme
			lTest,		//Testo trukme
			lIter;		//Vienos testo iteracijos trukme
	
	KPerfTestas();
	void Pradeti();
	void Baigti();
	void Issaugoti(const char *_pav, const char *_fPav, int _kart);
};


#endif
