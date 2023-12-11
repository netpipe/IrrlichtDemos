// Implementierung der Klasse DreiecksMesh
#include "DreiecksMesh.h"

// ======================================================
// Structs
// ======================================================
// Vertices zusammenzaehlen
unsigned int DunGen::SOctreeKnoten::ZaehleVertices()
{
	// Vertices der Kinder aufaddieren
	if (Kind[0]!=NULL)
	{
		VertexAnzahl = 0;
		for (unsigned int i=0; i<8; i++)
			VertexAnzahl += Kind[i]->ZaehleVertices();
	}
	// errechnete VertexAnzahl zuruckgeben
	return VertexAnzahl;
}

// Vergleichsoperator, zwecks Sortierung in Liste fuer Hashtabelleneintraege
bool DunGen::SVertexInformationen::operator<(const SVertexInformationen _AndereVI)
{
	// sortiert wird nach Y-Koordinate
	return (Y<_AndereVI.Y);
}

// ======================================================
// Einstellungen
// ======================================================
// setzen
void DunGen::CDreiecksMesh::SetzeZufallsSeedVerwackeln(unsigned int _ZufallsSeed)
{
	ZufallsSeed = _ZufallsSeed;
}
void DunGen::CDreiecksMesh::SetzeVerwacklungsStaerke(double _VerwacklungsStaerke)
{
	VerwacklungsStaerke = _VerwacklungsStaerke;
	// begrenzen
	if (VerwacklungsStaerke > MaxClampDistanz)
		VerwacklungsStaerke = MaxClampDistanz;
	if (VerwacklungsStaerke < 0.0)
		VerwacklungsStaerke = 0.0;
}

void DunGen::CDreiecksMesh::SetzeOptionVerwackeln(bool _Verwackeln)
{
	Verwackeln = _Verwackeln;
}

void DunGen::CDreiecksMesh::SetzeOptionGlaetten(bool _Glaetten)
{
	Glaetten = _Glaetten;
}

// lesen
unsigned int DunGen::CDreiecksMesh::LeseZufallsSeedVerwackeln()
{
	return ZufallsSeed;
}
double DunGen::CDreiecksMesh::LeseVerwacklungsStaerke()
{
	return VerwacklungsStaerke;
}

bool DunGen::CDreiecksMesh::LeseOptionVerwackeln()
{
	return Verwackeln;
}

bool DunGen::CDreiecksMesh::LeseOptionGlaetten()
{
	return Glaetten;
}

void DunGen::CDreiecksMesh::SetzeNormalenwichtungsmethode(ENormalenWichtungsmethode _NormalenWichtungsmthode)
{
	NormalenWichtungsmethode = _NormalenWichtungsmthode;
}

DunGen::ENormalenWichtungsmethode DunGen::CDreiecksMesh::LeseNormalenwichtungsmethode()
{
	return NormalenWichtungsmethode;
}

// ======================================================
// Initialisierung
// ======================================================
DunGen::CDreiecksMesh::CDreiecksMesh(CVoxelRaum* _VoxelRaum, CZufallsGenerator* _ZufallsGenerator, CZeitmesser* _Zeitmesser)
{
	// Werte uebernehmen
	VoxelRaum = _VoxelRaum;
	ZufallsGenerator = _ZufallsGenerator;
	Zeitmesser = _Zeitmesser;

	// Werte initialisieren
	VerwacklungsStaerke = 0.35;
	ZufallsSeed = 0;
	Verwackeln = true;
	Glaetten = true;
	NormalenWichtungsmethode = ENormalenWichtungsmethode::_NORMALENWICHTUNG_FLAECHENINHALT_;

	// leeren Initialmeshe erstellen
	irr::scene::SMesh* mesh = new irr::scene::SMesh();
	mesh->recalculateBoundingBox();
	MeshVoxel.push_back(mesh);

	// Adjazenslisten vorbereiten: Objekte anlegen
	for (unsigned int i=0; i<MaxAnzahlVertices; i++)
		Adjazenzlisten[i] = new std::vector<unsigned int>();

	// Hashtabelle initialisieren
	for (unsigned int i=0; i<CVoxelRaum::VoxelraumX+1; i++)
		for (unsigned int j=0; j<CVoxelRaum::VoxelraumY+1; j++)
			HashTabelle[i][j] = NULL;

	// Look-Up-Tabelle initialisieren:
	// Adresse: Voxel auf 1 bzw. 0
	// Reihenfolge der Voxel: X aufsteigend, Y aufsteigend, Z aufsteigend
	// d.h. (X,Y,Z) = (-1,-1,-1),(0,-1,-1),(-1,0,-1),(0,0,-1), (-1,-1,0),(0,-1,0),(-1,0,0),(0,0,0)

	std::cout << "[Dreiecksmesh]: Baue Look-Up-Tabelle fuer Mesh-Glaettungs-Parameter auf ..." << std::endl;

	// Speicher pre-allokieren
	unsigned char voxel1, voxel2, voxel3, voxel4, voxel5, voxel6, voxel7, voxel8;
	int xNegativ, xPositiv, yNegativ, yPositiv, zNegativ, zPositiv;
	int tempInt;

	// alle Indices bearbeiten
	for (unsigned short i=0; i<256; i++)
	{
		// Voxel setzen: nach Bits schauen
		voxel1 = ((i & 1) > 0) ? 1 : 0;
		voxel2 = ((i & 2) > 0) ? 1 : 0;
		voxel3 = ((i & 4) > 0) ? 1 : 0;
		voxel4 = ((i & 8) > 0) ? 1 : 0;
		voxel5 = ((i & 16) > 0) ? 1 : 0;
		voxel6 = ((i & 32) > 0) ? 1 : 0;
		voxel7 = ((i & 64) > 0) ? 1 : 0;
		voxel8 = ((i & 128) > 0) ? 1 : 0;

		// Wichtungen fuer Richtungen initialisieren
		xNegativ = 0, xPositiv = 0;
		yNegativ = 0, yPositiv = 0;
		zNegativ = 0, zPositiv = 0;

		// X-Richtung negativ:
		tempInt = voxel1 + voxel3 + voxel5 +voxel7;
		if ((tempInt < 4) && (tempInt > 1)) // zeigt eine Kante in diese Richtung?
		{
			xNegativ = 1;
			// zeigt eine 2 Felder grosse Flaeche in diese Richtung?
			if ((tempInt == 2) && (voxel1 != voxel7))
					xNegativ = 2;
		}
		// X-Richtung positiv:
		tempInt = voxel2 + voxel4 + voxel6 +voxel8;
		if ((tempInt < 4) && (tempInt > 1)) // zeigt eine Kante in diese Richtung?
		{
			xPositiv = 1;
			// zeigt eine 2 Felder grosse Flaeche in diese Richtung?
			if ((tempInt == 2) && (voxel2 != voxel8))
					xPositiv = 2;
		}

		// Y-Richtung negativ:
		tempInt = voxel1 + voxel2 + voxel5 +voxel6;
		if ((tempInt < 4) && (tempInt > 1)) // zeigt eine Kante in diese Richtung?
		{
			yNegativ = 1;
			// zeigt eine 2 Felder grosse Flaeche in diese Richtung?
			if ((tempInt == 2) && (voxel1 != voxel6))
					yNegativ = 2;
		}
		// Y-Richtung positiv:
		tempInt = voxel3 + voxel4 + voxel7 +voxel8;
		if ((tempInt < 4) && (tempInt > 1)) // zeigt eine Kante in diese Richtung?
		{
			yPositiv = 1;
			// zeigt eine 2 Felder grosse Flaeche in diese Richtung?
			if ((tempInt == 2) && (voxel3 != voxel8))
					yPositiv = 2;
		}

		// Z-Richtung negativ:
		tempInt = voxel1 + voxel2 + voxel3 +voxel4;
		if ((tempInt < 4) && (tempInt > 1)) // zeigt eine Kante in diese Richtung?
		{
			zNegativ = 1;
			// zeigt eine 2 Felder grosse Flaeche in diese Richtung?
			if ((tempInt == 2) && (voxel1 != voxel4))
					zNegativ = 2;
		}
		// Z-Richtung positiv:
		tempInt = voxel5 + voxel6 + voxel7 + voxel8;
		if ((tempInt < 4) && (tempInt > 1)) // zeigt eine Kante in diese Richtung?
		{
			zPositiv = 1;
			// zeigt eine 2 Felder grosse Flaeche in diese Richtung?
			if ((tempInt == 2) && (voxel5 != voxel8))
					zPositiv = 2;
		}

		// setzen der Modifikatoren: erlaubte Verwacklungsrichtungen
		if (xPositiv == xNegativ)
			VertexModifikatoren[i].RichtungX = 0;	// beide Richtungen erlaubt
		else if (xPositiv > xNegativ)
			VertexModifikatoren[i].RichtungX = 1;	// nur positive Richtung erlaubt
		else
			VertexModifikatoren[i].RichtungX = -1;	// nur negative Richtung erlaubt

		if (yPositiv == yNegativ)
			VertexModifikatoren[i].RichtungY = 0;	// beide Richtungen erlaubt
		else if (yPositiv > yNegativ)
			VertexModifikatoren[i].RichtungY = 1;	// nur positive Richtung erlaubt
		else
			VertexModifikatoren[i].RichtungY = -1;	// nur negative Richtung erlaubt

		if (zPositiv == zNegativ)
			VertexModifikatoren[i].RichtungZ = 0;	// beide Richtungen erlaubt
		else if (zPositiv > zNegativ)
			VertexModifikatoren[i].RichtungZ = 1;	// nur positive Richtung erlaubt
		else
			VertexModifikatoren[i].RichtungZ = -1;	// nur negative Richtung erlaubt

	} // ENDE: Look-Up-Tabelle initialisieren

	std::cout << "[Dreiecksmesh]: Look-Up-Tabelle aufgebaut." << std::endl;

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Octree berechnen:
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Octree-Aufteilung bestimmen
	// -> durch konstante Voxelraumgroesse und konstante Vertex-Maximalzahl je Knoten ist Vorberechnung moeglich

	std::cout << "[Dreiecksmesh]: Vorberechnung der Octree-Aufteilung ..." << std::endl;

	// Root-Node erstellen
	SOctreeKnoten* aktuellerOctreeKnoten = new SOctreeKnoten();
	aktuellerOctreeKnoten->GrenzeMinX = CVoxelRaum::VoxelMinRandOffset;
	aktuellerOctreeKnoten->GrenzeMinY = CVoxelRaum::VoxelMinRandOffset;
	aktuellerOctreeKnoten->GrenzeMinZ = CVoxelRaum::VoxelMinRandOffset;
	aktuellerOctreeKnoten->GrenzeMaxX = CVoxelRaum::VoxelraumX - CVoxelRaum::VoxelMinRandOffset - 1;
	aktuellerOctreeKnoten->GrenzeMaxY = CVoxelRaum::VoxelraumY - CVoxelRaum::VoxelMinRandOffset - 1;
	aktuellerOctreeKnoten->GrenzeMaxZ = CVoxelRaum::VoxelraumZ - CVoxelRaum::VoxelMinRandOffset - 1;
	for (unsigned int i=0; i<8; i++)
		aktuellerOctreeKnoten->Kind[i] = NULL;
	aktuellerOctreeKnoten->Elterknoten = NULL;

	OcTree.push_back(aktuellerOctreeKnoten);

	unsigned int GrenzeX, GrenzeY, GrenzeZ;
	// Octree bis zur noetigen Tiefe aufbauen (MaxAnzahlVertices nicht ueberschreiten)
	// max. benoetigte Vertices = (DimX+1) * (DimY+1) * (DimZ+1)
	unsigned int aktuelleID = 0;
	while (aktuelleID<OcTree.size())
	{
		// naechsten Knoten zum Bearbeiten auswaehlen
		aktuellerOctreeKnoten = OcTree.at(aktuelleID);

		// wenn Groesse zu hoch: aufspalten
		if (
		((aktuellerOctreeKnoten->GrenzeMaxX-aktuellerOctreeKnoten->GrenzeMinX+1)*
		(aktuellerOctreeKnoten->GrenzeMaxX-aktuellerOctreeKnoten->GrenzeMinX+1)*
		(aktuellerOctreeKnoten->GrenzeMaxX-aktuellerOctreeKnoten->GrenzeMinX+1)) > MaxAnzahlVertices)
		{
			// Grenzen fuer Aufspaltung berechen
			GrenzeX = aktuellerOctreeKnoten->GrenzeMinX+(aktuellerOctreeKnoten->GrenzeMaxX-aktuellerOctreeKnoten->GrenzeMinX)/2;
			GrenzeY = aktuellerOctreeKnoten->GrenzeMinY+(aktuellerOctreeKnoten->GrenzeMaxY-aktuellerOctreeKnoten->GrenzeMinY)/2;
			GrenzeZ = aktuellerOctreeKnoten->GrenzeMinZ+(aktuellerOctreeKnoten->GrenzeMaxZ-aktuellerOctreeKnoten->GrenzeMinZ)/2;

			// Kindknoten erstellen
			aktuellerOctreeKnoten->Kind[0] = new SOctreeKnoten();
			aktuellerOctreeKnoten->Kind[0]->GrenzeMinX = aktuellerOctreeKnoten->GrenzeMinX;
			aktuellerOctreeKnoten->Kind[0]->GrenzeMaxX = GrenzeX;
			aktuellerOctreeKnoten->Kind[0]->GrenzeMinY = aktuellerOctreeKnoten->GrenzeMinY;
			aktuellerOctreeKnoten->Kind[0]->GrenzeMaxY = GrenzeY;
			aktuellerOctreeKnoten->Kind[0]->GrenzeMinZ = aktuellerOctreeKnoten->GrenzeMinZ;
			aktuellerOctreeKnoten->Kind[0]->GrenzeMaxZ = GrenzeZ;

			aktuellerOctreeKnoten->Kind[1] = new SOctreeKnoten();
			aktuellerOctreeKnoten->Kind[1]->GrenzeMinX = GrenzeX+1;
			aktuellerOctreeKnoten->Kind[1]->GrenzeMaxX = aktuellerOctreeKnoten->GrenzeMaxX;
			aktuellerOctreeKnoten->Kind[1]->GrenzeMinY = aktuellerOctreeKnoten->GrenzeMinY;
			aktuellerOctreeKnoten->Kind[1]->GrenzeMaxY = GrenzeY;
			aktuellerOctreeKnoten->Kind[1]->GrenzeMinZ = aktuellerOctreeKnoten->GrenzeMinZ;
			aktuellerOctreeKnoten->Kind[1]->GrenzeMaxZ = GrenzeZ;

			aktuellerOctreeKnoten->Kind[2] = new SOctreeKnoten();
			aktuellerOctreeKnoten->Kind[2]->GrenzeMinX = aktuellerOctreeKnoten->GrenzeMinX;
			aktuellerOctreeKnoten->Kind[2]->GrenzeMaxX = GrenzeX;
			aktuellerOctreeKnoten->Kind[2]->GrenzeMinY = GrenzeY+1;
			aktuellerOctreeKnoten->Kind[2]->GrenzeMaxY = aktuellerOctreeKnoten->GrenzeMaxY;
			aktuellerOctreeKnoten->Kind[2]->GrenzeMinZ = aktuellerOctreeKnoten->GrenzeMinZ;
			aktuellerOctreeKnoten->Kind[2]->GrenzeMaxZ = GrenzeZ;

			aktuellerOctreeKnoten->Kind[3] = new SOctreeKnoten();
			aktuellerOctreeKnoten->Kind[3]->GrenzeMinX = GrenzeX+1;
			aktuellerOctreeKnoten->Kind[3]->GrenzeMaxX = aktuellerOctreeKnoten->GrenzeMaxX;
			aktuellerOctreeKnoten->Kind[3]->GrenzeMinY = GrenzeY+1;
			aktuellerOctreeKnoten->Kind[3]->GrenzeMaxY = aktuellerOctreeKnoten->GrenzeMaxY;
			aktuellerOctreeKnoten->Kind[3]->GrenzeMinZ = aktuellerOctreeKnoten->GrenzeMinZ;
			aktuellerOctreeKnoten->Kind[3]->GrenzeMaxZ = GrenzeZ;

			aktuellerOctreeKnoten->Kind[4] = new SOctreeKnoten();
			aktuellerOctreeKnoten->Kind[4]->GrenzeMinX = aktuellerOctreeKnoten->GrenzeMinX;
			aktuellerOctreeKnoten->Kind[4]->GrenzeMaxX = GrenzeX;
			aktuellerOctreeKnoten->Kind[4]->GrenzeMinY = aktuellerOctreeKnoten->GrenzeMinY;
			aktuellerOctreeKnoten->Kind[4]->GrenzeMaxY = GrenzeY;
			aktuellerOctreeKnoten->Kind[4]->GrenzeMinZ = GrenzeZ+1;
			aktuellerOctreeKnoten->Kind[4]->GrenzeMaxZ = aktuellerOctreeKnoten->GrenzeMaxZ;

			aktuellerOctreeKnoten->Kind[5] = new SOctreeKnoten();
			aktuellerOctreeKnoten->Kind[5]->GrenzeMinX = GrenzeX+1;
			aktuellerOctreeKnoten->Kind[5]->GrenzeMaxX = aktuellerOctreeKnoten->GrenzeMaxX;
			aktuellerOctreeKnoten->Kind[5]->GrenzeMinY = aktuellerOctreeKnoten->GrenzeMinY;
			aktuellerOctreeKnoten->Kind[5]->GrenzeMaxY = GrenzeY;
			aktuellerOctreeKnoten->Kind[5]->GrenzeMinZ = GrenzeZ+1;
			aktuellerOctreeKnoten->Kind[5]->GrenzeMaxZ = aktuellerOctreeKnoten->GrenzeMaxZ;

			aktuellerOctreeKnoten->Kind[6] = new SOctreeKnoten();
			aktuellerOctreeKnoten->Kind[6]->GrenzeMinX = aktuellerOctreeKnoten->GrenzeMinX;
			aktuellerOctreeKnoten->Kind[6]->GrenzeMaxX = GrenzeX;
			aktuellerOctreeKnoten->Kind[6]->GrenzeMinY = GrenzeY+1;
			aktuellerOctreeKnoten->Kind[6]->GrenzeMaxY = aktuellerOctreeKnoten->GrenzeMaxY;
			aktuellerOctreeKnoten->Kind[6]->GrenzeMinZ = GrenzeZ+1;
			aktuellerOctreeKnoten->Kind[6]->GrenzeMaxZ = aktuellerOctreeKnoten->GrenzeMaxZ;

			aktuellerOctreeKnoten->Kind[7] = new SOctreeKnoten();
			aktuellerOctreeKnoten->Kind[7]->GrenzeMinX = GrenzeX+1;
			aktuellerOctreeKnoten->Kind[7]->GrenzeMaxX = aktuellerOctreeKnoten->GrenzeMaxX;
			aktuellerOctreeKnoten->Kind[7]->GrenzeMinY = GrenzeY+1;
			aktuellerOctreeKnoten->Kind[7]->GrenzeMaxY = aktuellerOctreeKnoten->GrenzeMaxY;
			aktuellerOctreeKnoten->Kind[7]->GrenzeMinZ = GrenzeZ+1;
			aktuellerOctreeKnoten->Kind[7]->GrenzeMaxZ = aktuellerOctreeKnoten->GrenzeMaxZ;

			// fuer alle Kindknoten:
			for (unsigned int i=0; i<8; i++)
			{
				// Elternknoten merken
				aktuellerOctreeKnoten->Kind[i]->Elterknoten = aktuellerOctreeKnoten;

				// Kindknoten als Blaetter
				for (unsigned int j=0; j<8; j++)
					aktuellerOctreeKnoten->Kind[i]->Kind[j] = NULL;

				// Kindknoten hinzufuegen
				OcTree.push_back(aktuellerOctreeKnoten->Kind[i]);
			}
		}
		// naechster Knoten zur Bearbeitung
		aktuelleID++;
	}
	std::cout << "[Dreiecksmesh]: Octree-Aufteilung komplett. #Octree-Knoten = " << OcTree.size() << std::endl;


} // ENDE: Konstruktor


// Konstanten
const double DunGen::CDreiecksMesh::MaxClampDistanz = 0.49;

// ======================================================
// Aufruf der Umwandlung
// ======================================================

void DunGen::CDreiecksMesh::WandleVoxelInMesh(unsigned int _DetailStufe)
{
	std::cout << "[Dreiecksmesh]: Voxel->Mesh gestartet, Normalenwichtung: ";
	switch (NormalenWichtungsmethode)
	{
		case ENormalenWichtungsmethode::_NORMALENWICHTUNG_FLAECHENINHALT_: std::cout << "per Flaecheninhalt"; break;
		case ENormalenWichtungsmethode::_NORMALENWICHTUNG_WINKEL_: std::cout << "per Winkel"; break;
		case ENormalenWichtungsmethode::_NORMALENWICHTUNG_UNIFORM: std::cout << "uniforme Wichtung"; break;
	}
	std::cout << std::endl;

	// alte Meshes droppen -> wird geloescht wenn nirgendswo mehr referenziert
	for (unsigned i=0; i<MeshVoxel.size(); i++)
		MeshVoxel.at(i)->drop();
	MeshVoxel.clear();
	VisibilityInformationen.clear();

	// Loeschen der alten Zerlegung
	for (unsigned i=0; i<MeshVoxelAufgeteilt.size(); i++)
	{
		for (unsigned j=0; j<MeshVoxelAufgeteilt.at(i)->size(); j++)
			MeshVoxelAufgeteilt.at(i)->at(j)->drop();
		delete MeshVoxelAufgeteilt.at(i);
	}
	MeshVoxelAufgeteilt.clear();

	// Eigentliche Umwandlungsroutine
	VoxelInMesh();

	// alle weiteren Detailstufen berechnen
	for (unsigned i=0; i<_DetailStufe; i++)
		BerechneNeueDetailstufe();			// Berechnung der neuen Detailstufe

	// Erstellung der Zerlegung (1 Meshbuffer je Mesh)
	std::vector<irr::scene::SMesh*>* neueZerlegung;
	irr::scene::SMesh* neuerMesh;

	for (unsigned i=0; i<MeshVoxel.size(); i++)
	{
		neueZerlegung = new std::vector<irr::scene::SMesh*>();
		for (unsigned j=0; j<MeshVoxel.at(i)->getMeshBufferCount(); j++)
		{
			// neuen Mesh fuer aktuellen Buffer
			neuerMesh = new irr::scene::SMesh();
			neuerMesh->addMeshBuffer(MeshVoxel.at(i)->getMeshBuffer(j));
			neuerMesh->recalculateBoundingBox();
			// Mesh speichern
			neueZerlegung->push_back(neuerMesh);
		}
		// komplette Zerlegung der aktuellen Detailstufe speichern
		MeshVoxelAufgeteilt.push_back(neueZerlegung);
	}

	// Sichtbarkeits-Informationen berechnen
	VisibilityInformationenBerechnen();

	std::cout << "[Dreiecksmesh]: Voxel->Mesh komplett." << std::endl;
}

// ======================================================
// Visibility-Informationen berechnen
// ======================================================

void DunGen::CDreiecksMesh::VisibilityInformationenBerechnen()
{
	std::cout << "[Dreiecksmesh]: Berechne Sichtbarkeitsinformationen ...";

	SVoxelGebietVisibilityEigenschaften* aktVisInfo;

	for(unsigned int v=0; v<VisibilityInformationen.size(); v++)
	{
		aktVisInfo = VisibilityInformationen.at(v);

		// X-positiv
		aktVisInfo->Xpos = true;
		// gibt es an mindestens einer Stelle der Grenze einen Freiraumvoxel (1er-Voxel) auf beiden Seiten der Grenze?
		// -> dann existiert Sichtbarkeits-Verbindung zu angrenzendem Gebiet
		for (unsigned int i=aktVisInfo->GrenzeMinY; i<aktVisInfo->GrenzeMaxY; i++)
			for (unsigned int j=aktVisInfo->GrenzeMinZ; j<aktVisInfo->GrenzeMaxZ; j++)
				if ((VoxelRaum->Voxel[aktVisInfo->GrenzeMaxX][i][j]==1)&&(VoxelRaum->Voxel[aktVisInfo->GrenzeMaxX+1][i][j]==1))
					aktVisInfo->Xpos = false;

		// X-negativ
		aktVisInfo->Xneg = true;
		// gibt es an mindestens einer Stelle der Grenze einen Freiraumvoxel (1er-Voxel) auf beiden Seiten der Grenze?
		// -> dann existiert Sichtbarkeits-Verbindung zu angrenzendem Gebiet
		for (unsigned int i=aktVisInfo->GrenzeMinY; i<aktVisInfo->GrenzeMaxY; i++)
			for (unsigned int j=aktVisInfo->GrenzeMinZ; j<aktVisInfo->GrenzeMaxZ; j++)
				if ((VoxelRaum->Voxel[aktVisInfo->GrenzeMinX][i][j]==1)&&(VoxelRaum->Voxel[aktVisInfo->GrenzeMinX-1][i][j]==1))
					aktVisInfo->Xneg = false;

		// Y-positiv
		aktVisInfo->Ypos = true;
		// gibt es an mindestens einer Stelle der Grenze einen Freiraumvoxel (1er-Voxel) auf beiden Seiten der Grenze?
		// -> dann existiert Sichtbarkeits-Verbindung zu angrenzendem Gebiet
		for (unsigned int i=aktVisInfo->GrenzeMinX; i<aktVisInfo->GrenzeMaxX; i++)
			for (unsigned int j=aktVisInfo->GrenzeMinZ; j<aktVisInfo->GrenzeMaxZ; j++)
				if ((VoxelRaum->Voxel[i][aktVisInfo->GrenzeMaxY][j]==1)&&(VoxelRaum->Voxel[i][aktVisInfo->GrenzeMaxY+1][j]==1))
					aktVisInfo->Ypos = false;

		// Y-negativ
		aktVisInfo->Yneg = true;
		// gibt es an mindestens einer Stelle der Grenze einen Freiraumvoxel (1er-Voxel) auf beiden Seiten der Grenze?
		// -> dann existiert Sichtbarkeits-Verbindung zu angrenzendem Gebiet
		for (unsigned int i=aktVisInfo->GrenzeMinX; i<aktVisInfo->GrenzeMaxX; i++)
			for (unsigned int j=aktVisInfo->GrenzeMinZ; j<aktVisInfo->GrenzeMaxZ; j++)
				if ((VoxelRaum->Voxel[i][aktVisInfo->GrenzeMinY][j]==1)&&(VoxelRaum->Voxel[i][aktVisInfo->GrenzeMinY-1][j]==1))
					aktVisInfo->Yneg = false;

		// Z-positiv
		aktVisInfo->Zpos = true;
		// gibt es an mindestens einer Stelle der Grenze einen Freiraumvoxel (1er-Voxel) auf beiden Seiten der Grenze?
		// -> dann existiert Sichtbarkeits-Verbindung zu angrenzendem Gebiet
		for (unsigned int i=aktVisInfo->GrenzeMinX; i<aktVisInfo->GrenzeMaxX; i++)
			for (unsigned int j=aktVisInfo->GrenzeMinY; j<aktVisInfo->GrenzeMaxY; j++)
				if ((VoxelRaum->Voxel[i][j][aktVisInfo->GrenzeMaxZ]==1)&&(VoxelRaum->Voxel[i][j][aktVisInfo->GrenzeMaxZ+1]==1))
					aktVisInfo->Zpos = false;

		// Z-positiv
		aktVisInfo->Zneg = true;
		// gibt es an mindestens einer Stelle der Grenze einen Freiraumvoxel (1er-Voxel) auf beiden Seiten der Grenze?
		// -> dann existiert Sichtbarkeits-Verbindung zu angrenzendem Gebiet
		for (unsigned int i=aktVisInfo->GrenzeMinX; i<aktVisInfo->GrenzeMaxX; i++)
			for (unsigned int j=aktVisInfo->GrenzeMinY; j<aktVisInfo->GrenzeMaxY; j++)
				if ((VoxelRaum->Voxel[i][j][aktVisInfo->GrenzeMinZ]==1)&&(VoxelRaum->Voxel[i][j][aktVisInfo->GrenzeMinZ-1]==1))
					aktVisInfo->Zneg = false;
	}

	std::cout << " abgeschlossen." << std::endl;
}
