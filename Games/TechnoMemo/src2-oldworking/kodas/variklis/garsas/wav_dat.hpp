/***********************************************************
	WAV_dat lib'as

	© 2009 A-E Software ®
	Arvydas Burdulis
	http://ae.gamedev.lt  aab@auto.lt


	WAV garso lygio duomenys 

	Pradeta:
	    2008-02-21
	Paskutini karta keista:
	    2009-01-18
************************************************************/

/*      WAV_duom
 	0-3		unsigned int	duomDydis;		//Duomenu dalies dydis
 	4-5		unsigned short	daznis;			//Daznis
 	6-7		unsigned short	dalSk;			//Daliu skaicius
 	...		unsigned int	duomDydis		//Garso lygiai
*/


#ifndef WAV_DAT_HPP
#define WAV_DAT_HPP
#include <defines.hpp>
#ifdef DEBUG
#include <variklis/logas.hpp>
#endif
#include <stdio.h>


class KWavDat{
	public:
	
	unsigned int	duomDydis;
	unsigned short	daznis;
	unsigned int	*duom;
	bool			uzkrauta;
	
	KWavDat();
	~KWavDat();
	
	void Uzkrauti(const char*);
	unsigned int Gauti(float);
	void Naikinti();
};


#endif
