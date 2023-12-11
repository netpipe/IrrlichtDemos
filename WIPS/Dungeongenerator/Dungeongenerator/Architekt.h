// zur Verhinderung doppelter Definition
#ifndef ARCHITEKT_H
#define ARCHITEKT_H

#include <iostream>
#include <vector>
#include <irrlicht.h>

#include "Spline.h"
#include "SubSzene.h"
#include "VoxelRaum.h"
#include "DreiecksMesh.h"
#include "ZufallsGenerator.h"
#include "Zeitmesser.h"

// Namespace DunGen : DungeonGenerator
namespace DunGen
{
	// Arten der Raumsektorbelegung
	enum ERaumSektorBelegung {_VOXEL_, _RAUM_, _GANG_, _FREI_, _FREI_MIT_VOXELNACHBAR_,  _FREI_MIT_VOXELNACHBAR2_, _FREI_MIT_RAUMNACHBAR_};
	// _VOXEL_					... von Voxeln belegt
	// _RAUM_					... Raum wurde platziert
	// _GANG_					... Sektor wird durch hindurchlaufenden Gang belegt
	// _FREI_MIT_VOXELNACHBAR_	... direkte Nachbarschaft zu Voxel-belegtem Sektor
	// _FREI_MIT_VOXELNACHBAR2_ ... erst in 2 Sektoren Abstand Voxel-belegter Sektor
	// _FREI_MIT_RAUMNACHBAR_	... Raum in Nachbarschaft

	// die Beschreibung eines Raums im Dungeon
	struct SDungeonRaum
	{
		// Position, Rotation und Skalierung des Raums
		irr::core::vector3d<double> Position;
		irr::core::vector3d<double> Rotation;
		irr::core::vector3d<double> Skalierung;
		// die zugrundeliegende SubSzene
		CSubSzene* SubSzene;
		// die Andockstellen des Raums: Nord, Ost, Sued, West - schon entsprechend Rotation und Position transformiert
		// falls die Rotation mehr als 45 Grad betraegt, wurden die Himmelsrichtungen ebenfalls neu angepasst
		SSplineAndockStelle Andockstelle[4];

		// ID der angedockten Gaenge (-1 ... nicht vorhanden)
		int GangID[4];
		// Richtungs-ID Offset: Offset zwischen aktuelle Himmelsrichtung und Originalhimmelsrichtung
		// wichtig um beim Loeschen ueberfluessiger Geometrien (Andock bzw. Verschluss) die richtigen Objekte zu adressieren
		unsigned int Offset;
	};

	struct SRaumSektorAndockKarte; // Deklaration nach CArchitekt

	// fuer Beschreibung eines Sektors im Voxelraum
	struct SRaumSektor
	{
		// Konstruktor
		SRaumSektor();
		// Destruktor
		~SRaumSektor();

		// Ausdehnung
		int GrenzeMinX, GrenzeMaxX, GrenzeMinY, GrenzeMaxY, GrenzeMinZ, GrenzeMaxZ;
		// Belegung des Sektors
		ERaumSektorBelegung Belegung;
		// Andockkarten, falls Nachbar zu Voxel-belegtem Sektor
		// Himmelsrichtungen Nord, Sued, Ost, West mit Nord = X-Positiv, Ost = Z-Negativ, ...
		SRaumSektorAndockKarte* Karte[4];

		// ID des Raums, falls mit _RAUM_ belegt
		int RaumID;
	};

	// Vorlage fuer ein Detailobjekt im Gang
	struct SSplineDetailobjektVorlage
	{
		// Pfad des Meshes
		irr::io::path MeshPfad;
		// zugrundeliegender Mesh
		irr::scene::IMesh* ObjektMesh;

		// Benennung
		irr::core::stringc Name;

		// Skalierung
		irr::core::vector3d<double> Skalierung;
		// Basisrotation fuer Spline-Richtung (1,0,0)
		irr::core::vector3d<double> Rotation;
		// Position in Gang-Querschnittskoordinaten gerechnet (-1...1, -1...1)
		irr::core::vector2d<double> Position;
		// Abstand zweier Objekte auf dem Spline
		double AbstandsAbtastung;					// Abstastung (i.A. sinnvoll: gleiche Abtastung wie Spline-Schlauch)
		unsigned int AbstandNumFaktor;				// Objekte werden in folgenden Abstanden platziert: Abstastung*AbstandNumFaktor*(AbstandNumMin...AbstandNumMax)
		unsigned int AbstandNumMin;					// bei Detailobjekt-Abstastung = Spine-Schlauchabtastung: AbstandNumFaktor = 2^Detailstufe -> bis Detailstufe definitv korrekt bzgl. Spline-Schlauch platziert
		unsigned int AbstandNumMax;					//
		unsigned int AbstandNumMinErstesElement;	// Werte fuer das erste Objekt, Abstand von P(t=0) aus gemessen
		unsigned int AbstandNumMaxErstesElement;	//
		bool ObjektAn1;								// wird ein Objekt an P(t=1) platziert, auch wenn der Abstand zum Vorgaengerobjekt zu klein waere?
	};

	// Die Klasse des Architekten: setzt Raeume und verlegt Gaenge
	class CArchitekt
	{
	public:
		// Konstruktor
		CArchitekt(CVoxelRaum* _VoxelRaum, CDreiecksMesh* _DreiecksMesh, CZufallsGenerator* _ZufallsGenerator, CZeitmesser* _Zeitmesser);
		void Initialisiere(irr::IrrlichtDevice* _IrrDevice);

		// Konstruiere Raeume und Gaenge des Dungeons ausgehend von gegebener Voxelhoehle
		// traegt entsprechende Gang- und Raum-ID's in die Sektoren ein, und aktualisiert deren Belegung
		void KonstruiereDungeon(unsigned int _RaumSektorAusdehnung);

		// Werte setzen
		void SetzeGangBreite(unsigned int _GangBreite);
		void SetzeAbstandSplineSegmente(double _AbstandSplineSegmente);
		void SetzeAbstandTexturYProAbstand1(double _AbstandTexturYProAbstand1);
		void SetzeAnzahlSplineDetailstufen(unsigned int _AnzahlSplineDetailstufen);

		void SetzeZusaetzlicheAusfraestiefe(unsigned int _ZusaetzlicheAusfraestiefe);
		void SetzeAbstandVomBodenMax(unsigned int _AbstandVomBodenMax);
		void SetzeTiefeAbstandsTestMin(unsigned int _TiefeAbstandsTestMin);
		void SetzeAbstandAndockHoehleMin(unsigned int _AbstandAndockHoehleMin);
		void SetzeZusaetzlicherSicherheitsabstand(double _ZusaetzlicherSicherheitsabstand);

		void SetzePlatzierungsWahrscheinlichkeitsFaktorAnHoehleLeereSubSzene(unsigned int _PlatzierungsWahrscheinlichkeitsFaktorAnHoehleLeereSubSzene);
		void SetzePlatzierungsWahrscheinlichkeitsFaktorAnRaumLeereSubSzene(unsigned int _PlatzierungsWahrscheinlichkeitsFaktorAnRaumLeereSubSzene);
		void SetzeWahrscheinlichkeitZusaetzlicherGangRaumHoehle(double _WahrscheinlichkeitZusaetzlicherGangRaumHoehle);
		void SetzeWahrscheinlichkeitZusaetzlicherGangRaumRaum(double _WahrscheinlichkeitZusaetzlicherGangRaumRaum);

		// Werte auslesen
		unsigned int LeseGangBreite();
		double LeseAbstandSplineSegmente();
		double LeseAbstandTexturYProAbstand1();
		unsigned int LeseAnzahlSplineDetailstufen();

		unsigned int LeseZusaetzlicheAusfraestiefe();
		unsigned int LeseAbstandVomBodenMax();
		unsigned int LeseTiefeAbstandsTestMin();
		unsigned int LeseAbstandAndockHoehleMin();
		double LeseZusaetzlicherSicherheitsabstand();

		unsigned int LesePlatzierungsWahrscheinlichkeitsFaktorAnHoehleLeereSubSzene();
		unsigned int LesePlatzierungsWahrscheinlichkeitsFaktorAnRaumLeereSubSzene();
		double LeseWahrscheinlichkeitZusaetzlicherGangRaumHoehle();
		double LeseWahrscheinlichkeitZusaetzlicherGangRaumRaum();

		// Die Gaenge und Raeume
		std::vector<SDungeonRaum*> Raeume;
		std::vector<CSpline*> Gaenge;

		// Raumvorlagen
		std::vector<CSubSzene*> SubSzenen;
		// Detailobjektvorlagen
		std::vector<SSplineDetailobjektVorlage*> DetailobjektVorlagen;
		// Gangquerschnitt (Rohversion, Punktkoordinaten in [-1,1])
		SSplineQuerschnitt QuerschnittGang_Normiert;

	private:
		// Raumsektor-Daten ermitteln
		void BerechneRaumSektoren(unsigned int _RaumSektorAusdehnung);

		// Raum erstellen
		void ErstelleRaum(CSubSzene* _SubSzene, irr::core::vector3d<double> _Position, irr::core::vector3d<double> _Rotation);

		// 2 Raeume miteinander verbinden
		// Himmelsrichtungen sind 0...Nord (X positiv), 1...Ost (Z negativ), 2...Sued (X negativ), 3...West (Z positiv)
		void VerbindeRaeume(unsigned int _IdRaum1, unsigned int _HimmelsrichtungRaum1, unsigned int _IdRaum2, unsigned int _HimmelsrichtungRaum2);

		// versuchen Raum per Gang mit Hoehle zu verbinden, Himmelsrichtung siehe oben (vom Raum bzw. Sektor aus gesehen)
		// Erfolg: Gang ist erstellt, true wird zurueckgeliefert
		// Misserfolg: kein Gang wurde erstellt, false wird zurueckgeliefert
		bool VerbindeRaumMitHoehle(unsigned int _IdRaum, int _SektorX, int _SektorY, int _SektorZ, unsigned int _Himmelsrichtung);

		// Verbindet Raum mit Andockposition
		// Andockposition in Hoehenkartenkoordinaten uebergeben: Eckpunkt der Gang-Begrenzung mit den kleinsten Koordinaten (links unten bzw. rechts unten)
		bool VerbindeRaumMitHoehlenAndockposition(unsigned int _IdRaum, int _SektorX, int _SektorY, int _SektorZ,
			unsigned int _Himmelsrichtung, unsigned int _KartenKoordinate1, unsigned int _KartenKoordinate2,
			int _ScanMin, int _ScanMax, int _Ausfraestiefe);

		// Revalidiere alle Andockkarten bezueglich Konfliktgebiet
		void RevalidiereAndockKarten(int _SektorX, int _SektorY, int _SektorZ,
			int _KonfliktgebietMinX, int _KonfliktgebietMaxX, int _KonfliktgebietMinY, int _KonfliktgebietMaxY,	int _KonfliktgebietMinZ, int _KonfliktgebietMaxZ);

		// Revalidiere eine Andockkarte bezueglich Konfliktgebiet
		void RevalidiereAndockKarte(int _SektorX, int _SektorY, int _SektorZ, unsigned int _Himmelsrichtung,
			int _KonfliktgebietMinX, int _KonfliktgebietMaxX, int _KonfliktgebietMinY, int _KonfliktgebietMaxY,	int _KonfliktgebietMinZ, int _KonfliktgebietMaxZ);

		// Testet auf eine potentielle Kollision von Hoehle und Gang: true .. Kollision moeglich, false ... keine Kollision moeglich
		// getestet werden alle Punkte auf gegebener Hoehnkarte
		bool TesteKollisionHoehleGang(irr::core::vector3d<double> _Position1, irr::core::vector3d<double> _Position2, irr::core::vector3d<double> _Ableitung2,
			SRaumSektorAndockKarte* _HoehenKarte, unsigned int _Himmelsrichtung);

		// die Irrlicht-Device: wird zum Erstellen von SubSzenen benoetigt
		irr::IrrlichtDevice* IrrDevice;

		// Zeitmessung
		CZeitmesser* Zeitmesser;
		// Zufallsgenerator
		CZufallsGenerator* ZufallsGenerator;
		// Daten der Voxel-Hoehle
		CVoxelRaum* VoxelRaum;
		CDreiecksMesh* DreiecksMesh;

		// Gang-Eigenschaften
		unsigned int GangBreite;				// Breite des Gangs in Voxeln (Gangquerschnitt ist Breite*Breite), darf hoechstens 2*RaumSektorAusdehnung betragen (besser deutlich weniger)
		unsigned int GangBreiteHalbKlein;		// (Gangbreite - 1 Mittelvoxel)/2 aberundet
		unsigned int GangBreiteHalbGross;		// (Gangbreite - 1 Mittelvoxel)/2 aufgerundet
												// GangBreiteHalbKlein + GangBreiteHalbGross + 1 = GangBreite
		unsigned int ZusaetzlicheAusfraestiefe;	// zusaetzliche Tiefe beim Ausfraesen in Voxeln gemessen
		unsigned int AbstandVomBodenMax;		// Maximaler Abstand des Gangs vom Boden (gemessen vom unteren Gangteil), 0 ... wird ignoriert
		unsigned int TiefeAbstandsTestMin;		// Erforderliche Minimal-Tiefe fuer Abstandstest zum Boden
		unsigned int AbstandAndockHoehleMin;	// Minimal-Abstand zwischen Sektor eines Raums und Andockstelle der Hoehle (empfohlen RaumSektorAusdehnung/2, min.0, max. RaumSektorAusdehnung)
		double ZusaetzlicherSicherheitsabstand;	// Zusaetzlicher Sicherheitsabstand fuer Kollisionstest Gang<->Hoehle, wird als Faktor gerechnet 1.0 ... kein Zusatzabstand

		double AbstandSplineSegmente;
		double AbstandTexturYProAbstand1;
		unsigned int AnzahlSplineDetailstufen;	// Anzahl zusaetzlicher Detailstufen fuer die Spline-Gaenge

		// Gangquerschnitt (schon mit Gangbreite/2 multipliziert)
		SSplineQuerschnitt QuerschnittGang;

		// Weitere Eigenschaften
		unsigned int PlatzierungsWahrscheinlichkeitsFaktorAnHoehleLeereSubSzene;	// Wahrscheinlichkeitsfaktor, dafuer dass leere Szene an Hoehle platziert wird
		unsigned int PlatzierungsWahrscheinlichkeitsFaktorAnRaumLeereSubSzene;		// Wahrscheinlichkeitsfaktor, dafuer dass leere Szene an Raum platziert wird
		double WahrscheinlichkeitZusaetzlicherGangRaumHoehle;						// Wahrscheinlichkeit fuer einen zusaetzlichen Gang von Raum zu Hoehle
		double WahrscheinlichkeitZusaetzlicherGangRaumRaum;							// Wahrscheinlichkeit fuer einen zusaetzlichen Gang von Raum zu Raum

		// Kantenlaenge eines Raumsektors in Voxeln
		unsigned int RaumSektorAusdehnung;

		// Anzahl der Raumsektoren
		unsigned int RaumSektorAnzahlX;
		unsigned int RaumSektorAnzahlY;
		unsigned int RaumSektorAnzahlZ;

		// Raumsektoren
		SRaumSektor*** Raumsektor;
	};

	// fuer Beschreibung eines Sektors im Voxelraum: Karten mit Andockinformationen
	struct SRaumSektorAndockKarte
	{
		// Konstruktor
		SRaumSektorAndockKarte(unsigned int _RaumSektorAusdehnung, unsigned int _GangBreite, unsigned int _AbstandVomBodenMax, int _SektorVoxelMin1, int _SektorVoxelMin2);
		// Destruktor
		~SRaumSektorAndockKarte();

		// Hoehenkarte: Hoehen-Scan der Voxel in jeweilige Achsenrichtung, vom Raumsektor aus
		int** Hoehenkarte;
		// Valide Hoeheninformation: der Bereich vor diesem Voxel ist frei (kein Gang o.a. davor)
		// 0 ... nicht valide, 1 ... valide
		unsigned char** ValideHoeheninformation;

		// Ausdehnung der Hoehenkarte
		unsigned int Ausdehnung1, Ausdehnung2;

		// Offsets der Hoehenkarte zum Voxelraum:
		// VoxelKoord+Offset = KartenKoord
		int Offset1, Offset2;
	};



} // END NAMESPACE DunGen

#endif
