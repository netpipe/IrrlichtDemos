/***********************************************************
	Log

	© 2009 A-E Software ®
	Arvydas Burdulis
	http://ae.gamedev.lt  aab@auto.lt


	Funkcijos, ismetancios errora arba ispejima.
	Log'inimas (i faila arba stdout).

	Pradeta:
	    2007-06-?
	Paskutini karta keista:
	    2009-01-20
************************************************************/


#ifndef LOGAS_HPP
#define LOGAS_HPP
#include <defines.hpp>
#include <cstdarg>

#ifdef OS_WIN32
#include <windows.h>
#endif

#include <iostream>


class Logas
{
	static FILE *failas;

	public:
	Logas();
	~Logas();
	static void Klaida(char*, ...);
	static void Ispejimas(char*, ...);
	#ifdef DEBUG
	static void LogFailas(char*);
	static void Log(const char*, ...);
	#endif
};


#endif
