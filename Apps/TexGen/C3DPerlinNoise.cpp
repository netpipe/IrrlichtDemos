#include "StdAfx.h"
#include "C3DPerlinNoise.h"

namespace TexGen
{

C3DPerlinNoise::C3DPerlinNoise( u32 seed )
{

	// Create an instance of the random number generator, making sure to pass in a seed.
	m_RandSeed = seed;
	srand(m_RandSeed);

	// Use the bitwise left shift operator to make sure the array of permutation values is a multiple of 2
	int nbVals = (1 << 8); // result is 256
	int *ms_perm = new int[nbVals];

	// set values in temp perm array as "unused", denoted by -1
	for (int i = 0; i < nbVals; i++)
	{
		ms_perm[i] = -1;
	}

	for (int i = 0; i < nbVals; i++)
	{
		// for each value, find an empty spot, and place it in it
		while (true)
		{
			// generate rand # with max a nbvals
			int p = rand() % nbVals;
			if (ms_perm[p] == -1)
			{
				ms_perm[p] = i;
				break;
			}
		}
	}

	for (int i = 0; i < nbVals; i++)
	{
		m_p[nbVals + i] = m_p[i] = ms_perm[i];
	}
}


C3DPerlinNoise::~C3DPerlinNoise(void)
{
}

double C3DPerlinNoise::Noise(double x, double y, double z)
{

	// Find unit grid cell containing point
	int X = (int)floor(x) & 255;
	int Y = (int)floor(y) & 255;
	int Z = (int)floor(z) & 255;

	// Get relative xyz coord within the cell
	x = x - floor(x);
	y = y - floor(y);
	z = z - floor(z);

	// calculate the fade values
	double u = fade(x);
	double v = fade(y);
	double w = fade(z);

	int A = m_p[X    ] + Y, AA = m_p[A] + Z, AB = m_p[A + 1] + Z;
	int B = m_p[X + 1] + Y, BA = m_p[B] + Z, BB = m_p[B + 1] + Z;

	return lerp(w, lerp(v, lerp(u, grad(m_p[AA  ], x  , y  , z   ),
                                      grad(m_p[BA  ], x - 1, y  , z   )),
                             lerp(u, grad(m_p[AB  ], x  , y - 1, z   ),
                                      grad(m_p[BB  ], x - 1, y - 1, z   ))),
                   lerp(v, lerp(u, grad(m_p[AA + 1], x  , y  , z - 1 ),
                                      grad(m_p[BA + 1], x - 1, y  , z - 1 )),
                             lerp(u, grad(m_p[AB + 1], x  , y - 1, z - 1 ),
                                      grad(m_p[BB + 1], x - 1, y - 1, z - 1 ))));

}

double C3DPerlinNoise::fade(double t)
{
	return (t * t * t * (t * (t * 6 - 15) + 10));
}

double C3DPerlinNoise::lerp(double t, double a, double b)
{
	return (a + t * (b - a));
}

double C3DPerlinNoise::grad(int hash, double x, double y, double z)
{
	int h = hash & 15;
	double u = h<8 ? x : y;
	double v = h<4 ? y : h==12||h==14 ? x : z;

	return ((h&1) == 0 ? u : -u) + ((h&2) == 0 ? v : -v);
}

double C3DPerlinNoise::fBm(double x, double y, double z, int octaves, float lacunarity, float gain)
{
    double frequency = 1.0f;
    double amplitude = 1.0f;
	double amplitudeSum = 0.0f;
    double sum = 0.0f;

    for (int i = 0; i < octaves; i++)
    {
        sum += Noise(x * frequency, y * frequency, z * frequency) * amplitude;
        frequency *= lacunarity;
		amplitudeSum += amplitude;
        amplitude *= gain;

    }
    return sum / amplitudeSum;
}

double C3DPerlinNoise::RidgedMF(double x, double y, double z, int octaves, float persistance, float gain, float offset)
{
	double sum = 0.0f;
	float amplitude = 0.5f;
	float frequency = 1.0f;
	double prev = 1.0f;

	for (int i = 0; i < octaves; i++)
	{
		double n = ridge(Noise(x * frequency, y * frequency, z * frequency), offset);
		sum += n * amplitude * prev;
		prev = n;
		frequency *= persistance;
		amplitude *= gain;
	}

	return sum;
}

double C3DPerlinNoise::ridge(double h, float offset)
{
	h = abs(h);
	h = offset - h;
	h = h * h;
	return h;
}

} // namespace texgen
