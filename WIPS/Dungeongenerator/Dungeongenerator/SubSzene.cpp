#include "SubSzene.h"

// ======================================================
// Adjazenzlisten
// ======================================================

void DunGen::SAdjazenzListe::AdjazenzHinzufuegen(unsigned int _AdjazenterVertex)
{
	// suchen ob schon vorhanden
	for (unsigned int i=0; i<AdjazenteVertices.size(); i++)
		if (_AdjazenterVertex == AdjazenteVertices.at(i).Index)
		{
			AdjazenteVertices.at(i).Mehrfach = true;	// Vertex schon vorhanden
			return;
		}

	// wenn noch nicht vorhanden -> hinzufuegen
	SAdjazenterVertex neuerAdjazenterVertex;
	neuerAdjazenterVertex.Index = _AdjazenterVertex;
	neuerAdjazenterVertex.Mehrfach = false;
	AdjazenteVertices.push_back(neuerAdjazenterVertex);
}

// ======================================================
// Initialisierung und De-Initialisierung
// ======================================================

DunGen::CSubSzene::CSubSzene(irr::IrrlichtDevice* _IrrDevice, const irr::io::path _Dateiname)
{
	std::cout << "[SubSzene]: Lade Szene " << _Dateiname.c_str() << " ..." << std::endl;

	Pfad = _Dateiname;	// Pfad/Dateiname merken

	// Dateisystem bestimmen			
	irr::io::IFileSystem* fileSystem = _IrrDevice->getFileSystem();
	// eigenen Szenenmanager erstellen
	EigenerSceneManager =  _IrrDevice->getSceneManager()->createNewSceneManager(false);	// Inhalt des Haupt-Scenemanagers nicht klonen

	// jetziges Arbeitsverzeichnis merken
	irr::io::path arbeitsVerzeichnis = fileSystem->getWorkingDirectory();

	// alte Szene loeschen
	//EigenerSceneManager->clear();

	// Arbeitsverzeichnis auf den Pfad des .irr-Files setzen, damit die dort referenzierten Dateien korrekt geladen werden
	fileSystem->changeWorkingDirectoryTo(fileSystem->getFileDir(_Dateiname));
	
	// Szene einladen, Angabe des Dateinamens ohne Pfad
	EigenerSceneManager->loadScene(fileSystem->getFileBasename(_Dateiname));
	
	// Ermitteln der Andockstellen
	AndockstelleBerechnen(0);
	AndockstelleBerechnen(1);
	AndockstelleBerechnen(2);
	AndockstelleBerechnen(3);

	// vorheriges Arbeitsverzeichnis wiederherstellen
	fileSystem->changeWorkingDirectoryTo(arbeitsVerzeichnis);
	std::cout << "[SubSzene]: Laden der Szene und Berechnen der Andockstellen komplett." << std::endl;

	// Ableitungsfaktoren einstellen
	AbleitungsFaktorRaumRaum = 0.5;
	AbleitungsFaktorRaumHoehle = 1.2;
	AbleitungsFaktorHoehleRaum = 0.75;
	// Platzierungswahrscheinlichkeits-Faktoren einstellen
	PlatzierungsWahrscheinlichkeitsFaktorAnHoehle = 1;
	PlatzierungsWahrscheinlichkeitsFaktorAnRaum = 1;
	// Skalierung einstellen
	Skalierung = irr::core::vector3d<double>(1.0,1.0,1.0);
}

DunGen::CSubSzene::~CSubSzene()
{	
	// eigenen Szenenmanager incl. Szene loeschen
	EigenerSceneManager->drop();
}


// ======================================================
// Szene einfuegen
// ======================================================

// Szene in anderen Szenegraphen kopieren
void DunGen::CSubSzene::SzeneHinzufuegen(irr::scene::ISceneNode* _Elternknoten, irr::scene::ISceneManager* _SceneManager)
{	
	// alle Kinder des Root-Nodes des eigenen Szenegraphen unter neuen Knoten des anderen Szenegraphen kopieren
	irr::core::list<irr::scene::ISceneNode*> const & kindKnoten = EigenerSceneManager->getRootSceneNode()->getChildren();
        for (irr::core::list<irr::scene::ISceneNode*>::ConstIterator kind = kindKnoten.begin(); kind != kindKnoten.end(); kind++)
				(*kind)->clone(_Elternknoten,_SceneManager);
}

// ======================================================
// Andockstellen bestimmen
// ======================================================

void DunGen::CSubSzene::AndockstelleBerechnen(unsigned int _Andockstelle)
{
	// Andockstelle finden
	irr::scene::IMeshSceneNode* andockstellenMeshNode;
	
	// Norden: positive X-Achse des .irr-Files
	switch (_Andockstelle)
	{
		case 0: andockstellenMeshNode = (irr::scene::IMeshSceneNode*) EigenerSceneManager->getSceneNodeFromName("Nord_Andockstelle");
			std::cout << "[SubSzene]: Berechnung der Nord_Andockstelle..." << std::endl;
			break;
		case 1: andockstellenMeshNode = (irr::scene::IMeshSceneNode*) EigenerSceneManager->getSceneNodeFromName("Ost_Andockstelle");
			std::cout << "[SubSzene]: Berechnung der Ost_Andockstelle..." << std::endl;
			break;
		case 2: andockstellenMeshNode = (irr::scene::IMeshSceneNode*) EigenerSceneManager->getSceneNodeFromName("Sued_Andockstelle");
			std::cout << "[SubSzene]: Berechnung der Sued_Andockstelle..." << std::endl;
			break;
		case 3: andockstellenMeshNode = (irr::scene::IMeshSceneNode*) EigenerSceneManager->getSceneNodeFromName("West_Andockstelle");
			std::cout << "[SubSzene]: Berechnung der West_Andockstelle..." << std::endl;
			break;
	}	
	if (!andockstellenMeshNode)
	{
		std::cout << "[SubSzene]: WARNUNG: Andockstellen-Mesh wurde nicht gefunden!" << std::endl;
		return;
	}

	// ADS: Mesh besteht aus einem Meshbuffer (Quelle z.B. .obj-Datei mit einer Group)	
	irr::scene::IMeshBuffer* andockstellenMeshBuffer = andockstellenMeshNode->getMesh()->getMeshBuffer(0);

	// Adjazenzlisten vorbereiten
	SAdjazenzListe** adjazenzListen = new SAdjazenzListe*[andockstellenMeshBuffer->getVertexCount()];
	for (unsigned int i = 0; i<andockstellenMeshBuffer->getVertexCount(); i++)
		adjazenzListen[i] = new SAdjazenzListe();

	// Adjazenzlisten fuellen
	for (unsigned int i=0; i<andockstellenMeshBuffer->getIndexCount(); i+=3)
	{		
		// 3 Kanten pro Dreieck, 2 Adjazenzen pro Kante
		adjazenzListen[andockstellenMeshBuffer->getIndices()[i]]->AdjazenzHinzufuegen(andockstellenMeshBuffer->getIndices()[i+1]);
		adjazenzListen[andockstellenMeshBuffer->getIndices()[i]]->AdjazenzHinzufuegen(andockstellenMeshBuffer->getIndices()[i+2]);
		adjazenzListen[andockstellenMeshBuffer->getIndices()[i+1]]->AdjazenzHinzufuegen(andockstellenMeshBuffer->getIndices()[i]);
		adjazenzListen[andockstellenMeshBuffer->getIndices()[i+1]]->AdjazenzHinzufuegen(andockstellenMeshBuffer->getIndices()[i+2]);
		adjazenzListen[andockstellenMeshBuffer->getIndices()[i+2]]->AdjazenzHinzufuegen(andockstellenMeshBuffer->getIndices()[i]);
		adjazenzListen[andockstellenMeshBuffer->getIndices()[i+2]]->AdjazenzHinzufuegen(andockstellenMeshBuffer->getIndices()[i+1]);
	}

	// ~~~~~~~~~~~~~~~~~~~
	// Randkurve bestimmen
	// ~~~~~~~~~~~~~~~~~~~

	unsigned int aktuellerVertex = 0;
	unsigned int startVertex = andockstellenMeshBuffer->getVertexCount();
	bool schleifenBedingung;
	unsigned int tempIndex;

	// Start-Randvertex wird gesucht:
	while ((startVertex >= andockstellenMeshBuffer->getVertexCount()) )// && (aktuellerVertex < andockstellenMeshBuffer->getVertexCount()))
	{
		// suche nach nicht doppelter Kante (Bedingung fuer Randvertex)
		tempIndex = 0;
		schleifenBedingung = adjazenzListen[aktuellerVertex]->AdjazenteVertices.at(tempIndex).Mehrfach;
		while (schleifenBedingung)
		{
			tempIndex++;
			// erste Bedingnung: Index-Test
			schleifenBedingung = tempIndex < adjazenzListen[aktuellerVertex]->AdjazenteVertices.size();
			// wenn diese erfuellt: eigentlicher Test (mehrfache Kante)
			if (schleifenBedingung)
				schleifenBedingung = adjazenzListen[aktuellerVertex]->AdjazenteVertices.at(tempIndex).Mehrfach;
		}
		
		// kein Randvertex
		if (tempIndex >= adjazenzListen[aktuellerVertex]->AdjazenteVertices.size())
			aktuellerVertex++;
		// Randvertex
		else
			startVertex = aktuellerVertex;
	}

	// der Randkurve folgend:
	irr::core::vector3df tempVec;
	tempVec = andockstellenMeshBuffer->getPosition(startVertex);
	// hinzufuegen des Vertex zur Andockstelle
	Andockstelle[_Andockstelle].Punkt.push_back(irr::core::vector3d<double>(tempVec.X,tempVec.Y,tempVec.Z));

	// restlichen Rand ablaufen
	aktuellerVertex = adjazenzListen[aktuellerVertex]->AdjazenteVertices.at(tempIndex).Index;
	unsigned int letzterVertex = startVertex;

	// temporaere Normale bestimmen: wichtig fuer spaetere Reihenfolge der Punkte
	// Annahme & Vorraussetzung: Polar-Winkel der Punkte in (X,Y)-Ebene streng monoton steigend bzw. fallend
	tempVec = andockstellenMeshBuffer->getPosition(aktuellerVertex);
	irr::core::vector3df tempNormale = irr::core::vector3df(andockstellenMeshBuffer->getPosition(aktuellerVertex).X,andockstellenMeshBuffer->getPosition(aktuellerVertex).Y,0)
								.crossProduct(irr::core::vector3df(andockstellenMeshBuffer->getPosition(letzterVertex).X,andockstellenMeshBuffer->getPosition(letzterVertex).Y,0));

	double maxAbstandZpositiv = 0;
	double maxAbstandZnegativ = 0;
	while (aktuellerVertex != startVertex)
	{
		// hinzufuegen des Vertex zur Andockstelle
		tempVec = andockstellenMeshBuffer->getPosition(aktuellerVertex);
		Andockstelle[_Andockstelle].Punkt.push_back(irr::core::vector3d<double>(tempVec.X,tempVec.Y,tempVec.Z));

		// maximaler Abstand eines Punktes zur Ebene
		maxAbstandZpositiv = std::max(maxAbstandZpositiv,(double)tempVec.Z);
		maxAbstandZnegativ = std::max(maxAbstandZnegativ,-(double)tempVec.Z);

		// nachsten Vertex auf dem Rand finden
		tempIndex = 0;
		while ((adjazenzListen[aktuellerVertex]->AdjazenteVertices.at(tempIndex).Mehrfach)
			|| (adjazenzListen[aktuellerVertex]->AdjazenteVertices.at(tempIndex).Index == letzterVertex)
			) 
			tempIndex++;

		// einen Vertex weitergehen
		letzterVertex = aktuellerVertex;
		aktuellerVertex = adjazenzListen[aktuellerVertex]->AdjazenteVertices.at(tempIndex).Index;
	}

	// Adjazenzlisten loeschen
	for (unsigned int i = 0; i<andockstellenMeshBuffer->getVertexCount(); i++)
		delete adjazenzListen[i];
	delete adjazenzListen;

	// ~~~~~~~~~~~~~~~~~~~~~~~
	// Weitere Daten bestimmen
	// ~~~~~~~~~~~~~~~~~~~~~~~

	// Mittelpunkt bestimmen:
	Andockstelle[_Andockstelle].Mittelpunkt = irr::core::vector3d<double>(0,0,0);
	// Normale bestimmen: Z-Achse des Andockstellen-Meshes, vom Mittelpunkt der SubSzene wegzeigend
	Andockstelle[_Andockstelle].Normale = irr::core::vector3d<double>(0,0,1);
	// Ausdehnung vorerst auf 1 setzen, wird noch erweitert wenn Normalenrichtung feststeht
	Andockstelle[_Andockstelle].Ausdehnung = 1.0;

	// Update der Transformationsmatrizen bis zum Knoten des Andockstellen-Meshs
	std::stack<irr::scene::ISceneNode*> updateTransformationStack;
	irr::scene::ISceneNode* tempSceneNode = andockstellenMeshNode;
	while (tempSceneNode != EigenerSceneManager->getRootSceneNode())
	{
		updateTransformationStack.push(tempSceneNode);
		tempSceneNode = tempSceneNode->getParent();
	}
	while (!updateTransformationStack.empty())
	{
		tempSceneNode = updateTransformationStack.top();
		tempSceneNode->updateAbsolutePosition();
		updateTransformationStack.pop();
	}
	// Bestimmung der auf dem Andockstellen-Mesh liegenden Transformation
	irr::core::matrix4 absoluteTransformation = andockstellenMeshNode->getAbsoluteTransformation();
	// Andockstelle passend transformieren
	Andockstelle[_Andockstelle].TransformiereAndockstelle(absoluteTransformation);
	
	// Normale umkehren?
	// wenn (Mittelpunkt der Andockstelle - Nullpunkt der Szene) nicht in Richtung der Normale zeigt
	// Nullpunkt der Szene ist (0,0,0)
	if (Andockstelle[_Andockstelle].Mittelpunkt.dotProduct(Andockstelle[_Andockstelle].Normale)<0)
	{
		Andockstelle[_Andockstelle].Normale = -Andockstelle[_Andockstelle].Normale;
		Andockstelle[_Andockstelle].Ausdehnung *= maxAbstandZnegativ;
	}
	else
		Andockstelle[_Andockstelle].Ausdehnung *= maxAbstandZpositiv;

	// Temporaere Normale transformieren
	absoluteTransformation.rotateVect(tempNormale);
	tempNormale.normalize();

	// Reihenfolge der Andockstellenpunkte umkehren? -> wenn beide Normalen nicht in gleiche Richtung zeigen
	if (irr::core::vector3d<double>(tempNormale.X,tempNormale.Y,tempNormale.Z).dotProduct(Andockstelle[_Andockstelle].Normale) < 0)
	{
		// umgekehrte Reihenfolge bestimmen
		std::vector<irr::core::vector3d<double>> tempPunkte;
		for (unsigned i=1; i<=Andockstelle[_Andockstelle].Punkt.size(); i++)
			tempPunkte.push_back(Andockstelle[_Andockstelle].Punkt.at(Andockstelle[_Andockstelle].Punkt.size()-i));
		// und uebernehmen
		Andockstelle[_Andockstelle].Punkt.clear();
		for (unsigned i=0; i<tempPunkte.size(); i++)
			Andockstelle[_Andockstelle].Punkt.push_back(tempPunkte.at(i));
	}

	// Andockstellen-Szeneknoten loeschen
	andockstellenMeshNode->remove();

	std::cout << "[SubSzene]: Berechnung der Andockstelle komplett. Anzahl der Punkte = " << Andockstelle[_Andockstelle].Punkt.size() << std::endl;
}

// ======================================================
// Werte setzen
// ======================================================

// Ableitungsfaktoren setzen
void DunGen::CSubSzene::SetzeAbleitungsFaktorRaumRaum(double _AbleitungsFaktorRaumRaum)
{
	AbleitungsFaktorRaumRaum = _AbleitungsFaktorRaumRaum;
}
void DunGen::CSubSzene::SetzeAbleitungsFaktorRaumHoehle(double _AbleitungsFaktorRaumHoehle)
{
	AbleitungsFaktorRaumHoehle = _AbleitungsFaktorRaumHoehle;
}
void DunGen::CSubSzene::SetzeAbleitungsFaktorHoehleRaum(double _AbleitungsFaktorHoehleRaum)
{
	AbleitungsFaktorHoehleRaum = _AbleitungsFaktorHoehleRaum;
}

// Platzierungswahrscheinlichkeitsfaktoren setzen
void DunGen::CSubSzene::SetzePlatzierungsWahrscheinlichkeitsFaktorAnHoehle(unsigned int _PlatzierungsWahrscheinlichkeitsFaktorAnHoehle)
{
	PlatzierungsWahrscheinlichkeitsFaktorAnHoehle = _PlatzierungsWahrscheinlichkeitsFaktorAnHoehle;
}
void DunGen::CSubSzene::SetzePlatzierungsWahrscheinlichkeitsFaktorAnRaum(unsigned int _PlatzierungsWahrscheinlichkeitsFaktorAnRaum)
{
	PlatzierungsWahrscheinlichkeitsFaktorAnRaum = _PlatzierungsWahrscheinlichkeitsFaktorAnRaum;
}

// Skalierung setzen
void DunGen::CSubSzene::SetzeSkalierung(irr::core::vector3d<double> _Skalierung)
{
	Skalierung = _Skalierung;
}

// Benennung setzen
void DunGen::CSubSzene::SetzeName(irr::core::stringc _Name)
{
	Name = _Name;
}

// ======================================================
// Werte auslesen
// ======================================================

// Andockstellen auslesen
DunGen::SSplineAndockStelle DunGen::CSubSzene::LeseAndockstelle(unsigned int _Andockstelle)
{
	return Andockstelle[_Andockstelle];
}

// Ableitungsfaktoren auslesen
double DunGen::CSubSzene::LeseAbleitungsFaktorRaumRaum()
{
	return AbleitungsFaktorRaumRaum;
}
double DunGen::CSubSzene::LeseAbleitungsFaktorRaumHoehle()
{
	return AbleitungsFaktorRaumHoehle;
}
double DunGen::CSubSzene::LeseAbleitungsFaktorHoehleRaum()
{
	return AbleitungsFaktorHoehleRaum;
}

// Platzierungswahrscheinlichkeitsfaktoren auslesen
unsigned int DunGen::CSubSzene::LesePlatzierungsWahrscheinlichkeitsFaktorAnHoehle()
{
	return PlatzierungsWahrscheinlichkeitsFaktorAnHoehle;
}
unsigned int DunGen::CSubSzene::LesePlatzierungsWahrscheinlichkeitsFaktorAnRaum()
{
	return PlatzierungsWahrscheinlichkeitsFaktorAnRaum;
}

// Dateipfad der Subszene auslesen
irr::io::path DunGen::CSubSzene::LesePfad()
{
	return Pfad;
}

// Skalierung auslesen
irr::core::vector3d<double> DunGen::CSubSzene::LeseSkalierung()
{
	return Skalierung;
}

// Benennung auslesen
irr::core::stringc DunGen::CSubSzene::LeseName()
{
	return Name;
}