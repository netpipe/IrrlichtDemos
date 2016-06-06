#include "taskas.hpp"


void KTaskas::operator = (const KTaskas t)
{
	this->x = t.x;
	this->y = t.y;
	this->z = t.z;
}


void KTaskas::operator = (const KTaskas *t)
{
	this->x = t->x;
	this->y = t->y;
	this->z = t->z;
}


void KTaskas::operator += (const KTaskas t)
{
	this->x += t.x;
	this->y += t.y;
	this->z += t.z;
}


void KTaskas::operator += (const KTaskas *t)
{
	this->x += t->x;
	this->y += t->y;
	this->z += t->z;
}


KTaskas * KTaskas::operator *= (float val)
{
	this->x *= val;
	this->y *= val;
	this->z *= val;
	return this;
}


KTaskas KTaskas::operator - (const KTaskas t)
{
	return *(new KTaskas(this->x - t.x, this->y - t.y, this->z - t.z));
}


KTaskas KTaskas::operator - (const KTaskas *t)
{
	return *(new KTaskas(this->x - t->x, this->y - t->y, this->z - t->z));
}


float KTaskas::operator ^ (KTaskas t)
{
	return (*this ^ &t);
}


float KTaskas::operator ^ (KTaskas *t)
{
	return ((this->x * t->x + this->y * t->y + this->z * t->z) / this->Ilgis() / t->Ilgis());
}


//------------------------------------------------------------------------------


KTaskas::KTaskas()
{
	x = y = z = 0.0;
}


KTaskas::KTaskas(float _x, float _y, float _z)
{
	x = _x;
	y = _y;
	z = _z;
}


KTaskas::~KTaskas()
{
}


//------------------------------------------------------------------------------


//Grazinamas vektoriaus ilgis
const float KTaskas::Ilgis()
{
	return sqrt(x * x + y * y + z * z);
}


//Vektoriaus ilgis nustatomas i ilg
KTaskas * KTaskas::Ilgis(float ilg)
{
	return (*this *= ilg / Ilgis());
}


//Jei vektoriaus ilgis didesnis uz maks, sumazinamas iki maks
KTaskas * KTaskas::IlgisMaks(float maks)
{
	return ((this->Ilgis() > maks) ? this->Ilgis(maks) : this);
}


//Skaiciuojamas atstumas iki tasko task
float KTaskas::Atstumas(KTaskas &task)
{
	return sqrt((x - task.x) * (x - task.x) + (y - task.y) * (y - task.y) + (z - task.z) * (z - task.z));
}


//Skaiciuojama plokstumos normale (pagal pasisukimo kampus)
void KTaskas::Normale(float kh, float kv)
{
	float x1, x2, y1, y2, z1, z2;
	x1 = cos(kh * M_PI / 180.0);
	y1 = sin(kh * M_PI / 180.0);
	z1 = 0.0;
	x2 = -sin(kh * M_PI / 180.0) * cos(kv * M_PI / 180.0);
	y2 = cos(kh * M_PI / 180.0) * cos(kv * M_PI / 180.0);
	z2 = sin(kv * M_PI / 180.0);
	
	x = y1 * z2 - z1 * y2;
	y = z1 * x2 - x1 * z2;
	z = x1 * y2 - y1 * x2;
}
