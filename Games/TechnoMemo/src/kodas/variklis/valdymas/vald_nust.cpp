#include "vald_nust.hpp"


KValdNust::KValdNust(): sukurta(false)
{
	for(int i = 0; i < SP_DYDIS; i++){
		iSp[i] = 0;
		fSp[i] = 0.0;
		mapKlav[i] = 0;
		mapJoy[i] = 0;
	}
}


KValdNust::~KValdNust()
{
}


void KValdNust::Sukurti()
{
	sukurta = true;
}


void KValdNust::Atnaujinti()
{
	if(!sukurta)
		return;
	for(int i = 0; i < SP_DYDIS; i++)
		iSp[i] = klav.sp[mapKlav[i]];
	if(JoystickDevs::sk > 0){
		
	}
}


void KValdNust::Naikinti()
{
}
