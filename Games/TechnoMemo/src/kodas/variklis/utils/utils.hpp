/***********************************************************
	Utilities

	© 2009 A-E Software ®
	Arvydas Burdulis
	http://ae.gamedev.lt  aab@auto.lt


	Ivairios funkcijos. Windows ir GNU/Linix versijos.

	Pradeta:
	    2008-02-22
	Paskutini karta keista:
	    2009-01-18
************************************************************/


#ifndef UTILS_HPP
#define UTILS_HPP

#ifdef OS_WIN32
#include <windows.h>
#endif
#ifdef OS_UNIX
#include <dirent.h>
#endif
#include <time.h>
#include <variklis/grafika/bmp.hpp>


class KUtils{
	private:
	static time_t
			laikas,
			laikasPask;				//Paskutini karta daryto ScreenShot'o laikas
	static int	sum;
	
	public:
	KUtils();
	~KUtils();
	
	static char ** CmdToArgv(char*);	//Win komandines eilutes parametrai keiciami i argv tipo eilutes
	static char * GautiDir();			//Gaunama programos direktorija
	static void GautiDir(char*);
	static char * GautiPav(char*);		//Is pilno failo adreso gaunamas failo vardas
	static char * GautiPlet(char*);		//Is failo vardo gaunamas jo pletinys
	static void GautiData(char*);		//Gaunama data
	static void GautiLaika(char*);		//Gaunamas laikas
	static void ScreenShot(int, int);	//Padaromas ScreenShot'as
};


#endif
