/***********************************************************
	Garsas

	© 2009 A-E Software ®
	Arvydas Burdulis
	http://ae.gamedev.lt  aab@auto.lt


	.

	Pradeta:
	    200?
	Paskutini karta keista:
	    2009-01-29
************************************************************/


#ifndef GARSAS_HPP
#define GARSAS_HPP
#include <variklis/matke/taskas.hpp>
#include <variklis/garsas/wav.hpp>
#include <AL/al.h>
#define GARSU_SK 32
#define GARSU_PAV_ILG 32


class KGarsas{
    public:
    ALuint	src;
    ALint state;
    KTaskas	*poz;
    bool	sukurtas;
    bool	ij;

    KGarsas();
    ~KGarsas();
    void Sukurti(ALuint, KTaskas*, ALboolean);
    void Atnaujinti();
    void Groti();
    void Pauze();
    void Stabd();
    void Garsumas(float);
    void Naikinti();
};


#endif
