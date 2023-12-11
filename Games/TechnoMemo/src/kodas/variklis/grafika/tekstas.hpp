/***********************************************************
	Tekstas

	© 2009 A-E Software ®
	Arvydas Burdulis
	http://ae.gamedev.lt  aab@auto.lt


	KTekstas - teksto piesimas. Palaikomi lietuviski simboliai.

	Pradeta:
	    2007-06-?
	Paskutini karta redaguota:
	    2009-01-24
************************************************************/


#ifndef TEKSTAS_HPP
#define TEKSTAS_HPP
#include <defines.hpp>
#ifdef DEBUG
#include <variklis/logas.hpp>
#endif
#ifdef OS_WIN32
#include <windows.h>
#endif
#ifdef OS_UNIX
#include <X11/Xlib.h>
#include <GL/glx.h>
#endif
#include <GL/gl.h>


class KTekstas{
    public:
	GLuint	fontDispList;	//Display listai tekstui
	bool	sukurtas;		//Ar jau sukurtas tekstas?
	
	KTekstas();
	~KTekstas();
	#ifdef OS_WIN32
	bool Sukurti(HDC);
	bool Sukurti(HDC, int, const char*);
	#endif
	#ifdef OS_UNIX
	bool Sukurti(Display*);
	bool Sukurti(Display*, int, const char*);
	#endif
	void Piesti(const char*);
	void PiestiNF(const char*, ...);
	void Naikinti();
};


/*class KTekstF{
	public:
	int		eilSk;			//Eiluciu skaicius
	char	*eil[EIL_SK];	//Tekstas
	float	dydis;			//Tarpo tarp eiluciu dydis
	bool	uzkrautas;		//Ar jau uzkrautas tekstas?
	
	KTekstF();
	~KTekstF();
	void Uzkrauti(const char*);
	void Piesti(GLfloat x, GLfloat y);
	void Naikinti();
};*/


#endif
