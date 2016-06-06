#include "ogg.hpp"


KOggFailas::KOggFailas(): uzkrauta(false)
{
}


KOggFailas::~KOggFailas()
{
}


void KOggFailas::Uzkrauti(const char *pav) throw(const char*)
{
	int klaida;
	int bitStream;
	long baitu;
	char buff[OGG_BUFFER];
	vorbis_info *info;
	OggVorbis_File ogg;
	FILE *fd;
	
	//Atidaromas failas
	fd = fopen(pav, "rb");
	if(fd == NULL)
		throw("failas neatidarytas");
	fseek(fd, 0, SEEK_END);
	cout<<"Failo dydis: "<<ftell(fd)<<endl;
	fseek(fd, 0, SEEK_SET);
	
	klaida = ov_open(fd, &ogg, NULL, 0);
	if(klaida < 0)
		throw("ov_open");
	
	//Gaunama informacija apie garsa
	info = ov_info(&ogg, -1);
	if (info->channels == 1)
		alFormatas = AL_FORMAT_MONO16;
	else
		alFormatas = AL_FORMAT_STEREO16;
	daznis = info->rate;
	
	//Skaityti garso duomenis
	do{
		baitu = ov_read(&ogg, buff, OGG_BUFFER, 0, 2, 1, &bitStream);
		buffer.insert(buffer.end(), buff, buff + baitu);
	} while (baitu > 0);

	ov_clear(&ogg);
	
	uzkrauta = true;
}


void KOggFailas::Naikinti()
{
	buffer.clear();
	uzkrauta = false;
}

