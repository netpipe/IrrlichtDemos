/***********************************************************
	Failu sarasas

	© 2009 A-E Software ®
	Arvydas Burdulis
	http://ae.gamedev.lt  aab@auto.lt


	Gaunamas aplanko failu sarasas. Failai atrenkami pagal
	pasirinkta pletini. Jei sistema GNU/Linux, failu sarasas
	skaitomas is failo "failai.txt".

	Pradeta:
	    2008-02-18
	Paskutini karta keista:
	    2009-01-30
************************************************************/


#ifndef F_UZKROVIMAS_HPP
#define F_UZKROVIMAS_HPP
#include <defines.hpp>
#ifdef DEBUG
#include <variklis/logas.hpp>
#endif
#include <variklis/utils/utils.hpp>
#ifdef OS_WIN32	//Windows
#include <windows.h>
#endif
#ifdef OS_UNIX	//GNU/Linux
#include <fstream>
using namespace std;
#endif
#define F_UZKR_PAV	128


class FUzkrovimas{
	public:
	char	*pav[F_UZKR_PAV];
	int		sk;
	int		num;
	bool	uzkrauta;
		
	FUzkrovimas();
	~FUzkrovimas();
	void Pradeti(char*, char*);
	char * Gauti();
	void Naikinti();
};


#endif
