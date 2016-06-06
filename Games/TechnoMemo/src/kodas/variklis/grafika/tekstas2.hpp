/***********************************************************
	Tekstas

	© 2008 A-E Software ®
	Arvydas Burdulis
	http://ae.gamedev.lt  aab@auto.lt


	KTekstas - teksto piesimas. Palaikomi lietuviski simboliai.

	Pradeta:
	    2007-06-?
	Paskutini karta redaguota:
	    2008-03-07
************************************************************/


#ifndef TEKSTAS_HPP
#define TEKSTAS_HPP

#ifndef GNU
#include <cstdio>
#else
#include <X11/Xlib.h>
#include <GL/glx.h>
#endif


struct SSimbParam
{
	char
		x,		//Dydis
		y,
		poz_x,	//Pozicija
		poz_y,
		posl_x,	//Sekancio simbolio poslinkis
		posl_y;
};


class KTekstas{
    public:
	int		dydis;			//Srifto dydis
	int		dydis2;			//Teksturu dydis
	int		duomDydis;		//Teksturu duomenu dydis
	unsigned char
			*duom[256];		//Teksturu duomenys
	GLuint	tek[256];		//Teksturu indeksai
	SSimbParam param[256];	//Simboliu bitmap'u parametrai
	unsigned char uch;		//Kintamasis, naudojamas simbolio piesimui
			
	bool	sukurtas;		//Ar jau sukurtas tekstas?
	
	KTekstas();
	~KTekstas();
	#ifndef GNU
	bool Sukurti(int bold = FW_BOLD);
	bool Sukurti(int, const char*);
	bool GautiDuom();
	//bool SukurtiTekstura(GLuint, unsigned char*);
	#else
	bool Sukurti(Display*);
	bool Sukurti(Display*, int, const char*);
	#endif
	void Piesti(const char* eil);
	void PiestiNeform(const char *fmt, ...);
	void Naikinti();
	//Simboliu piesimas
	void Simbolis(char);
	void Tekstas(char*);
} tekstas, tekstas2;


#include "tekstas.cpp"
#endif
