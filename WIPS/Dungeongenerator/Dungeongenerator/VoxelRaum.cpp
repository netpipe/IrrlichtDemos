// Implementierung der Klasse VoxelRaum
#include "VoxelRaum.h"

// ======================================================
// Initialisierung
// ======================================================

// COMPILER: VS 2010 SP1 Compiler daran hindern, bei Optimierung den gesamten Voxelraum in .exe-File zu packen
// durch: __declspec(noinline) vor Konstruktor
 DunGen::CVoxelRaum::CVoxelRaum(CZufallsGenerator* _ZufallsGenerator, CZeitmesser* _Zeitmesser)
{
	// Werte uebernehmen
	ZufallsGenerator = _ZufallsGenerator;
	Zeitmesser = _Zeitmesser;

	// Initialwerte
	VoxelRandOffset = VoxelMinRandOffset;
	MinRadius = 2;

	// Voxelraum leeren
	memset(Voxel,0,sizeof(unsigned char)*VoxelraumX*VoxelraumY*VoxelraumZ);
}

void DunGen::CVoxelRaum::SetzeRand(unsigned int _VoxelRandOffset)
{
	if (_VoxelRandOffset >= VoxelMinRandOffset)
		VoxelRandOffset = _VoxelRandOffset;
	else
		std::cout << "[Voxelraum]: Rand nicht geaendert, da zu klein gewaehlt: " << _VoxelRandOffset << " < " << VoxelRandOffset << std::endl;
}

void DunGen::CVoxelRaum::SetzeMinimalenStrichradius(unsigned int _MinRadius)
{
	MinRadius = _MinRadius;
}

// ======================================================
// Lesefunktionen fuer Parameter
// ======================================================
unsigned int DunGen::CVoxelRaum::LeseRand()
{
	return VoxelRandOffset;
}

unsigned int DunGen::CVoxelRaum::LeseMinimalenStrichradius()
{
	return MinRadius;
}

// ======================================================
// Lesefunktionen fuer Voxelraum
// ======================================================
unsigned char DunGen::CVoxelRaum::LeseVoxel(int _X, int _Y, int _Z)
{
	// auf gueltigen Bereich testen
	if (_X<0 || _Y<0 || _Z<0 || _X>=VoxelraumX || _Y>=VoxelraumY || _Z>=VoxelraumZ)
	{
		// std::cout << "[Voxelraum]: Zugriffsverletzung (lesen): " << _X << "," << _Y << "," << _Z << std::endl;
		// Bereich ungueltig: 0-Voxel zurueckliefern
		return 0;
	}
	else
		// wenn Bereich gueltig: passenden Voxel zuerueckliefern
		return Voxel[_X][_Y][_Z];
}

// ======================================================
// Zeichenfunktionen fuer Voxelraum
// ======================================================

inline void DunGen::CVoxelRaum::ZeichneVoxel(int _X, int _Y, int _Z)
{
	// Test auf Gueltigkeit
	//if (_X<VoxelMinRandOffset || _Y<VoxelMinRandOffset || _Z<VoxelMinRandOffset || _X>=VoxelraumX-VoxelMinRandOffset || _Y>=VoxelraumY-VoxelMinRandOffset || _Z>=VoxelraumZ-VoxelMinRandOffset)
	//	std::cout << "[Voxelraum]: Zugriffsverletzung (schreiben): " << _X << "," << _Y << "," << _Z << std::endl;
	//else
		// Voxel setzen, wenn gueltig
		Voxel[_X][_Y][_Z] = 1;
}

inline void DunGen::CVoxelRaum::ZeichneVoxelBlock(int _X, int _Y, int _Z)
{
	// Block von Voxeln um Zentralvoxel zeichnen

	// Voxel zeichnen
	Voxel[_X][_Y][_Z] = 1;

	// 6er Nachbarschaft
	Voxel[_X-1][_Y][_Z] = 1;	Voxel[_X][_Y-1][_Z] = 1;	Voxel[_X][_Y][_Z-1] = 1;
	Voxel[_X+1][_Y][_Z] = 1;	Voxel[_X][_Y+1][_Z] = 1;	Voxel[_X][_Y][_Z+1] = 1;

	// 18er Nachbarschaft
	Voxel[_X-1][_Y-1][_Z] = 1;	Voxel[_X-1][_Y+1][_Z] = 1;	Voxel[_X+1][_Y-1][_Z] = 1;	Voxel[_X+1][_Y+1][_Z] = 1;
	Voxel[_X-1][_Y][_Z-1] = 1;	Voxel[_X-1][_Y][_Z+1] = 1;	Voxel[_X+1][_Y][_Z-1] = 1;	Voxel[_X+1][_Y][_Z+1] = 1;
	Voxel[_X][_Y-1][_Z-1] = 1;	Voxel[_X][_Y-1][_Z+1] = 1;	Voxel[_X][_Y+1][_Z-1] = 1;	Voxel[_X][_Y+1][_Z+1] = 1;
}

inline void DunGen::CVoxelRaum::ZeichneVoxelInVektor(int _X, int _Y, int _Z, std::vector<SVoxelZuZeichnen>* _Vektor)
{
	// Voxel erzeugen
	SVoxelZuZeichnen neuerVoxel;
	neuerVoxel.X = _X;
	neuerVoxel.Y = _Y;
	neuerVoxel.Z = _Z;
	// Voxel in den Vektor laden
	_Vektor->push_back(neuerVoxel);
}

// ======================================================
// Bresenham in 3D
// ======================================================

// 3D extension of Bresenham’s algorithm (C Program), Quelle: pib.sagepub.com/content/216/3/459.full.pdf (modifiziert fuer alle moeglichen Faelle)
// laengste Achse: X
inline void DunGen::CVoxelRaum::Bresenham3dX(int _Xs,int _Ys,int _Zs,int _Xe,int _Ye,int _Ze, std::vector<SVoxelZuZeichnen>* _Vektor)
{	// Eingabe: Endpunkte

	// Richtungen festlegen
	int xinc = 1; if (_Xe<_Xs) xinc = -1;
	int yinc = 1; if (_Ye<_Ys) yinc = -1;
	int zinc = 1; if (_Ze<_Zs) zinc = -1;

	// Berechne dx,dy,dz
	int dx=abs (_Xe-_Xs);
	int dy=abs (_Ye-_Ys);
	int dz=abs (_Ze-_Zs);

	// Berechne Start-Entscheidungsparameter
	int ey=2*dy-dx; int ez=2*dz-dx;
	// Konstanten berechenen fuer Y
	int twoDy=2*dy; int twoDyDx=2*(dy-dx);
	// Konstanten berechenen fuer Z
	int twoDz=2*dz; int twoDzDx=2*(dz-dx);
	// Initialisierung der Position
	int x=_Xs; int y=_Ys; int z=_Zs;

	// Schleife zum Zeichnen der Linie
	for (int i=0; i<=dx; i++)
	{
		// Zeichne Punkt
		ZeichneVoxelInVektor(x,y,z,_Vektor);

		// Start der Berechnung der naechsten Koordinaten
		x+=xinc;
		if (ey>=0) {
			y+=yinc; ey+=twoDyDx;
		}
		else
			ey+=twoDy;

		if (ez>=0) {
			z+=zinc; ez+=twoDzDx;
		}
		else
			ez+=twoDz;
		// Ende der Berechnung der naechsten Koordinaten
	}
}

// laengste Achse: Y
inline void DunGen::CVoxelRaum::Bresenham3dY(int _Xs,int _Ys,int _Zs,int _Xe,int _Ye,int _Ze, std::vector<SVoxelZuZeichnen>* _Vektor)
{	// Eingabe: Endpunkte

	// Richtungen festlegen
	int xinc = 1; if (_Xe<_Xs) xinc = -1;
	int yinc = 1; if (_Ye<_Ys) yinc = -1;
	int zinc = 1; if (_Ze<_Zs) zinc = -1;

	// Berechne dx,dy,dz
	int dx=abs (_Xe-_Xs);
	int dy=abs (_Ye-_Ys);
	int dz=abs (_Ze-_Zs);

	// Berechne Start-Entscheidungsparameter
	int ex=2*dx-dy; int ez=2*dz-dy;
	// Konstanten berechenen fuer X
	int twoDx=2*dx; int twoDxDy=2*(dx-dy);
	// Konstanten berechenen fuer Z
	int twoDz=2*dz; int twoDzDy=2*(dz-dy);
	// Initialisierung der Position
	int x=_Xs; int y=_Ys; int z=_Zs;

	// Schleife zum Zeichnen der Linie
	for (int i=0; i<=dy; i++)
	{
		// Zeichne Punkt
		ZeichneVoxelInVektor(x,y,z,_Vektor);

		// Start der Berechnung der naechsten Koordinaten
		y+=yinc;
		if (ex>=0) {
			x+=xinc; ex+=twoDxDy;
		}
		else
			ex+=twoDx;

		if (ez>=0) {
			z+=zinc; ez+=twoDzDy;
		}
		else
			ez+=twoDz;
		// Ende der Berechnung der naechsten Koordinaten
	}
}

// laengste Achse: Z
inline void DunGen::CVoxelRaum::Bresenham3dZ(int _Xs,int _Ys,int _Zs,int _Xe,int _Ye,int _Ze, std::vector<SVoxelZuZeichnen>* _Vektor)
{	// Eingabe: Endpunkte

	// Richtungen festlegen
	int xinc = 1; if (_Xe<_Xs) xinc = -1;
	int yinc = 1; if (_Ye<_Ys) yinc = -1;
	int zinc = 1; if (_Ze<_Zs) zinc = -1;

	// Berechne dx,dy,dz
	int dx=abs (_Xe-_Xs);
	int dy=abs (_Ye-_Ys);
	int dz=abs (_Ze-_Zs);

	// Berechne Start-Entscheidungsparameter
	int ey=2*dy-dz; int ex=2*dx-dz;
	// Konstanten berechenen fuer Y
	int twoDy=2*dy; int twoDyDz=2*(dy-dz);
	// Konstanten berechenen fuer Z
	int twoDx=2*dx; int twoDxDz=2*(dx-dz);
	// Initialisierung der Position
	int x=_Xs; int y=_Ys; int z=_Zs;

	// Schleife zum Zeichnen der Linie
	for (int i=0; i<=dz; i++)
	{
		// Zeichne Punkt
		ZeichneVoxelInVektor(x,y,z,_Vektor);

		// Start der Berechnung der naechsten Koordinaten
		z+=zinc;
		if (ey>=0) {
			y+=yinc; ey+=twoDyDz;
		}
		else
			ey+=twoDy;

		if (ex>=0) {
			x+=xinc; ex+=twoDxDz;
		}
		else
			ex+=twoDx;
		// Ende der Berechnung der naechsten Koordinaten
	}
}

// Bresenham-Aufruf
void DunGen::CVoxelRaum::Bresenham3d(int _Xs,int _Ys,int _Zs,int _Xe,int _Ye,int _Ze, std::vector<SVoxelZuZeichnen>* _Vektor)
{
	// Seitenlaengen (der BoundingBox der Linie) bestimmen
	int dx=abs (_Xe-_Xs);
	int dy=abs (_Ye-_Ys);
	int dz=abs (_Ze-_Zs);

	if ((dx>dy)&&(dx>dz)) Bresenham3dX(_Xs,_Ys,_Zs,_Xe,_Ye,_Ze,_Vektor);		// laengste Seite: X
	else if (dy>dz) Bresenham3dY(_Xs,_Ys,_Zs,_Xe,_Ye,_Ze,_Vektor);				// laengste Seite: Y
	else Bresenham3dZ(_Xs,_Ys,_Zs,_Xe,_Ye,_Ze,_Vektor);							// laengste Seite: Z
}

// ======================================================
// Weitere Zeichenfunktionen
// ======================================================

// Grafisches Primitiv: Zylinder
void DunGen::CVoxelRaum::ZeichneZylinder(int _Xs,int _Ys,int _Zs,int _Xe,int _Ye,int _Ze, int _Radius, irr::core::vector3d<double> _Links, irr::core::vector3d<double> _Oben)
{
	// Links und Oben sind Vektoren die die Deckflaeche aufspannen, muessen normiert sein damit Radius korrekt eingehalten wird

	// Radius um eins verringern, da Voxelbloecke statt einzelnen Voxeln gezeichnet werden
	_Radius--;

	// Buffer zum Hereinzeichnen
	std::vector<SVoxelZuZeichnen> voxelVektor1;
	std::vector<SVoxelZuZeichnen> voxelVektor2;
	std::vector<SVoxelZuZeichnen> voxelVektor3;

	// fuer den ersten Bresenham:
	int xn = d2i(_Radius * _Oben.X);
	int yn = d2i(_Radius * _Oben.Y);
	int zn = d2i(_Radius * _Oben.Z);

	// starte ersten Bresenham: Zylindergrundflaeche vertikal
	Bresenham3d(0,0,0,xn,yn,zn,&voxelVektor1);

	// fuer den zweiten Bresenham:
	xn = d2i(_Radius * _Links.X);
	yn = d2i(_Radius * _Links.Y);
	zn = d2i(_Radius * _Links.Z);

	// starte zweiten Bresenham: Zylindergrundflaeche horizontal
	Bresenham3d(0,0,0,xn,yn,zn,&voxelVektor2);
	// starte dritten Bresenham: Zentralachse
	Bresenham3d(_Xs,_Ys,_Zs,_Xe,_Ye,_Ze,&voxelVektor3);

	// Speicher pre-allokieren
	SVoxelZuZeichnen aktuellerVoxel1;
	SVoxelZuZeichnen aktuellerVoxel2;
	SVoxelZuZeichnen aktuellerVoxel3;
	int xm, ym, zm;

	// fuer innere Abfrage
	int radiusQuadrat = _Radius * _Radius;

	// arbeite Voxel ab
	for (unsigned int i=0; i<voxelVektor1.size(); i++)
	{
		aktuellerVoxel1 = voxelVektor1.at(i);
		for (unsigned int j=0; j<voxelVektor2.size(); j++)
		{
			aktuellerVoxel2 = voxelVektor2.at(j);

			// Abstand zum Mittelpunkt der Deckflaeche bestimmen
			xn = aktuellerVoxel1.X + aktuellerVoxel2.X;
			yn = aktuellerVoxel1.Y + aktuellerVoxel2.Y;
			zn = aktuellerVoxel1.Z + aktuellerVoxel2.Z;

			// Voxel auf Deckflaeche des Zylinders?
			if ((xn*xn+yn*yn+zn*zn) <= radiusQuadrat)
			{
				// Werte fuer Spiegelung
				xm = aktuellerVoxel1.X - aktuellerVoxel2.X;
				ym = aktuellerVoxel1.Y - aktuellerVoxel2.Y;
				zm = aktuellerVoxel1.Z - aktuellerVoxel2.Z;

				for (unsigned int k=0; k<voxelVektor3.size(); k++)
				{
					aktuellerVoxel3 = voxelVektor3.at(k);

					// ersten Voxelblock zeichnen: Viertelkreis
					ZeichneVoxelBlock(aktuellerVoxel3.X+xn,aktuellerVoxel3.Y+yn,aktuellerVoxel3.Z+zn);
					// Spiegelung 1
					ZeichneVoxelBlock(aktuellerVoxel3.X-xn,aktuellerVoxel3.Y-yn,aktuellerVoxel3.Z-zn);
					// Spiegelung 2
					ZeichneVoxelBlock(aktuellerVoxel3.X+xm,aktuellerVoxel3.Y+ym,aktuellerVoxel3.Z+zm);
					// Spiegelung 3
					ZeichneVoxelBlock(aktuellerVoxel3.X-xm,aktuellerVoxel3.Y-ym,aktuellerVoxel3.Z-zm);
				}
			}
		}
	}
}

// Grafisches Primitiv: Kugel
void DunGen::CVoxelRaum::ZeichneKugel(int _X,int _Y,int _Z, int _Radius)
{
	// Variablen fuer schnellen Zugriff
	int radiusQuadrat = _Radius*_Radius;
	int xQuadrat;
	int xQuadratPlusYQuadrat;

	// laufe ueber umgebenden Wuerfel
	for (int i = -_Radius; i<=_Radius; i++)
	{
		// x^2 berechnen
		xQuadrat = i*i;
		for (int j = -_Radius; j<=_Radius; j++)
		{
			// x^2 + y^2 berechnen
			xQuadratPlusYQuadrat = xQuadrat + j*j;

			for (int k = -_Radius; k<=_Radius; k++)
				// teste ob in Kugel: wenn ja -> Voxel zeichnen
				if ((xQuadratPlusYQuadrat+k*k) <= radiusQuadrat)
					Voxel[_X+i][_Y+j][_Z+k]=1;
					//ZeichneVoxel(_X+i,_Y+j,_Z+k);
		}
	}
}

// ======================================================
// Funktionen fuer Dichte (XY-Ebene von oben)
// ======================================================

// Projektion des Voxelraums in Ebene
void DunGen::CVoxelRaum::BerechneDichte()
{
	std::cout << "[Voxelraum]: Berechne Dichte...";

	// alte Dichte-Karten loeschen
	for (unsigned int i=0; i<VoxelraumX; i++)
		for (unsigned int j=0; j<VoxelraumY; j++)
			DichteXY[i][j]=0;

	for (unsigned int i=0; i<VoxelraumX; i++)
		for (unsigned int j=0; j<VoxelraumZ; j++)
			DichteXZ[i][j]=0;

	for (unsigned int i=0; i<VoxelraumY; i++)
		for (unsigned int j=0; j<VoxelraumZ; j++)
			DichteYZ[i][j]=0;

	// Dichte-Karten berechnen
	for (unsigned int i=0; i<VoxelraumX; i++)
		for (unsigned int j=0; j<VoxelraumY; j++)
			for (unsigned int k=0; k<VoxelraumZ; k++)
				if (Voxel[i][j][k]>0)
				{
					DichteXY[i][j]++;
					DichteXZ[i][k]++;
					DichteYZ[j][k]++;
				}

	// Maxima bestimmen
	DichteMaxXY = 0;
	DichteMaxXZ = 0;
	DichteMaxYZ = 0;

	for (unsigned int i=0; i<VoxelraumX; i++)
		for (unsigned int j=0; j<VoxelraumY; j++)
			if (DichteXY[i][j]>=DichteMaxXY)
				DichteMaxXY = DichteXY[i][j];

	for (unsigned int i=0; i<VoxelraumX; i++)
		for (unsigned int j=0; j<VoxelraumZ; j++)
			if (DichteXZ[i][j]>=DichteMaxXZ)
				DichteMaxXZ = DichteXZ[i][j];

	for (unsigned int i=0; i<VoxelraumY; i++)
		for (unsigned int j=0; j<VoxelraumZ; j++)
			if (DichteYZ[i][j]>=DichteMaxYZ)
				DichteMaxYZ = DichteYZ[i][j];

	std::cout << " abgeschlossen." << std::endl;
}

// Auslesen der Dichte
unsigned short DunGen::CVoxelRaum::LeseDichteAn(unsigned int _A, unsigned int _B, EEbene _Ebene)
{
	if (_Ebene==_EBENE_XY_)
		return DichteXY[_A][_B];
	else if (_Ebene==_EBENE_XZ_)
		return DichteXZ[_A][_B];
	return DichteYZ[_A][_B];
}
// Auslesen des Maximums der Dichte
unsigned short DunGen::CVoxelRaum::LeseDichteMaximum(EEbene _Ebene)
{
	if (_Ebene==_EBENE_XY_)
		return DichteMaxXY;
	else if (_Ebene==_EBENE_XZ_)
		return DichteMaxXZ;
	return DichteMaxYZ;
}

// ======================================================
// Funktionen fuer Dreiecksnetz
// ======================================================

void DunGen::CVoxelRaum::ErmittleDreiecksNetzDaten()
{
	unsigned int polygonZaehler = 0;

	unsigned int xMax = VoxelraumX - VoxelMinRandOffset;
	unsigned int yMax = VoxelraumY - VoxelMinRandOffset;
	unsigned int zMax = VoxelraumZ - VoxelMinRandOffset;

	// laufe ueber Voxelraum
	for (unsigned int i=VoxelMinRandOffset; i<=xMax; i++)
		for (unsigned int j=VoxelMinRandOffset; j<=yMax; j++)
			for (unsigned int k=VoxelMinRandOffset; k<=zMax; k++)
			{
				// zaehle die Uebergaenge
				if (Voxel[i-1][j][k]!=Voxel[i][j][k]) {polygonZaehler++;}
				if (Voxel[i][j-1][k]!=Voxel[i][j][k]) {polygonZaehler++;}
				if (Voxel[i][j][k-1]!=Voxel[i][j][k]) {polygonZaehler++;}
			}

	std::cout << "[Voxelraum]: zu erwartende Anzahl der Dreiecke: " << polygonZaehler*2 << std::endl;
}


// ======================================================
// Zeichenfunktionen fuer Turtlegrafik
// ======================================================

// Fraktal in Voxelraum zeichnen
void DunGen::CVoxelRaum::ZeichneGrafik(std::string _Anweisungen, double _WinkelGier, double _WinkelNick, double _WinkelRoll, double _StartRadius, double _RadiusFaktor, double _RadiusDekrementor)
{
	std::cout << "[Voxelraum]: Zeichnen wird vorbereitet. Anzahl der Zeichenanweisungen = " << _Anweisungen.size() << std::endl;
	Zeitmesser->Start(0);

	// Winkel umrechnen vom Gradmass in Bogenmass
	double winkelGierRAD = _WinkelGier/180 * M_PI;
	double winkelNickRAD = _WinkelNick/180 * M_PI;
	double winkelRollRAD = _WinkelRoll/180 * M_PI;

	// sinus und cosinus Werte ausrechnen, fuer die negativen Winkel gilt: cos(-x) = cos(x), sin(-x) = -sin(x)
	double cosWinkelGier = cos(winkelGierRAD);
	double sinWinkelGier = sin(winkelGierRAD);
	double cosWinkelNick = cos(winkelNickRAD);
	double sinWinkelNick = sin(winkelNickRAD);
	double cosWinkelRoll = cos(winkelRollRAD);
	double sinWinkelRoll = sin(winkelRollRAD);

	// Voxelraum leeren
	memset(Voxel,0,sizeof(unsigned char)*VoxelraumX*VoxelraumY*VoxelraumZ);

	// Zustandsstack
	std::stack<SZeichnenZustand> stackZustand;
	// Startzustand
	SZeichnenZustand aktZustand;
	SZeichnenZustand oldZustand;

	// zum Skalieren
	double xMin = 0.0;	double xMax = 0.0;
	double yMin = 0.0;	double yMax = 0.0;
	double zMin = 0.0;	double zMax = 0.0;
	double xMid, yMid, zMid;

	// Start einstellen
	double fWert = 1.0;	// Laenge eines Striches
	aktZustand.Position = irr::core::vector3d<double>(0,0,0);
	if (_StartRadius<MinRadius)
		_StartRadius = MinRadius;

	// Speicher pre-allokieren
	int Xalt, Yalt, Zalt, Xneu, Yneu, Zneu, RadiusNeu;

	// fuer Fortschrittsanzeige
	unsigned int counterAkt = 0;
	unsigned int counterInc = _Anweisungen.size()/100; // 1 Punkt = 1 Prozent

	int modus = 0; // modus 0: Bounding Box bestimmen, modus 1: Zeichnen
	_Anweisungen += ' '; // damit Zusammenfassung langer Linien immer ein Abschlusszeichen !='F'/'[' hat
	while (modus<2)
	{
		// Startwerte
		aktZustand.Radius = _StartRadius;
		aktZustand.Vorn = irr::core::vector3d<double>(1,0,0);
		aktZustand.Links = irr::core::vector3d<double>(0,0,1);
		aktZustand.Oben = irr::core::vector3d<double>(0,1,0);

		while (!stackZustand.empty()) stackZustand.pop(); // Stack leeren

		// String parsen
		for (unsigned int i=0; i<_Anweisungen.size(); i++)
		{
			// Fortschrittsanzeige
			if ((modus == 1)&&(i-counterAkt > counterInc))
			{
				counterAkt+=counterInc;
				std::cout << ".";
			}

			// --------------------------------------
			// Anfang der Zeichen-Befehl-Auswertung
			// -------------------------------------

			// Linie zeichnen
			if (_Anweisungen.at(i) == 'F')
			{
				oldZustand = aktZustand;

				// neue Position ermitteln
				aktZustand.Position += fWert*aktZustand.Vorn;

				// Optimierung: lange Linien zusammenfassen
				while (_Anweisungen.at(i+1) == 'F' || _Anweisungen.at(i+1) == '[')
				{
					if (_Anweisungen.at(i+1) == 'F')
						aktZustand.Position += fWert*aktZustand.Vorn;
					else
						stackZustand.push(aktZustand);
					i++;
				}

				if (modus == 0)
				{
					// Bounding Box neu berechnen
					if (aktZustand.Position.X < xMin) xMin = aktZustand.Position.X;
					if (aktZustand.Position.X > xMax) xMax = aktZustand.Position.X;
					if (aktZustand.Position.Y < yMin) yMin = aktZustand.Position.Y;
					if (aktZustand.Position.Y > yMax) yMax = aktZustand.Position.Y;
					if (aktZustand.Position.Z < zMin) zMin = aktZustand.Position.Z;
					if (aktZustand.Position.Z > zMax) zMax = aktZustand.Position.Z;
				}
				else
				{
					// Koordinaten konvertieren
					Xalt = (int) oldZustand.Position.X;
					Yalt = (int) oldZustand.Position.Y;
					Zalt = (int) oldZustand.Position.Z;
					Xneu = (int) aktZustand.Position.X;
					Yneu = (int) aktZustand.Position.Y;
					Zneu = (int) aktZustand.Position.Z;
					RadiusNeu = (int) aktZustand.Radius;

					// Zeichnen nur, wenn Aenderung es erfordert
					if(Xalt!=Xneu || Yalt!=Yneu || Zalt!=Zneu || RadiusNeu<(int)oldZustand.Radius)
					{
						// Strich
						ZeichneZylinder(Xalt,Yalt,Zalt,Xneu,Yneu,Zneu, RadiusNeu, aktZustand.Links, aktZustand.Oben);
						// Abschluss: Kugel wegen eventuellen nachfolgenden Richtungs-/Radiusaenderungen
						ZeichneKugel(Xneu,Yneu,Zneu, RadiusNeu);
					}
				}

			}
			// Radius veraendern
			else if (_Anweisungen.at(i) == '!')
			{
				aktZustand.Radius= _RadiusFaktor * aktZustand.Radius - _RadiusDekrementor;
				if (aktZustand.Radius < MinRadius)
					aktZustand.Radius = MinRadius;
			}

			// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			// STACKOPERATIONEN
			// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			// Stackoperationen: auf Stack legen
			else if (_Anweisungen.at(i) == '[')
				stackZustand.push(aktZustand);
			// Stackoperationen: vom Stack nehmen
			else if ((_Anweisungen.at(i) == ']')&&(!stackZustand.empty()))
			{
				aktZustand = stackZustand.top();
				stackZustand.pop();
			}

			// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			// ROTATIONEN
			// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			// Drehwinkelangaben: mathematisch positiver Sinn
			//
			// Winkel varieren: WinkelGier um Oben drehen (nach links): Roben(WinkelGier)
			else if (_Anweisungen.at(i) == '+')
			{
				// neue Vektoren berechnen & normalisieren fuer numerische Stabilitaet (bei absolut korrekter Berechnung sind die Vektoren schon Laenge 1)
				aktZustand.Vorn = cosWinkelGier * aktZustand.Vorn + sinWinkelGier * aktZustand.Links;
				aktZustand.Vorn.normalize();
				//aktZustand.Links = -sinWinkelGier * aktZustand.VornAlt + cosWinkelGier * aktZustand.LinksAlt;
				aktZustand.Links = aktZustand.Vorn.crossProduct(aktZustand.Oben);	// numerisch stabiler!
				aktZustand.Links.normalize();
			}
			// Winkel varieren: -WinkelGier um Oben drehen (nach rechts): Roben(-WinkelGier)
			else if (_Anweisungen.at(i) == '-')
			{
				// neue Vektoren berechnen
				aktZustand.Vorn = cosWinkelGier * aktZustand.Vorn - sinWinkelGier * aktZustand.Links;
				aktZustand.Vorn.normalize();
				aktZustand.Links = aktZustand.Vorn.crossProduct(aktZustand.Oben);
				aktZustand.Links.normalize();
			}
			// Winkel varieren: WinkelNick um Links drehen (nach unten): Rlinks(WinkelNick)
			else if (_Anweisungen.at(i) == 'u')
			{
				// neue Vektoren berechnen
				aktZustand.Vorn = cosWinkelNick * aktZustand.Vorn - sinWinkelNick * aktZustand.Oben;
				aktZustand.Vorn.normalize();
				aktZustand.Oben = aktZustand.Links.crossProduct(aktZustand.Vorn);
				aktZustand.Oben.normalize();
			}
			// Winkel varieren: -WinkelNick um Links drehen (nach oben): Rlinks(-WinkelNick)
			else if (_Anweisungen.at(i) == 'o')
			{
				// neue Vektoren berechnen
				aktZustand.Vorn = cosWinkelNick * aktZustand.Vorn + sinWinkelNick * aktZustand.Oben;
				aktZustand.Vorn.normalize();
				aktZustand.Oben = aktZustand.Links.crossProduct(aktZustand.Vorn);
				aktZustand.Oben.normalize();
			}
			// Winkel varieren: WinkelRoll um Vorn drehen (im UZS): Rvorn(WinkelRoll)
			else if (_Anweisungen.at(i) == 'z')
			{
				// neue Vektoren berechnen
				aktZustand.Links = cosWinkelRoll * aktZustand.Links + sinWinkelRoll * aktZustand.Oben;
				aktZustand.Links.normalize();
				aktZustand.Oben = aktZustand.Links.crossProduct(aktZustand.Vorn);
				aktZustand.Oben.normalize();
			}
			// Winkel varieren: -WinkelRoll um Vorn drehen (gegen UZS): Rvorn(-WinkelRoll)
			else if (_Anweisungen.at(i) == 'g')
			{
				// neue Vektoren berechnen
				aktZustand.Links = cosWinkelRoll * aktZustand.Links - sinWinkelRoll * aktZustand.Oben;
				aktZustand.Links.normalize();
				aktZustand.Oben = aktZustand.Links.crossProduct(aktZustand.Vorn);
				aktZustand.Oben.normalize();
			}
			// Winkel varieren: 180 Grad um Oben drehen
			else if (_Anweisungen.at(i) == '|')
			{
				aktZustand.Vorn = -aktZustand.Vorn;
				aktZustand.Links = -aktZustand.Links;
			}
			// Winkel varieren: horizontale Ausrichtung
			// Vorn und Links sollen wieder die XZ-Ebene aufspannen
			else if (_Anweisungen.at(i) == '$')
			{
				// Oben steht senkrecht auf XZ-Ebene
				aktZustand.Oben = irr::core::vector3d<double>(0,1,0);

				// numerisch stabileren Vektor ermitteln: steht moeglichst senkrecht auf Oben
				double linksWert = abs(aktZustand.Oben.dotProduct(aktZustand.Links));
				double vornWert = abs(aktZustand.Oben.dotProduct(aktZustand.Vorn));

				// weitere Vektoren ausrechnen: auf linkshaendiges Koordiatensystem achten (->Reihenfolge der Operanden vertauscht)
				// normalisieren zwecks numerischer Stabilitaet
				if (linksWert<vornWert)	// Links ist stabiler
				{
					// zuerst Vorn ausrechnen
					aktZustand.Vorn = aktZustand.Oben.crossProduct(aktZustand.Links);
					aktZustand.Vorn.normalize();
					// dann Links
					aktZustand.Links = aktZustand.Vorn.crossProduct(aktZustand.Oben);
					aktZustand.Links.normalize();
				}
				else // Vorn ist stabiler
				{
					// zuerst Links ausrechnen
					aktZustand.Links = aktZustand.Vorn.crossProduct(aktZustand.Oben);
					aktZustand.Links.normalize();
					// dann Vorn
					aktZustand.Vorn = aktZustand.Oben.crossProduct(aktZustand.Links);
					aktZustand.Vorn.normalize();
				}
			}
			// --------------------------------------
			// Ende der Zeichen-Befehl-Auswertung
			// -------------------------------------

		} // end for

		// Anpassen auf Zeichenbereich
		if (modus == 0)
		{
			// Zentrum bestimmen
			xMid = xMin + 0.5*(xMax - xMin);
			yMid = yMin + 0.5*(yMax - yMin);
			zMid = zMin + 0.5*(zMax - zMin);

			// Gueltige Werte fuer Skalierungsberechnung ermoeglichen
			if (xMax==xMin)	xMax++;
			if (yMax==yMin)	yMax++;
			if (zMax==zMin)	zMax++;

			// Randoffset = 2*Radius + 2*Rand + Rundungsoffset
			double randOffset = 2.0*(VoxelRandOffset+_StartRadius)+2.0;
			// Achsenverhaeltnisse ermitteln
			double skalierungX = (VoxelraumX-randOffset)/(xMax-xMin);
			double skalierungY = (VoxelraumY-randOffset)/(yMax-yMin);
			double skalierungZ = (VoxelraumZ-randOffset)/(zMax-zMin);

			// Minimum bestimmen: die globale Skalierung entspricht dem Minimum (nirgends soll der Zeichenbereich ueberschritten werden)
			fWert = std::min(skalierungX,std::min(skalierungY,skalierungZ));

			// neuen Startpunkt setzen (so dass der Mittelpunkt der Zeichnung im Mittelpunkt des Voxelraums liegt)
			aktZustand.Position.X = 0.5*((double)(VoxelraumX-1)) - fWert*xMid;
			aktZustand.Position.Y = 0.5*((double)(VoxelraumY-1)) - fWert*yMid;
			aktZustand.Position.Z = 0.5*((double)(VoxelraumZ-1)) - fWert*zMid;
			// Start zeichnen
			ZeichneKugel(aktZustand.Position.X,aktZustand.Position.Y,aktZustand.Position.Z,_StartRadius);

			std::cout << "[Voxelraum]: starte Zeichnen mit F-Laenge: " << fWert << " an: " << aktZustand.Position.X << "," << aktZustand.Position.Y << "," << aktZustand.Position.Z << std::endl;
		}

		modus++;

	} // end while

	std::cout << std::endl << "[Voxelraum]: Zeichnen abgeschlossen." << std::endl;
	std::cout << "[Voxelraum]: Benoetigte Zeit zum Zeichnen: ";
	Zeitmesser->Stop(0);

} // end ZeichneGrafik

// ======================================================
// Nachbearbeitung
// ======================================================

// Ist dieser Voxel Randvoxel?: wird von Erosion verwendet
inline bool DunGen::CVoxelRaum::IstRandVoxel(int _X,int _Y,int _Z)
{
	// Ein Randvoxel muss ein 0-Voxel sein
	if (Voxel[_X][_Y][_Z]!=0)
		return false;

	// Voxelnachbarn in 6er Nachbarschaft mit 1-Belegung?
	if (((Voxel[_X-1][_Y][_Z] | Voxel[_X+1][_Y][_Z] |
		Voxel[_X][_Y-1][_Z] | Voxel[_X][_Y+1][_Z] |
		Voxel[_X][_Y][_Z-1] | Voxel[_X][_Y][_Z+1]) & 1) == 1)
		return true;

	// sonst: kein Randvoxel
	return false;
}

// Ist dieser Voxel Randvoxel?: wird von Filterung verwendet
inline bool DunGen::CVoxelRaum::IstRandVoxelErweitert(int _X,int _Y,int _Z)
{
	// Ein Randvoxel muss ein 0-Voxel sein
	if (Voxel[_X][_Y][_Z]!=0)
		return false;

	// Voxelnachbarn in 6er Nachbarschaft mit 1-Belegung?
	if (((Voxel[_X-1][_Y][_Z] | Voxel[_X+1][_Y][_Z] |
		Voxel[_X][_Y-1][_Z] | Voxel[_X][_Y+1][_Z] |
		Voxel[_X][_Y][_Z-1] | Voxel[_X][_Y][_Z+1]) & 1) == 1)
		return true;

	// Erweitern auf 18er Nachbarschaft
	if (((Voxel[_X-1][_Y-1][_Z] | Voxel[_X-1][_Y+1][_Z] |
		Voxel[_X+1][_Y-1][_Z] | Voxel[_X+1][_Y+1][_Z] |

		Voxel[_X-1][_Y][_Z-1] | Voxel[_X-1][_Y][_Z+1] |
		Voxel[_X+1][_Y][_Z-1] | Voxel[_X+1][_Y][_Z+1] |

		Voxel[_X][_Y-1][_Z-1] | Voxel[_X][_Y-1][_Z+1] |
		Voxel[_X][_Y+1][_Z-1] | Voxel[_X][_Y+1][_Z+1]) & 1) == 1)
		return true;

	// sonst: kein Randvoxel
	return false;
}

// Ist dieser Voxel Anfang eines Randvoxelgebietes?: wird von Filterung verwendet
inline bool DunGen::CVoxelRaum::IstRandVoxelAnfang(int _X,int _Y,int _Z)
{
	// Ein Randvoxel muss ein 0-Voxel sein
	if (Voxel[_X][_Y][_Z]!=0)
		return false;

	// Voxelnachbarn mit 1-Belegung?
	if (Voxel[_X+1][_Y][_Z]==1)
		return true;

	// sonst: kein Randvoxel
	return false;
}


// Entfernung aller 0-Voxel die nicht zur aeusseren Zusammenhangskomponente der 0-Voxel gehoeren
void DunGen::CVoxelRaum::FilterAnwenden()
{
	std::cout << "[Voxelraum]: Filteranwendung wird gestartet." << std::endl;
	Zeitmesser->Start(0);
	unsigned int letztesXderSuche = 0;

	// beginne Schritt 1: auessere Huelle bestimmen
	std::cout << "Schritt 1, ";
	SVoxelZuZeichnen startVoxel;
	// Startvoxel fuer auessere Huelle finden
	for (unsigned int i=VoxelFilterRandOffset; i<VoxelraumX-VoxelFilterRandOffset; i++)
		for (unsigned int j=VoxelFilterRandOffset; j<VoxelraumY-VoxelFilterRandOffset; j++)
			for (unsigned int k=VoxelFilterRandOffset; k<VoxelraumZ-VoxelFilterRandOffset; k++)
				if (IstRandVoxelAnfang(i,j,k)) // wichtig hierfuer: X aufsteigend verwenden! (damit auessere Huelle)
				{
					// Voxel merken
					startVoxel.X=i; startVoxel.Y=j; startVoxel.Z=k;
					letztesXderSuche = i;
					// Suche abbrechen
					i = VoxelraumX; j = VoxelraumY; k = VoxelraumZ;
				}

	// Queue fuer Breitensuche ueber die Randvoxel
	// (Tiefensuch per Stack nicht empfehlenswert, da "Graph" nich kreisfrei -> extrem hoher Speicherbedarf (getestet))
	std::queue<SVoxelZuZeichnen> queueVoxel;
	SVoxelZuZeichnen aktuellerVoxel;
	queueVoxel.push(startVoxel);

	// aussere Huelle markieren
	while (!queueVoxel.empty())
	{
		// Voxel aus der Queue holen
		int test = queueVoxel.size();
		aktuellerVoxel = queueVoxel.front();
		queueVoxel.pop();

		// alle Nachbarvoxel betrachten (6er Nachbarschaft)
		if (IstRandVoxelErweitert(aktuellerVoxel.X-1,aktuellerVoxel.Y,aktuellerVoxel.Z))
		{
			// neuen Voxel erstellen
			SVoxelZuZeichnen neuerVoxel;
			neuerVoxel.X = aktuellerVoxel.X-1; neuerVoxel.Y = aktuellerVoxel.Y; neuerVoxel.Z = aktuellerVoxel.Z;
			// und in die Queue laden
			queueVoxel.push(neuerVoxel);
			// dann Voxel  markieren
			Voxel[neuerVoxel.X][neuerVoxel.Y][neuerVoxel.Z] = 2;
		}
		if (IstRandVoxelErweitert(aktuellerVoxel.X+1,aktuellerVoxel.Y,aktuellerVoxel.Z))
		{
			// neuen Voxel erstellen
			SVoxelZuZeichnen neuerVoxel;
			neuerVoxel.X = aktuellerVoxel.X+1; neuerVoxel.Y = aktuellerVoxel.Y; neuerVoxel.Z = aktuellerVoxel.Z;
			// und in die Queue laden
			queueVoxel.push(neuerVoxel);
			// dann Voxel  markieren
			Voxel[neuerVoxel.X][neuerVoxel.Y][neuerVoxel.Z] = 2;
		}
		if (IstRandVoxelErweitert(aktuellerVoxel.X,aktuellerVoxel.Y-1,aktuellerVoxel.Z))
		{
			// neuen Voxel erstellen
			SVoxelZuZeichnen neuerVoxel;
			neuerVoxel.X = aktuellerVoxel.X; neuerVoxel.Y = aktuellerVoxel.Y-1; neuerVoxel.Z = aktuellerVoxel.Z;
			// und in die Queue laden
			queueVoxel.push(neuerVoxel);
			// dann Voxel  markieren
			Voxel[neuerVoxel.X][neuerVoxel.Y][neuerVoxel.Z] = 2;
		}
		if (IstRandVoxelErweitert(aktuellerVoxel.X,aktuellerVoxel.Y+1,aktuellerVoxel.Z))
		{
			// neuen Voxel erstellen
			SVoxelZuZeichnen neuerVoxel;
			neuerVoxel.X = aktuellerVoxel.X; neuerVoxel.Y = aktuellerVoxel.Y+1; neuerVoxel.Z = aktuellerVoxel.Z;
			// und in die Queue laden
			queueVoxel.push(neuerVoxel);
			// dann Voxel  markieren
			Voxel[neuerVoxel.X][neuerVoxel.Y][neuerVoxel.Z] = 2;
		}
		if (IstRandVoxelErweitert(aktuellerVoxel.X,aktuellerVoxel.Y,aktuellerVoxel.Z-1))
		{
			// neuen Voxel erstellen
			SVoxelZuZeichnen neuerVoxel;
			neuerVoxel.X = aktuellerVoxel.X; neuerVoxel.Y = aktuellerVoxel.Y; neuerVoxel.Z = aktuellerVoxel.Z-1;
			// und in die Queue laden
			queueVoxel.push(neuerVoxel);
			// dann Voxel  markieren
			Voxel[neuerVoxel.X][neuerVoxel.Y][neuerVoxel.Z] = 2;
		}
		if (IstRandVoxelErweitert(aktuellerVoxel.X,aktuellerVoxel.Y,aktuellerVoxel.Z+1))
		{
			// neuen Voxel erstellen
			SVoxelZuZeichnen neuerVoxel;
			neuerVoxel.X = aktuellerVoxel.X; neuerVoxel.Y = aktuellerVoxel.Y; neuerVoxel.Z = aktuellerVoxel.Z+1;
			// und in die Queue laden
			queueVoxel.push(neuerVoxel);
			// dann Voxel  markieren
			Voxel[neuerVoxel.X][neuerVoxel.Y][neuerVoxel.Z] = 2;
		}
	}

	// beginne Schritt 2: freischwebende Voxelkomponenten loeschen
	std::cout << "Schritt 2 ";
	unsigned int anzahlFragmente = 0;
	// suche nach verbleibenden Randvoxeln
	for (unsigned int i=letztesXderSuche+2; i<VoxelraumX-VoxelFilterRandOffset; i++)
	{
		std::cout << ".";
		for (unsigned int j=VoxelFilterRandOffset; j<VoxelraumY-VoxelFilterRandOffset; j++)
			for (unsigned int k=VoxelFilterRandOffset; k<VoxelraumZ-VoxelFilterRandOffset; k++)
				if (IstRandVoxelAnfang(i,j,k))
				{
					// ein weiteres Fragment gefunden
					anzahlFragmente++;

					// Voxel merken
					startVoxel.X=i; startVoxel.Y=j; startVoxel.Z=k;
					queueVoxel.push(startVoxel);
					// Voxel loeschen
					Voxel[i][j][k] = 1;

					// loesche komplette Komponente
					while (!queueVoxel.empty())
					{
						// Voxel aus der Queue holen
						aktuellerVoxel = queueVoxel.front();
						queueVoxel.pop();

						// alle Nachbarvoxel betrachten (6er Nachbarschaft)
						// Pruefung auf Gueltigkeitsbereich nicht noetig, da diese Komponenten nicht am Rand des VoxelRaums auftreten
						if (Voxel[aktuellerVoxel.X-1][aktuellerVoxel.Y][aktuellerVoxel.Z]==0)
							{
								SVoxelZuZeichnen neuerVoxel;
								neuerVoxel.X = aktuellerVoxel.X-1; neuerVoxel.Y = aktuellerVoxel.Y; neuerVoxel.Z = aktuellerVoxel.Z;
								queueVoxel.push(neuerVoxel);
								// Voxel loeschen
								Voxel[neuerVoxel.X][neuerVoxel.Y][neuerVoxel.Z] = 1;
							}
						if (Voxel[aktuellerVoxel.X+1][aktuellerVoxel.Y][aktuellerVoxel.Z]==0)
							{
								SVoxelZuZeichnen neuerVoxel;
								neuerVoxel.X = aktuellerVoxel.X+1; neuerVoxel.Y = aktuellerVoxel.Y; neuerVoxel.Z = aktuellerVoxel.Z;
								queueVoxel.push(neuerVoxel);
								// Voxel loeschen
								Voxel[neuerVoxel.X][neuerVoxel.Y][neuerVoxel.Z] = 1;
							}
						if (Voxel[aktuellerVoxel.X][aktuellerVoxel.Y-1][aktuellerVoxel.Z]==0)
							{
								SVoxelZuZeichnen neuerVoxel;
								neuerVoxel.X = aktuellerVoxel.X; neuerVoxel.Y = aktuellerVoxel.Y-1; neuerVoxel.Z = aktuellerVoxel.Z;
								queueVoxel.push(neuerVoxel);
								// Voxel loeschen
								Voxel[neuerVoxel.X][neuerVoxel.Y][neuerVoxel.Z] = 1;
							}
						if (Voxel[aktuellerVoxel.X][aktuellerVoxel.Y+1][aktuellerVoxel.Z]==0)
							{
								SVoxelZuZeichnen neuerVoxel;
								neuerVoxel.X = aktuellerVoxel.X; neuerVoxel.Y = aktuellerVoxel.Y+1; neuerVoxel.Z = aktuellerVoxel.Z;
								queueVoxel.push(neuerVoxel);
								// Voxel loeschen
								Voxel[neuerVoxel.X][neuerVoxel.Y][neuerVoxel.Z] = 1;
							}
						if (Voxel[aktuellerVoxel.X][aktuellerVoxel.Y][aktuellerVoxel.Z-1]==0)
							{
								SVoxelZuZeichnen neuerVoxel;
								neuerVoxel.X = aktuellerVoxel.X; neuerVoxel.Y = aktuellerVoxel.Y; neuerVoxel.Z = aktuellerVoxel.Z-1;
								queueVoxel.push(neuerVoxel);
								// Voxel loeschen
								Voxel[neuerVoxel.X][neuerVoxel.Y][neuerVoxel.Z] = 1;
							}
						if (Voxel[aktuellerVoxel.X][aktuellerVoxel.Y][aktuellerVoxel.Z+1]==0)
							{
								SVoxelZuZeichnen neuerVoxel;
								neuerVoxel.X = aktuellerVoxel.X; neuerVoxel.Y = aktuellerVoxel.Y; neuerVoxel.Z = aktuellerVoxel.Z+1;
								queueVoxel.push(neuerVoxel);
								// Voxel loeschen
								Voxel[neuerVoxel.X][neuerVoxel.Y][neuerVoxel.Z] = 1;
							}
					}
				}
	}
	std::cout << " Anzahl der geloeschten Fragmente = " << anzahlFragmente;

	// beginne Schritt 3: Originalmarkierung wiederherstellen
	std::cout << " , Schritt 3" << std::endl;

	for (unsigned int i=letztesXderSuche; i<VoxelraumX-VoxelFilterRandOffset; i++)
		for (unsigned int j=VoxelFilterRandOffset; j<VoxelraumY-VoxelFilterRandOffset; j++)
			for (unsigned int k=VoxelFilterRandOffset; k<VoxelraumZ-VoxelFilterRandOffset; k++)
				if (Voxel[i][j][k] == 2)
					Voxel[i][j][k] = 0;		// Markierung wiederherstellen

	std::cout << "[Voxelraum]: Filteranwendung abgeschlossen." << std::endl;
	std::cout << "[Voxelraum]: Benoetigte Zeit fuer Filterung: ";
	Zeitmesser->Stop(0);
}

// Erodiere Voxelgebilde: Achtung: Rand des Voxelraums muss mindestens VoxelMinRandOffset Felder dick bleiben (fuer korrektes FilterAnwenden)
// Erosionwahrscheinlichkeit gibt Wahrscheinlichkeit fuer Erosion an:
// 1 ... es wird immer erodiert, 0 ... es wird nie erodiert
void DunGen::CVoxelRaum::Erodiere(double _ErosionsWahrscheinlichkeit)
{
	std::cout << "[Voxelraum]: Starte Erosion mit Erosionswahrscheinlichkeit " << _ErosionsWahrscheinlichkeit << ", markiere ";
	Zeitmesser->Start(0);

	// Schritt 1: markieren
	for (unsigned int i=VoxelMinRandOffset; i<VoxelraumX-VoxelMinRandOffset; i++)
	{
		std::cout << ".";
		for (unsigned int j=VoxelMinRandOffset; j<VoxelraumY-VoxelMinRandOffset; j++)
			for (unsigned int k=VoxelMinRandOffset; k<VoxelraumZ-VoxelMinRandOffset; k++)
				if (IstRandVoxel(i,j,k))
					// wenn Zufallswert im richtigen Bereich: markiere Randfeld fuer Erosion
					if (ZufallsGenerator->GeneriereZufallsWert_01()<=_ErosionsWahrscheinlichkeit)
						Voxel[i][j][k] = 2;
	}

	std::cout << " , loesche ...";
	// Schritt 2: loeschen
	for (unsigned int i=VoxelMinRandOffset; i<VoxelraumX-VoxelMinRandOffset; i++)
		for (unsigned int j=VoxelMinRandOffset; j<VoxelraumY-VoxelMinRandOffset; j++)
			for (unsigned int k=VoxelMinRandOffset; k<VoxelraumZ-VoxelMinRandOffset; k++)
				if (Voxel[i][j][k] == 2)
					Voxel[i][j][k] = 1;

	std::cout << std::endl << "[Voxelraum]: Erosion abgeschlossen." << std::endl;
	std::cout << "[Voxelraum]: Benoetigte Zeit fuer Erosion: ";
	Zeitmesser->Stop(0);
}
