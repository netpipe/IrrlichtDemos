#include "valdymas.hpp"


KValdymas::KValdymas():sukurta(false)
{
	for(int i = 0; i < SP_DYDIS; i++){
		iSp[i] = 0;
		fSp[i] = 0.0;
		mapKlav[i] = 0;
		mapJoy[i] = 0;
	}
}


KValdymas::~KValdymas()
{
}


void KValdymas::Sukurti()
{
	sukurta = true;
}


void KValdymas::Atnaujinti()
{
	if(!sukurta)
		return;
	for(int i = 0; i < SP_DYDIS; i++)
		iSp[i] = klav.sp[mapKlav[i]];
	if(JoystickDevs::sk > 0){
		
	}
}


void KValdymas::Naikinti()
{
}
