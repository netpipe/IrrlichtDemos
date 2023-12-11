#include "tekstura_duom.hpp"


KTeksturaDuom::KTeksturaDuom(): duom(0), x(0), y(0), bpp(0), duomDydis(0)
{
}


KTeksturaDuom::~KTeksturaDuom()
{
}


GLuint KTeksturaDuom::Sukurti()
{
	glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    //_TODO: teksturos parametrus perkelti i kintamuosius
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, glTipas1, x, y, 0, glTipas2, glFormatas, duom);
	int klaida = gluBuild2DMipmaps(GL_TEXTURE_2D, glTipas1, x, y, glTipas2, glFormatas, duom);
	#ifdef DEBUG
	if(klaida != 0)
		Logas::Log("TeksturaDuom->Sukurti->Klaida generuojant MipMap'us");
	#endif
	glBindTexture(GL_TEXTURE_2D, 0);
	return id;
}
