#include "wav_dat.hpp"


KWavDat::KWavDat(): duomDydis(0), daznis(0), duom(NULL), uzkrauta(false)
{
}


KWavDat::~KWavDat()
{
}


void KWavDat::Uzkrauti(const char *pav)
{
	FILE *failas = fopen(pav, "rb");
	if(failas == NULL)
		return;
	
	fread(&duomDydis, 4, 1, failas);
	fread(&daznis, 4, 1, failas);
	duom = new unsigned int[duomDydis];
	fread(duom, 4, duomDydis, failas);	
	fclose(failas);
	uzkrauta = true;
}


//Gaunamas garsumas laiku 'laik'
unsigned int KWavDat::Gauti(float laik)
{
	if(!uzkrauta)
		return 0;
	if((int)(laik * daznis) >= duomDydis)
		return 0;
	else
		return duom[(int)(laik * daznis)];
}


void KWavDat::Naikinti()
{
	if(duom != NULL)
		delete[] duom;
}
