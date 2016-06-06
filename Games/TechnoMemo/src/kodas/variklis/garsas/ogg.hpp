/***********************************************************
	OGG lib'as

	© 2009 A-E Software ®
	Arvydas Burdulis
	http://ae.gamedev.lt  aab@auto.lt


	OGG Vorbis garso failu uzkrovimas naudojant OGG Vorbis lib'a

	Pradeta:
	    2008-02-28
	Paskutini karta keista:
	    2009-01-18
************************************************************/


#ifndef OGG_HPP
#define OGG_HPP
#include <defines.hpp>
#ifdef DEBUG
#include <variklis/logas.hpp>
#endif
#include <AL/al.h>
#include <vorbis/vorbisfile.h>
#include <cstdio>
#include <vector>
#include <iostream>
using namespace std;
#define OGG_BUFFER   32768		//32 KB buferis


class KOggFailas{
	public:
	ALenum			alFormatas;
	ALsizei			daznis;
	vector <char>	buffer;
	bool			uzkrauta;

	KOggFailas();
	~KOggFailas();
	void Uzkrauti(const char*) throw(const char*);
	void Naikinti();
};


#endif
