#include "teksto_laukas.hpp"


KTekstoLaukas::KTekstoLaukas(): x(10), y(10), dydisX(81), dydisY(4),
  trukme(4.0), alfaKoef(0.4), eil(NULL), alfa(NULL), ij(false)
{
	spalva[0] = 1.0f;
	spalva[1] = 0.0f;
	spalva[2] = 0.0f;
	spalva[3] = 1.0f;
}


KTekstoLaukas::~KTekstoLaukas()
{
}


void KTekstoLaukas::Sukurti()
{
	alfa = new float[dydisY + 1];
	eil = new char*[dydisY + 1];
	for(int i = 0; i < dydisY; i++){
		eil[i] = new char[dydisX];
		memset(eil[i], 0, dydisX);
		alfa[i] = 0.0f;
	}
	eil[dydisY] = NULL;
	alfa[dydisY] = 0.0f;
	ij = true;
}


void KTekstoLaukas::Sukurti(int dx, int dy)
{
	dydisX = dx;
	dydisY = dy;
	Sukurti();
}


void KTekstoLaukas::Prideti(const char *_eil)
{
	//Perstumti visus irasus
	for(int i = dydisY - 1; i >= 0; i--){
		eil[i + 1] = eil[i];
		alfa[i + 1] = alfa[i];
	}
	eil[0] = eil[dydisY];
	alfa[0] = trukme;
	strcpy(eil[0], _eil);
}


void KTekstoLaukas::Atnaujinti()
{
	for(int i = 0; i < dydisY; i++){
		alfa[i] -= laikas * alfaKoef;
		if(alfa[i] < 0.0f)
			alfa[i] = 0.0f;
	}
}


void KTekstoLaukas::Piesti()
{
	if(ij)
		for(int i = 0; i < dydisY; i++)
			if(alfa[i] > 0.01f){
				if(alfa[i] > 1.0f)
					glColor4fv(spalva);
				else
					glColor4f(spalva[0], spalva[1], spalva[2], alfa[i]);
				glRasterPos2i(x, y + (i * 12));
				tekstas.Piesti(eil[i]);
			}
}


void KTekstoLaukas::Valyti()
{
	for(int i = 0; i < dydisY; i++){
		memset(eil[i], 0, dydisX);
		alfa[i] = 0.0f;
	}
}


void KTekstoLaukas::Naikinti()
{
	if(eil != NULL){
		for(int i = 0; i < dydisY; i++)
			delete[] eil[i];
		delete[] eil;
		eil = NULL;
	}
	ij = false;
}
