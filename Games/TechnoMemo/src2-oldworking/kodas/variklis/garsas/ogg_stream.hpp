/***********************************************************
	OGG Streaminimo lib'as

	© 2009 A-E Software ®
	Arvydas Burdulis
	http://ae.gamedev.lt  aab@auto.lt


	OGG Vorbis garso failu uzkrovimas naudojant OGG Vorbis lib'a.
	Garsas streaminamas - iskoduojamas ir grojamas dalimis.

	Pradeta:
	    2008-02-29
	Paskutini karta keista:
	    2009-01-18
************************************************************/


#ifndef OGG_STREAM_HPP
#define OGG_STREAM_HPP
#include <defines.hpp>
#ifdef DEBUG
#include <variklis/logas.hpp>
#endif
#include <AL/al.h>
#include <vorbis/vorbisfile.h>
#include <cstdio>
#include <vector>
#include <iostream>
using namespace std;
#define OGG_BUFFER	65536 * 4		//256 KB buferis


//CallBack funkcijos
size_t OggCallBackRead(void *ptr, size_t size, size_t nmemb, void *datasource);
int OggCallBackSeek(void *datasource, ogg_int64_t offset, int whence);
int OggCallBackClose(void *datasource);
long OggCallBackTell(void *datasource);

class KOggStreamFailas{
	public:
	static KOggStreamFailas *oggThis;
	unsigned int	skaitPtr;
	char			*duom;
	unsigned int	dydis;
	OggVorbis_File	ogg;
	ALenum			alFormatas;
	ALsizei			daznis;
	char			buffer[OGG_BUFFER];
	bool			loop;
	bool			uzkrauta;

	KOggStreamFailas();
	~KOggStreamFailas();
	void Uzkrauti(const char*) throw(const char*);
	bool GenBuf();
	void Naikinti();
};


#endif
