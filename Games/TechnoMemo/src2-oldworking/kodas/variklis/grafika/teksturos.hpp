/***********************************************************
	Teksturu uzkrovimas

	© 2009 A-E Software ®
	Arvydas Burdulis
	http://ae.gamedev.lt  aab@auto.lt


	Uzkraunamos TGA ir BMP teksturos.

	Pradeta:
	    2007-06-25
	Paskutini karta keista:
	    2009-01-31
************************************************************/


#ifndef TEKSTUROS_HPP
#define TEKSTUROS_HPP
#include <defines.hpp>
#ifdef DEBUG
#include <variklis/logas.hpp>
#endif
#include <variklis/utils/utils.hpp>
#include <variklis/grafika/tga.hpp>
#include <variklis/grafika/bmp.hpp>
#include <variklis/utils/f_uzkrovimas.hpp>
#define TEKST_SK 128
#define TEKST_PAV_ILG 32


class KTeksturos{
    public:
    GLuint  tekst[TEKST_SK];
    char    pav[TEKST_SK][TEKST_PAV_ILG];
    int     tekstSk;
    bool    alpha,
    		mipMap,		//Ar generuoti MipMap'us?
    		uzkrauta;
    
    KTeksturos();
    ~KTeksturos();
    GLuint UzkrautiTGA(char*);
    GLuint UzkrautiBMP(char*);
    void Uzkrauti(char*);
    bool Yra(const char*);
    GLuint ID(const char*);
    void Naikinti();
};


#endif
