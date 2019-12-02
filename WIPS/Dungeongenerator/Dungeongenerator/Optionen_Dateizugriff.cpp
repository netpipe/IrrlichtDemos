// Implementierung der Klasse Optionen
#include "Optionen.h"

void DunGen::COptionen::Arbeitsverzeichnis_wiederherstellen()
{
	// Arbeitsverzeichnis wiederherstellen
	FileSystem->changeWorkingDirectoryTo(ArbeitsVerzeichnis);
}

// ======================================================
// Dateizugriff: Verschiedene
// ======================================================
void DunGen::COptionen::Dateizugriff_Gangtextur_Auswahl(const irr::io::path& _filename)
{
	// Dateinamen uebernehmen
	Szene->SetzeGangTexturPfad(_filename);
}
void DunGen::COptionen::Dateizugriff_Detailobjekt_Auswahl(const irr::io::path& _filename)
{
	// neues letztes Element auswaehlen
	ListBox_Detailobjekte_ID = Architekt->DetailobjektVorlagen.size();

	// neues Detailobjekt hinzufuegen
	SSplineDetailobjektVorlage* neuesDetailObjekt = new SSplineDetailobjektVorlage();
	neuesDetailObjekt->MeshPfad = _filename;
	Architekt->DetailobjektVorlagen.push_back(neuesDetailObjekt);
	
	// Werte einstellen
	Button_Uebernehmen_Detailobjekt();
}

void DunGen::COptionen::Dateizugriff_Subszene_Auswahl(const irr::io::path& _filename)
{
	// neues letztes Element auswaehlen
	ListBox_Subszenen_ID = Architekt->SubSzenen.size();

	// neue SubSzene hinzufuegen
	CSubSzene* neueSubSzene= new CSubSzene(IrrDevice, _filename);
	Architekt->SubSzenen.push_back(neueSubSzene);
	
	// Werte einstellen
	Button_Uebernehmen_Subszene();
}

// ======================================================
// Dateizugriff: Ergebnisse
// ======================================================

void DunGen::COptionen::Speichere_Ergebnisse_Fraktalstrings(const irr::io::path& _filename)
{
	// Datei speichern
	std::cout << "[Optionen]: Speichere L-System mit Parametern und Ableitungen in Datei: " << _filename.c_str() << std::endl;
	Zeitmesser->Start(0);
	irr::io::IWriteFile* datei = FileSystem->createAndWriteFile(_filename);
	
	irr::core::stringw tempStrW;

	// Wenn Dateizugriff erfolgreich:
	if (datei)
	{
		std::cout << "Beginne mit Schreiben ... ";
		// XML-Writer erstellen
		irr::io::IXMLWriter* xmlWriter = FileSystem->createXMLWriter(datei);
		xmlWriter->writeXMLHeader();
		xmlWriter->writeLineBreak();

		xmlWriter->writeElement(L"LSystemAbleitungen");
		xmlWriter->writeLineBreak();
	
		xmlWriter->writeElement(L"LSystemParameter");
		xmlWriter->writeLineBreak();

		xmlWriter->writeElement(L"Winkel",true,
			L"WinkelGier",irr::core::stringw(Fraktal->WinkelGier).c_str(),
			L"WinkelNick",irr::core::stringw(Fraktal->WinkelNick).c_str(),
			L"WinkelRoll",irr::core::stringw(Fraktal->WinkelRoll).c_str());
		xmlWriter->writeLineBreak();

		xmlWriter->writeElement(L"Radius",true,
			L"StartRadius",irr::core::stringw(Fraktal->StartRadius).c_str(),
			L"RadiusFaktor",irr::core::stringw(Fraktal->RadiusFaktor).c_str(),
			L"RadiusDekrementor",irr::core::stringw(Fraktal->RadiusDekrementor).c_str());
		xmlWriter->writeLineBreak();

		xmlWriter->writeClosingTag(L"LSystemParameter");
		xmlWriter->writeLineBreak();

		for (unsigned int i = 0; i<Fraktal->FraktalIteration.size() ; i++)
		{
			std::cout << "Iteration " << i << ", ";
			// bei langen Strings sehr langsame Variante ueber Irrlicht XML-Writer: (da Strings auf Sonderzeichen geprueft werden, und jedes Zeichen mit .append neu angefuegt wird)
			//xmlWriter->writeElement(L"Iteration",true, L"Nummer",irr::core::stringw(i).c_str(), L"String",irr::core::stringw(Fraktal->FraktalIteration.at(i).c_str()).c_str());

			// schnellere, direkte Variante:
			datei->write(L"\t", sizeof(wchar_t)); // Tabulator
			datei->write(L"<Iteration Nummer=\"", 19*sizeof(wchar_t)); // Anfang
			tempStrW = irr::core::stringw(i);
			datei->write(tempStrW.c_str(), tempStrW.size()*sizeof(wchar_t)); // Nummer
			
			datei->write(L"\" String=\"", 10*sizeof(wchar_t));
			tempStrW = irr::core::stringw(Fraktal->FraktalIteration.at(i).c_str());
			datei->write(tempStrW.c_str(), tempStrW.size()*sizeof(wchar_t)); // String

			datei->write(L"\" />", 4*sizeof(wchar_t)); // Abschluss
			xmlWriter->writeLineBreak();
		}
		
		xmlWriter->writeClosingTag(L"LSystemAbleitungen");
		xmlWriter->writeLineBreak();
		// Datei schliessen
		xmlWriter->drop();
		datei->drop();
		std::cout << "Schreiben abgeschlossen." << std::endl;
	}
	else
		std::cout << "Fehler beim Zugriff auf die Datei. Daten wurden nicht gespeichert." << std::endl;
	std::cout << "[Optionen]: Benoetigte Zeit zum Speichern: ";
	Zeitmesser->Stop(0);
}

void DunGen::COptionen::Speichere_Ergebnisse_Dichtekarte(EEbene _Ebene,const irr::io::path& _filename)
{
	// Datei speichern
	std::cout << "[Optionen]: Speichere VoxelRaum-Dichte-Karte in Datei " << _filename.c_str() << std::endl;
	Zeitmesser->Start(0);
	irr::io::IWriteFile* datei = FileSystem->createAndWriteFile(_filename);

	// Wenn Dateizugriff erfolgreich:
	if (datei)
	{
		std::cout << "Beginne mit Schreiben ... ";
		VideoDriver->writeImageToFile (Szene->LeseDichteBild(_Ebene),datei,0);
		// Datei schliessen
		datei->drop();
		std::cout << "Schreiben abgeschlossen." << std::endl;
	}
	else
		std::cout << "Fehler beim Zugriff auf die Datei. VoxelRaum-Dichte-Karte wurde nicht gespeichert." << std::endl;
	std::cout << "[Optionen]: Benoetigte Zeit zum Speichern: ";
	Zeitmesser->Stop(0);
}

void DunGen::COptionen::Speichere_Ergebnisse_Dreicksmesh_Hoehle(const irr::io::path& _filename)
{
	// Datei speichern
	std::cout << "[Optionen]: Speichere Hoehlen-Dreiecksmesh in Datei " << _filename.c_str() << std::endl;
	Zeitmesser->Start(0);
	irr::io::IWriteFile* datei = FileSystem->createAndWriteFile(_filename);
	
	// Wenn Dateizugriff erfolgreich:
	if (datei)
	{
		std::cout << "Beginne mit Schreiben ... ";
		irr::scene::IMeshWriter* meshWriter = new irr::scene::COBJMeshWriter_Modified(SceneManager,FileSystem);
		meshWriter->writeMesh(datei,DreiecksMesh->MeshVoxel.at(0),0);
		// Datei schliessen
		meshWriter->drop();
		datei->drop();
		std::cout << "Schreiben abgeschlossen." << std::endl;
	}
	else
		std::cout << "Fehler beim Zugriff auf die Datei. Hoehlen-Dreiecksmesh wurde nicht gespeichert." << std::endl;
	std::cout << "[Optionen]: Benoetigte Zeit zum Speichern: ";
	Zeitmesser->Stop(0);
}

// ======================================================
// Dateizugriff: Ergebnisse - Dungeon
// ======================================================

void DunGen::COptionen::Speichere_Ergebnisse_Dungeon(const irr::io::path& _filename)
{
	// in .irr-Szenen-Datei sollen alle Dateipfade so gesetzt sein, dass Meshes und Texturen im gleichen Verzeichnis wie das .irr-File adressiert werden (also relativ)
	// -> Anpassung der Pfadinformationen notwendig

	// Datei speichern
	std::cout << "[Optionen]: Speichere Dungeon in Datei " << _filename.c_str() << std::endl;
	Zeitmesser->Start(0);
	// Szene fuer Export neu erstellen
	Szene->SzeneLoeschen(false);
	Szene->SzeneErstellen(DunGen::EZeichenModus::_ZEICHENMODUS_DUNGEON_, true);

	// Pfad setzen: alle Dateien im gewaehlten Ordner speichern
	FileSystem->changeWorkingDirectoryTo(FileSystem->getFileDir(_filename));
	// Dateinamen ohne Endung extrahieren
	irr::io::path basisDateiname = FileSystem->getFileBasename(_filename, false);
	irr::io::IWriteFile* datei;
	irr::io::path dateiName;

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Schritt 1: Hoehlen- und Gangmeshes speichern & Dateinamen zu Meshcache hinzufuegen
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	// MeshWriter erstellen
	irr::scene::IMeshWriter* meshWriter = new irr::scene::COBJMeshWriter_Modified(SceneManager,FileSystem); // eigener Meshwriter, Export mit und ohne TexCoords moeglich
		
	// Hoehlenmeshes speichern
	std::cout << "1. Speichere Hoehlenmeshes" << std::endl;
	for (unsigned int i=0; i<DreiecksMesh->MeshVoxelAufgeteilt.size(); i++)		// alle Detailstufen
		for (unsigned int j=0; j<DreiecksMesh->MeshVoxelAufgeteilt.at(i)->size(); j++)	// alle Hoehlenteile
		{
			// Dateinamen bestimmen
			dateiName = basisDateiname + "_HoehlenMesh" +  irr::core::stringc(j) + "_Detailstufe" + irr::core::stringc(i) + ".obj";
			// Datei erzeugen
			datei = FileSystem->createAndWriteFile(dateiName);
	
			// Wenn Dateizugriff erfolgreich:
			if (datei)
			{
				// Im MeshCache vermerken: dazu temporaeren AnimatedMesh-erstellen, da dieser Typ erforderlich fuer Meshcache
				irr::scene::SAnimatedMesh* neuerAnimierterMesh= new irr::scene::SAnimatedMesh(DreiecksMesh->MeshVoxelAufgeteilt.at(i)->at(j));
				SceneManager->getMeshCache()->addMesh(dateiName, neuerAnimierterMesh);
				neuerAnimierterMesh->drop();

				std::cout << "Speichere Datei " << dateiName.c_str() << std::endl;
				meshWriter->writeMesh(datei,DreiecksMesh->MeshVoxelAufgeteilt.at(i)->at(j),0);
				// Datei schliessen
				datei->drop();
			}
			else
				std::cout << "Fehler beim Zugriff auf Datei " << dateiName.c_str() << std::endl;
		}
	std::cout << "Hoehlenmeshes gespeichert." << std::endl;


	// Gangmeshes speichern
	std::cout << "2. Speichere Gangmeshes" << std::endl;
	for (unsigned int i=0; i<Architekt->Gaenge.size(); i++)
	{
		// Detailstufen speichern
		for (unsigned int j=0; j<Architekt->Gaenge.at(i)->MeshSpline.size(); j++)
		{
			// Dateinamen bestimmen
			dateiName = basisDateiname + "_Gang" + irr::core::stringc(i) + "_Detailstufe_" +  irr::core::stringc(j) + ".obj";
			// Datei erzeugen
			datei = FileSystem->createAndWriteFile(dateiName);
	
			// Wenn Dateizugriff erfolgreich:
			if (datei)
			{
				// Im MeshCache vermerken: dazu temporaeren AnimatedMesh-erstellen, da dieser Typ erforderlich fuer Meshcache
				irr::scene::SAnimatedMesh* neuerAnimierterMesh= new irr::scene::SAnimatedMesh(Architekt->Gaenge.at(i)->MeshSpline.at(j));
				SceneManager->getMeshCache()->addMesh(dateiName, neuerAnimierterMesh);
				neuerAnimierterMesh->drop();
				
				std::cout << "Speichere Datei " << dateiName.c_str() << std::endl;
				meshWriter->writeMesh(datei,Architekt->Gaenge.at(i)->MeshSpline.at(j),1);
				// Datei schliessen
				datei->drop();
			}
			else
				std::cout << "Fehler beim Zugriff auf Datei " << dateiName.c_str() << std::endl;
		}

		// Adapter speichern:
		for (unsigned int j=0; j<2; j++)
		{
			// Dateinamen bestimmen
			dateiName = basisDateiname + "_Gang" + irr::core::stringc(i) + "_Adapter_" +  irr::core::stringc(j) + ".obj";
			// Datei erzeugen
			datei = FileSystem->createAndWriteFile(dateiName);
	
			// Wenn Dateizugriff erfolgreich:
			if (datei)
			{
				// Im MeshCache vermerken: dazu temporaeren AnimatedMesh-erstellen, da dieser Typ erforderlich fuer Meshcache
				irr::scene::SAnimatedMesh* neuerAnimierterMesh= new irr::scene::SAnimatedMesh(Architekt->Gaenge.at(i)->MeshAdapter[j]);
				SceneManager->getMeshCache()->addMesh(datei->getFileName(), neuerAnimierterMesh);
				neuerAnimierterMesh->drop();
				
				std::cout << "Speichere Datei " << dateiName.c_str() << std::endl;
				meshWriter->writeMesh(datei,Architekt->Gaenge.at(i)->MeshAdapter[j],1);
				// Datei schliessen
				datei->drop();
			}
			else
				std::cout << "Fehler beim Zugriff auf Datei " << dateiName.c_str() << std::endl;
		}
	}
	std::cout << "Gangmeshes gespeichert." << std::endl;

	meshWriter->drop();

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Schritt 2: Meshcache und Texturebuffer durchlaufen: alle Dateipfade zu blosse Dateinamen kuerzen (Originalwerte merken)
	// wichtig, damit Namen im .irr-File korrekt sind (keine absoluten Pfade)
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	
	std::cout << "3. Meshbuffer & Texturecache: Dateipfade kuerzen ...";
	// durch Umbenennung koennen sich die Indices im Meshcache bzw. Texturebuffer aendern, d.h. Umbenenung nicht ueber Indices durchfuehren

	std::vector<irr::video::ITexture*> texturen;
	std::vector<irr::io::path> texturNamen;
	irr::io::path tempName;
	
	// Texturen merken, deren Name geaendert werden muss
	for (unsigned int i=0; i<VideoDriver->getTextureCount(); i++)
	{
		tempName = VideoDriver->getTextureByIndex(i)->getName();
		if (tempName != FileSystem->getFileBasename(tempName))
		{
			// Textur merken
			texturen.push_back(VideoDriver->getTextureByIndex(i));
			// Namen merken
			texturNamen.push_back(tempName);		
		}
	}
	
	// Texturen umbenennen
	for (unsigned int i=0; i<texturen.size(); i++)
		VideoDriver->renameTexture(texturen.at(i), FileSystem->getFileBasename(texturNamen.at(i)));

	std::vector<irr::scene::IAnimatedMesh*> meshes;
	std::vector<irr::io::path> meshNamen;

	//  Meshes merken, deren Name geaendert werden muss
	for (unsigned int i=0; i<SceneManager->getMeshCache()->getMeshCount(); i++)
	{
		tempName = SceneManager->getMeshCache()->getMeshName(i);
		if (tempName != FileSystem->getFileBasename(tempName))
		{
			// Mesh merken
			meshes.push_back(SceneManager->getMeshCache()->getMeshByIndex(i));
			// Namen merken
			meshNamen.push_back(tempName);
		}
	}
	
	// Meshes umbenennen
	for (unsigned int i=0; i<meshes.size(); i++)
		SceneManager->getMeshCache()->renameMesh(meshes.at(i), FileSystem->getFileBasename(meshNamen.at(i)));

	std::cout << " abgeschlossen." << std::endl;

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Schritt 3: Export der Szene als .irr
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	
	std::cout << "4. Speichere Szene ...";
	// Kamera entfernen vorm Export
	irr::scene::ICameraSceneNode* camera = SceneManager->getActiveCamera(); 
	if (camera)
	{
		camera->grab();								// RefCounter erhoehen, damit nicht geloescht
		SceneManager->getActiveCamera()->remove();	// aus der Szene entfernen
	}

	// Szene speichern
	SceneManager->saveScene(_filename);

	// Kamera wieder hinzuefuegen, nach Export
	if (camera)
	{
		SceneManager->getRootSceneNode()->addChild(camera);
		SceneManager->setActiveCamera(camera);
		camera->drop();									// RefCounter wieder auf OriginalWert
	}
	std::cout << " abgeschlossen." << std::endl;

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Schritt 4: Meshcache und Texturebuffer durchlaufen: Originalwerte der Dateipfade wiederherstellen
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	std::cout << "5. Meshbuffer & Texturecache: Dateipfade wiederherstellen ...";

	// Texturen umbenennen
	for (unsigned int i=0; i<texturen.size(); i++)
		VideoDriver->renameTexture(texturen.at(i),texturNamen.at(i));

	// Meshes umbenennen
	for (unsigned int i=0; i<meshes.size(); i++)
		SceneManager->getMeshCache()->renameMesh(meshes.at(i),meshNamen.at(i));

	std::cout << " abgeschlossen." << std::endl;

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Schritt 5: Zusatzinformationen als .xml exportieren
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ueber Extra-Funktion
	dateiName = basisDateiname + "_Zusatzinformationen.xml";
	Speichere_Ergebnisse_Zusatzinformationen(dateiName);

	std::cout << "[Optionen]: Speicherung des Dungeons abgeschlossen." << std::endl;
	std::cout << "[Optionen]: Benoetigte Zeit zum Speichern: ";
	Zeitmesser->Stop(0);
}

void DunGen::COptionen::Lade_Ergebnisse_Dungeon(const irr::io::path& _filename)
{
	// Datei laden
	std::cout << "[Optionen]: Lade Dungeon aus Datei " << _filename.c_str() << std::endl;
	
	// Pfad setzen
	FileSystem->changeWorkingDirectoryTo(FileSystem->getFileDir(_filename));

	// alte Szene loeschen
	Szene->SzeneLoeschen(false);
	// .irr-Datei laden
	SceneManager->loadScene(_filename);
	// Kamera erstellen
	Szene->SzeneErstellen(DunGen::EZeichenModus::_ZEICHENMODUS_KAMERA_, true);

	// Materialien fuer Hoehle und Gaenge setzen
	// & Szene reparsen
	Szene->SzeneReparsen();
}


void DunGen::COptionen::Speichere_Ergebnisse_Zusatzinformationen(const irr::io::path& _filename)
{
	// Datei speichern
	std::cout << "6. Speichere Zusatzinformationen (Visibility etc.) in Datei: " << _filename.c_str() << std::endl;
	irr::io::IWriteFile* datei = FileSystem->createAndWriteFile(_filename);
	 
	// Wenn Dateizugriff erfolgreich:
	if (datei)
	{
		std::cout << "Beginne mit Schreiben ...";
		// XML-Writer erstellen
		irr::io::IXMLWriter* xmlWriter = FileSystem->createXMLWriter(datei);
		xmlWriter->writeXMLHeader();
		xmlWriter->writeLineBreak();

		xmlWriter->writeElement(L"DungeonInformationen");
		xmlWriter->writeLineBreak();

		// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		xmlWriter->writeElement(L"Raueme");
		xmlWriter->writeLineBreak();

		// alle Raume bearbeiten
		for (unsigned i=0; i<Architekt->Raeume.size(); i++)
		{
			xmlWriter->writeElement(L"Raum",false,
				L"ID",irr::core::stringw(i).c_str());
			xmlWriter->writeLineBreak();

			// Himmelsrichtungen auf Ursprungssubszene zurueckrechnen und speichern
			xmlWriter->writeElement(L"GangIDs",true,
				L"Gang0Nord",irr::core::stringw(Architekt->Raeume.at(i)->GangID[(0+Architekt->Raeume.at(i)->Offset)%4]).c_str(),
				L"Gang1Ost",irr::core::stringw(Architekt->Raeume.at(i)->GangID[(1+Architekt->Raeume.at(i)->Offset)%4]).c_str(),
				L"Gang2Sued",irr::core::stringw(Architekt->Raeume.at(i)->GangID[(2+Architekt->Raeume.at(i)->Offset)%4]).c_str(),
				L"Gang3West",irr::core::stringw(Architekt->Raeume.at(i)->GangID[(3+Architekt->Raeume.at(i)->Offset)%4]).c_str());
			xmlWriter->writeLineBreak();

			xmlWriter->writeClosingTag(L"Raum");
			xmlWriter->writeLineBreak();
		}

		xmlWriter->writeClosingTag(L"Raueme");
		xmlWriter->writeLineBreak();

		// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		xmlWriter->writeElement(L"Gaenge",false,
			L"GangBreite",irr::core::stringw(Architekt->LeseGangBreite()).c_str());
		xmlWriter->writeLineBreak();

		irr::core::vector3d<double> tempVec3;

		// alle Gaenge bearbeiten
		for (unsigned i=0; i<Architekt->Gaenge.size(); i++)
		{
			xmlWriter->writeElement(L"Gang",false,
				L"ID",irr::core::stringw(i).c_str(),
				L"DefinitivBlickdicht",irr::core::stringw(Architekt->Gaenge.at(i)->LeseDefinitivBlickdicht()).c_str());
			xmlWriter->writeLineBreak();

			xmlWriter->writeElement(L"RaumIDs",true,
				L"RaumAndockstelle0",irr::core::stringw(Architekt->Gaenge.at(i)->LeseAndocksstellenId0()).c_str(),
				L"RaumAndockstelle1",irr::core::stringw(Architekt->Gaenge.at(i)->LeseAndocksstellenId1()).c_str());
			xmlWriter->writeLineBreak();

			tempVec3 = Architekt->Gaenge.at(i)->LesePosition(0);			
			xmlWriter->writeElement(L"Position0",true,
				L"X",irr::core::stringw(tempVec3.X).c_str(),
				L"Y",irr::core::stringw(tempVec3.Y).c_str(),
				L"Z",irr::core::stringw(tempVec3.Z).c_str());
			xmlWriter->writeLineBreak();

			tempVec3 = Architekt->Gaenge.at(i)->LesePosition(1);			
			xmlWriter->writeElement(L"Position1",true,
				L"X",irr::core::stringw(tempVec3.X).c_str(),
				L"Y",irr::core::stringw(tempVec3.Y).c_str(),
				L"Z",irr::core::stringw(tempVec3.Z).c_str());
			xmlWriter->writeLineBreak();

			tempVec3 = Architekt->Gaenge.at(i)->LeseAbleitung(0);			
			xmlWriter->writeElement(L"Richtung0",true,
				L"X",irr::core::stringw(tempVec3.X).c_str(),
				L"Y",irr::core::stringw(tempVec3.Y).c_str(),
				L"Z",irr::core::stringw(tempVec3.Z).c_str());
			xmlWriter->writeLineBreak();

			tempVec3 = Architekt->Gaenge.at(i)->LeseAbleitung(1);			
			xmlWriter->writeElement(L"Richtung1",true,
				L"X",irr::core::stringw(-tempVec3.X).c_str(),
				L"Y",irr::core::stringw(-tempVec3.Y).c_str(),
				L"Z",irr::core::stringw(-tempVec3.Z).c_str());
			xmlWriter->writeLineBreak();

			xmlWriter->writeClosingTag(L"Gang");
			xmlWriter->writeLineBreak();
		}

		xmlWriter->writeClosingTag(L"Gaenge");
		xmlWriter->writeLineBreak();

		// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		xmlWriter->writeElement(L"Hoehle");
		xmlWriter->writeLineBreak();

		// alle Hoehlenteile bearbeiten
		for (unsigned i=0; i<DreiecksMesh->VisibilityInformationen.size(); i++)
		{
			xmlWriter->writeElement(L"HoehlenTeil",false,
				L"ID",irr::core::stringw(i).c_str());
			xmlWriter->writeLineBreak();

			xmlWriter->writeElement(L"VoxelMin",true,
				L"X",irr::core::stringw(DreiecksMesh->VisibilityInformationen.at(i)->GrenzeMinX).c_str(),
				L"Y",irr::core::stringw(DreiecksMesh->VisibilityInformationen.at(i)->GrenzeMinY).c_str(),
				L"Z",irr::core::stringw(DreiecksMesh->VisibilityInformationen.at(i)->GrenzeMinZ).c_str());
			xmlWriter->writeLineBreak();

			xmlWriter->writeElement(L"VoxelMax",true,
				L"X",irr::core::stringw(DreiecksMesh->VisibilityInformationen.at(i)->GrenzeMaxX).c_str(),
				L"Y",irr::core::stringw(DreiecksMesh->VisibilityInformationen.at(i)->GrenzeMaxY).c_str(),
				L"Z",irr::core::stringw(DreiecksMesh->VisibilityInformationen.at(i)->GrenzeMaxZ).c_str());
			xmlWriter->writeLineBreak();

			xmlWriter->writeElement(L"BlickdichtNegativeRichtung",true,
				L"X",irr::core::stringw(DreiecksMesh->VisibilityInformationen.at(i)->Xneg).c_str(),
				L"Y",irr::core::stringw(DreiecksMesh->VisibilityInformationen.at(i)->Yneg).c_str(),
				L"Z",irr::core::stringw(DreiecksMesh->VisibilityInformationen.at(i)->Zneg).c_str());
			xmlWriter->writeLineBreak();

			xmlWriter->writeElement(L"BlickdichtPositiveRichtung",true,
				L"X",irr::core::stringw(DreiecksMesh->VisibilityInformationen.at(i)->Xpos).c_str(),
				L"Y",irr::core::stringw(DreiecksMesh->VisibilityInformationen.at(i)->Ypos).c_str(),
				L"Z",irr::core::stringw(DreiecksMesh->VisibilityInformationen.at(i)->Zpos).c_str());
			xmlWriter->writeLineBreak();

			xmlWriter->writeClosingTag(L"HoehlenTeil");
			xmlWriter->writeLineBreak();
		}
		
		xmlWriter->writeClosingTag(L"Hoehle");
		xmlWriter->writeLineBreak();

		// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		xmlWriter->writeClosingTag(L"DungeonInformationen");
		xmlWriter->writeLineBreak();
		// Datei schliessen
		xmlWriter->drop();
		datei->drop();

		std::cout << " abgeschlossen." << std::endl;
	}
	else
		std::cout << "Fehler beim Zugriff auf Datei!" << std::endl;

}

// ======================================================
// Screenshot anfertigen
// ======================================================

void DunGen::COptionen::ScreenshotAnfertigen()
{	
	// Screenshot auslesen
	irr::video::IImage* screenShot = VideoDriver->createScreenShot();

	// Zeitinformationen bestimmen
	time_t zeitStempel;
	tm *zeitInformationen;
	zeitStempel = time(NULL);
	zeitInformationen = localtime(&zeitStempel);
	
	// Dateinamen generieren
	irr::io::path zeitDateiNamenTeil = irr::core::stringc(zeitInformationen->tm_year+1900) + "-";
	if (zeitInformationen->tm_mon < 9) zeitDateiNamenTeil += "0";
	zeitDateiNamenTeil += irr::core::stringc(zeitInformationen->tm_mon+1) + "-";
	if (zeitInformationen->tm_mday < 10) zeitDateiNamenTeil += "0";
	zeitDateiNamenTeil += irr::core::stringc(zeitInformationen->tm_mday) + "_";

	if (zeitInformationen->tm_hour < 10) zeitDateiNamenTeil += "0";
	zeitDateiNamenTeil += irr::core::stringc(zeitInformationen->tm_hour) + "-";
	if (zeitInformationen->tm_min < 10) zeitDateiNamenTeil += "0";
	zeitDateiNamenTeil += irr::core::stringc(zeitInformationen->tm_min) + "-";
	if (zeitInformationen->tm_sec < 10) zeitDateiNamenTeil += "0";
	zeitDateiNamenTeil += irr::core::stringc(zeitInformationen->tm_sec);

	irr::io::path pfad = "Screenshots/";
	irr::io::path dateiName = "Screenshot_";
	dateiName = dateiName + zeitDateiNamenTeil + ".bmp";

	// Datei speichern
	std::cout << "[Optionen]: Speichere Screenshot in Datei " << dateiName.c_str() << " ...";
	dateiName = pfad + dateiName;
	irr::io::IWriteFile* datei = FileSystem->createAndWriteFile(dateiName);

	// Wenn Dateizugriff erfolgreich:
	if (datei)
	{
		// Screenshot speichern
		VideoDriver->writeImageToFile(screenShot,datei,0);
		// Datei schliessen
		datei->drop();
		std::cout << " abgeschlossen." << std::endl;
	}
	else
		std::cout << " Fehler beim Zugriff auf Datei!" << std::endl;

	// Screenshot aus Arbeitspeicher loeschen
	screenShot->drop();	
}