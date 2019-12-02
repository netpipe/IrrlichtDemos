// zur Verhinderung doppelter Definition
#ifndef VOXELRAUM_H
#define VOXELRAUM_H

#include <math.h>
#include <string>
#include <stack>
#include <queue>
#include <vector>
#include <iostream>

#include <irrlicht.h>
#include "Hilfsfunktionen.h"
#include "ZufallsGenerator.h"
#include "Zeitmesser.h"

// Namespace DunGen : DungeonGenerator
namespace DunGen
{
	// moegliche Projektionsebenen
	enum EEbene {_EBENE_XY_, _EBENE_XZ_, _EBENE_YZ_};

	// Zustand der Turtle-Grafik
	struct SZeichnenZustand
	{
		// Koordinaten
		irr::core::vector3d<double> Position;

		// lokales Koordinatensystem
		irr::core::vector3d<double> Vorn;
		irr::core::vector3d<double> Links;
		irr::core::vector3d<double> Oben;

		// Zeichenradius: in Voxeln angegeben!
		double Radius;
	};

	// Beschreibung eines Voxels
	struct SVoxelZuZeichnen
	{
		// Voxelkoordinaten
		int X, Y, Z;
	};

	// Voxelraum mit Zeichenoptionen fuer Turtle-Grafik
	class CVoxelRaum
	{
	public:
		// Initialisierung:
		CVoxelRaum(CZufallsGenerator* _ZufallsGenerator, CZeitmesser* _Zeitmesser);
		// Voxelrand festlegen
		void SetzeRand(unsigned int _VoxelRandOffset);
		// Minimal Strichradius einstellen
		void SetzeMinimalenStrichradius(unsigned int _MinRadius);

		// Parameter auslesen
		unsigned int LeseRand();
		unsigned int LeseMinimalenStrichradius();

		// Zeichnen einer Turtlegrafik
		void ZeichneGrafik(std::string _Anweisungen, double _WinkelGier, double _WinkelNick, double _WinkelRoll, double _StartRadius, double _RadiusFaktor, double _RadiusDekrementor);
		// Erodieren
		void Erodiere(double _ErosionsWahrscheinlichkeit);
		// Aufraeumen
		void FilterAnwenden();

		// Voxel auslesen
		unsigned char LeseVoxel(int _X, int _Y, int _Z);

		// Dichte berechnen und auslesen
		unsigned short LeseDichteAn(unsigned int _A, unsigned int _B, EEbene _Ebene);
		unsigned short LeseDichteMaximum(EEbene _Ebene);
		void BerechneDichte();
		// Statistik ermitteln
		void ErmittleDreiecksNetzDaten();

		// Ausdehnung des Voxelraums in X,Y,Z
		static const unsigned int VoxelraumX = 512;
		static const unsigned int VoxelraumY = 512;
		static const unsigned int VoxelraumZ = 512;
		// min. Abstand gesetzter Voxel zum Rand des Voxelraums (mind. 3 (fuer korrektes FilterAnwenden))
		// dieser Rand ermoeglicht es, keine Sonderfaelle am Rand betrachten zu muessen
		// -> deutliche Beschleunigung von Routinen mit vielen Zugriffen auf den VoxelRaum
		static const unsigned int VoxelMinRandOffset = 3;
		static const unsigned int VoxelFilterRandOffset = VoxelMinRandOffset-1;

		// Voxelraum
		unsigned char Voxel[VoxelraumX][VoxelraumY][VoxelraumZ];

	private:
		// Zeichenfunktionen fuer Voxel
		inline void ZeichneVoxel(int _X, int _Y, int _Z);
		inline void ZeichneVoxelBlock(int _X, int _Y, int _Z);
		inline void ZeichneVoxelInVektor(int _X, int _Y, int _Z, std::vector<SVoxelZuZeichnen>* _Vektor);

		// Zeichenfunktionen fuer Linien in 3D
		inline void Bresenham3dX(int _Xs,int _Ys,int _Zs,int _Xe,int _Ye,int _Ze, std::vector<SVoxelZuZeichnen>* _Vektor);
		inline void Bresenham3dY(int _Xs,int _Ys,int _Zs,int _Xe,int _Ye,int _Ze, std::vector<SVoxelZuZeichnen>* _Vektor);
		inline void Bresenham3dZ(int _Xs,int _Ys,int _Zs,int _Xe,int _Ye,int _Ze, std::vector<SVoxelZuZeichnen>* _Vektor);
		void Bresenham3d(int _Xs,int _Ys,int _Zs,int _Xe,int _Ye,int _Ze, std::vector<SVoxelZuZeichnen>* _Vektor);

		// Zeichenfunktionen fuer Grafische Primitve
		void ZeichneZylinder(int _Xs,int _Ys,int _Zs,int _Xe,int _Ye,int _Ze, int _Radius, irr::core::vector3d<double> _Links, irr::core::vector3d<double> _Oben);
		void ZeichneKugel(int _X,int _Y,int _Z, int _Radius);

		// Test ob Voxel Randvoxel (bzgl. Rand der Grafik)
		inline bool IstRandVoxel(int _X,int _Y,int _Z);
		inline bool IstRandVoxelErweitert(int _X,int _Y,int _Z);
		// Test ob Voxel Anfang von Randvoxelgebiet
		inline bool IstRandVoxelAnfang(int _X,int _Y,int _Z);

		// Zeitmessung
		CZeitmesser* Zeitmesser;
		// Zufallsgenerator
		CZufallsGenerator* ZufallsGenerator;

		// min. Abstand der Grafik zum Rand des Voxelraums (mind. VoxelMinRandOffset)
		unsigned int VoxelRandOffset;
		// minimaler Strichradius
		unsigned int MinRadius;

		// Dichte
		unsigned short DichteXY[VoxelraumX][VoxelraumY];
		unsigned short DichteXZ[VoxelraumX][VoxelraumZ];
		unsigned short DichteYZ[VoxelraumY][VoxelraumZ];
		unsigned short DichteMaxXY,DichteMaxXZ,DichteMaxYZ;
	};

} // END NAMESPACE DunGen

#endif
