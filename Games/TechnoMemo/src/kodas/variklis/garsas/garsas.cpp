#include "garsas.hpp"


KGarsas::KGarsas(): src(0), poz(NULL), sukurtas(false), ij(false)
{
}


KGarsas::~KGarsas()
{
}


void KGarsas::Sukurti(ALuint garsas, KTaskas *pozn, ALboolean loop)
{
	//if(!garsIj || !garsai.sukurta)
	//	return;
	if(sukurtas){
		Logas::Log("Klaida: garsas kuriamas antra karta");
		return;
	}

	alGenSources(1, &src);
	if(alGetError() != AL_NO_ERROR)
        Logas::Log("Klaida: alGenSources");

    poz = pozn;
    alSourcei (src, AL_BUFFER,   garsas);
	alSourcef (src, AL_PITCH,    1.0);
	alSourcef (src, AL_GAIN,     1.0);
	alSource3f(src, AL_VELOCITY, 0.0f, 0.0f, 0.0f);
	alSourcei (src, AL_LOOPING,  loop);
	alSourcef (src, AL_ROLLOFF_FACTOR, 0.07);

	sukurtas = true;
}


void KGarsas::Atnaujinti()
{
	if(!sukurtas || !ij)
		return;
	alSource3f(src, AL_POSITION, poz->x, poz->y, poz->z);
	alGetSourcei(src, AL_SOURCE_STATE, &state);
	if(state == AL_STOPPED)
		ij = false;
}


void KGarsas::Groti()
{
	if(ij) return;
    alSourcePlay(src);
    ij = true;
}


void KGarsas::Pauze()
{
	if(!ij) return;
	alSourceStop(src);
}


void KGarsas::Stabd()
{
	if(!ij) return;
	alSourceStop(src);
	ij = false;
}


void KGarsas::Garsumas(float val)
{
	alSourcef(src, AL_GAIN, val);
}


void KGarsas::Naikinti()
{
	if(!sukurtas)
		return;
	alSourceStop(src);
    alDeleteSources(1, &src);
    sukurtas = false;
    ij = false;
}
