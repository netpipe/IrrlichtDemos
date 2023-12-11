#include "ogg_stream.hpp"
#include <memory.h>

KOggStreamFailas *KOggStreamFailas::oggThis = NULL;


KOggStreamFailas::KOggStreamFailas(): skaitPtr(0), duom(NULL), dydis(0),
  loop(false), uzkrauta(false)
{
}


KOggStreamFailas::~KOggStreamFailas()
{
}


void KOggStreamFailas::Uzkrauti(const char *pav) throw(const char*)
{
	int klaida;
	ov_callbacks callbacks;
	vorbis_info *info;
	FILE *fd;

	if(uzkrauta)
		return;
	//Atidaromas failas
	fd = fopen(pav, "rb");
	if(fd == NULL)
		throw("failas neatidarytas");
	fseek(fd, 0, SEEK_END);
	dydis = ftell(fd);
	fseek(fd, 0, SEEK_SET);
	duom = new char[dydis];
	klaida = fread(duom, 1, dydis, fd);
	if(klaida != dydis)
		throw("neteisingas nuskaitymas");
	fclose(fd);

	oggThis = this;
	callbacks.read_func = OggCallBackRead;
	callbacks.seek_func = OggCallBackSeek;
	callbacks.close_func = OggCallBackClose;
	callbacks.tell_func = OggCallBackTell;
	klaida = ov_open_callbacks(duom, &ogg, NULL, 0, callbacks);
	if(klaida < 0)
		throw("ov_open_callbacks");

	//Gaunama informacija apie garsa
	info = ov_info(&ogg, -1);
	if (info->channels == 1)
		alFormatas = AL_FORMAT_MONO16;
	else
		alFormatas = AL_FORMAT_STEREO16;
	daznis = info->rate;

	uzkrauta = true;
}


//Generuojamas vienas buferis
bool KOggStreamFailas::GenBuf()
{
	int dydis = 0;
	int gauta;
	int sekcija;
	static bool rekursija = false;
	oggThis = this;

	do{
		gauta = ov_read(&ogg, buffer + dydis, OGG_BUFFER - dydis, 0, 2, 1, &sekcija);
		if(gauta)
			dydis += gauta;
		else
			break;
	} while(gauta && dydis < OGG_BUFFER);

	//Jei baigesi failo duomenys ir isjungtas kartojimas, grazinti false
	if(gauta <= 0 && !loop)
		return false;

	//Jeigu baigesi failo duomenys, pradeti nuo pradziu
	if(gauta <= 0 && !rekursija && loop){
		rekursija = true;
		ov_pcm_seek(&ogg, 0);
		GenBuf();
		rekursija = false;
	}

	return true;
}


void KOggStreamFailas::Naikinti()
{
	if(!uzkrauta)
		return;
	ov_clear(&ogg);
	if(duom)
		delete[] duom;
	uzkrauta = false;
}


//CallBack funkcijos -----------------------------------------------------------


size_t OggCallBackRead(void *ptr, size_t size, size_t nmemb, void *datasource)
{
	size_t skaitDydis = size * nmemb;	//Kiek baitu galima perskaityti
	if(skaitDydis > KOggStreamFailas::oggThis->dydis - KOggStreamFailas::oggThis->skaitPtr)
		//Tiek baitu nera, reikia sumazinti
		skaitDydis = KOggStreamFailas::oggThis->dydis - KOggStreamFailas::oggThis->skaitPtr;
	if(skaitDydis > 0){
		memcpy(ptr, (char*)datasource + KOggStreamFailas::oggThis->skaitPtr, skaitDydis);
		KOggStreamFailas::oggThis->skaitPtr += skaitDydis;
	}
	return skaitDydis;
}


int OggCallBackSeek(void *datasource, ogg_int64_t offset, int whence)
{
	unsigned int naujaPoz = KOggStreamFailas::oggThis->skaitPtr;
	//Skaiciuojama skaitymo pointerio pozicija
	switch(whence){
		case SEEK_SET:
			naujaPoz = offset;
			break;
		case SEEK_CUR:
			naujaPoz += offset;
			break;
		case SEEK_END:
			naujaPoz = KOggStreamFailas::oggThis->dydis - offset;
			break;
	}

	//Tikrinama, ar nauja pozicija yra intervale (0, dydis)
	if(offset > KOggStreamFailas::oggThis->dydis)
		offset = KOggStreamFailas::oggThis->dydis;
	if(offset < 0)
		offset = 0;
	KOggStreamFailas::oggThis->skaitPtr = naujaPoz;
}


int OggCallBackClose(void *datasource)
{
	return 1;
}


long OggCallBackTell(void *datasource)
{
	return KOggStreamFailas::oggThis->skaitPtr;
}
