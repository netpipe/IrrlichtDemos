#include "pele.hpp"


KPele::KPele(): naujRat(false), naujKair(false), naujDes(false), naujVid(false),
  dydisX(640), dydisY(480), pozXC(320), pozYC(240),
  x(0), y(0), xDiff(0), yDiff(0), rat(0), ratDiff(0), spKair(0),
  spDes(0), spVid(0), ij(true)
{
	/*aktyvParam[0] = 0.0f;
	aktyvParam[1] = 1.0f;
	aktyvParam[2] = 0.7f;
	aktyvParam[3] = 0.3f;*/
}


KPele::~KPele()
{
}


void KPele::Params(KNustatymai &nust)
{
	dydisX = nust.dydisX;
	dydisY = nust.dydisY;
	pozXC = nust.pozX + nust.dydisX / 2;
	pozYC = nust.pozY + nust.dydisY / 2;
	SetCursorPos(pozXC, pozYC);
	
}


void KPele::Ratukas(int pas)
{
	if(!ij || pas == 0){
		naujRat = false;
		ratDiff = 0;
		return;
	}
	naujRat = true;
	ratDiff = pas;
	rat += ratDiff;
}


void KPele::Kair(bool pasp)
{
	if(pasp)
		spKair = 2;
	else
		spKair = 1;
	naujKair = true;
}


void KPele::Des(bool pasp)
{
	if(pasp)
		spDes = 2;
	else
		spDes = 1;
	naujDes = true;
}


void KPele::Vid(bool pasp)
{
	if(pasp)
		spVid = 2;
	else
		spVid = 1;
	naujVid = true;
}


void KPele::NustPoz(int _x, int _y)
{
	SetCursorPos(_x, _y);
	x = _x;
	y = _y;
}


void KPele::Atnaujinti()
{
	if(!ij)
		return;
	
	//Pozicijos atnaujinimas
	GetCursorPos(&poz);
	xDiff = poz.x - pozXC;
	yDiff = pozYC - poz.y;
	x += xDiff;
	y += yDiff;
	if(x < 0)		x = 0;
	if(x >= dydisX)	x = dydisX - 1;
	if(y < 0)		y = 0;
	if(y >= dydisY)	y = dydisY - 1;
	if(xDiff != 0 || yDiff != 0)
		SetCursorPos(pozXC, pozYC);
	
	//Ratuko atnaujinimas
	if(naujRat)
		naujRat = false;
	else
		ratDiff = 0;
	//Kairio spauduko atnaujinimas
	if(naujKair)
		naujKair = false;
	else{
		if(spKair == 1)
			spKair = 0;
		if(spKair == 2)
			spKair = 3;
	}
	//Desinio spauduko atnaujinimas
	if(naujDes)
		naujDes = false;
	else{
		if(spDes == 1)
			spDes = 0;
		if(spDes == 2)
			spDes = 3;
	}
	//Vidurinio spauduko atnaujinimas
	if(naujVid)
		naujVid = false;
	else{
		if(spVid == 1)
			spVid = 0;
		if(spVid == 2)
			spVid = 3;
	}
	
	
	//Aktyvumo laikas
	/*if(abs(pozXDiff) > 0 || abs(pozYDiff) > 0){
		aktyvLaik += laik;
		if(aktyvLaik > aktyvParam[1])
			aktyvLaik = aktyvParam[1];
		//Tikrinti, ar reikia ijungti aktyvumo
		if(!aktyvi && aktyvLaik > aktyvParam[2])
			aktyvi = true;
	}
	else{
		aktyvLaik -= laik;
		if(aktyvLaik <= aktyvParam[0])
			aktyvLaik = aktyvParam[0];
		//Tikrinti, ar reikia isjungti aktyvumo
		if(aktyvi && aktyvLaik < aktyvParam[3])
			aktyvi = false;
	}*/
}
