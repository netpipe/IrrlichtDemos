/***********************************************************
	Garsai

	© 2009 A-E Software ®
	Arvydas Burdulis
	http://ae.gamedev.lt  aab@auto.lt


	.

	Pradeta:
	    2009-01-10
	Paskutini karta keista:
	    2009-01-10
************************************************************/


#ifndef GARSAI_HPP
#define GARSAI_HPP
#include <defines.hpp>
#ifdef DEBUG
#include <variklis/logas.hpp>
#endif
#include <variklis/matke/taskas.hpp>
#include <variklis/utils/utils.hpp>
#include <variklis/utils/f_uzkrovimas.hpp>
#include "wav.hpp"
#define GARSU_SK 32
#define GARSU_PAV_ILG 32
#include <AL/al.h>
#include <AL/alc.h>


class KGarsai{
	public:
	ALCdevice	*garsoDev;
	ALCcontext	*context;
	ALuint	garsas[GARSU_SK];
	char	pav[GARSU_SK][GARSU_PAV_ILG];
	int		garSk;
	KTaskas	*usr;
	ALfloat	ori[6];
	bool	sukurta,
			uzkrauta;
	KWav	wavas[GARSU_SK];

	KGarsai();
	~KGarsai();
	void Sukurti();
	void UzkrautiGarsa(char*);
	void Uzkrauti(char*);
	ALuint ID(const char*);
	void Atnaujinti();
	void Naikinti();
};


#endif
