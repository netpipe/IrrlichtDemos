/***********************************************************
	3DO failu uzkrovimas

	© 2009 A-E Software ®
	Arvydas Burdulis
	http://ae.gamedev.lt  aab@auto.lt


	Uzkraunami 3DO failai, generuojami display list'ai.

	Pradeta:
	    2007-07-06
	Paskutini karta keista:
	    2009-01-29
************************************************************/

//	3DO headeris (80 baitu)
// 0x00 1 B - kontrolinis baitas, visada 0xAE (174)
// 0x01 1 B - objekto ID
// 0x02 1 B - Collision tasku skaicius
// 0x03 1 B - pagalbiniu tasku skaicius
// 0x04 4 B - Apskritimo, gaubiancio visus taskus, spindulys
// 0x08 4 B - L1 tasku skaicius
// 0x0C 4 B - L1 trikampiu skaicius
// 0x10 4 B - L2 tasku skaicius
// 0x14 4 B - L2 trikampiu skaicius
// 0x18 4 B - L3 tasku skaicius
// 0x1C 4 B - L3 trikampiu skaicius
// 0x20 32 B - teksturos pavadinimas
// 0x40 16 B - Bounding Box reiksmes (kaire, desine, virsus, apacia)
//
//	Duomenu dalis:
// Collision taskai, kiekvienas po 8 B
// Pagalbiniai taskai (2 * 3 * 4 = 24 B) - centro pozicija, kryptis
// L1 taskai ((3 + 3 + 2) * 4 B = 32 B) - koordinate, normale, teksturos koordinates
// L1 trikampiai (3 * 4 B = 12 B) - tasku numeriai
// L2 taskai
// L2 trikampiai
// L3 taskai
// L3 trikampiai


#ifndef TDO_HPP
#define TDO_HPP
#include <defines.hpp>
#ifdef DEBUG
#include <variklis/logas.hpp>
#endif
#include <GL/gl.h>
#define TDO_DISP_LIST 3


class TDOFailas{
	public:
	unsigned char
			ID,				//Objekto ID
			collSk,			//Collision tasku skaicius
			pagSk;			//Pagalbiniu tasku skaicius
	float	*coll,			//Collision taskai
			*pag;			//Pagalbiniai taskai
	char	teks[32];		//Teksturos pavadinimas
	GLuint	tekst;			//Teksturos numeris
	GLuint	dispList;		//Display listai LOD'ams (0, 1, 2), seseliui (3)
	GLfloat spind;			//Apskritimo, gaubiancio visa objekta, spindulys
	float	bounding[4];	//Bounding Box reiksmes (kaire, desine, virsus, apacia)
	bool	uzkrauta;		//Ar uzkrautas objektas?
		
	TDOFailas();
	~TDOFailas();
	void Uzkrauti(const char*) throw(const char*);
	void Naikinti();
};


#endif
