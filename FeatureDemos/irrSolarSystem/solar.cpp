/*
Original source code by Stephen R. Schmitt written in zeno. Ported to mel and c++ by Tobias Houfek.
http://home.att.net/~srschmitt/planetorbits.html
The Irrlicht Engine License

Copyright © 2002-2005 Nikolaus Gebhardt

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.

*/
#include <string>
#include <iostream>
#include <irrlicht.h>
#include "solar.h"

#pragma comment(lib,"irrlicht.lib")
using namespace irr;


// ----------------------------------------------------------------------------------------------------------------------
// day number to/from J2000 (12:00 UT, Jan 1, 2000)
float SolarSystem::day_number( int y, int m, int d, int hour, int mins)
{
	float rv;
	float h = float(hour + mins) / 60;
	rv = 367.0f * y - floor(7.0f*(y + floor((m + 9.0f)/12.0f))/4.0f) + floor(275.0f*m/9.0f) + d - 730531.5f + h/24.0f;
	return rv;
}
// ----------------------------------------------------------------------------------------------------------------------
// return an angle in the range 0 to 2pi radians
float SolarSystem::mod2pi( float x )
{
	float b = x / (2*core::PI);
	float a = (2*core::PI)*( b - abs(floor(b)) );
	if (a < 0){
		a = (2*core::PI) + a;
	}
	return a;
}
// ----------------------------------------------------------------------------------------------------------------------
// compute the true anomaly from mean anomaly using iteration
//  M - mean anomaly in radians
//  e - orbit eccentricity
float SolarSystem::true_anomaly( float M, float e )
{
	float V;
	float E1;
	float degs = 180 / core::PI;
	float rads = core::PI / 180;
	float eps  = 0.000001;

	// initial approximation of eccentric anomaly
	float E = M + e * sin(M)*(1.0f + e* cos(M));
	// iterate to improve accuracy

	do
	{
		E1 = E;
		E = E1 - (E1 - e* sin(E1) - M)/(1.0f - e* cos(E1));
	}
	while (abs( E - E1 ) > eps);

	// convert eccentric anomaly to true anomaly
	V = 2.0f*atan(sqrt((1.0f + e)/(1.0f - e))* tan(0.5f*E));

	if (V < 0) {
		// modulo 2pi
		V = V + (2*core::PI);
	}
	return V;
}
// ----------------------------------------------------------------------------------------------------------------------
// compute helio-centric-coordinates
core::vector3df SolarSystem::heliocentric_coordinates( float R, float O, float V, float w, float i )
{
	core::vector3df HelioCoords;
	float AU = 149598000.0;
	float XH = R*(cos(O)*cos(V + w - O) - sin(O)*sin(V + w - O)*cos(i));
	float YH = R*(sin(O)*cos(V + w - O) + cos(O)*sin(V + w - O)*cos(i));
	float ZH = R*(sin(V + w - O)*sin(i));
	HelioCoords = core::vector3df((YH*AU), (ZH*AU), (XH*AU));
	return HelioCoords;
}
// ----------------------------------------------------------------------------------------------------------------------
// compute helio-centric-radius
float SolarSystem::heliocentric_radius( float a, float e, float V)
{
	float HelioRadius;
	HelioRadius = (a*(1-(e * e))) / (1 + e * cos(V));
	return HelioRadius;
}
// ----------------------------------------------------------------------------------------------------------------------
// Compute the mean elements of the orbit for planet-i at day number-d
core::array<float> SolarSystem::calculateMeanElements(int i, float d )
{
	/*

	Orbit Elemts

	Mean Orbital Elements (J2000)
	(Epoche = J2000 = 1.5 January 2000)

	Planet	a(AU)		e			i[°]		Omega[°]	~omega[°]	L[°]
	--------------------------------------------------------------------------------
	Merkur 	0.38709893 	0.20563069 	7.00487 	48.33167 	77.45645 	252.25084
	Venus 	0.72333199 	0.00677323 	3.39471 	76.68069 	131.53298 	181.97973
	Erde 	1.00000011 	0.01671022 	0.00005 	-11.26064 	102.94719 	100.46435
	Mars 	1.52366231 	0.09341233 	1.85061 	49.57854 	336.04084 	355.45332
	Jupiter 5.20336301 	0.04839266 	1.30530 	100.55615 	14.75385 	34.40438
	Saturn 	9.53707032 	0.05415060 	2.48446 	113.71504 	92.43194 	49.94432
	Uranus 	19.19126393 0.04716771 	0.76986 	74.22988 	170.96424 	313.23218
	Neptun 	30.06896348 0.00858587 	1.76917 	131.72169 	44.97135 	304.88003
	Pluto 	39.48168677 0.24880766 	17.14175 	110.30347 	224.06676 	238.92881


	Changing per century

	Planet	a(AU)		e			i[°]		Omega[°]	~omega[°]	L[°]
	AU/Ct		/Ct			"/Ct		"/Ct		"/Ct		"/Ct
	--------------------------------------------------------------------------------
	Merkur  0.00000066 	0.00002527 	-23.51 		-446.30 	573.57 		538101628.29
	Venus 	0.00000092 	-0.00004938 -2.86 		-996.89 	-108.80 	210664136.06
	Erde 	-0.00000005 -0.00003804 -46.94 		-18228.25 	1198.28 	129597740.63
	Mars 	-0.00007221 0.00011902 	-25.47 		-1020.19 	1560.78 	68905103.78
	Jupiter 0.00060737 	-0.00012880 -4.15 		1217.17 	839.93 		10925078.35
	Saturn 	-0.00301530 -0.00036762  6.11 		-1591.05 	-1948.89 	4401052.95
	Uranus 	0.00152025 	-0.00019150 -2.09 		-1681.40 	1312.56 	1542547.79
	Neptun 	-0.00125196 0.0000251 	-3.64 		-151.25 	-844.43 	786449.21
	Pluto 	-0.00076912 0.00006465 	11.07 		-37.33 		-132.25 	522747.90


	Legend:

	"     orbital seconds
	Ct    julian century
	a     semi-major-axis
	e     eccentricity
	i     inclination
	Omega  longitude of ascending node
	~omega  longitude of perihelion
	L     mean longitude
	*/
	//planet Mean Elements
	core::array<float> pme;

	float rads = core::PI / 180;

	// centuries since J2000
	float cy = d / 36525;

	//0 a - mean distance (AU) = SemiMajorAxis +/- AU/Ct
	//1 e - eccentricity = Eccentricity +/- e/Ct
	//2 i - inclination = Inclination +/- "/Ct
	//3 O - longitude of ascending node = Ascending Node +/- "/Ct
	//4 w - longitude of perihelion = Long of Periocenter +/- "/Ct
	//5 L - mean longitude +/- "/Ct
	//6 M - mean anomaly
	//7 V - true anomaly
	//8 R - helio-centric-radius

	switch (i)
	{
		// Mercury
	case 0:
		pme.push_back(0.38709893 + 0.00000066* cy);
		pme.push_back(0.20563069 + 0.00002527* cy);
		pme.push_back(( 7.00487  -  23.51*cy/3600)* rads);
		pme.push_back((48.33167  - 446.30*cy/3600)* rads);
		pme.push_back((77.45645  + 573.57*cy/3600)* rads);
		pme.push_back(mod2pi((252.25084 + 538101628.29*cy/3600)*rads));
		pme.push_back(pme[5] - pme[4]);
		pme.push_back(true_anomaly( pme[6], pme[1]));
		pme.push_back(heliocentric_radius( pme[0], pme[1], pme[7]));

		break;
		// Venus
	case 1:
		pme.push_back(0.72333199 + 0.00000092* cy);
		pme.push_back(0.00677323 - 0.00004938* cy);
		pme.push_back((  3.39471 -   2.86*cy/3600)* rads);
		pme.push_back(( 76.68069 - 996.89*cy/3600)* rads);
		pme.push_back((131.53298 - 108.80*cy/3600)* rads);
		pme.push_back(mod2pi((181.97973 + 210664136.06*cy/3600)*rads));
		pme.push_back(pme[5] - pme[4]);
		pme.push_back(true_anomaly( pme[6], pme[1]));
		pme.push_back(heliocentric_radius( pme[0], pme[1], pme[7]));
		break;
		// Earth/Sun
	case 2:
		pme.push_back(1.00000011 - 0.00000005* cy);
		pme.push_back(0.01671022 - 0.00003804* cy);
		pme.push_back((  0.00005 -    46.94*cy/3600)* rads);
		pme.push_back((-11.26064 - 18228.25*cy/3600)* rads);
		pme.push_back((102.94719 +  1198.28*cy/3600)* rads);
		pme.push_back(mod2pi((100.46435 + 129597740.63*cy/3600)*rads));
		pme.push_back(pme[5] - pme[4]);
		pme.push_back(true_anomaly( pme[6], pme[1]));
		pme.push_back(heliocentric_radius( pme[0], pme[1], pme[7]));
		break;
		// Mars
	case 3:
		pme.push_back(1.52366231 - 0.00007221* cy);
		pme.push_back(0.09341233 + 0.00011902* cy);
		pme.push_back((  1.85061 -   25.47*cy/3600)* rads);
		pme.push_back(( 49.57854 - 1020.19*cy/3600)* rads);
		pme.push_back((336.04084 + 1560.78*cy/3600)* rads);
		pme.push_back(mod2pi((355.45332 + 68905103.78*cy/3600)*rads));
		pme.push_back(pme[5] - pme[4]);
		pme.push_back(true_anomaly( pme[6], pme[1]));
		pme.push_back(heliocentric_radius( pme[0], pme[1], pme[7]));
		break;
		// Jupiter
	case 4:
		pme.push_back(5.20336301 + 0.00060737* cy);
		pme.push_back(0.04839266 - 0.00012880* cy);
		pme.push_back((  1.30530 -    4.15*cy/3600)* rads);
		pme.push_back((100.55615 + 1217.17*cy/3600)* rads);
		pme.push_back(( 14.75385 +  839.93*cy/3600)* rads);
		pme.push_back(mod2pi((34.40438 + 10925078.35*cy/3600)*rads));
		pme.push_back(pme[5] - pme[4]);
		pme.push_back(true_anomaly( pme[6], pme[1]));
		pme.push_back(heliocentric_radius( pme[0], pme[1], pme[7]));
		break;
		// Saturn
	case 5:
		pme.push_back(9.53707032 - 0.00301530* cy);
		pme.push_back(0.05415060 - 0.00036762* cy);
		pme.push_back((  2.48446 +    6.11*cy/3600)* rads);
		pme.push_back((113.71504 - 1591.05*cy/3600)* rads);
		pme.push_back(( 92.43194 - 1948.89*cy/3600)* rads);
		pme.push_back(mod2pi((49.94432 + 4401052.95*cy/3600)*rads));
		pme.push_back(pme[5] - pme[4]);
		pme.push_back(true_anomaly( pme[6], pme[1]));
		pme.push_back(heliocentric_radius( pme[0], pme[1], pme[7]));
		break;
		// Uranus
	case 6:
		pme.push_back(19.19126393 + 0.00152025* cy);
		pme.push_back( 0.04716771 - 0.00019150* cy);
		pme.push_back((  0.76986  -    2.09*cy/3600)* rads);
		pme.push_back(( 74.22988  - 1681.40*cy/3600)* rads);
		pme.push_back((170.96424  + 1312.56*cy/3600)* rads);
		pme.push_back(mod2pi((313.23218 + 1542547.79*cy/3600)*rads));
		pme.push_back(pme[5] - pme[4]);
		pme.push_back(true_anomaly( pme[6], pme[1]));
		pme.push_back(heliocentric_radius( pme[0], pme[1], pme[7]));
		break;
		// Neptune
	case 7:
		pme.push_back(30.06896348 - 0.00125196* cy);
		pme.push_back( 0.00858587 + 0.00002510* cy);
		pme.push_back((  1.76917  -   3.64*cy/3600)* rads);
		pme.push_back((131.72169  - 151.25*cy/3600)* rads);
		pme.push_back(( 44.97135  - 844.43*cy/3600)* rads);
		pme.push_back(mod2pi((304.88003 + 786449.21*cy/3600)*rads));
		pme.push_back(pme[5] - pme[4]);
		pme.push_back(true_anomaly( pme[6], pme[1]));
		pme.push_back(heliocentric_radius( pme[0], pme[1], pme[7]));
		break;
		// Pluto
	case 8:
		pme.push_back(39.48168677 - 0.00076912* cy);
		pme.push_back( 0.24880766 + 0.00006465* cy);
		pme.push_back(( 17.14175  +  11.07*cy/3600)* rads);
		pme.push_back((110.30347  -  37.33*cy/3600)* rads);
		pme.push_back((224.06676  - 132.25*cy/3600)* rads);
		pme.push_back(mod2pi((238.92881 + 522747.90*cy/3600)*rads));
		pme.push_back(pme[5] - pme[4]);
		pme.push_back(true_anomaly( pme[6], pme[1]));
		pme.push_back(heliocentric_radius( pme[0], pme[1], pme[7]));
		break;

	}

	return pme;
}
// ----------------------------------------------------------------------------------------------------------------------
// compute all coordinates at day
core::array<core::vector3df> SolarSystem::calculateAtDay(float d, int p)
{
	core::array<float> Mercury;
	core::array<float> Venus;
	core::array<float> Earth;
	core::array<float> Mars;
	core::array<float> Jupiter;
	core::array<float> Saturn;
	core::array<float> Uranus;
	core::array<float> Neptune;
	core::array<float> Pluto;

	core::vector3df mercury_coordinates;
	core::vector3df venus_coordinates;
	core::vector3df earth_coordinates;
	core::vector3df mars_coordinates;
	core::vector3df jupiter_coordinates;
	core::vector3df saturn_coordinates;
	core::vector3df uranus_coordinates;
	core::vector3df neptune_coordinates;
	core::vector3df pluto_coordinates;

	if(p==-1){
		Mercury = calculateMeanElements(0, d);
		Venus = calculateMeanElements(1, d);
		Earth = calculateMeanElements(2, d);
		Mars = calculateMeanElements(3, d);
		Jupiter = calculateMeanElements(4, d);
		Saturn = calculateMeanElements(5, d);
		Uranus = calculateMeanElements(6, d);
		Neptune = calculateMeanElements(7, d);
		Pluto = calculateMeanElements(8, d);

		mercury_coordinates = heliocentric_coordinates(Mercury[8], Mercury[3], Mercury[7], Mercury[4], Mercury[2]);
		venus_coordinates = heliocentric_coordinates(Venus[8], Venus[3], Venus[7], Venus[4], Venus[2]);
		earth_coordinates = heliocentric_coordinates(Earth[8], Earth[3], Earth[7], Earth[4], Earth[2]);
		mars_coordinates = heliocentric_coordinates(Mars[8], Mars[3], Mars[7], Mars[4], Mars[2]);
		jupiter_coordinates = heliocentric_coordinates(Jupiter[8], Jupiter[3], Jupiter[7], Jupiter[4], Jupiter[2]);
		saturn_coordinates = heliocentric_coordinates(Saturn[8], Saturn[3], Saturn[7], Saturn[4], Saturn[2]);
		uranus_coordinates = heliocentric_coordinates(Uranus[8], Uranus[3], Uranus[7], Uranus[4], Uranus[2]);
		neptune_coordinates = heliocentric_coordinates(Neptune[8], Neptune[3], Neptune[7], Neptune[4], Neptune[2]);
		pluto_coordinates = heliocentric_coordinates(Pluto[8], Pluto[3], Pluto[7], Pluto[4], Pluto[2]);

		core::array<core::vector3df> results;
		results.push_back(mercury_coordinates);
		results.push_back(venus_coordinates);
		results.push_back(earth_coordinates);
		results.push_back(mars_coordinates);
		results.push_back(jupiter_coordinates);
		results.push_back(saturn_coordinates);
		results.push_back(uranus_coordinates);
		results.push_back(neptune_coordinates);
		results.push_back(pluto_coordinates);
		return results;

	}if(p!=-1){
		core::array<float> nPlanet = calculateMeanElements(p, d);
		core::vector3df nPlanetPos = heliocentric_coordinates(nPlanet[8], nPlanet[3], nPlanet[7], nPlanet[4], nPlanet[2]);
		core::array<core::vector3df> results;
		results.push_back(nPlanetPos);
		return results;
	}
}
