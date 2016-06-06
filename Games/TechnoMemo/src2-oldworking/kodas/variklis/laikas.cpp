#include "laikas.hpp"


/*KLaikmatis::KLaikmatis()
{
}


KLaikmatis::~KLaikmatis()
{
}


void KLaikmatis::Naikinti()
{
}*/


//------------------------------------------------------------------------------


KLaikas::KLaikas()
{
}


KLaikas::~KLaikas()
{
}


void KLaikas::Sukurti()
{
    QueryPerformanceFrequency(&daznis);
    QueryPerformanceCounter(&prad);
	
	dab = prad;
    kadro = 0.0f;
    fpsLaik = 0.0f;
	fps = 0;
	fpsSk = 0;
	gr = 1.0;
	stop = false;
}


void KLaikas::Gauti()
{
    fpsSk++;
    pask = dab;
    
    //Gaunamas dabartinis laikas
    QueryPerformanceCounter(&dab);
    kadro = (float)(dab.QuadPart - pask.QuadPart) / daznis.QuadPart;
    veikimo = (float)(dab.QuadPart - prad.QuadPart) / daznis.QuadPart;
    
    fpsLaik += kadro;
    kadro *= gr;
    if(fpsLaik > 1.0f){
        fpsLaik -= 1.0f;
        fps = fpsSk;
        fpsSk = 0;
    }
    if(stop)
    	kadro = 0.0;
}


void KLaikas::Stabdyti()
{
	stop = true;
}


void KLaikas::Paleisti()
{
	stop = false;
}


/*unsigned int KLaikas::Laikmatis(float)
{
	return 0;
}


bool KLaikas::Baigesi(unsigned int)
{
	return true;
}


void KLaikas::LaikmatisNaikinti(unsigned int)
{
}*/
