/***********************************************************
	Lango valdymas

	© 2009 A-E Software ®
	Arvydas Burdulis
	http://ae.gamedev.lt  aab@auto.lt


	Lango valdymas.

	Pradeta:
	    2009-01-18
	Paskutini karta keista:
	    2009-01-18
************************************************************/


#ifndef LANGAS_HPP
#define LANGAS_HPP
#include <defines.hpp>
#include <globals.hpp>
#ifdef DEBUG
#include <variklis/logas.hpp>
#endif
#include <vector>
#include <windows.h>
#include <variklis/nustatymai.hpp>
#include <variklis/valdymas/klaviatura.hpp>


/*class KLangas;
struct SLangas
{
	KLangas *lang;
	HWND	hWnd;
};*/


class KLangas
{
	public:
	//static vector<SLangas> langai;	//Sukurti langai
	HDC			hDC;		//GDI Device Context
	HGLRC		hRC;		//Rendering Context
	HWND		hWnd;		//Window Handle
	HINSTANCE	hInst;		//App instance
	MSG			msg;
	bool		sukurta;	//Ar sukurtas langas?
	static bool	baigti;		//Ar jau baigti?
	
	KLangas();
	~KLangas();
	bool Sukurti(bool rod = true);
	static void KeistiDydi(int, int);
	void Rodyti();
	static LRESULT CALLBACK Callback(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam);
	//LRESULT Zinute(UINT msg, WPARAM wParam, LPARAM lParam);
	void Atnaujinti();
	void SukeistiBuferius();
	void Naikinti();
};


#endif
