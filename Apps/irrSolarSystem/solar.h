#include <string>
//#include <direct.h>
#include <iostream>
#include <irrlicht.h>

using namespace irr;

class SolarSystem
{
public:
	SolarSystem();
	//ouch
	core::array<core::array<core::vector3df> > calculateFieldLines(int year, int dayM, int hour, int minute, int second);
	core::array<core::vector3df> calculateAtDay(f64 d, int p);
	f64 gregToJ2000( int y, int m, int d, int hour, int mins, int secs);
	core::stringw J2000ToGreg(f64 JD);
	int year;
	int month;
	int day;
	int dayM;
	int hour;
	int minute;
	int second;
	~SolarSystem();

private:
	f64 mod2pi( f64 x );
	f64 true_anomaly( f64 M, f64 e );
	f64 heliocentric_radius( f64 a, f64 e, f64 V);
	core::vector3df heliocentric_coordinates( f64 R, f64 O, f64 V, f64 w, f64 i );
	core::array<f64> calculateMeanElements(int i, f64 d );

};
