#include <string>
#include <iostream>
#include <irrlicht.h>

using namespace irr;

class SolarSystem
{
public:
	core::array<core::vector3df> calculateAtDay(float d, int p);
	float day_number( int y, int m, int d, int hour, int mins);

private:
	float mod2pi( float x );
	float true_anomaly( float M, float e );
	float heliocentric_radius( float a, float e, float V);
	core::vector3df heliocentric_coordinates( float R, float O, float V, float w, float i );
	core::array<float> calculateMeanElements(int i, float d );
};
