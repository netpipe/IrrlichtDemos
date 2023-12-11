#include "wav.hpp"
#include <string.h>

KWav::KWav(): failas(NULL), formIv(false), duomIv(false), genLyg(false), trukme(1.0),
  dm(NULL), dmDydis(0), duom(NULL), duomDydis(0), duomDaznis(WAV_DAZNIS)
{
}


KWav::~KWav()
{
}


//Uzkrauti WAV faila
void KWav::Uzkrauti(const char *pav) throw(const char*)
{
	failas = fopen(pav, "rb");
	if(failas == NULL)
		throw("failas nerastas");

	//Skaitomas headeris (pagrindinis Chunk)
	SkaitHead();
	fseek(failas, 4, SEEK_CUR);
	//Skaitomi kiti Chunk'ai, kol randami 'fmt' ir 'data'
	while(!formIv || !duomIv){
		if(ftell(failas) > 10240)
			break;
		SkaitHead();
		//Skaitomas 'format' chunk'as
		if(strcmp(chunkID, "fmt ") == 0){
			SkaitFormat();
			continue;
		}
		//Skaitomas 'data' chunk'as
		if(strcmp(chunkID, "data") == 0){
			SkaitDuom();
			continue;
		}
		fseek(failas, chunkSize, SEEK_CUR);
	}
	fclose(failas);

	if(!formIv)
		throw("nerastas 'fmt ' blokas");
	if(!duomIv)
		throw("nerastas 'data' blokas");
}


void KWav::Naikinti()
{
	if(duom != NULL)
		delete[] duom;
}


void KWav::SkaitHead()
{
	fread(chunkID, 4, 1, failas);
	fread(&chunkSize, 1, 4, failas);
	chunkID[4] = '\0';
	#ifdef DEBUG
	//cout<<"\n\nChunk: "<<chunkID<<"  DataSize: "<<chunkSize;
	#endif
}


//Skaityti formato aprasymo bloka
void KWav::SkaitFormat()
{
	formIv = true;

	fread(&formatas, 2, 1, failas);
	fread(&kanalu, 2, 1, failas);
	fread(&sampleRate, 4, 1, failas);
	fread(&byteRate, 4, 1, failas);
	fread(&blockAlign, 2, 1, failas);
	fread(&bitsPerSample, 2, 1, failas);

	alFormatas = AL_FORMAT_MONO16;
	if(kanalu == 1 && bitsPerSample == 8)
		alFormatas = AL_FORMAT_MONO8;
	if(kanalu == 2 && bitsPerSample == 8)
		alFormatas = AL_FORMAT_STEREO8;
	if(kanalu == 1 && bitsPerSample == 16)
		alFormatas = AL_FORMAT_MONO16;
	if(kanalu == 2 && bitsPerSample == 16)
		alFormatas = AL_FORMAT_STEREO16;

	#ifdef DEBUG
	/*cout	<<"\nFormatas: "<<formatas
			<<"\nKanalu: "<<kanalu
			<<"\nSampleRate: "<<sampleRate
			<<"\nByteRate: "<<byteRate
			<<"\nBlockAlign: "<<blockAlign
			<<"\nbitsPerSample: "<<bitsPerSample;*/
	#endif
}


//Skaityti duomenu bloka
void KWav::SkaitDuom()
{
	if(!formIv)
		return;

	duomIv = true;
	dmDydis = chunkSize * 8 / bitsPerSample;
	dm = new signed short[dmDydis + 1];
	fread(dm, 1, chunkSize, failas);
	trukme = (float)chunkSize / byteRate;
	#ifdef DEBUG
	//cout<<"\nTrukme: "<<trukme;
	#endif
}


//Generuoti lygius
void KWav::GenLygius()
{
	if(!formIv || !duomIv)
		return;

	genLyg = true;
	long sum = 0, num = 0;
	duomDydis = (int)(trukme * duomDaznis);
	duom = new unsigned int[duomDydis];
	for(int i = 0, j = 0; i < chunkSize / 2; i++, ++j){
		if(dm[i] > 0)
			sum += dm[i];
		else
			sum -= dm[i];
		if(j == sampleRate / duomDaznis){
			j = 0;
			duom[num++] = (unsigned int)((float)sum * duomDaznis / sampleRate / kanalu);
			sum = 0;
		}
	}
}


//Issaugoti lygius
void KWav::IssaugLygius(char *pav)
{
	if(!genLyg)
		return;

	FILE *fr = fopen(pav, "wb");
	fwrite(&duomDydis, 4, 1, fr);
	fwrite(&duomDaznis, 4, 1, fr);
	fwrite(duom, 4, duomDydis, fr);
	fflush(fr);
	fclose(fr);
}
