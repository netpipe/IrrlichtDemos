/***********************************************************
	TGA uzkrovimas

	© 2009 A-E Software ®
	Arvydas Burdulis
	http://ae.gamedev.lt  aab@auto.lt


	TGA lib'as, skirtas TGA paveiksliukams skaityti.
	Nepalaikomas RLE suspaudimas, color index.
	_TODO: Padaryti issaugojima

	Pradeta:
	    2007-06-25
	Paskutini karta keista:
	    2009-01-28
************************************************************/

/*  TGA headeris (18 baitu)
    0-1     unsigned char       id
    1-2     unsigned char       color map type
    2-3     unsigned char       paveiksliuko tipas
    3-5     short int           colour map first entry
    5-7     short int           colour map length
    7-8     unsigned char       map entry size
    9-10    short int           horizontal origin
    10-12   short int           vertical origin
    12-14   short int           plotis
    14-16   short int           aukstis
    16-17   unsigned char       spalvu gylis
    17-18   unsigned char       image descriptor
*/


#ifndef TGA_HPP
#define TGA_HPP
#include <defines.hpp>
#ifdef DEBUG
#include <variklis/logas.hpp>
#endif
#include <variklis/grafika/ogl_constants.hpp>
#include <variklis/grafika/tekstura_duom.hpp>
#include <GL/gl.h>
#include <fstream>


class KTga: public KTeksturaDuom
{
	public:
	unsigned char   header[18]; //Paveiksliuko headeris
	unsigned char   tipas;      //Paveiksliuko tipas
	
	KTga();
	~KTga();
	void    Uzkrauti(const char*) throw(const char*);
	void    Naikinti();
};


#endif
