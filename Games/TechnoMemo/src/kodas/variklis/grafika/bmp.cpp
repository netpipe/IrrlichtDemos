#include "bmp.hpp"


KBmp::KBmp()
{
}


KBmp::~KBmp()
{
}


void KBmp::Uzkrauti(char *vardas) throw(const char*)
{
	FILE *failas;
	int head;
	failas = fopen(vardas, "rb");
	if(failas == NULL){
		throw("failas nerastas");
		return;
	}
	fread(header, 54, 1, failas);
	
	//Apskaiciuojamos reiksmes
	if(!(header[0] == 'B' && header[1] == 'M')){
		throw("failas nera BMP formato");
		return;
	}
	//Patikrinama, ar nenaudojamas suspaudimas
	if(header[30] != 0 && header[28] != 16){
		throw("failas naudoja suspaudima");
		return;
	}
	//Patikrinama, ar nenaudojamos indeksuotos spalvos
	if(header[46] != 0){
		throw("naudojamos indeksuotos spalvos");
		return;
	}
	head = header[14];
	x = header[19] * 256 + header[18];
	y = header[23] * 256 + header[22];
	bpp = header[29] * 256 + header[28];
	duomDydis = header[37]*256*256*256 + header[36]*256*256 + header[35]*256 + header[34];
	
	duom = new unsigned char[duomDydis];
	//Jei headeris didesnis uz standartini (54), ji reikia papildomai nuskaityti
	if(head > 40)
		fread(duom, head - 40, 1, failas);
	
	fread(duom, duomDydis, 1, failas);
	fclose(failas);
	
	glFormatas = GL_UNSIGNED_BYTE;
	switch(bpp){
		case 8:
			glTipas1 = GL_INTENSITY;
			glTipas2 = GL_INTENSITY;
			break;
		case 16:
			glTipas1 = GL_RGB5_A1;
			glTipas2 = GL_RGBA;
			glFormatas = GL_UNSIGNED_SHORT_1_5_5_5_REV;
			break;
		case 24:
			glTipas1 = GL_RGB;
			glTipas2 = GL_BGR;
			break;
		case 32:
			glTipas1 = GL_RGBA;
			glTipas2 = GL_BGRA;
			break;
		default:
			throw("Netinkamas bpp");
			return;
			break;
	}
}


void KBmp::Issaugoti(char *pav)
{
	/*ofstream fr(pav, ios::binary);
	fr.write((char*)header, sizeof(header));
	fr.write((char*)duom, duomDydis);
	fr.close();*/
}


void KBmp::Issaugoti(unsigned int xx, unsigned int yy,
	  unsigned int bppp, unsigned char *duomm, char *pav)
{
	/*int dydis = xx * yy * 3;
	int dydisv = dydis + 54;
	unsigned char head[54];
	for(int i = 0; i < 54; i++)
		head[i] = 0;
	head[0] = 'B';
	head[1] = 'M';
	head[2] = dydisv % 256;
	head[3] = dydisv / 256 % 256;
	head[4] = dydisv / 256 / 256;
	head[10] = 54;
	head[14] = 40;
	head[30] = 0;
	head[46] = 0;
	head[19] = (char)(xx / 256);
	head[18] = xx % 256;
	head[23] = (char)(yy / 256);
	head[22] = yy % 256;
	head[28] = bppp % 256;
	
	ofstream fr(pav, ios::binary);
	fr.write((char*)head, sizeof(head));
	fr.write((char*)duomm, dydis);
	fr.close();*/
}


void KBmp::Naikinti()
{
        delete[] duom;
}
