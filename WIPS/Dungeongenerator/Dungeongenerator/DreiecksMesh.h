// zur Verhinderung doppelter Definition
#ifndef DREIECKSMESH_H
#define DREIECKSMESH_H

#include <vector>
#include <queue>
#include <list>

#include <iostream>
#include <irrlicht.h>
#include "VoxelRaum.h"
#include "Hilfsfunktionen.h"
#include "ZufallsGenerator.h"
#include "Zeitmesser.h"

// Namespace DunGen : DungeonGenerator
namespace DunGen
{
	// Methoden zur Normalenwichtung
	enum ENormalenWichtungsmethode {_NORMALENWICHTUNG_FLAECHENINHALT_ = 0, _NORMALENWICHTUNG_WINKEL_ = 1, _NORMALENWICHTUNG_UNIFORM = 2};

	// Octree-Knoten fuer Umwandlung VoxelRaum->Dreiecksmesh
	struct SOctreeKnoten
	{
		SOctreeKnoten* Kind[8];
		SOctreeKnoten* Elterknoten;
		unsigned int GrenzeMinX, GrenzeMaxX, GrenzeMinY, GrenzeMaxY, GrenzeMinZ, GrenzeMaxZ;
		unsigned int VertexAnzahl;

		unsigned int ZaehleVertices();
	};
	// wie duerfen die Vertex-Koordinaten beim evtl. Verwackeln modifiziert werden?
	struct SVertexModifikatoren
	{
		// erlaubte Richtungen: -1...negativ, +1...positiv, 0...negativ & positiv
		int RichtungX, RichtungY, RichtungZ;
	};

	// Structs fuer Normalenberechnung:
	// wo kommt ein Vertex vor?
	struct SVertexAdresse
	{
		unsigned int MeshbufferID;
		unsigned int VertexID;
	};
	// weiter Informationen ueber einen Vertex
	struct SVertexInformationen
	{
		unsigned int Y;							// Y-Koordinate (X,Z werden durch Hash-Tabelle festgelegt)
		irr::core::vector3d<double> Normale;	// Normale
		SVertexAdresse Adresse;					// Vorkommen dieses Vertex

		// Vergleichsoperator fuer Sortieren in Hashtabelle
		bool operator<(const SVertexInformationen _AndereVI);
	};

	// Informationen fuer die Sichtbarkeitseigenschaften der Voxelgebiete, die zu Vertices gewandelt werden
	struct SVoxelGebietVisibilityEigenschaften
	{
		// Gebietsgrenzen in Voxeln gerechnet (die Octreeknoten-Grenzen, nicht die tatsachliche Ausdehnung!)
		unsigned int GrenzeMinX, GrenzeMaxX, GrenzeMinY, GrenzeMaxY, GrenzeMinZ, GrenzeMaxZ;
		// Blickdicht: true... blickdicht, false ... in diese Richtung kann geblickt werden
		bool Xpos, Xneg, Ypos, Yneg, Zpos, Zneg;
	};

	// Klasse fuer Speichern und Generieren eines Dreiecksmeshes
	class CDreiecksMesh
	{
	public:
		// Initialisierung: Voxelraum uebernehmen
		 CDreiecksMesh(CVoxelRaum* _VoxelRaum, CZufallsGenerator* _ZufallsGenerator, CZeitmesser* _Zeitmesser);

		// Voxel in Mesh umwandeln
		void WandleVoxelInMesh(unsigned int _DetailStufe);

		// Eigenschaften einstellen und auslesen
		void SetzeZufallsSeedVerwackeln(unsigned int _ZufallsSeed);
		void SetzeVerwacklungsStaerke(double _VerwacklungsStaerke);
		unsigned int LeseZufallsSeedVerwackeln();
		double LeseVerwacklungsStaerke();

		void SetzeOptionVerwackeln(bool _Verwackeln);
		void SetzeOptionGlaetten(bool _Glaetten);
		bool LeseOptionVerwackeln();
		bool LeseOptionGlaetten();

		void SetzeNormalenwichtungsmethode(ENormalenWichtungsmethode _NormalenWichtungsmthode);
		ENormalenWichtungsmethode LeseNormalenwichtungsmethode();

		// Finale Vertex-Koordinaten berechnen, dabei eventuell verwackeln und glaetten
		irr::core::vector3d<irr::f32> BerechneVertexKoordinaten(unsigned int _X, unsigned int _Y, unsigned int _Z);

		// Dreiecks-Meshes aller Detailstufen (0...Original)
		std::vector<irr::scene::SMesh*> MeshVoxel;
		// Zerlegte Meshes: 1 Meshbuffer je Mesh
		std::vector<std::vector<irr::scene::SMesh*>*> MeshVoxelAufgeteilt;

		// Informationen zu jedem Meshbuffer: Sichtbarkeits-Eigenschaften
		std::vector<SVoxelGebietVisibilityEigenschaften*> VisibilityInformationen;

	private:

		// Umwandlungsroutine: Voxel in Mesh (Zerlegung per Octree)
		void VoxelInMesh();
		// Berechnung der Normalen
		void NormalenBerechnen();
		// Berechnung der Visibility-Informationen (Sichtbarkeiten)
		void VisibilityInformationenBerechnen();

		// Berechnung der Detailstufen
		void BerechneNeueDetailstufe();
		void BerechneNeueDetailstufeNormalen();

		// Berechnet die Eigenschaften des Vertex: Grenzvertex, Andockvertex -> Speichrung als Textur-Koordinaten
		irr::core::vector2d<irr::f32> BerechneVertexEigenschaften(unsigned int _X, unsigned int _Y, unsigned int _Z,
						unsigned int _GrenzeXmin, unsigned int _GrenzeYmin, unsigned int _GrenzeZmin,
						unsigned int _GrenzeXmax, unsigned int _GrenzeYmax, unsigned int _GrenzeZmax);

		// Testet ob Vertex einen Grenzvertex darstellt (-1.0 kein Grenzvertex, 1.0 Grenzvertex), fuer Speicherung als Texture.X-Koordinate
		inline double IstVertexGrenzvertex(unsigned int _X, unsigned int _Y, unsigned int _Z,
			unsigned int _GrenzeXmin, unsigned int _GrenzeYmin, unsigned int _GrenzeZmin,
			unsigned int _GrenzeXmax, unsigned int _GrenzeYmax, unsigned int _GrenzeZmax);

		// Testet ob Vertex einen Vertex an einer Andockstelle darstellt (-1.0 kein Andockvertex, 1.0 Andockvertex), fuer Speicherung als Texture.Y-Koordinate
		inline double IstVertexAndockvertex(unsigned int _X, unsigned int _Y, unsigned int _Z);

		// Zeitmessung
		CZeitmesser* Zeitmesser;
		// Zufallsgenerator
		CZufallsGenerator* ZufallsGenerator;
		// VoxelRaum
		CVoxelRaum* VoxelRaum;

		// Optionen
		bool Verwackeln;									// Vertices beim Umwandeln verwackeln
		ENormalenWichtungsmethode NormalenWichtungsmethode;	// wie werden die Face-Normalen bei der Berechnung der Vertex-Normalen gewichtet
		// Verwacklungseigenschaften
		unsigned int ZufallsSeed;
		double VerwacklungsStaerke;
		bool Glaetten;

		// maximal zulaessiger Manhatten-Abstand je Ebene beim Clamping
		static const double MaxClampDistanz;
		// maximale Anzahl von Vertices fuer einen Meshbuffer
		static const unsigned int MaxAnzahlVertices = 65000;

		// Globale Sweepebene: schnellerer Zugriff als bei dynamisch erstelltem mehrdimensionalen Array
		// benoetigt fuer das Zaehlen und Merken von Vertices
		unsigned int GlobaleSweepEbene[2][CVoxelRaum::VoxelraumY+1][CVoxelRaum::VoxelraumZ+1];	// hier deklariert, da zu gross fuer den Stack

		// Hashtabelle fuer Normalenberechnung: Hashwert = (X,Z) in Ganzzahlen gerundet
		// (XZ, da i.d.R. weniger Objekte pro Eintrag als bei XY oder YZ -> Annahme: eher ebene Objekte, Y ist Hoehenachse)
		// wichtig: VerwacklungsStaerke darf hoechstens 0.5 gewesen sein fuer korrektes Hashing! (wird durch SetzeVerwacklungsStaerke() gewaehrleistet)
		std::list<SVertexInformationen>* HashTabelle[CVoxelRaum::VoxelraumX+1][CVoxelRaum::VoxelraumZ+1];

		// Look-Up-Tabelle fuer Vertex-Modifikatoren
		SVertexModifikatoren VertexModifikatoren[256];

		// Octree
		std::vector<SOctreeKnoten*> OcTree;

		// Adjazenzlistentabelle fuer LOD
		std::vector<unsigned int>* Adjazenzlisten[MaxAnzahlVertices];
		// fuer LOD Markierungen zum Bestimmen doppelter Nachbarn
		unsigned int VerticesFlagsOriginal[MaxAnzahlVertices];
		unsigned int VerticesFlagsReduziert[MaxAnzahlVertices];
	};

} // END NAMESPACE DunGen

#endif
