// zur Verhinderung doppelter Definition
#ifndef SPLINE_H
#define SPLINE_H

#include <vector>
#include <iostream>
#include <irrlicht.h>
#include "Hilfsfunktionen.h"
#include "ZufallsGenerator.h"

// Namespace DunGen : DungeonGenerator
namespace DunGen
{
	// Die Struktur des Splinequerschnitts
	// eine Kette von 2D-Punkten: geschlossene Kurve
	// angegebenen im Uhrzeigersinn!
	// wichtig: Winkel der Punkte zirkulaer aufsteigend, 0-Pkt. von Kurve umschlossen -> Splinequerschnitt sollte mindestens 3 Punkte besitzen
	struct SSplineQuerschnitt
	{
		std::vector<irr::core::vector2d<double> > Punkt;	// Punkte des Querschnitts
		std::vector<double> TextureX;					// X-Texturkoordinate fuer jeden Punkt
	};

	// Die Struktur einer Splineandockstelle
	// eine Kette von 3D-Punkten: geschlossene Kurve
	// angegebenen im Uhrzeigersinn!
	// wichtig: Winkel der Punkte zirkulaer aufsteigend, 0-Pkt. von Kurve umschlossen
	struct SSplineAndockStelle
	{
		std::vector<irr::core::vector3d<double> > Punkt;	// Punkte der Andockstelle
		irr::core::vector3d<double> Normale;			// Normale der durch die Punkte approximierten Flaeche der Andockstelle
		irr::core::vector3d<double> Mittelpunkt;		// Mittelpunkt der durch die Punkte approximierten Flaeche der Andockstelle
		double Ausdehnung;								// maximale Ausdehnung der Andockstelle in Normalenrichtung

		void TransformiereAndockstelle(irr::core::matrix4 _TransformationsMatrix); // Andockstelle mit enstprechender Transformationsmatrix transformieren
	};

	// fuer das Zeichnen des Splineadapters (interne Nutzung)
	struct SSplineAdapterZeichenanweisung
	{
		irr::core::vector3d<double> Punkt;		// zu zeichnender Punkt
		irr::core::vector2d<double> Textur;		// Texturkoordinaten des Punktes
		double Winkel;							// welchen Winkel hat der Punkt wenn er auf die Andockstellenebene projiziert wird
		unsigned int korrespondierenderIndex;	// welcher Spline-Index ist vom Winkel her am aehnlichsten
		bool TexturXvonSpline;					// wurde TexturX direkt vom Spline uebernommen?
	};

	// Detailobjekt entlang des Splines: z.B. Fackeln in einem Gang
	struct SSplineDetailobjekt
	{
		// Benennung
		irr::core::stringc Name;
		// Mesh und Skalierung bleiben konstant fuer einen Objekttyp
		irr::scene::IMesh* ObjektMesh;
		irr::core::vector3d<double> Skalierung;
		// Position und Rotation aendern sich entlang des Splineverlaufs
		std::vector<irr::core::vector3d<double> > Position;
		std::vector<irr::core::vector3d<double> > Rotation;
	};

	// fuer Visibility-Test:
	// Region auf dem Spline fuer Test bzgl. Viewing Volume
	struct SSplineViewRegionOfInterest
	{
		// Position des Mittelpunktes des Querschnitts
		irr::core::vector3d<double> Position;
		// Vektoren die den Querschnitt aufspannen
		irr::core::vector3d<double> Links;
		irr::core::vector3d<double> Oben;
		// Ausdehnung des Querschnitts
		irr::core::rect<double> QuerschnittBB;
	};

	// Viewing-Volume fuer Test auf Blickdichtigkeit
	struct SSplineViewingVolume
	{
		// Bilde Viewing Volume aus 2 Region of Interests
		SSplineViewingVolume(SSplineViewRegionOfInterest* _Region1, SSplineViewRegionOfInterest* _Region2);

		// Bounding Box des Viewing Volumes auf der Projektionsebene
		irr::core::rect<double> ViewingVolumeBB;
		// Mittelpunkt der Projektionsebene
		irr::core::vector3d<double> Bezugspunkt;
		// Vektoren die die Projektionsebene aufspannen
		irr::core::vector3d<double> Links;
		irr::core::vector3d<double> Oben;
	};

	// Die Klasse des Splines
	class CSpline
	{
	public:
		// Konstruktor: Spline erstellen, hier: Richtung zum Gang
		// wichtig: Richtung sollte nicht dem Hoehenvektor Y entsprechen!
		CSpline(SSplineQuerschnitt _QuerSchnitt, SSplineAndockStelle _AndockStelle1, SSplineAndockStelle _AndockStelle2,
			irr::core::vector3d<double> _Position1, irr::core::vector3d<double> _Position2, irr::core::vector3d<double> _Ableitung1, irr::core::vector3d<double> _Ableitung2,
			double _Abstand, double _AbstandTexturYProAbstand1, unsigned int _NumDetailstufen);
		 // Destruktor
		~CSpline();

		// Detailobjekt platzieren: Rotation ist Ausgangsrotation, wenn Spline nach (0,0,1) zeigend, Position in 2D bzgl. der Ebene des Querschnitts des Spline-Schlauchs
		// Objekt wird in mit dem zufaelligen Abstand*(AbstandNumMin bis AbstandNumMax) platziert
		// erstes Element wird in Abstand*(AbstandNumMinEE bis AbstandNumMaxEE) von P(t=0) platziert
		// ObjektAn1 gibt an, ob ein Objekt an P(t=1) platziert werden soll, auch wenn der Abstand nicht ausreicht
		void PlatziereDetailobjekt(irr::core::stringc _Name, irr::scene::IMesh* _ObjektMesh, irr::core::vector3d<double> _Skalierung, irr::core::vector3d<double> _Rotation, irr::core::vector2d<double> _Position,
			double _AbstandsAbtastung, unsigned int _AbstandNumFaktor, unsigned int _AbstandNumMin, unsigned int _AbstandNumMax,
			unsigned int _AbstandNumMinErstesElement, unsigned int _AbstandNumMaxErstesElement,	bool _ObjektAn1, CZufallsGenerator* _ZufallsGenerator);

		// ID's der Andockstellen einstellen (Informationen womit der Spline an diesen Enden verbunden ist)
		void SetzeAndocksstellenIds(int _IDAndockstelle0, int _IDAndockstelle1);
		// Andockstellen-ID's auslesen
		int LeseAndocksstellenId0();
		int LeseAndocksstellenId1();
		// Blickdichtigkeit auslesen
		bool LeseDefinitivBlickdicht();
		// Position und Ableitung auslesen
		irr::core::vector3d<double> LesePosition(unsigned int _Ende);
		irr::core::vector3d<double> LeseAbleitung(unsigned int _Ende);

		// Dreiecks-Mesh des Splines in den verschiedenen Detailstufen (0...Original)
		std::vector<irr::scene::SMesh*> MeshSpline;
		// Dreiecks-Meshes der Adapter: Verbindung von Adockstelle und Spline
		irr::scene::SMesh* MeshAdapter[2];

		// Detailobjekte
		std::vector<SSplineDetailobjekt*> Detailobjekte;

		// Standardvektor fuer Oben, definiert also die Hoehenachse
		static const irr::core::vector3d<double> ObenStandard;

		// Genauigkeit fuer Abstand bei suche des Parameters t
		static const double Genauigkeit;
		// Toleranz fuer Bisektionssuche
		//static const double Toleranz;

	private:
		// Erstellung des Dreiecksnetzes des Splines: _Abstand -> gewuenschter Abstand zwischen den einzelnen Querschnittssegmenten des Splines
		void ErstelleSpline(SSplineQuerschnitt _QuerSchnitt, SSplineAndockStelle _AndockStelle1, SSplineAndockStelle _AndockStelle2,
			double _Abstand, double _AbstandTexturYProAbstand1, unsigned int _Detailstufe);

		// Erstelle Adapter: Verbindung von Adockstelle und Spline
		// _AbstandTexturYProAbstand1 ... _AbstandTexturY/_Abstand -> normierter Textur-Abstand
		void ErstelleAdapter(unsigned int _Ende, SSplineAndockStelle _Andockstelle, SSplineQuerschnitt _QuerSchnitt, double _LetztesTextureY, double _AbstandTexturYProAbstand1);

		// Berechnung des naechsten t-Werts, fuer Punkt in angegebenem Abstand, Rueckgabe: (t,Abstand der Punkte vom letzten t zu neuem t)
		inline irr::core::vector2d<double> ErmittleT(irr::core::vector3d<double> _LetztePosition, double _LetztesT, double _Abstand);

		// Berechnung der Position zu gegebenem T
		inline irr::core::vector3d<double> BestimmePosition(double _T);
		// Berechnung der Ableitung zu gegebenem T
		inline irr::core::vector3d<double> BestimmeAbleitung(double _T);

		// Berechnung des Interpolationsfaktors fuer Position P neuer Vertices in Andockstelle, P liegt auf Strecke AB vom Dreieck AMB
		inline double BerechneInterpolationsFaktorPosition(irr::core::vector2d<double> _MA, irr::core::vector2d<double> _MB, double _WinkelAMP);

		// Projiziere einen Vektor auf eine Ebene mit gegebenen Links und Oben Vektor (beide normiert)
		// Rueckgabe: (X,Y)-Koordinaten in der Ebene mit Xpos ... -links, Ypos ... oben
		inline irr::core::vector2d<double> ProjiziereVektorAufEbene(irr::core::vector3d<double> _Vektor, irr::core::vector3d<double> _Links, irr::core::vector3d<double> _Oben);

		// Positions- und Ableitungskoeffizienten berechnen
		void BerechneResultierendeKoeffizienten();

		// Teste ob Spline-Schlauch incl. Adapter blickdicht
		//void BereiteVisibilityTestVor(SSplineQuerschnitt _QuerSchnitt, SSplineAndockStelle _AndockStelle1, SSplineAndockStelle _AndockStelle2);
		void VisibilityTest(SSplineQuerschnitt _QuerSchnitt, SSplineAndockStelle _AndockStelle1, SSplineAndockStelle _AndockStelle2);

		// Testet ob eine Region of Interest das ausserhalb des Vieving Volumes liegt -> ja bedeutet blickdicht
		inline bool IstBlickdicht(SSplineViewingVolume* _ViewingVolume, SSplineViewRegionOfInterest* _RegionOfInterest);

		// Position und erste Ableitungen der beiden Endpunkte
		irr::core::vector3d<double> Position[2];
		irr::core::vector3d<double> Ableitung[2];

		// Koeffizienten zur Berechnung der Position P(t): 3x4 Matrix
		// P(t) = (t^3,t^2,t,1) * PositionsKoeffizienten[0...3]
		irr::core::vector3d<double> PositionsKoeffizienten[4];
		// Koeffizienten zur Berechnung der ersten Ableitung P1(t): 3x3 Matrix
		// P1(t) = (t^2,t,1) * AbleitungsKoeffizienten[0...2]
		irr::core::vector3d<double> AbleitungsKoeffizienten[3];

		// Ist der Spline-Schlauch blickdicht? -> Kann man von einem Ende zum anderen sehen?
		bool DefinitivBlickdicht;

		// Anzahl von Abstastungen von RegionOfInterests (lokaler Maxima bzgl. Abstand Pkt<->Achse P[1]-P[0]) -> im Regelfall 1 oder 2 Werte
		// zugehoerige Querschnitte muessen in jeder Detailstufe enthalten sein, damit Visibility-Informationen korrekt
		std::vector<unsigned int> ROINumAbtast;
		std::vector<SSplineViewRegionOfInterest> ViewTestRegionen;	// Interessante Regionen fuer Visibility-Test, aufsteigen bzgl. t entlang des Splines geordnet

		// ID's bezueglich der Andockstellen: Informationen womit der Spline an diesen Enden verbunden ist
		int IDAndockstelle0, IDAndockstelle1;

		// maximale Anzahl von Vertices fuer einen Meshbuffer
		static const unsigned int MaxAnzahlVertices = 65000;
	};

} // END NAMESPACE DunGen

#endif
