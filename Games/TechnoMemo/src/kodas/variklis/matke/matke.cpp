#include "matke.hpp"


//-----------------------------------------------------------------------------------------------------------------------------
//Bendros funkcijos -----------------------------------------------------------------------------------------------------------

float abs(float sk)
{
	if(sk >= 0.0f)
		return sk;
	else
		return (sk * -1.0f);
}


//-----------------------------------------------------------------------------------------------------------------------------
//Skaiciuojami atstumai tarp dvieju tasku -------------------------------------------------------------------------------------
/*
float Matke::Atstumas2D(float x1, float y1, float x2, float y2)
{
	return sqrt(((x2 - x1) * (x2 - x1)) + ((y2 - y1) * (y2 - y1)));
}

float Matke::Atstumas2D(float *t1, float *t2)
{
	return sqrt(((t2[0] - t1[0]) * (t2[0] - t1[0])) + ((t2[1] - t1[1]) * (t2[1] - t1[1])));
}

float Matke::Atstumas2D(STaskas &t1, STaskas &t2)
{
	return sqrt(((t2.x - t1.x) * (t2.x - t1.x)) + ((t2.y - t1.y) * (t2.y - t1.y)));
}

float Matke::Atstumas2D(float *sk)
{
	return sqrt(((sk[2] - sk[0]) * (sk[2] - sk[0])) + ((sk[3] - sk[1]) * (sk[3] - sk[1])));
}

float Matke::Atstumas2D(STaskas &t1)
{
	return sqrt(t1.x * t1.x + t1.y * t1.y);
}


//-----------------------------------------------------------------------------------------------------------------------------
//Skaiciuojami atstumu tarp dvieju tasku kvadratai ----------------------------------------------------------------------------

float Matke::Atstumas2D2(float x1, float y1, float x2, float y2)
{
	return (((x2 - x1) * (x2 - x1)) + ((y2 - y1) * (y2 - y1)));
}

float Matke::Atstumas2D2(float *t1, float *t2)
{
	return (((t2[0] - t1[0]) * (t2[0] - t1[0])) + ((t2[1] - t1[1]) * (t2[1] - t1[1])));
}

float Matke::Atstumas2D2(STaskas &t, float x, float y)
{
	return (((x - t.x) * (x - t.x)) + ((y - t.y) * (y - t.y)));
}

float Matke::Atstumas2D2(STaskas &t1, STaskas &t2)
{
	return (((t2.x - t1.x) * (t2.x - t1.x)) + ((t2.y - t1.y) * (t2.y - t1.y)));
}

float Matke::Atstumas2D2(float *sk)
{
	return (((sk[2] - sk[0]) * (sk[2] - sk[0])) + ((sk[3] - sk[1]) * (sk[3] - sk[1])));
}


//-----------------------------------------------------------------------------------------------------------------------------
//Skaiciuojami atstumai tarp dvieju tasku (3D) --------------------------------------------------------------------------------

float Matke::Atstumas3D(float x1, float y1, float z1, float x2, float y2, float z2)
{
	return sqrt(((x2 - x1) * (x2 - x1)) + ((y2 - y1) * (y2 - y1)) + ((z2 - z1) * (z2 - z1)));
}

float Matke::Atstumas3D(float *t1, float *t2)
{
	return sqrt(((t2[0] - t1[0]) * (t2[0] - t1[0])) + ((t2[1] - t1[1]) * (t2[1] - t1[1])) + ((t2[2] - t1[2]) * (t2[2] - t1[2])));
}

float Matke::Atstumas3D(STaskas &t1, STaskas &t2)
{
	return sqrt(((t2.x - t1.x) * (t2.x - t1.x)) + ((t2.y - t1.y) * (t2.y - t1.y)) + ((t2.z - t1.z) * (t2.z - t1.z)));
}

float Matke::Atstumas3D(float *sk)
{
	return sqrt(((sk[3] - sk[0]) * (sk[3] - sk[0])) + ((sk[4] - sk[1]) * (sk[4] - sk[1])) + ((sk[5] - sk[2]) * (sk[5] - sk[2])));
}


//-----------------------------------------------------------------------------------------------------------------------------
//Skaiciuojami atstumu tarp dvieju tasku (3D) kvadratai -----------------------------------------------------------------------

float Matke::Atstumas3D2(float x1, float y1, float z1, float x2, float y2, float z2)
{
	return (((x2 - x1) * (x2 - x1)) + ((y2 - y1) * (y2 - y1)) + ((z2 - z1) * (z2 - z1)));
}

float Matke::Atstumas3D2(float *t1, float *t2)
{
	return (((t2[0] - t1[0]) * (t2[0] - t1[0])) + ((t2[1] - t1[1]) * (t2[1] - t1[1])) + ((t2[2] - t1[2]) * (t2[2] - t1[2])));
}

float Matke::Atstumas3D2(STaskas &t, float x, float y, float z)
{
	return (((x - t.x) * (x - t.x)) + ((y - t.y) * (y - t.y)) + ((z - t.z) * (z - t.z)));
}

float Matke::Atstumas3D2(STaskas &t1, STaskas &t2)
{
	return (((t2.x - t1.x) * (t2.x - t1.x)) + ((t2.y - t1.y) * (t2.y - t1.y)) + ((t2.z - t1.z) * (t2.z - t1.z)));
}

float Matke::Atstumas3D2(float *sk)
{
	return (((sk[3] - sk[0]) * (sk[3] - sk[0])) + ((sk[4] - sk[1]) * (sk[4] - sk[1])) + ((sk[5] - sk[2]) * (sk[5] - sk[2])));
}


//-----------------------------------------------------------------------------------------------------------------------------
//Skaiciuojamas kampas tarp duotos linijos ir Y+ asies ------------------------------------------------------------------------

float Matke::Kampas(float x1, float y1, float x2, float y2)
{
	float kamp =  -90.0 / M_PI_2 * asin((x2 - x1) / sqrt(((x2 - x1) * (x2 - x1)) + ((y2 - y1) * (y2 - y1))));
	if(y2 < y1){
		if(x2 > x1) kamp = -180.0 - kamp;
		else kamp = 180 - kamp;
	}
	//if(x2 < x1 && y2 < y1)
	//	kamp = 180.0f - kamp;
	//if(x2 > x1 && y2 > y1)
	//	kamp = -kamp;
	//if(x2 > x1 && y2 < y1)
	//	kamp -= 180.0f;
	return kamp;
	//kh = 90.0 / M_PI_2 * asin(sqrt((vektNorm.x * vektNorm.x) / ((vektNorm.x * vektNorm.x) + (vektNorm.y * vektNorm.y))));
}

float Matke::Kampas(float *t1, float *t2)
{
	return (90.0 / M_PI_2 * asin(sqrt(((t2[0] - t1[0]) * (t2[0] - t1[0])) / (((t2[0] - t1[0]) * (t2[0] - t1[0])) + ((t2[1] - t1[1]) * (t2[1] - t1[1]))))));
}

float Matke::Kampas(STaskas &t1, STaskas &t2){
	//return (90.0 / M_PI_2 * asin(sqrt(((t2.x - t1.x) * (t2.x - t1.x)) / (((t2.x - t1.x) * (t2.x - t1.x)) + ((t2.y - t1.y) * (t2.y - t1.y))))));
	float kamp = -90.0 / M_PI_2 * asin((t2.x - t1.x) / sqrt(((t2.x - t1.x) * (t2.x - t1.x)) + ((t2.y - t1.y) * (t2.y - t1.y))));
	if(t2.y < t1.y){
		if(t2.x > t1.x) kamp = -180.0 - kamp;
		else kamp = 180 - kamp;
	}
	return kamp;
}

float Matke::Kampas(STaskas &t1)
{
	float kamp = 90.0 / M_PI_2 * asin(sqrt((t1.x * t1.x) / ((t1.x * t1.x) + (t1.y * t1.y))));
	if(t1.x < 0.0f && t1.y < 0.0f)		kamp = -180.0f - kamp;
	else if(t1.x > 0.0f && t1.y < 0.0f)	kamp -= 180.0f;
	else if(t1.x > 0.0f && t1.y > 0.0f)	kamp *= -1.0f;
	while(kamp > 180.0f)	kamp -= 360.0f;
	while(kamp < -180.0f)	kamp += 360.0f;
	return kamp;
}


//-----------------------------------------------------------------------------------------------------------------------------
//Skaiciuojamas dvieju tiesiu susikirtimo taskas ------------------------------------------------------------------------------

STaskas	Matke::KoefKB(float *t1, float *t2)
{
	//k = (t2y - t1y) / (t2x - t1x);
	//b = t1y - (t1x * k);
	STaskas koef;
	koef.x = (t2[1] - t1[1]) / (t2[0] - t1[0]);
	koef.y = t1[1] - (t1[0] * koef.x);
	return koef;
}

STaskas	Matke::KoefKB(STaskas &t1, STaskas &t2)
{
	STaskas koef;
	koef.x = (t2.y - t1.y) / (t2.x - t1.x);
	koef.y = t1.y - (t1.x * koef.x);
	return koef;
}

STaskas Matke::Susikirtimas(float *t1, float *t2)
{
	//t1, t2 - tiesiu koeficientai k ir b
	//sx = (b1 - b2) / (k2 - k1);
	//sy = k1 * sx + b1;
	STaskas sus;
	sus.x = (t1[1] - t2[1]) / (t2[0] - t1[0]);
	sus.y = sus.x * t1[0] + t1[1];
	return sus;
}

STaskas Matke::Susikirtimas(float *t1, STaskas &t2)
{
	STaskas sus;
	sus.x = (t1[1] - t2.y) / (t2.x - t1[0]);
	sus.y = sus.x * t1[0] + t1[1];
	return sus;
}

STaskas Matke::Centras(STaskas *pab, float *t1d, float *t1k, float *t2d, float *t2k)
{
	//pab - apskritimo lanko pabaigos taskas
	//td(5) - atkarpos duomenys - pradzios ir pabaigos taskai, kampas
	//tk(2) - tieses koeficientai - k ir b
	//centras x, y - apskritimo pozicija, z - apskritimo spindulys
	STaskas centras, tarp;
	
	////Keliu susikirtimo taskas
	//	taskas = matke.Susikirtimas(&keliaiKr[4], &keliaiKr[16]);
	//	//Kampas tarp keliu
	//	sk1 = (keliai[24] * 0.5) + (keliai[9] * 0.5) + 90.0;
	//	//Tieses tarp keliu k ir b koeficientai
	//	taskasPab.x = taskas.x - sin(sk1 / 180.0 * M_PI);
	//	taskasPab.y = taskas.y + cos(sk1 / 180.0 * M_PI);
	//	taskasKoef = matke.KoefKB(taskas, taskasPab);
	//	//Apskritimo centras
	//	taskasC = matke.Susikirtimas(&keliaiKr[18], taskasKoef);
	//	apskrIlg = matke.Atstumas2D(taskasC.x, taskasC.y, keliai[22], keliai[23]);
	
	
	//Tiesiu susikirtimo taskas
	*pab = Susikirtimas(t1k, t2k);
	//Kampas tarp tiesiu
	pab->z = (t1d[4] * 0.5) + (t2d[4] * 0.5) + 90.0;
	//Tarpines tieses koeficientai
	tarp.x = pab->x - sin(pab->z / 180.0 * M_PI);
	tarp.y = pab->y + cos(pab->z / 180.0 * M_PI);
	tarp = KoefKB(*pab, tarp);
	//Apskritimo centras
	centras = Susikirtimas(&t1k[2], tarp);
	//Spindulys
	centras.z = Atstumas2D(centras.x, centras.y, t1d[2], t1d[3]);
	//Lanko pabaigos taskas
	pab->z = Atstumas2D(pab->x, pab->y, t1d[2], t1d[3]);
	pab->x -= sin(t2d[4] / 180.0 * M_PI) * pab->z;	
	pab->y += cos(t2d[4] / 180.0 * M_PI) * pab->z;
	
	return centras;
}
*/

//-----------------------------------------------------------------------------------------------------------------------------
//Kampu palyginimas -----------------------------------------------------------------------------------------------------------

float Matke::KampaiMod(float k1, float k2)
{
	float km = KampaiSkirt(k1, k2);
	if(km < 0.0) km *= -1.0;
	return km;
}

float Matke::KampaiSkirt(float k1, float k2)
{
	//while(k1 < 0.0) k1 += 360.0;
	//while(k1 > 360.0) k1 -= 360.0;
	//while(k2 < 0.0) k2 += 360.0;
	//while(k2 > 360.0) k2 -= 360.0;
	float km = k2 - k1;
	while(km > 180.0) km -= 360.0;
	while(km < -180.0) km += 360.0;
	return km;
}

float Matke::KampaiSkirtMod(float k1, float k2)
{
	float km = k2 - k1;
	while(km > 180.0) km -= 360.0;
	while(km < -180.0) km += 360.0;
	if(km < 0.0f)
		km *= -1.0f;
	return km;
}

void Matke::KampasMod(float *k1)
{
	while(*k1 < -180.0f)
		*k1 += 360.0f;
	while(*k1 > 180.0f)
		*k1 -= 360.0f;
}

float Matke::KampasMod(float k1)
{
	while(k1 < -180.0f)
		k1 += 360.0f;
	while(k1 > 180.0f)
		k1 -= 360.0f;
	return k1;
}

//-----------------------------------------------------------------------------------------------------------------------------
//Skaiciu palyginimas ---------------------------------------------------------------------------------------------------------

float Matke::SkirtMod(float k1, float k2)
{
	if(k1 > k2)
		return (k1 - k2);
	else
		return (k2 - k1);
}

int Matke::SkirtMod(int k1, int k2)
{
	if(k1 > k2)
		return (k1 - k2);
	else
		return (k2 - k1);
}

float Matke::Mod(float sk, float mod)
{
	float ats = sk;
	while(ats > mod)
		ats -= mod;
	return ats;
}

float Matke::Zenklas(float sk)
{
	if(sk > 0.0f)
		return 1.0f;
	else
		return -1.0f;
}
