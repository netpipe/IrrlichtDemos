/***********************************************************
	Teksturos duomenys

	© 2009 A-E Software ®
	Arvydas Burdulis
	http://ae.gamedev.lt  aab@auto.lt


	Bazine teksturos duomenu klase.

	Pradeta:
	    2009-01-27
	Paskutini karta keista:
	    2009-01-28
************************************************************/


#ifndef TEKSTURA_DUOM_HPP
#define TEKSTURA_DUOM_HPP
#include <defines.hpp>
#ifdef DEBUG
#include <variklis/logas.hpp>
#endif
#include <variklis/grafika/ogl_constants.hpp>
#include <GL/gl.h>
#include <GL/glu.h>


class KTeksturaDuom
{
	public:
	unsigned int x,			//Teksturos plotis
				y,			//Teksturos aukstis
				bpp,		//Spalvu gylis (Bits per Pixel)
				duomDydis;	//Duomenu dydis baitais
	unsigned char	*duom;	//Duomenys
	GLuint		id,			//Sukurto teksturos objekto ID
				glTipas1,	//OpenGL tipas - GL_RGB,...
				glTipas2,	//OpenGL tipas - GL_RGB, GL_BGR,...
				glFormatas;	//penGL formatas - GL_UNSIGNED_BYTE,...
	
	KTeksturaDuom();
	~KTeksturaDuom();
	virtual void Uzkrauti(const char *pav) throw(const char*){};
	GLuint Sukurti();
	virtual void Naikinti(){};
};


#endif
