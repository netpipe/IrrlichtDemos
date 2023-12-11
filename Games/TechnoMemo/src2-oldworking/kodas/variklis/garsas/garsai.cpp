#include "garsai.hpp"
#include <string.h>

KGarsai::KGarsai():garSk(0), usr(NULL), sukurta(false), uzkrauta(false)
{
    ori[0] = 0.0f;
    ori[1] = 1.0f;
    ori[2] = 0.0f;
    ori[3] = 0.0f;
    ori[4] = 0.0f;
    ori[5] = 1.0f;
}


KGarsai::~KGarsai()
{
}


void KGarsai::Sukurti()
{
    //if(!alutInit(NULL, 0))
    //    Klaida("Klaida alutInit: %s", alutGetErrorString(alutGetError()));

    //Tikrinama, ar ijungtas garsas
    /*if(!garsIj){
		cout<<" (garsas isjungtas)";
		return;
	}*/

    //Inicializuojamas garso irenginys
	//if(strlen(garsPav) > 0)
	//	garsoDev = alcOpenDevice(NULL);	//_TODO
	//else
		garsoDev = alcOpenDevice(NULL);
	if(garsoDev == NULL){
		#ifdef DEBUG
		Logas::Log("Garsai->Sukurti: Klaida inicializuojant garso irengini");
		#endif
		return;
	}

	//Sukuriamas kontekstas
	context = alcCreateContext(garsoDev, NULL);
	if(context == NULL){
		Logas::Log("Klaida sukuriant garso irenginio konteksta: %d", alcGetError(garsoDev));
		alcCloseDevice(garsoDev);
		return;
	}

	//Kontekstas padaromas darbiniu
	if(alcMakeContextCurrent(context) == ALC_FALSE){
		Logas::Log("Klaida: nepavyko garso irenginio konteksto padaryti darbiniu: %d", alcGetError(garsoDev));
		alcDestroyContext(context);
		alcCloseDevice(garsoDev);
		return;
	}

    alListener3f(AL_VELOCITY, 0.0, 0.0, 0.0);
	alListenerfv(AL_ORIENTATION, ori);
	if(alGetError() != AL_NO_ERROR){
		Logas::Log("Klaida inicializuojant garsa");
		return;
	}
	//garsIj = true;
    sukurta = true;
}


void KGarsai::UzkrautiGarsa(char *pav1)
{
	ALenum klaida;
	char *pav2 = KUtils::GautiPav(pav1);

	if(garSk >= GARSU_SK){
		Logas::Log("Pasiektas garsu limitas");
		return;
	}
	if(strlen(pav2) >= GARSU_PAV_ILG){
		Logas::Log("Garso pavadinimas '%s' per ilgas, bus sutrumpintas", pav1);
	}

	alGenBuffers(1, &garsas[garSk]);
	klaida = alGetError();
	if(klaida != AL_NO_ERROR)
		Logas::Log("Klaida alGenBuffers: %d", klaida);
	try{ wavas[garSk].Uzkrauti(pav1); }
	catch(const char *eil){
		Logas::Log("Klaida uzkraunant garsa '%s': %s", pav1, eil);
		return;
	}
	alBufferData(garsas[garSk], wavas[garSk].alFormatas, wavas[garSk].dm, wavas[garSk].dmDydis * 2, wavas[garSk].sampleRate);
	if(garsas[garSk] == AL_NONE){
		Logas::Log("Klaida uzkraunant garsa '%s': alBufferData", pav1);
	}

	strncpy(pav[garSk], pav2, GARSU_PAV_ILG - 1);
	pav[garSk][GARSU_PAV_ILG - 1] = '\0';

	#ifdef DEBUG
		Logas::Log("Uzkrautas[%i]: %i  %s", garSk, garsas[garSk], pav[garSk]);
	#endif

	//wavas.Naikinti();
	garSk++;
}


void KGarsai::Uzkrauti(char *pavad)
{
	FUzkrovimas failai;
    char *pav;
    failai.Pradeti(pavad, "wav");
    if(failai.sk > 0){
		while((pav = failai.Gauti()) != NULL){
			UzkrautiGarsa(pav);
		}
		uzkrauta = true;
	}
	else
		Logas::Log("Nerasta garsu aplanke '%s'", pavad);
    failai.Naikinti();
}


ALuint KGarsai::ID(const char *pavad)
{
	Logas::Log("Prasoma ID: %s", pavad);
	for(int i = 0; i < garSk; i++)
		if(strcmp(pav[i], pavad) == 0){
			return garsas[i];
		}
	Logas::Log("Garsas %s nerastas", pavad);
	return 0;
}


void KGarsai::Atnaujinti()
{
	alListener3f(AL_POSITION, usr->x, usr->y, usr->z);
}


void KGarsai::Naikinti()
{
	if(uzkrauta)
		alDeleteBuffers(garSk, garsas);
	garSk = 0;
	if(sukurta){
		alcMakeContextCurrent(NULL);
		alcDestroyContext(context);
		if(alcCloseDevice(garsoDev) == ALC_FALSE)
			Logas::Log("Klaida atlaisvinant garso irengini");

	}

	sukurta = false;
	uzkrauta = false;
}
