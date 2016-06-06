#include "tga.hpp"


KTga::KTga(): tipas(0)
{
}


KTga::~KTga()
{
}


void KTga::Uzkrauti(const char *pav) throw(const char*)
{
    FILE *failas;
    failas = fopen(pav, "rb");
    if(failas == NULL){
		throw("Nepavyko atidaryti failo");
        return;
	}
    fread(header, 1, 18, failas);
    
    tipas = header[2];
    x = header[12] + header[13] * 256;
    y = header[14] + header[15] * 256;
    bpp = header[16];
    
    if(bpp == 24){
		glTipas2 = GL_BGR;
		glTipas1 = GL_RGB;
	}
	if(bpp == 32){
		glTipas2 = GL_BGRA;
		glTipas1 = GL_RGBA;
	}
    if(bpp == 8){
		glTipas2 = GL_ALPHA8;
		glTipas1 = GL_ALPHA;
	}
	if(tipas < 2){
		throw("Netinkamas failo tipas");
		return;
	}
	glFormatas = GL_UNSIGNED_BYTE;

    duomDydis = x * y * (bpp / 8);
    try{ duom = new unsigned char[duomDydis]; }
	catch(...){ throw("Neisskirta atmintis!"); }
    fread(duom, 1, duomDydis, failas);
    
    //Logas::Log("%i %i %i %i", x, y, bpp, duomDydis);

    fclose(failas);
}


void KTga::Naikinti()
{
    if(duom != NULL)
		delete[] duom;
}
