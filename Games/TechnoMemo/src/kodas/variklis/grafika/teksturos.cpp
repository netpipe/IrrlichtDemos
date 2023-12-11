#include "teksturos.hpp"


KTeksturos::KTeksturos(): tekstSk(0), alpha(false), mipMap(true), uzkrauta(false)
{
}


KTeksturos::~KTeksturos()
{
}


GLuint KTeksturos::UzkrautiTGA(char *vard1)
{
	//Gaunamas teksturos pavadinimas
	char *vard2 = KUtils::GautiPav(vard1);
	
	if(Yra(vard2))
		return ID(vard2);
		
    KTga tek;
    try{ tek.Uzkrauti(vard1); }
    catch(const char *klaida){
		Logas::Log("Klaida uzkraunant tekstura %s: %s", vard1, klaida);
		return 0;
	}

    glGenTextures(1, &tekst[tekstSk]);
    glBindTexture(GL_TEXTURE_2D, tekst[tekstSk]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	/*if(alpha){
		tek.glTipas = GL_ALPHA8;
		tek.glTipas2 = GL_ALPHA;
	}*/
    glTexImage2D(GL_TEXTURE_2D, 0, tek.glTipas1, tek.x, tek.y, 0, tek.glTipas2, GL_UNSIGNED_BYTE, tek.duom);
	if(mipMap){
		if(gluBuild2DMipmaps(GL_TEXTURE_2D, tek.glTipas1, tek.x, tek.y, tek.glTipas2, GL_UNSIGNED_BYTE, tek.duom) != 0)
			Logas::Log("Klaida generuojant MipMap'us teksturai '%s'", vard2);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	tek.Naikinti();

	if(!glIsTexture(tekst[tekstSk])){
		Logas::Log("Nesugeneruota tekstura '%s'", vard2);
		return 0;
	}

    strcpy(pav[tekstSk], vard2);
	tekstSk++;
    
    #ifdef DEBUG
    Logas::Log("Teksturos: %s %i", vard2, tekst[tekstSk - 1]);
    #endif
    return ID(vard2);
}


GLuint KTeksturos::UzkrautiBMP(char *vard1)
{
	//Gaunamas teksturos pavadinimas
	char *vard2 = KUtils::GautiPav(vard1);
	
	if(Yra(vard2))
		return ID(vard2);
		
    KBmp tek;
    try{ tek.Uzkrauti(vard1); }
    catch(const char *klaida){
		Logas::Log("Klaida uzkraunant tekstura %s: %s", vard1, klaida);
		return 0;
	}

    glGenTextures(1, &tekst[tekstSk]);
    glBindTexture(GL_TEXTURE_2D, tekst[tekstSk]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, tek.glTipas1, tek.x, tek.y, 0, tek.glTipas2, GL_UNSIGNED_BYTE, tek.duom);
	if(mipMap){
		if(gluBuild2DMipmaps(GL_TEXTURE_2D, tek.glTipas1, tek.x, tek.y, tek.glTipas2, tek.glFormatas, tek.duom) != 0)
			Logas::Log("Klaida generuojant MipMap'us teksturai '%s'", vard2);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	tek.Naikinti();

	if(!glIsTexture(tekst[tekstSk])){
		Logas::Log("Nesugeneruota tekstura '%s'", vard2);
		return 0;
	}

    strcpy(pav[tekstSk], vard2);
	tekstSk++;
    
    #ifdef DEBUG
    Logas::Log("Teksturos: %s %i", vard2, tekst[tekstSk - 1]);
    #endif
    return ID(vard2);
}


void KTeksturos::Uzkrauti(char *pavad)
{
    FUzkrovimas failai;
    char *pav;
    
    //TGA teksturos
    failai.Pradeti(pavad, "tga");
    if(failai.sk > 0){
		while((pav = failai.Gauti()) != NULL){
			UzkrautiTGA(pav);
		}
		uzkrauta = true;
	}
    failai.Naikinti();
    
    //BMP teksturos
    failai.Pradeti(pavad, "bmp");
    if(failai.sk > 0){
		while((pav = failai.Gauti()) != NULL){
			UzkrautiBMP(pav);
		}
		uzkrauta = true;
	}
    failai.Naikinti();
}


bool KTeksturos::Yra(const char *pavad)
{
	for(int i = 0; i < tekstSk; i++)
        if(strcmp(pav[i], pavad) == 0)
            return true;
    return false;
}


GLuint KTeksturos::ID(const char *pavad)
{
	for(int i = 0; i < tekstSk; i++)
		if(strcmp(pav[i], pavad) == 0)
		return tekst[i];
	Logas::Log("Nerasta tekstura \"%s\"", pavad);
	return 0;
}


void KTeksturos::Naikinti()
{
	if(uzkrauta && tekstSk > 0){
    	glDeleteTextures(tekstSk, tekst);
    	tekstSk = 0;
    	uzkrauta = false;
	}
}
