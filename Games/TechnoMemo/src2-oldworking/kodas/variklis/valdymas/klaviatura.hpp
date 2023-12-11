/***********************************************************
	Klaviaturos valdymas

	© 2009 A-E Software ®
	Arvydas Burdulis
	http://ae.gamedev.lt  aab@auto.lt


	Klaviaturos valdymas.

	Pradeta:
	    2009-01-17
	Paskutini karta keista:
	    2009-01-24
************************************************************/


#ifndef KLAVIATURA_HPP
#define KLAVIATURA_HPP
#include <defines.hpp>
#ifdef DEBUG
#include <variklis/logas.hpp>
#endif
#ifdef OS_WIN32
#include <windows.h>
#endif
#define SP_SK 256


class KKlaviatura
{
	public:
	char	sp[SP_SK];
	
	KKlaviatura();
	~KKlaviatura();
	void Down(short);
	void Up(short);
	void Atnaujinti();
};


#endif
