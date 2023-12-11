/*
The Irrlicht Engine License

Copyright © 2002-2005 Nikolaus Gebhardt

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.



Original source code by Stephen R. Schmitt written in zeno. Ported to mel and c++ by Tobias Houfek.
http://home.att.net/~srschmitt/planetorbits.html

*/
#include <string>
//#include <direct.h>
#include <iostream>
#include <irrlicht.h>
#include "solar.h"
#include "Geopack-2008.h"
#include "t96_01.h"
//#include "ts05.h"

#pragma comment(lib,"irrlicht.lib")
//#pragma comment(lib,"libIrrlicht.a")
using namespace irr;


const f64 hourSeconds = 3600.0;
const f64 secondsM = 60.0;
const f64 dayH = 24.0;
const f64 yearD = 365.00;

//Constructor
SolarSystem::SolarSystem()
: year(0), month(0), day(0), dayM(0), hour(0), minute(0), second(0)
{
	year = 0;
	month = 0;
	day = 0;
	dayM = 0;
	hour = 0;
	minute = 0;
	second = 0;
}

//Destructor
SolarSystem::~SolarSystem()
{
}


// ----------------------------------------------------------------------------------------------------------------------
// trace field Lines
core::array<core::array<core::vector3df> > SolarSystem::calculateFieldLines(int year, int dayM, int hour, int minute, int second)
{
	//DEFINE THE UNIVERSAL TIME AND PREPARE THE COORDINATE TRANSFORMATION PARAMETERS
	//BY INVOKING THE SUBROUTINE RECALC_08
	integer fyear = (integer)year;
	integer fdayM = (integer)dayM;
	integer fhour = (integer)hour;
	integer fminute = (integer)minute;
	integer fsecond = (integer)second;

	//IF ONLY QUESTIONABLE INFORMATION (OR NO INFORMATION AT ALL) IS AVAILABLE
	//ON THE SOLAR WIND SPEED, OR, IF THE STANDARD GSM AND/OR SM COORDINATES ARE
	//INTENDED TO BE USED, THEN SET VGSEX=-400.0 AND VGSEY=VGSEZ=0. IN THIS CASE,
	//THE GSW COORDINATE SYSTEM BECOMES IDENTICAL TO THE STANDARD GSM.
	real vgsex = -400.f;
	real vgsey = 0.0f;
	real vgsez = 0.0f;

	/*PARMOD -  A 10-ELEMENT ARRAY CONTAINING INPUT PARAMETERS NEEDED FOR A UNIQUE */
	/*SPECIFICATION OF THE EXTERNAL FIELD MODEL. THE CONCRETE MEANING OF THE COMPONENTS */
	/*OF PARMOD DEPENDS ON A SPECIFIC VERSION OF THAT MODEL. */
	//------------------------------------------------------------------------------------------
	/* DATA-BASED MODEL CALIBRATED BY */
	/*(1) SOLAR WIND PRESSURE PDYN (NANOPASCALS), */
	/*(2) DST (NANOTESLA), */
	/*(3) BYIMF, AND */
	/*(4) BZIMF (NANOTESLA). */
	/* THESE INPUT PARAMETERS SHOULD BE PLACED IN THE FIRST 4 ELEMENTS */
	/* OF THE ARRAY PARMOD(10). */
	real parmod[10];
	parmod[0] = 3.f;
	parmod[1] = -20.f;
	parmod[2] = 3.f;
	parmod[3] = -5.f;

	recalc_08__(&fyear, &fdayM, &fhour, &fminute, &fsecond, &vgsex, &vgsey, &vgsez );

	core::array<core::array<core::vector3df> > data;

	for(int longitude = -180; longitude < 140; longitude += 40) // *9
	{
		for(int latitude = 5; latitude < 85; latitude += 5)  // *33 -> 33 * 9 = 297;
		{
			real re = 1.f;
			real colat = (90.f - (real)latitude) * .01745329f;
			real xlon = (real)longitude * .01745329f;

			static integer j = 1;

			//output
			real xgeo = 0.f;
			real ygeo = 0.f;
			real zgeo = 0.f;

			//CONVERT SPHERICAL COORDS INTO CARTESIAN
			sphcar_08__(&re, &colat, &xlon, &xgeo, &ygeo, &zgeo, &j);
			//-----------------------------------------------------------------------------

			//output
			real xgsw = 0.f;
			real ygsw = 0.f;
			real zgsw = 0.f;

			//TRANSFORM GEOGRAPHICAL GEOCENTRIC COORDS INTO SOLAR WIND MAGNETOSPHERIC ONES
			geogsw_08__(&xgeo, &ygeo, &zgeo, &xgsw, &ygsw, &zgsw, &j);
			//-----------------------------------------------------------------------------

			//TRACE THE FIELD LINE:
			//SPECIFY TRACING PARAMETERS

			/*LMAX - MAXIMAL LENGTH OF THE ARRAYS XX,YY,ZZ, IN WHICH COORDINATES OF THE FIELD */
			/*LINE POINTS ARE STORED. LMAX SHOULD BE SET EQUAL TO THE ACTUAL LENGTH OF */
			/*THE ARRAYS, DEFINED IN THE MAIN PROGRAM AS ACTUAL ARGUMENTS OF THIS SUBROUTINE. */
			//------------------------------------------------------------------------------------------------
			/*  LMAX IS THE UPPER LIMIT ON THE NUMBER OF FIELD LINE POINTS RETURNED BY THE TRACER. */
			/*  IT CAN BE SET ARBITRARILY LARGE, DEPENDING ON THE SPECIFICS OF A PROBLEM UNDER STUDY. */
			/*  IN THIS EXAMPLE, LMAX IS TENTATIVELY SET EQUAL TO 500. */
			static integer lmax = 500;

			/*ERR - PERMISSIBLE STEP ERROR. A REASONABLE ESTIMATE PROVIDING A SUFFICIENT ACCURACY FOR MOST */
			/*APPLICATIONS IS ERR=0.0001. SMALLER/LARGER VALUES WILL RESULT IN LARGER/SMALLER NUMBER */
			/*OF STEPS AND, HENCE, OF OUTPUT FIELD LINE POINTS. NOTE THAT USING MUCH SMALLER VALUES */
			/*OF ERR MAY REQUIRE USING A DOUBLE PRECISION VERSION OF THE ENTIRE PACKAGE. */
			//------------------------------------------------------------------------------------------------
			//PERMISSIBLE STEP ERROR SET AT ERR=0.0001
			static real err = 0.0001f;

			/*RLIM - RADIUS OF A SPHERE (IN RE), DEFINING THE OUTER BOUNDARY OF THE TRACING REGION; */
			/*IF THE FIELD LINE REACHES THAT BOUNDARY FROM INSIDE, ITS OUTBOUND TRACING IS */
			/*TERMINATED AND THE CROSSING POINT COORDINATES XF,YF,ZF ARE CALCULATED. */
			//------------------------------------------------------------------------------------------------
			//LIMIT THE TRACING REGION WITHIN R=60 Re
			static real rlim = 250.f;

			/*IOPT - A MODEL INDEX; CAN BE USED FOR SPECIFYING A VERSION OF THE EXTERNAL FIELD */
			/*MODEL (E.G., A NUMBER OF THE KP-INDEX INTERVAL). ALTERNATIVELY, ONE CAN USE THE ARRAY */
			/*PARMOD FOR THAT PURPOSE (SEE BELOW); IN THAT CASE IOPT IS JUST A DUMMY PARAMETER. */
			//------------------------------------------------------------------------------------------------
			//IN THIS EXAMPLE IOPT IS JUST A DUMMY PARAMETER,
			//WHOSE VALUE DOES NOT MATTER
			integer iopt = 0;

			/*DIR - SIGN OF THE TRACING DIRECTION: IF DIR=1.0 THEN THE TRACING IS MADE ANTIPARALLEL */
			/*TO THE TOTAL FIELD VECTOR (E.G., FROM NORTHERN TO SOUTHERN CONJUGATE POINT); */
			/*IF DIR=-1.0 THEN THE TRACING PROCEEDS IN THE OPPOSITE DIRECTION, THAT IS, PARALLEL TO */
			/*THE TOTAL FIELD VECTOR. */
			//------------------------------------------------------------------------------------------------
			//TRACE THE LINE WITH A FOOTPOINT IN THE NORTHERN HEMISPHERE, THAT IS,
            //ANTIPARALLEL TO THE MAGNETIC FIELD
			real dir = 1.f;

			/*DSMAX - UPPER LIMIT ON THE STEPSIZE (SETS A DESIRED MAXIMAL SPACING BETWEEN */
			/*THE FIELD LINE POINTS) */
			//------------------------------------------------------------------------------------------------
			//MAXIMAL SPACING BETWEEN THE FIELD LINE POINTS SET EQUAL TO 1 RE
			static real dsmax = 1.f;

			/*R0 -  RADIUS OF A SPHERE (IN RE), DEFINING THE INNER BOUNDARY OF THE TRACING REGION */
			/*(USUALLY, EARTH'S SURFACE OR THE IONOSPHERE, WHERE R0~1.0) */
			/*IF THE FIELD LINE REACHES THAT SPHERE FROM OUTSIDE, ITS INBOUND TRACING IS */
			/*TERMINATED AND THE CROSSING POINT COORDINATES XF,YF,ZF  ARE CALCULATED. */
			//------------------------------------------------------------------------------------------------
			//LANDING POINT WILL BE CALCULATED ON THE SPHERE R=1,
			//I.E. ON THE EARTH'S SURFACE
			static real r0 = 1.f;

			/*EXNAME - NAME OF A SUBROUTINE PROVIDING COMPONENTS OF THE EXTERNAL MAGNETIC FIELD */
			/*(E.G., T89, OR T96_01, ETC.). */
			/*INNAME - NAME OF A SUBROUTINE PROVIDING COMPONENTS OF THE INTERNAL MAGNETIC FIELD */
			/*(EITHER DIP_08 OR IGRF_GSW_08). */

			//EXTERNAL MODELS
			//t96_01__ - Example conf.
			//t04_s__

			//INTERNAL MODELS
			//igrf_gsw_08__ - Example conf.
			//dip_08__

			//------------------------------------------------------------------------------------------------
			//------------------------------------------------------------------------------------------------
			//output
			//XF,YF,ZF - GSW COORDINATES OF THE ENDPOINT OF THE TRACED FIELD LINE.
			//XX,YY,ZZ - ARRAYS OF LENGTH LMAX, CONTAINING COORDINATES OF THE FIELD LINE POINTS.
			//L - ACTUAL NUMBER OF FIELD LINE POINTS, GENERATED BY THIS SUBROUTINE.

			/*  IN THE GSW SYSTEM, X AXIS IS ANTIPARALLEL TO THE OBSERVED DIRECTION OF THE SOLAR WIND FLOW. */
			/*  TWO OTHER AXES, Y AND Z, ARE DEFINED IN THE SAME WAY AS FOR THE STANDARD GSM, THAT IS, */
			/*  Z AXIS ORTHOGONAL TO X AXIS, POINTS NORTHWARD, AND LIES IN THE PLANE DEFINED BY THE X- */
			/*  AND GEODIPOLE AXIS. THE Y AXIS COMPLETES THE RIGHT-HANDED SYSTEM. */

			/*  THE GSW SYSTEM BECOMES IDENTICAL TO THE STANDARD GSM IN THE CASE OF */
			/*   A STRICTLY RADIAL SOLAR WIND FLOW. */

			real xf = 0.f;
			real yf = 0.f;
			real zf = 0.f;

			real xx[500];
			real yy[500];
			real zz[500];

			integer l = 0;

			trace_08__(&xgsw, &ygsw, &zgsw, &dir, &dsmax, &err, &rlim, &r0, &iopt, parmod,
					   (S_fp)t96_01__, (S_fp)igrf_gsw_08__,
					   &xf, &yf, &zf,
					   xx, yy, zz,
					   &l, &lmax);

			//printf("%d\n",(int)l);
			core::array<core::vector3df> line;
			for(int p=0; p<l; p++)
			{
				line.push_back(core::vector3df(xx[p],yy[p],zz[p]));
			}
			data.push_back(line);
		}
	}
	return data;
}
// ----------------------------------------------------------------------------------------------------------------------
// days since/from J2000 (12:00 UT, Jan 1, 2000)
f64 SolarSystem::gregToJ2000( int y, int m, int d, int hour, int mins, int secs){
/*
NOTE: We're calculating days, later we will calculate centuries... from http://fer3.com/arc/m2.aspx?i=022034&y=200502

The common units of time are hopelessly complicated.  There are too many of them; years, months, days, hours, minutes,
and seconds.  Time must be changed into a single unit.  The chosen unit is the number of centuries after noon 1 January 2000.
Time has a value of -.5 at 1200 on 1 January 1950, 0 at 1200 on 1 January 2000, and +.5 at 1200 on 1 January 2050.

Van Flandern and Pulkkinen give a short formula for converting common UTC (or GMT) time to UTC centuries that is valid from
March 1900 to February 2100.
Tu = (367*yr-trunc(7*(yr+trunc((mo+9)/12))/4)+trunc(275*mo/9)
     +day+(hr+(min+(sec/60))/60)/24
	 -730531.5)
	 /36525

The formula fails outside those dates because 1900 and 2100 are both years divisible by 4 which are not leap years.  The function
trunc is the integer part of the number within the brackets; any fractional part is dropped.  To have an accuracy of one second,
the value of time must have 11 significant digits because there are over 6 billion seconds in two centuries.
The way the formula works is a little vague and is best explained working backwards.  At the end of the formula, 36525 converts the
units from days to centuries.  The 730531.5 makes the formula have a value of 0 at noon on 1 January 2000.
The term "day+(hr+(min+(sec/60))/60)/24" converts the date of the month and time into the number of days since the beginning of the
last day of the previous month.  The remaining part of the formula handles the changing number of days in the months and accounts
for leap years.  You can get an idea of how it works by solving it for the first day of each month for four consecutive years writing
down the numbers from within each set of parentheses.
*/
	f64 yD = f64(y);
	f64 mD = f64(m);
	f64 dD = f64(d);
	f64 hD = f64(hour);
	f64 miD = f64(mins);
	f64 sD = f64(secs);

	f64 rv;
	//rv = (367.0f * y - floor(7.0f*(y + floor((m + 9.0f)/12.0f))/4.0f) + floor(275.0f*m/9.0f) + d - 730531.5f + h/24.0f);
	rv = (367.0 * yD - floor(7.0*(yD + floor((mD + 9.0)/12.0))/4.0) + floor(275.0*mD/9.0)
		+ dD +(hD+(miD+(sD/secondsM))/secondsM)/dayH
		- 730531.5);

	return rv;
}

// ----------------------------------------------------------------------------------------------------------------------
// convert back to Gregorian Date
core::stringw SolarSystem::J2000ToGreg(f64 JD)
{
	int ja, jb, jc, jd, je;

	f64 t;
	f64 julianDay = 2451545.0 + JD;
	f64 dayremainder = JD - floor(JD);

	ja = core::round32(julianDay);
	t = dayH * (julianDay - ja + 0.5);

	if (ja >= 2299161) {
		jb = floor(((ja - 1867216.0) - 0.25) / 36524.25);
		ja = ja + 1 + jb - floor(jb / 4.0);
	}

	jb = ja + 1524;
	jc = floor(6680.0 + ((jb - 2439870.0) - 122.1) / 365.25);
	jd = yearD * jc + floor(jc / 4.0);
	je = floor((jb - jd) / 30.6001);
	day = jb - jd - floor(30.6001 * je);
	month = je - 1;
	if (month > 12)
		month = month - 12;
	year = jc - 4715;
	if (month > 2)
		year = year - 1;
	if (year < 0)
		year = year - 1;
	hour = floor(dayremainder*dayH);
	minute = floor( (dayremainder*dayH*secondsM)-(hour*secondsM) );
	second = floor( (dayremainder*dayH*hourSeconds)-(hour*hourSeconds+minute*secondsM) );
	hour += 12.0;
	if(hour>23.0){
		hour -= dayH;
	}
	core::stringw result = "";
	result += year;
	result += "/";
	if(month<10)
		result += "0";
	result += month;
	result += "/";
	if(day<10)
		result += "0";
	result += day;
	result += " - ";
	if(hour<10)
		result += "0";
	result += hour;
	result += ":";
	if(minute<10)
		result += "0";
	result += minute;
	result += ":";
	if(second<10)
		result += "0";
	result += second;

	//tired of date systems
	//approximation used for magnetic field
	dayM = (int)floor( ((month-1)+(day/30.0f)) * 30.4166667f );

	//this->dayM = dayM;
	//printf("%d\n",this->dayM);

	return result;
}
// ----------------------------------------------------------------------------------------------------------------------
// return an angle in the range 0 to 2pi radians
f64 SolarSystem::mod2pi( f64 x )
{
	f64 b = x / (2*core::PI);
	f64 a = (2*core::PI)*( b - abs(floor(b)) );
	if (a < 0){
		a = (2*core::PI) + a;
	}
	return a;
}
// ----------------------------------------------------------------------------------------------------------------------
// compute the true anomaly from mean anomaly using iteration
//  M - mean anomaly in radians
//  e - orbit eccentricity
f64 SolarSystem::true_anomaly( f64 M, f64 e )
{
	f64 V;
	f64 E1;
	f64 degs = 180 / core::PI;
	f64 rads = core::PI / 180;
	f64 eps  = 0.000001;

	// initial approximation of eccentric anomaly
	f64 E = M + e * sin(M)*(1.0 + e* cos(M));
	// iterate to improve accuracy

	do
	{
		E1 = E;
		E = E1 - (E1 - e* sin(E1) - M)/(1.0 - e* cos(E1));
	}
	while (abs( E - E1 ) > eps);

	// convert eccentric anomaly to true anomaly
	V = 2.0*atan(sqrt((1.0 + e)/(1.0 - e))* tan(0.5*E));

	if (V < 0) {
		// modulo 2pi
		V = V + (2*core::PI);
	}
	return V;
}
// ----------------------------------------------------------------------------------------------------------------------
// compute helio-centric-coordinates
core::vector3df SolarSystem::heliocentric_coordinates( f64 R, f64 O, f64 V, f64 w, f64 i )
{
	core::vector3df HelioCoords;
	f64 AU = 149598000.0;
	f64 XH = R*(cos(O)*cos(V + w - O) - sin(O)*sin(V + w - O)*cos(i));
	f64 YH = R*(sin(O)*cos(V + w - O) + cos(O)*sin(V + w - O)*cos(i));
	f64 ZH = R*(sin(V + w - O)*sin(i));
	HelioCoords = core::vector3df((YH*AU), (ZH*AU), (XH*AU));
	return HelioCoords;
}
// ----------------------------------------------------------------------------------------------------------------------
// compute helio-centric-radius
f64 SolarSystem::heliocentric_radius( f64 a, f64 e, f64 V)
{
	f64 HelioRadius;
	HelioRadius = (a*(1-(e * e))) / (1 + e * cos(V));
	return HelioRadius;
}
// ----------------------------------------------------------------------------------------------------------------------
// Compute the mean elements of the orbit for planet-i at day number-d
core::array<f64> SolarSystem::calculateMeanElements(int i, f64 d )
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
	core::array<f64> pme;

	f64 rads = core::PI / 180;

	// centuries since J2000
	f64 cy = d / 36525;

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
		pme.push_back(( 7.00487  -  23.51*cy/hourSeconds)* rads);
		pme.push_back((48.33167  - 446.30*cy/hourSeconds)* rads);
		pme.push_back((77.45645  + 573.57*cy/hourSeconds)* rads);
		pme.push_back(mod2pi((252.25084 + 538101628.29*cy/hourSeconds)*rads));
		pme.push_back(pme[5] - pme[4]);
		pme.push_back(true_anomaly( pme[6], pme[1]));
		pme.push_back(heliocentric_radius( pme[0], pme[1], pme[7]));

		break;
		// Venus
	case 1:
		pme.push_back(0.72333199 + 0.00000092* cy);
		pme.push_back(0.00677323 - 0.00004938* cy);
		pme.push_back((  3.39471 -   2.86*cy/hourSeconds)* rads);
		pme.push_back(( 76.68069 - 996.89*cy/hourSeconds)* rads);
		pme.push_back((131.53298 - 108.80*cy/hourSeconds)* rads);
		pme.push_back(mod2pi((181.97973 + 210664136.06*cy/hourSeconds)*rads));
		pme.push_back(pme[5] - pme[4]);
		pme.push_back(true_anomaly( pme[6], pme[1]));
		pme.push_back(heliocentric_radius( pme[0], pme[1], pme[7]));
		break;
		// Earth/Sun
	case 2:
		pme.push_back(1.00000011 - 0.00000005* cy);
		pme.push_back(0.01671022 - 0.00003804* cy);
		pme.push_back((  0.00005 -    46.94*cy/hourSeconds)* rads);
		pme.push_back((-11.26064 - 18228.25*cy/hourSeconds)* rads);
		pme.push_back((102.94719 +  1198.28*cy/hourSeconds)* rads);
		pme.push_back(mod2pi((100.46435 + 129597740.63*cy/hourSeconds)*rads));
		pme.push_back(pme[5] - pme[4]);
		pme.push_back(true_anomaly( pme[6], pme[1]));
		pme.push_back(heliocentric_radius( pme[0], pme[1], pme[7]));
		break;
		// Mars
	case 3:
		pme.push_back(1.52366231 - 0.00007221* cy);
		pme.push_back(0.09341233 + 0.00011902* cy);
		pme.push_back((  1.85061 -   25.47*cy/hourSeconds)* rads);
		pme.push_back(( 49.57854 - 1020.19*cy/hourSeconds)* rads);
		pme.push_back((336.04084 + 1560.78*cy/hourSeconds)* rads);
		pme.push_back(mod2pi((355.45332 + 68905103.78*cy/hourSeconds)*rads));
		pme.push_back(pme[5] - pme[4]);
		pme.push_back(true_anomaly( pme[6], pme[1]));
		pme.push_back(heliocentric_radius( pme[0], pme[1], pme[7]));
		break;
		// Jupiter
	case 4:
		pme.push_back(5.20336301 + 0.00060737* cy);
		pme.push_back(0.04839266 - 0.00012880* cy);
		pme.push_back((  1.30530 -    4.15*cy/hourSeconds)* rads);
		pme.push_back((100.55615 + 1217.17*cy/hourSeconds)* rads);
		pme.push_back(( 14.75385 +  839.93*cy/hourSeconds)* rads);
		pme.push_back(mod2pi((34.40438 + 10925078.35*cy/hourSeconds)*rads));
		pme.push_back(pme[5] - pme[4]);
		pme.push_back(true_anomaly( pme[6], pme[1]));
		pme.push_back(heliocentric_radius( pme[0], pme[1], pme[7]));
		break;
		// Saturn
	case 5:
		pme.push_back(9.53707032 - 0.00301530* cy);
		pme.push_back(0.05415060 - 0.00036762* cy);
		pme.push_back((  2.48446 +    6.11*cy/hourSeconds)* rads);
		pme.push_back((113.71504 - 1591.05*cy/hourSeconds)* rads);
		pme.push_back(( 92.43194 - 1948.89*cy/hourSeconds)* rads);
		pme.push_back(mod2pi((49.94432 + 4401052.95*cy/hourSeconds)*rads));
		pme.push_back(pme[5] - pme[4]);
		pme.push_back(true_anomaly( pme[6], pme[1]));
		pme.push_back(heliocentric_radius( pme[0], pme[1], pme[7]));
		break;
		// Uranus
	case 6:
		pme.push_back(19.19126393 + 0.00152025* cy);
		pme.push_back( 0.04716771 - 0.00019150* cy);
		pme.push_back((  0.76986  -    2.09*cy/hourSeconds)* rads);
		pme.push_back(( 74.22988  - 1681.40*cy/hourSeconds)* rads);
		pme.push_back((170.96424  + 1312.56*cy/hourSeconds)* rads);
		pme.push_back(mod2pi((313.23218 + 1542547.79*cy/hourSeconds)*rads));
		pme.push_back(pme[5] - pme[4]);
		pme.push_back(true_anomaly( pme[6], pme[1]));
		pme.push_back(heliocentric_radius( pme[0], pme[1], pme[7]));
		break;
		// Neptune
	case 7:
		pme.push_back(30.06896348 - 0.00125196* cy);
		pme.push_back( 0.00858587 + 0.00002510* cy);
		pme.push_back((  1.76917  -   3.64*cy/hourSeconds)* rads);
		pme.push_back((131.72169  - 151.25*cy/hourSeconds)* rads);
		pme.push_back(( 44.97135  - 844.43*cy/hourSeconds)* rads);
		pme.push_back(mod2pi((304.88003 + 786449.21*cy/hourSeconds)*rads));
		pme.push_back(pme[5] - pme[4]);
		pme.push_back(true_anomaly( pme[6], pme[1]));
		pme.push_back(heliocentric_radius( pme[0], pme[1], pme[7]));
		break;
		// Pluto
	case 8:
		pme.push_back(39.48168677 - 0.00076912* cy);
		pme.push_back( 0.24880766 + 0.00006465* cy);
		pme.push_back(( 17.14175  +  11.07*cy/hourSeconds)* rads);
		pme.push_back((110.30347  -  37.33*cy/hourSeconds)* rads);
		pme.push_back((224.06676  - 132.25*cy/hourSeconds)* rads);
		pme.push_back(mod2pi((238.92881 + 522747.90*cy/hourSeconds)*rads));
		pme.push_back(pme[5] - pme[4]);
		pme.push_back(true_anomaly( pme[6], pme[1]));
		pme.push_back(heliocentric_radius( pme[0], pme[1], pme[7]));
		break;

	}

	return pme;
}
// ----------------------------------------------------------------------------------------------------------------------
// compute all coordinates at day
core::array<core::vector3df> SolarSystem::calculateAtDay(f64 d, int p)
{
	core::array<f64> Mercury;
	core::array<f64> Venus;
	core::array<f64> Earth;
	core::array<f64> Mars;
	core::array<f64> Jupiter;
	core::array<f64> Saturn;
	core::array<f64> Uranus;
	core::array<f64> Neptune;
	core::array<f64> Pluto;

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
		core::array<f64> nPlanet = calculateMeanElements(p, d);
		core::vector3df nPlanetPos = heliocentric_coordinates(nPlanet[8], nPlanet[3], nPlanet[7], nPlanet[4], nPlanet[2]);
		core::array<core::vector3df> results;
		results.push_back(nPlanetPos);
		return results;
	}
}

