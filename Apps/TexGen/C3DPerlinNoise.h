#pragma once

namespace TexGen
{

using namespace irr;

class C3DPerlinNoise
{
private:

	s32 m_p[512];

	double Noise (double x, double y, double z);
	double fade(double t);
	double lerp(double t, double a, double b);
	double grad(int hash, double x, double y, double z);

	u32 m_RandSeed;

public:
	C3DPerlinNoise( u32 seed  = 0 );
	~C3DPerlinNoise(void);

	double fBm(double x, double y, double z, int octaves, float lacunarity, float gain);
	double RidgedMF(double x, double y, double z, int octaves, float lacunarity, float gain, float offset);
	double ridge(double h, float offset);

};

}	// namespace texgen