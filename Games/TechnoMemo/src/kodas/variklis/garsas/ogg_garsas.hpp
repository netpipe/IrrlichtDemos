/***********************************************************
	OGG garsas

	© 2009 A-E Software ®
	Arvydas Burdulis
	http://ae.gamedev.lt  aab@auto.lt


	OGG Vorbis garso failo valdymas

	Pradeta:
	    2008-02-29
	Paskutini karta redaguota:
	    2009-01-18
************************************************************/


#ifndef OGG_GARSAS_HPP
#define OGG_GARSAS_HPP
#include <defines.hpp>
#ifdef DEBUG
#include <variklis/logas.hpp>
#endif
#include <variklis/matke/taskas.hpp>
#include "ogg_stream.hpp"
#include <AL/al.h>
#include <vorbis/vorbisfile.h>
#include <cstdio>
#include <iostream>
using namespace std;
#define	OGG_BUF_SK	4


class KOggGarsas{
	public:
	KOggStreamFailas	ogg;
	ALuint				src;
	ALuint				buf;
    ALint 				state;
    KTaskas				*poz;
    bool				ij;
	bool				sukurtas;

	KOggGarsas();
	~KOggGarsas();
	void Sukurti(const char*, KTaskas*, ALboolean);
	void Atnaujinti();
	void Groti();
	void Pauze();
	void Stabd();
	void Garsumas(float);
	void GenBuf();
	void Naikinti();
};


#endif
