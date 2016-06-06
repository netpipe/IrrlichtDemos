#include "ogg_garsas.hpp"


KOggGarsas::KOggGarsas(): src(0), poz(NULL), sukurtas(false), ij(false)
{
}


KOggGarsas::~KOggGarsas()
{
}


//pav - failo pavadinimas, pozn - rodykle i pozicija, loop - ar loop'inti garsa
void KOggGarsas::Sukurti(const char* pav, KTaskas *pozn, ALboolean loop)
{
	if(sukurtas)
		Naikinti();

	//Uzkrauti garsa
	try{ ogg.Uzkrauti(pav); }
	catch(const char *eil){
		Logas::Log("Klaida uzkraunant garsa '%s': %s", pav, eil);
		return;
	}

	ogg.loop = loop;
	poz = pozn;

	alGenSources(1, &src);
	if(alGetError() != AL_NO_ERROR){
		Logas::Log("Klaida uzkraunant faila '%s': alGenSources", pav);
		return;
	}

	alSourcef (src, AL_PITCH,    1.0);
	alSourcef (src, AL_GAIN,     1.0);
	alSource3f(src, AL_VELOCITY, 0.0, 0.0, 0.0);
	alSourcei (src, AL_LOOPING,  AL_NONE);
	alSourcef (src, AL_ROLLOFF_FACTOR, 0.07);

	for(int i = 0; i < OGG_BUF_SK; i++){
		alGenBuffers(1, &buf);
		GenBuf();
	}

	sukurtas = true;
}


void KOggGarsas::Atnaujinti()
{
	if(!sukurtas)
		return;
	int laisva;
	alGetSourcei(src, AL_BUFFERS_PROCESSED, &laisva);
	if(laisva > 0){
		alSourceUnqueueBuffers(src, 1, &buf);
		GenBuf();
	}
	alSource3f(src, AL_POSITION, poz->x, poz->y, poz->z);
	alGetSourcei(src, AL_SOURCE_STATE, &state);
	if(state != AL_PLAYING && ij)
		alSourcePlay(src);
}


void KOggGarsas::Groti()
{
	if(!sukurtas)
		return;
	alSourcePlay(src);
}


void KOggGarsas::Pauze()
{
	if(!sukurtas)
		return;
	alSourcePause(src);
}


void KOggGarsas::Stabd()
{
	if(!sukurtas)
		return;
	alSourceStop(src);
}


void KOggGarsas::Garsumas(float val)
{
	alSourcef(src, AL_GAIN, val);
}


//I source eile itraukiamas dar vienas buferis
void KOggGarsas::GenBuf()
{
	if(!ogg.GenBuf()){
		//cout<<"Baigesi duomenys\n";
		return;
	}
	alBufferData(buf, ogg.alFormatas, ogg.buffer, OGG_BUFFER, ogg.daznis);
	alSourceQueueBuffers(src, 1, &buf);
}


void KOggGarsas::Naikinti()
{
	if(!sukurtas)
		return;
	alSourceStop(src);
	alDeleteSources(1, &src);
	alDeleteBuffers(1, &buf);
	ogg.Naikinti();
	sukurtas = false;
}
