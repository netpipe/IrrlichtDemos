// zur Verhinderung doppelter Definition
#ifndef HILFSFUNKTIONEN_H
#define HILFSFUNKTIONEN_H

#include <irrlicht.h>

// Deklaration von PI
//float M_PI = 3.141592;

// Double in Integer umrechnen per Rundung, Quelle: Daniel E. http://www.c-plusplus.de/forum/39342
inline int d2i(double _d)
{
  return _d<0?_d-.5:_d+.5;
}

// sichere Division
inline double saveDiv(double _dividend, double _divisor)
{
  return _divisor==0?0.0:_dividend/_divisor;
}

// Gewicht fuer jeden Vertex eines Dreiecks berechnen, abhaengig vom Winkel
// Quelle: CMeshManipulator.cpp (Irrlicht Release 1.7.3) - Funktion getAngleWeight(), leicht modifiziert (nun mit double-Genauigkeit)
static inline irr::core::vector3d<double> berechneWinkelGewicht(irr::core::vector3d<double> _v1,
																irr::core::vector3d<double> _v2,
																irr::core::vector3d<double> _v3)
{
	// Berechne das Gewicht des Dreiecks, fuer jeden seiner Punkte
	// beginne mit der Berechnung der Seitenlaengen
	double a = _v2.getDistanceFromSQ(_v3);
	double asqrt = sqrt(a);
	double b = _v1.getDistanceFromSQ(_v3);
	double bsqrt = sqrt(b);
	double c = _v1.getDistanceFromSQ(_v2);
	double csqrt = sqrt(c);

	// nutze diese um den Winkel an jedem Punkt zu bestimmen
	return irr::core::vector3d<double>(
		acos((b + c - a) / (2.f * bsqrt * csqrt)),
		acos((-b + c + a) / (2.f * asqrt * csqrt)),
		acos((b - c + a) / (2.f * bsqrt * asqrt)));
}

#endif
