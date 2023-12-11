/*
  CWindGenerator.h - thank you Klasker :)
  http://irrlicht.sourceforge.net/phpBB2/viewtopic.php?t=10762&start=15
 */

#include "IWindGenerator.h"

namespace irr
{
namespace scene
{

class CWindGenerator : public IWindGenerator
{

public:

	inline void setStrength(const f32 strength) { Strength = strength; }

	inline void setRegularity(const f32 regularity) { Regularity = regularity; }

	inline f32 getStrength() const { return Strength; }

	inline f32 getRegularity() const { return Regularity; }

	const core::vector2df& getWind(const core::vector3df& position, const u32 timeMs);

private:

	f32 Strength;

	f32 Regularity;
};

f32 rndGenerator(s32 x)
{
	x = (x<<13) ^ x;
	return ( (f32)(1.0 - ( (x * (x * x * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0));
}

f32 cosInterpolater(const f32 a, const f32 b, const f32 x)
{
	const f32 ft = x * 3.1415927f;
	const f32 f = (1 - cos(ft)) * 0.5f;

	return a*(1-f) + b*f;
}

f32 windSmoother(const s32 x)
{
	return rndGenerator(x)/2  +  rndGenerator(x-1)/4  +  rndGenerator(x+1)/4;
}

f32 noiseInterpolate(const f32 x)
{
	const s32 intX = (int)(x);
	const f32 fracX = x - intX;

	const f32 v1 = windSmoother(intX);
	const f32 v2 = windSmoother(intX + 1);

	return cosInterpolater(v1 , v2 , fracX);
}

f32 noise(const f32 x)
{
	f32 total = 0;
	const f32 p = 0.50;

	const s32 n = 4;

	f32 frequency = 1.0f;
	f32 amplitude = 1.0f;

	s32 i;
	for (i = 0; i < n; ++i)
	{
		total += noiseInterpolate(x*frequency)*amplitude;

		frequency = frequency + frequency;
		amplitude *= p;
	}

	return total;
}

const core::vector2df& CWindGenerator::getWind(const core::vector3df& position, const u32 timeMs)
{
	const f32 seed = (timeMs + position.X*7*cos(timeMs/120000.0f) + position.Z*7*sin(timeMs/120000.0f))/1000.0f;
	const f32 dir = 2*core::PI*noise(seed/Regularity);
	const f32 amp = Strength*sin(seed);

	this->wind.X = amp*cos(dir);
	this->wind.Y = amp*sin(dir);

	return this->wind;
}

IWindGenerator* createWindGenerator(const f32 strength, const f32 regularity)
{
	CWindGenerator* const wind = new CWindGenerator();
	wind->setStrength(strength);
	wind->setRegularity(regularity);

	return wind;
}

} // namespace scene
} // namespace irr

