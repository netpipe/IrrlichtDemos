#include "kamera.hpp"


KKamera::KKamera(): k(10.0, -50.0, 10.0), t(0.0, 0.0, 0.0), up(0.0, 0.0, 1.0),
  fov(45.0), ratio(4.0 / 3.0), arti(0.01), toli(1000.0),
  gr(0.0), gr2(0.0), grInert(0.0), atejo(true)
{
	PozToKampas();
}


KKamera::~KKamera()
{
}


void KKamera::Sukurti()
{
	ratio = float(nust.dydisX) / nust.dydisY;
}


void KKamera::Atnaujinti()
{
	float dif;
	
	//Atnaujinti greiti
	if(gr < gr2){
		gr += grInert * laikas.kadro;
		if(gr > gr2)
			gr = gr2;
	}
	else if(gr > gr2){
		gr -= grInert * laikas.kadro;
		if(gr < gr2)
			gr = gr2;
	}
	
	//Atnaujinti kameros pozicija (skristi link naujos pozicijos)
	k += (k2 - k).IlgisMaks(gr * laikas.kadro);
	
	//Atnaujinti taikinio pozicija (skristi link naujos taikinio pozicijos)
	t += (t2 - t).IlgisMaks(gr * laikas.kadro);
	
	if((k2 - k).Ilgis() < 0.01 && (t2 - t).Ilgis() < 0.01)
		atejo = true;
	
	PozToKampas();
}


void KKamera::KampaiMod()
{
	while(kh < -M_PI)
		kh += M_PI * 2;
	while(kh > M_PI)
		kh -= M_PI * 2;
	while(kv < -M_PI_2)
		kv += M_PI;
	while(kv > M_PI_2)
		kv -= M_PI;
}


void KKamera::PozToKampas()
{
	float x = t.x - k.x;
	float y = t.y - k.y;
	float z = t.z - k.z;
	kh = asin(x / sqrt(x * x + y * y));
	kv = asin(z / sqrt(x * x + y * y + z * z));
	if(y < 0.0){
		if(x > 0.0)
			kh = M_PI - kh;
		else
			kh = -M_PI - kh;
	}
}


void KKamera::KampasToPoz()
{
	KampaiMod();
	t.x = sin(kh) * cos(kv) + k.x;
	t.y = cos(kh) * cos(kv) + k.y;
	t.z = sin(kv) + k.z;
}


void KKamera::NustKam(float _x, float _y, float _z)
{
	k.x = _x;
	k.y = _y;
	k.z = _z;
}


void KKamera::NustTaik(float _x, float _y, float _z)
{
	t.x = _x;
	t.y = _y;
	t.z = _z;
}


void KKamera::Pasukti(float horiz, float vert)
{
	kh += horiz;
	kv += vert;
	KampasToPoz();
}


void KKamera::Eiti(float _x, float _y, float _z)
{
	float csh = cos(kh);
	float csv = cos(kv);
	float snh = sin(kh);
	float snv = sin(kv);
	//			x				y				z
	k.x +=  csh * csv * _x + snh * csv * _y + 0.0;
	k.y += -snh * csv * _x + csh * csv * _y + 0.0;
	k.z +=  0.0 * csv * _x +       snv * _y + csv * _z;
	KampasToPoz();
}


void KKamera::Eiti(KTaskas *_k = NULL, KTaskas *_t = NULL)
{
	if(_k != NULL)
		k2 = _k;
	if(_t != NULL)
		t2 = _t;
	atejo = false;
}


void KKamera::EitiKam(float kx, float ky, float kz)
{
	k2.x = kx;
	k2.y = ky;
	k2.z = kz;
	atejo = false;
}


void KKamera::EitiTaik(float tx, float ty, float tz)
{
	t2.x = tx;
	t2.y = ty;
	t2.z = tz;
	atejo = false;
}
