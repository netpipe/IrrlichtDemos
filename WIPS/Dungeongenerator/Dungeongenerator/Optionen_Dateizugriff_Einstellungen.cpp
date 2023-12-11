// Implementierung der Klasse Optionen
#include "Optionen.h"

// ======================================================
// Dateizugriff: Einstellungen
// ======================================================
void DunGen::COptionen::Speichere_Einstellungen(const irr::io::path& _filename)
{
	// Datei speichern
	std::cout << "[Optionen]: Speichere Einstellungen in Datei: " << _filename.c_str() << std::endl;
	irr::io::IWriteFile* datei = FileSystem->createAndWriteFile(_filename);
	 
	// Wenn Dateizugriff erfolgreich:
	if (datei)
	{
		std::cout << "[Optionen]: Beginne mit Schreiben ... " << std::endl;
		// XML-Writer erstellen
		irr::io::IXMLWriter* xmlWriter = FileSystem->createXMLWriter(datei);
		xmlWriter->writeXMLHeader();
		xmlWriter->writeLineBreak();

		xmlWriter->writeElement(L"DungeonGenerator");
		xmlWriter->writeLineBreak();

		// Fraktalparameter speichern
		if (Dateizugriff_Einstellungen_Fraktalparameter)
		{
			std::cout << "[Optionen]: Speichere LSystemParameter... " << std::endl;
			xmlWriter->writeElement(L"LSystemParameter");
			xmlWriter->writeLineBreak();
		
			xmlWriter->writeElement(L"Winkel",true,
				L"WinkelGier",irr::core::stringw(FraktalGenerator->LeseParameter(ELSystemEigenschaft::_LSYSTEMEIGENSCHAFT_WINKEL_GIER_)).c_str(),
				L"WinkelNick",irr::core::stringw(FraktalGenerator->LeseParameter(ELSystemEigenschaft::_LSYSTEMEIGENSCHAFT_WINKEL_NICK_)).c_str(),
				L"WinkelRoll",irr::core::stringw(FraktalGenerator->LeseParameter(ELSystemEigenschaft::_LSYSTEMEIGENSCHAFT_WINKEL_ROLL_)).c_str());
			xmlWriter->writeLineBreak();
			xmlWriter->writeElement(L"Radius",true,
				L"StartRadius",irr::core::stringw(FraktalGenerator->LeseParameter(ELSystemEigenschaft::_LSYSTEMEIGENSCHAFT_START_RADIUS_)).c_str(),
				L"RadiusFaktor",irr::core::stringw(FraktalGenerator->LeseParameter(ELSystemEigenschaft::_LSYSTEMEIGENSCHAFT_RADIUS_FAKTOR_)).c_str(),
				L"RadiusDekrementor",irr::core::stringw(FraktalGenerator->LeseParameter(ELSystemEigenschaft::_LSYSTEMEIGENSCHAFT_RADIUS_DEKREMENTOR_)).c_str());
			xmlWriter->writeLineBreak();
			xmlWriter->writeElement(L"Weitere",true,
				L"Start",irr::core::stringw(FraktalGenerator->LeseStart().c_str()).c_str(),
				L"Iterationsstufe",irr::core::stringw(Iterationsstufe).c_str());
			xmlWriter->writeLineBreak();

			// Regeln schreiben
			std::string tempString = "0"; 
			for (unsigned int i = 0; i < FraktalGenerator->LeseRegelAnzahl(); i++)
			{
				tempString.at(0) = FraktalGenerator->LeseRegel(i).Symbol;	// char -> string

				xmlWriter->writeElement(L"Regel",true,
					L"Symbol", (irr::core::stringw(tempString.c_str()).c_str()),
					L"Ersetzung", irr::core::stringw(FraktalGenerator->LeseRegel(i).Ersetzung.c_str()).c_str());
				xmlWriter->writeLineBreak();
			}
			
			xmlWriter->writeClosingTag(L"LSystemParameter");
			xmlWriter->writeLineBreak();
		}

		// Weitere Fraktaleinstellungen speichern
		if (Dateizugriff_Einstellungen_Fraktaleinstellungen_Weitere)
		{
			std::cout << "[Optionen]: Speichere FraktalgeneratorEinstellungen... " << std::endl;
			xmlWriter->writeElement(L"FraktalgeneratorEinstellungen");
			xmlWriter->writeLineBreak();
			
			xmlWriter->writeElement(L"WinkelGier",true,
				L"Minimum",irr::core::stringw(WinkelGier_Min).c_str(),
				L"Maximum",irr::core::stringw(WinkelGier_Max).c_str(),
				L"Kommastellen",irr::core::stringw(WinkelGier_Kommastellen).c_str());
			xmlWriter->writeLineBreak();
			xmlWriter->writeElement(L"WinkelNick",true,
				L"Minimum",irr::core::stringw(WinkelNick_Min).c_str(),
				L"Maximum",irr::core::stringw(WinkelNick_Max).c_str(),
				L"Kommastellen",irr::core::stringw(WinkelNick_Kommastellen).c_str());
			xmlWriter->writeLineBreak();
			xmlWriter->writeElement(L"WinkelRoll",true,
				L"Minimum",irr::core::stringw(WinkelRoll_Min).c_str(),
				L"Maximum",irr::core::stringw(WinkelRoll_Max).c_str(),
				L"Kommastellen",irr::core::stringw(WinkelRoll_Kommastellen).c_str());
			xmlWriter->writeLineBreak();
			xmlWriter->writeElement(L"StartRadius",true,
				L"Minimum",irr::core::stringw(StartRadius_Min).c_str(),
				L"Maximum",irr::core::stringw(StartRadius_Max).c_str(),
				L"Kommastellen",irr::core::stringw(StartRadius_Kommastellen).c_str());
			xmlWriter->writeLineBreak();
			xmlWriter->writeElement(L"RadiusFaktor",true,
				L"Minimum",irr::core::stringw(RadiusFaktor_Min).c_str(),
				L"Maximum",irr::core::stringw(RadiusFaktor_Max).c_str(),
				L"Kommastellen",irr::core::stringw(RadiusFaktor_Kommastellen).c_str());
			xmlWriter->writeLineBreak();
			xmlWriter->writeElement(L"RadiusDekrementor",true,
				L"Minimum",irr::core::stringw(RadiusDekrementor_Min).c_str(),
				L"Maximum",irr::core::stringw(RadiusDekrementor_Max).c_str(),
				L"Kommastellen",irr::core::stringw(RadiusDekrementor_Kommastellen).c_str());
			xmlWriter->writeLineBreak();
			
			xmlWriter->writeElement(L"Start",true,
				L"ErlaubteSymbole",irr::core::stringw(Start_ErlaubteSymbole.c_str()).c_str(),
				L"MinimalLaenge",irr::core::stringw(Start_MinLaenge).c_str(),
				L"MaximalLaenge",irr::core::stringw(Start_MaxLaenge).c_str());
			xmlWriter->writeLineBreak();

			// Regeleinstellungen
			std::string tempString = "0"; 
			tempString.at(0) = Symbol;	// char -> string
			xmlWriter->writeElement(L"Regeln",true,
				L"Symbol",irr::core::stringw(tempString.c_str()).c_str(),
				L"Ersetzung",irr::core::stringw(Symbolersetzung.c_str()).c_str(),
				L"ErsetzungErlaubteSymbole",irr::core::stringw(Ersetzung_ErlaubteSymbole.c_str()).c_str(),
				L"MinimalLaenge",irr::core::stringw(Ersetzung_MinLaenge).c_str(),
				L"MaximalLaenge",irr::core::stringw(Ersetzung_MaxLaenge).c_str());
			xmlWriter->writeLineBreak();

			xmlWriter->writeClosingTag(L"FraktalgeneratorEinstellungen");
			xmlWriter->writeLineBreak();
		}
		
		// Voxelraum-Einstellungen speichern
		if (Dateizugriff_Einstellungen_Voxelraum)
		{
			std::cout << "[Optionen]: Speichere VoxelraumEinstellungen... " << std::endl;
			xmlWriter->writeElement(L"VoxelraumEinstellungen");
			xmlWriter->writeLineBreak();

			xmlWriter->writeElement(L"VoxelRaum",true,
				L"Rand",irr::core::stringw(VoxelRaum->LeseRand()).c_str(),
				L"MinimalRadius",irr::core::stringw(VoxelRaum->LeseMinimalenStrichradius()).c_str());
			xmlWriter->writeLineBreak();

			xmlWriter->writeElement(L"Nachbearbeitung",true,
				L"Erosionswahrscheinlichkeit",irr::core::stringw(Erosionswahrscheinlichkeit).c_str(),
				L"ErosionsZufallsSeed",irr::core::stringw(ErosionsZufallsSeed).c_str(),
				L"VorgegebenenErosionsZufallsSeedNutzen",irr::core::stringw(VorgegebenenErosionsZufallsSeedNutzen).c_str(),
				L"SchwebendeFragmenteFiltern",irr::core::stringw(SchwebendeFragmenteFiltern).c_str());
			xmlWriter->writeLineBreak();
			
			// Erosionsschritte
			xmlWriter->writeElement(L"Erosionsschritte");
			xmlWriter->writeLineBreak();
			for (unsigned int i = 0; i < ErosionsSchritte.size(); i++)
			{
				xmlWriter->writeElement(L"Erosionsschritt",true,
					L"Erosionswahrscheinlichkeit",irr::core::stringw(ErosionsSchritte.at(i)).c_str());
				xmlWriter->writeLineBreak();
			}
			xmlWriter->writeClosingTag(L"Erosionsschritte");
			xmlWriter->writeLineBreak();

			xmlWriter->writeElement(L"Umwandeln",true,
				L"NormalenWichtungsmethode",irr::core::stringw((int)DreiecksMesh->LeseNormalenwichtungsmethode()).c_str(),
				L"Detailstufen",irr::core::stringw(DreiecksMesh_Detailstufen).c_str());
			xmlWriter->writeLineBreak();

			xmlWriter->writeElement(L"Verwacklungsparameter",true,
				L"VerticesVerwackeln",irr::core::stringw(DreiecksMesh->LeseOptionVerwackeln()).c_str(),
				L"VerwacklungsStaerke",irr::core::stringw(DreiecksMesh->LeseVerwacklungsStaerke()).c_str(),
				L"MeshGlaetten",irr::core::stringw(DreiecksMesh->LeseOptionGlaetten()).c_str());
			xmlWriter->writeLineBreak();

			xmlWriter->writeElement(L"VerwacklungsZufallsparameter",true,
				L"ZufallsSeed",irr::core::stringw(DreiecksMesh->LeseZufallsSeedVerwackeln()).c_str(),
				L"a",irr::core::stringw(ZufallsGeneratorVerwacklung->LeseA()).c_str(),
				L"c",irr::core::stringw(ZufallsGeneratorVerwacklung->LeseC()).c_str(),
				L"m",irr::core::stringw(ZufallsGeneratorVerwacklung->LeseM()).c_str());
			xmlWriter->writeLineBreak();

			xmlWriter->writeClosingTag(L"VoxelraumEinstellungen");
			xmlWriter->writeLineBreak();
		}

		// Gaenge-Einstellungen speichern
		if (Dateizugriff_Einstellungen_Gaenge)
		{
			std::cout << "[Optionen]: Speichere GaengeEinstellungen... " << std::endl;
			xmlWriter->writeElement(L"GaengeEinstellungen");
			xmlWriter->writeLineBreak();

			xmlWriter->writeElement(L"GangParameter",true,
				L"GangBreite",irr::core::stringw(Architekt->LeseGangBreite()).c_str(),
				L"GangsegmentAbstand",irr::core::stringw(Architekt->LeseAbstandSplineSegmente()).c_str(),
				L"TextureYproAbstand1",irr::core::stringw(Architekt->LeseAbstandTexturYProAbstand1()).c_str(),
				L"AnzahlDetailstufen",irr::core::stringw(Architekt->LeseAnzahlSplineDetailstufen()).c_str(),
				L"GangTextur",irr::core::stringw(Szene->LeseGangTexturPfad()).c_str());
			xmlWriter->writeLineBreak();

			// Querschnitt
			xmlWriter->writeElement(L"GangQuerschnitt");
			xmlWriter->writeLineBreak();
			for (unsigned int i = 0; i < Architekt->QuerschnittGang_Normiert.Punkt.size(); i++)
			{
				xmlWriter->writeElement(L"QuerschnittsPunkt",true,
				L"PositionX", irr::core::stringw(Architekt->QuerschnittGang_Normiert.Punkt.at(i).X).c_str(),
				L"PositionY", irr::core::stringw(Architekt->QuerschnittGang_Normiert.Punkt.at(i).Y).c_str(),
				L"TexturkoordinateX", irr::core::stringw(Architekt->QuerschnittGang_Normiert.TextureX.at(i)).c_str());
				xmlWriter->writeLineBreak();
			}
			xmlWriter->writeClosingTag(L"GangQuerschnitt");
			xmlWriter->writeLineBreak();

			xmlWriter->writeClosingTag(L"GaengeEinstellungen");
			xmlWriter->writeLineBreak();
		}

		// Detailobjekte-Einstellungen speichern
		if (Dateizugriff_Einstellungen_Detailobjekte)
		{
			std::cout << "[Optionen]: Speichere DetailobjekteEinstellungen... " << std::endl;
			xmlWriter->writeElement(L"DetailobjekteEinstellungen");
			xmlWriter->writeLineBreak();

			for (unsigned int i = 0; i < Architekt->DetailobjektVorlagen.size(); i++)
			{
				xmlWriter->writeElement(L"Detailobjekt", false,
					L"Benennung", irr::core::stringw(Architekt->DetailobjektVorlagen.at(i)->Name).c_str(),
					L"MeshDateipfad", irr::core::stringw( Architekt->DetailobjektVorlagen.at(i)->MeshPfad).c_str());
				xmlWriter->writeLineBreak();

				// Skalierung, Rotation, Position
				xmlWriter->writeElement(L"Skalierung", true,
					L"X", irr::core::stringw(Architekt->DetailobjektVorlagen.at(i)->Skalierung.X).c_str(),
					L"Y", irr::core::stringw(Architekt->DetailobjektVorlagen.at(i)->Skalierung.Y).c_str(),
					L"Z", irr::core::stringw(Architekt->DetailobjektVorlagen.at(i)->Skalierung.Z).c_str());
				xmlWriter->writeLineBreak();

				xmlWriter->writeElement(L"Rotation", true,
					L"X", irr::core::stringw(Architekt->DetailobjektVorlagen.at(i)->Rotation.X).c_str(),
					L"Y", irr::core::stringw(Architekt->DetailobjektVorlagen.at(i)->Rotation.Y).c_str(),
					L"Z", irr::core::stringw(Architekt->DetailobjektVorlagen.at(i)->Rotation.Z).c_str());
				xmlWriter->writeLineBreak();

				xmlWriter->writeElement(L"Position", true,
					L"X", irr::core::stringw(Architekt->DetailobjektVorlagen.at(i)->Position.X).c_str(),
					L"Y", irr::core::stringw(Architekt->DetailobjektVorlagen.at(i)->Position.Y).c_str());
				xmlWriter->writeLineBreak();

				// Weiteres
				xmlWriter->writeElement(L"NumAbstand", true,
					L"Faktor", irr::core::stringw(Architekt->DetailobjektVorlagen.at(i)->AbstandNumFaktor).c_str(),
					L"Min", irr::core::stringw(Architekt->DetailobjektVorlagen.at(i)->AbstandNumMin).c_str(),
					L"Max", irr::core::stringw(Architekt->DetailobjektVorlagen.at(i)->AbstandNumMax).c_str(),
					L"MinErstesElement", irr::core::stringw(Architekt->DetailobjektVorlagen.at(i)->AbstandNumMinErstesElement).c_str(),
					L"MaxErstesElement", irr::core::stringw(Architekt->DetailobjektVorlagen.at(i)->AbstandNumMaxErstesElement).c_str());
				xmlWriter->writeLineBreak();

				xmlWriter->writeElement(L"Weiteres", true,
					L"Abtastung", irr::core::stringw(Architekt->DetailobjektVorlagen.at(i)->AbstandsAbtastung).c_str(),
					L"ObjektAn1", irr::core::stringw(Architekt->DetailobjektVorlagen.at(i)->ObjektAn1).c_str());
				xmlWriter->writeLineBreak();

				xmlWriter->writeClosingTag(L"Detailobjekt");
				xmlWriter->writeLineBreak();
			}

			xmlWriter->writeClosingTag(L"DetailobjekteEinstellungen");
			xmlWriter->writeLineBreak();
		}

		// Architekt-Einstellungen speichern
		if (Dateizugriff_Einstellungen_Architekt)
		{
			std::cout << "[Optionen]: Speichere ArchitektEinstellungen... " << std::endl;
			xmlWriter->writeElement(L"ArchitektEinstellungen");
			xmlWriter->writeLineBreak();

			xmlWriter->writeElement(L"Generelles", true,
				L"KontruktionsZufallsSeed",irr::core::stringw(KonstruktionsZufallsSeed).c_str(),
				L"VorgegebenenKontruktionsZufallsSeedNutzen",irr::core::stringw(VorgegebenenKonstruktionsZufallsSeedNutzen).c_str(),
				L"RaumsektorAusdehnung", irr::core::stringw(Raumsektorausdehnung).c_str());
			xmlWriter->writeLineBreak();

			xmlWriter->writeElement(L"AndockEigenschaften", true,
				L"MinAbstandRaumHoehle", irr::core::stringw(Architekt->LeseAbstandAndockHoehleMin()).c_str(),
				L"AbstandBoden", irr::core::stringw(Architekt->LeseAbstandVomBodenMax()).c_str(),
				L"AbstandstestTiefe", irr::core::stringw(Architekt->LeseTiefeAbstandsTestMin()).c_str(),
				L"AusfraesTiefe",irr::core::stringw(Architekt->LeseZusaetzlicheAusfraestiefe()).c_str(),
				L"KollisionstestZusatzSicherheitsabstand", irr::core::stringw(Architekt->LeseZusaetzlicherSicherheitsabstand()).c_str());
			xmlWriter->writeLineBreak();

			xmlWriter->writeElement(L"Wahrscheinlichkeiten", true,
				L"WahrscheinlichkeitZusatzgangRaumHoehle", irr::core::stringw(Architekt->LeseWahrscheinlichkeitZusaetzlicherGangRaumHoehle()).c_str(),
				L"WahrscheinlichkeitZusatzgangRaumRaum", irr::core::stringw(Architekt->LeseWahrscheinlichkeitZusaetzlicherGangRaumRaum()).c_str(),
				L"WahrscheinlichkeitsfaktorLeererRaumAnHoehle", irr::core::stringw(Architekt->LesePlatzierungsWahrscheinlichkeitsFaktorAnHoehleLeereSubSzene()).c_str(),
				L"WahrscheinlichkeitsfaktorLeererRaumAnRaum", irr::core::stringw(Architekt->LesePlatzierungsWahrscheinlichkeitsFaktorAnRaumLeereSubSzene()).c_str());
			xmlWriter->writeLineBreak();

			xmlWriter->writeClosingTag(L"ArchitektEinstellungen");
			xmlWriter->writeLineBreak();
		}

		// Subszenen-Einstellungen speichern
		if (Dateizugriff_Einstellungen_Subszenen)
		{
			std::cout << "[Optionen]: Speichere SubszenenEinstellungen... " << std::endl;
			xmlWriter->writeElement(L"SubszenenEinstellungen");
			xmlWriter->writeLineBreak();

			for (unsigned int i = 0; i < Architekt->SubSzenen.size(); i++)
			{
				xmlWriter->writeElement(L"Subszene", false,
					L"Benennung", irr::core::stringw(Architekt->SubSzenen.at(i)->LeseName()).c_str(),
					L"WahrscheinlichkeitsfaktorAnHoehle", irr::core::stringw(Architekt->SubSzenen.at(i)->LesePlatzierungsWahrscheinlichkeitsFaktorAnHoehle()).c_str(),
					L"WahrscheinlichkeitsfaktorAnRaum", irr::core::stringw(Architekt->SubSzenen.at(i)->LesePlatzierungsWahrscheinlichkeitsFaktorAnRaum()).c_str(),
					L"Dateipfad", irr::core::stringw(Architekt->SubSzenen.at(i)->LesePfad()).c_str());
				xmlWriter->writeLineBreak();

				// Skalierung
				xmlWriter->writeElement(L"Skalierung", true,
					L"X", irr::core::stringw(Architekt->SubSzenen.at(i)->LeseSkalierung().X).c_str(),
					L"Y", irr::core::stringw(Architekt->SubSzenen.at(i)->LeseSkalierung().Y).c_str(),
					L"Z", irr::core::stringw(Architekt->SubSzenen.at(i)->LeseSkalierung().Z).c_str());
				xmlWriter->writeLineBreak();

				// Ableitungsfaktoren
				xmlWriter->writeElement(L"SplineAbleitungsFaktoren", true,
					L"RaumZuRaum", irr::core::stringw(Architekt->SubSzenen.at(i)->LeseAbleitungsFaktorRaumRaum()).c_str(),
					L"RaumZuHoehle", irr::core::stringw(Architekt->SubSzenen.at(i)->LeseAbleitungsFaktorRaumHoehle()).c_str(),
					L"HoehleZuRaum", irr::core::stringw(Architekt->SubSzenen.at(i)->LeseAbleitungsFaktorHoehleRaum()).c_str());
				xmlWriter->writeLineBreak();

				xmlWriter->writeClosingTag(L"Subszene");
				xmlWriter->writeLineBreak();
			}

			xmlWriter->writeClosingTag(L"SubszenenEinstellungen");
			xmlWriter->writeLineBreak();
		}

		// Erstellung-Einstellungen speichern
		if (Dateizugriff_Einstellungen_Erstellung)
		{
			std::cout << "[Optionen]: Speichere ErstellungEinstellungen... " << std::endl;
			xmlWriter->writeElement(L"ErstellungEinstellungen");
			xmlWriter->writeLineBreak();

			xmlWriter->writeElement(L"Fraktalzeichenanweisung",true,
				L"aktiviert",irr::core::stringw(Erstellung_Fraktalzeichenanweisung).c_str());
			xmlWriter->writeLineBreak();
			xmlWriter->writeElement(L"VoxelZeichnen_Erosion",true,
				L"aktiviert",irr::core::stringw(Erstellung_VoxelZeichnen_Erosion).c_str());
			xmlWriter->writeLineBreak();
			xmlWriter->writeElement(L"Filterung1",true,
				L"aktiviert",irr::core::stringw(Erstellung_Filterung1).c_str());
			xmlWriter->writeLineBreak();
			xmlWriter->writeElement(L"Architekt",true,
				L"aktiviert",irr::core::stringw(Erstellung_Architekt).c_str());
			xmlWriter->writeLineBreak();
			xmlWriter->writeElement(L"Filterung2",true,
				L"aktiviert",irr::core::stringw(Erstellung_Filterung2).c_str());
			xmlWriter->writeLineBreak();
			xmlWriter->writeElement(L"UmwandlungVoxel",true,
				L"aktiviert",irr::core::stringw(Erstellung_UmwandlungVoxel).c_str());
			xmlWriter->writeLineBreak();

			xmlWriter->writeClosingTag(L"ErstellungEinstellungen");
			xmlWriter->writeLineBreak();
		}

		// Weiteres-Einstellungen speichern
		if (Dateizugriff_Einstellungen_Weiteres)
		{
			std::cout << "[Optionen]: Speichere WeiteresEinstellungen... " << std::endl;
			xmlWriter->writeElement(L"WeiteresEinstellungen");
			xmlWriter->writeLineBreak();

			xmlWriter->writeElement(L"ZufallsparameterAllgemein",true,
				L"a",irr::core::stringw(ZufallsGeneratorAllgemein->LeseA()).c_str(),
				L"c",irr::core::stringw(ZufallsGeneratorAllgemein->LeseC()).c_str(),
				L"m",irr::core::stringw(ZufallsGeneratorAllgemein->LeseM()).c_str());
			xmlWriter->writeLineBreak();

			irr::video::SColorf farbeHoehle = Szene->LeseCallbackHoehlenShader()->LeseFarbe();
			
			xmlWriter->writeElement(L"Hoehlenfarbe",true,
				L"Rot",irr::core::stringw(farbeHoehle.r).c_str(),
				L"Gruen",irr::core::stringw(farbeHoehle.g).c_str(),
				L"Blau",irr::core::stringw(farbeHoehle.b).c_str());
			xmlWriter->writeLineBreak();

			xmlWriter->writeElement(L"Hintergrundfarbe",true,
				L"Rot",irr::core::stringw(Hintergrundfarbe.getRed()).c_str(),
				L"Gruen",irr::core::stringw(Hintergrundfarbe.getGreen()).c_str(),
				L"Blau",irr::core::stringw(Hintergrundfarbe.getBlue()).c_str());
			xmlWriter->writeLineBreak();

			xmlWriter->writeClosingTag(L"WeiteresEinstellungen");
			xmlWriter->writeLineBreak();
		}

		xmlWriter->writeClosingTag(L"DungeonGenerator");
		xmlWriter->writeLineBreak();
		// Datei schliessen
		xmlWriter->drop();
		datei->drop();
		std::cout << "[Optionen]: Schreiben abgeschlossen." << std::endl;
	}
	else
		std::cout << "Fehler beim Zugriff auf die Datei. Einstellungen wurden nicht gespeichert." << std::endl;
}

void DunGen::COptionen::Lade_Einstellungen(const irr::io::path& _filename)
{
	// Datei laden
	std::cout << "[Optionen]: Lade Einstellungen aus Datei: " << _filename.c_str() << std::endl;
	irr::io::IReadFile* datei = FileSystem->createAndOpenFile(_filename);
	 
	// Wenn Dateizugriff erfolgreich:
	if (datei)
	{
		std::cout << "[Optionen]: Beginne mit Lesen ... " << std::endl;
		// XML-Reader erstellen
		irr::io::IXMLReader* xmlReader = FileSystem->createXMLReader(datei);
		irr::core::stringc tempString;
		irr::core::stringc tempString2;

		// Parsen
		while(xmlReader->read())
        {
			if (xmlReader->getNodeType() == irr::io::EXML_NODE::EXN_ELEMENT)
			{
				// Fraktalparameter lesen
				if ((irr::core::stringw("LSystemParameter") == xmlReader->getNodeName()) && (Dateizugriff_Einstellungen_Fraktalparameter))
				{
					std::cout << "[Optionen]: Lese LSystemParameter... " << std::endl;
					FraktalGenerator->LoescheRegeln();
					// bis End-Tag erreicht
					while(! ((xmlReader->getNodeType() == irr::io::EXML_NODE::EXN_ELEMENT_END) && (irr::core::stringw("LSystemParameter") == xmlReader->getNodeName()) ) )
					{
						// Lesen
						xmlReader->read();
						// Verarbeiten
						if (xmlReader->getNodeType() == irr::io::EXML_NODE::EXN_ELEMENT)
							if (irr::core::stringw("Winkel") == xmlReader->getNodeName())
							{
								tempString = xmlReader->getAttributeValue(L"WinkelGier");
								FraktalGenerator->SetzeParameter(ELSystemEigenschaft::_LSYSTEMEIGENSCHAFT_WINKEL_GIER_, atof(tempString.c_str()));
								tempString = xmlReader->getAttributeValue(L"WinkelNick");
								FraktalGenerator->SetzeParameter(ELSystemEigenschaft::_LSYSTEMEIGENSCHAFT_WINKEL_NICK_, atof(tempString.c_str()));
								tempString = xmlReader->getAttributeValue(L"WinkelRoll");
								FraktalGenerator->SetzeParameter(ELSystemEigenschaft::_LSYSTEMEIGENSCHAFT_WINKEL_ROLL_, atof(tempString.c_str()));
							}
							else if (irr::core::stringw("Radius") == xmlReader->getNodeName())
							{
								tempString = xmlReader->getAttributeValue(L"StartRadius");
								FraktalGenerator->SetzeParameter(ELSystemEigenschaft::_LSYSTEMEIGENSCHAFT_START_RADIUS_, atof(tempString.c_str()));
								tempString = xmlReader->getAttributeValue(L"RadiusFaktor");
								FraktalGenerator->SetzeParameter(ELSystemEigenschaft::_LSYSTEMEIGENSCHAFT_RADIUS_FAKTOR_, atof(tempString.c_str()));
								tempString = xmlReader->getAttributeValue(L"RadiusDekrementor");
								FraktalGenerator->SetzeParameter(ELSystemEigenschaft::_LSYSTEMEIGENSCHAFT_RADIUS_DEKREMENTOR_, atof(tempString.c_str()));
							}
							else if (irr::core::stringw("Weitere") == xmlReader->getNodeName())
							{
								tempString = xmlReader->getAttributeValue(L"Start");
								FraktalGenerator->SetzeStart(tempString.c_str());
								Iterationsstufe = xmlReader->getAttributeValueAsInt(L"Iterationsstufe");
							}
							else if (irr::core::stringw("Regel") == xmlReader->getNodeName())
							{
								tempString = xmlReader->getAttributeValue(L"Symbol");
								tempString2 = xmlReader->getAttributeValue(L"Ersetzung");
								FraktalGenerator->FuegeRegelHinzu(tempString.c_str()[0],tempString2.c_str());
							}
					}
				}
				// Weitere Fraktaleinstellungen lesen
				else if  ((irr::core::stringw("FraktalgeneratorEinstellungen") == xmlReader->getNodeName()) && (Dateizugriff_Einstellungen_Fraktaleinstellungen_Weitere) )
				{
					std::cout << "[Optionen]: Lese FraktalgeneratorEinstellungen... " << std::endl;
					// bis End-Tag erreicht
					while(! ((xmlReader->getNodeType() == irr::io::EXML_NODE::EXN_ELEMENT_END) && (irr::core::stringw("FraktalgeneratorEinstellungen") == xmlReader->getNodeName()) ) )
					{
						// Lesen
						xmlReader->read();
						// Verarbeiten
						if (xmlReader->getNodeType() == irr::io::EXML_NODE::EXN_ELEMENT)
						{
							if (irr::core::stringw("WinkelGier") == xmlReader->getNodeName())
							{								
								tempString = xmlReader->getAttributeValue(L"Minimum");
								WinkelGier_Min = atof(tempString.c_str());
								tempString = xmlReader->getAttributeValue(L"Maximum");
								WinkelGier_Max = atof(tempString.c_str());
								tempString = xmlReader->getAttributeValue(L"Kommastellen");
								WinkelGier_Kommastellen = atoi(tempString.c_str());
							}
							else if (irr::core::stringw("WinkelNick") == xmlReader->getNodeName())
							{
								tempString = xmlReader->getAttributeValue(L"Minimum");
								WinkelNick_Min = atof(tempString.c_str());
								tempString = xmlReader->getAttributeValue(L"Maximum");
								WinkelNick_Max = atof(tempString.c_str());
								tempString = xmlReader->getAttributeValue(L"Kommastellen");
								WinkelNick_Kommastellen = atoi(tempString.c_str());
							}
							else if (irr::core::stringw("WinkelRoll") == xmlReader->getNodeName())
							{
								tempString = xmlReader->getAttributeValue(L"Minimum");
								WinkelRoll_Min = atof(tempString.c_str());
								tempString = xmlReader->getAttributeValue(L"Maximum");
								WinkelRoll_Max = atof(tempString.c_str());
								tempString = xmlReader->getAttributeValue(L"Kommastellen");
								WinkelRoll_Kommastellen = atoi(tempString.c_str());
							}
							else if (irr::core::stringw("StartRadius") == xmlReader->getNodeName())
							{
								tempString = xmlReader->getAttributeValue(L"Minimum");
								StartRadius_Min = atof(tempString.c_str());
								tempString = xmlReader->getAttributeValue(L"Maximum");
								StartRadius_Max = atof(tempString.c_str());
								tempString = xmlReader->getAttributeValue(L"Kommastellen");
								StartRadius_Kommastellen = atoi(tempString.c_str());
							}
							else if (irr::core::stringw("RadiusFaktor") == xmlReader->getNodeName())
							{
								tempString = xmlReader->getAttributeValue(L"Minimum");
								RadiusFaktor_Min = atof(tempString.c_str());
								tempString = xmlReader->getAttributeValue(L"Maximum");
								RadiusFaktor_Max = atof(tempString.c_str());
								tempString = xmlReader->getAttributeValue(L"Kommastellen");
								RadiusFaktor_Kommastellen = atoi(tempString.c_str());
							}
							else if (irr::core::stringw("RadiusDekrementor") == xmlReader->getNodeName())
							{
								tempString = xmlReader->getAttributeValue(L"Minimum");
								RadiusDekrementor_Min = atof(tempString.c_str());
								tempString = xmlReader->getAttributeValue(L"Maximum");
								RadiusDekrementor_Max = atof(tempString.c_str());
								tempString = xmlReader->getAttributeValue(L"Kommastellen");
								RadiusDekrementor_Kommastellen = atoi(tempString.c_str());
							}
							else if (irr::core::stringw("Start") == xmlReader->getNodeName())
							{
								tempString = xmlReader->getAttributeValue(L"ErlaubteSymbole");
								Start_ErlaubteSymbole = tempString.c_str();
								tempString = xmlReader->getAttributeValue(L"MinimalLaenge");
								Start_MinLaenge = atoi(tempString.c_str());
								tempString = xmlReader->getAttributeValue(L"MaximalLaenge");
								Start_MaxLaenge = atoi(tempString.c_str());
							}
							else if (irr::core::stringw("Regeln") == xmlReader->getNodeName())
							{		
								tempString = xmlReader->getAttributeValue(L"Symbol");
								Symbol = tempString.c_str()[0];
								tempString = xmlReader->getAttributeValue(L"Ersetzung");
								Symbolersetzung = tempString.c_str();	
								
								tempString = xmlReader->getAttributeValue(L"ErsetzungErlaubteSymbole");
								Ersetzung_ErlaubteSymbole = tempString.c_str();
								tempString = xmlReader->getAttributeValue(L"MinimalLaenge");
								Ersetzung_MinLaenge = atoi(tempString.c_str());
								tempString = xmlReader->getAttributeValue(L"MaximalLaenge");
								Ersetzung_MaxLaenge = atoi(tempString.c_str());
							}
						}
					}
				}
				// Voxelraum-Einstellungen lesen
				else if  ((irr::core::stringw("VoxelraumEinstellungen") == xmlReader->getNodeName()) && (Dateizugriff_Einstellungen_Voxelraum) )
				{
					std::cout << "[Optionen]: Lese VoxelraumEinstellungen... " << std::endl;
					ErosionsSchritte.clear();
					// bis End-Tag erreicht
					while(! ((xmlReader->getNodeType() == irr::io::EXML_NODE::EXN_ELEMENT_END) && (irr::core::stringw("VoxelraumEinstellungen") == xmlReader->getNodeName()) ) )
					{
						// Lesen
						xmlReader->read();
						// Verarbeiten
						if (xmlReader->getNodeType() == irr::io::EXML_NODE::EXN_ELEMENT)
							if (irr::core::stringw("VoxelRaum") == xmlReader->getNodeName())
							{
								tempString = xmlReader->getAttributeValue(L"Rand");
								VoxelRaum->SetzeRand(atoi(tempString.c_str()));
								tempString = xmlReader->getAttributeValue(L"MinimalRadius");
								VoxelRaum->SetzeMinimalenStrichradius(atoi(tempString.c_str()));
							}
							else if (irr::core::stringw("Nachbearbeitung") == xmlReader->getNodeName())
							{
								tempString = xmlReader->getAttributeValue(L"Erosionswahrscheinlichkeit");
								Erosionswahrscheinlichkeit = atof(tempString.c_str());
								tempString = xmlReader->getAttributeValue(L"ErosionsZufallsSeed");
								ErosionsZufallsSeed = atoi(tempString.c_str());
								tempString = xmlReader->getAttributeValue(L"VorgegebenenErosionsZufallsSeedNutzen");
								VorgegebenenErosionsZufallsSeedNutzen = atoi(tempString.c_str());
								tempString = xmlReader->getAttributeValue(L"SchwebendeFragmenteFiltern");
								SchwebendeFragmenteFiltern = atoi(tempString.c_str());
							}
							else if (irr::core::stringw("Erosionsschritte") == xmlReader->getNodeName())
							{
								// bis End-Tag erreicht
								while(! ((xmlReader->getNodeType() == irr::io::EXML_NODE::EXN_ELEMENT_END) && (irr::core::stringw("Erosionsschritte") == xmlReader->getNodeName()) ) )
								{
									// Lesen
									xmlReader->read();
									// Verarbeiten
									if ((xmlReader->getNodeType() == irr::io::EXML_NODE::EXN_ELEMENT) && (irr::core::stringw("Erosionsschritt") == xmlReader->getNodeName()) )
									{
										tempString = xmlReader->getAttributeValue(L"Erosionswahrscheinlichkeit");
										ErosionsSchritte.push_back(atof(tempString.c_str()));
									}
								}
							}
							else if (irr::core::stringw("Umwandeln") == xmlReader->getNodeName())
							{
								tempString = xmlReader->getAttributeValue(L"NormalenWichtungsmethode");
								DreiecksMesh->SetzeNormalenwichtungsmethode((ENormalenWichtungsmethode)atoi(tempString.c_str()));
								tempString = xmlReader->getAttributeValue(L"Detailstufen");
								DreiecksMesh_Detailstufen = atoi(tempString.c_str());
							}
							else if (irr::core::stringw("Verwacklungsparameter") == xmlReader->getNodeName())
							{
								tempString = xmlReader->getAttributeValue(L"VerticesVerwackeln");
								DreiecksMesh->SetzeOptionVerwackeln(atoi(tempString.c_str()));
								tempString = xmlReader->getAttributeValue(L"VerwacklungsStaerke");
								DreiecksMesh->SetzeVerwacklungsStaerke(atof(tempString.c_str()));
								tempString = xmlReader->getAttributeValue(L"MeshGlaetten");
								DreiecksMesh->SetzeOptionGlaetten(atoi(tempString.c_str()));
							}
							else if (irr::core::stringw("VerwacklungsZufallsparameter") == xmlReader->getNodeName())
							{
								tempString = xmlReader->getAttributeValue(L"VerwacklungsZufallsSeed");
								DreiecksMesh->SetzeZufallsSeedVerwackeln(atoi(tempString.c_str()));
								tempString = xmlReader->getAttributeValue(L"a");
								ZufallsGeneratorVerwacklung->SetzeA(atoi(tempString.c_str()));
								tempString = xmlReader->getAttributeValue(L"c");
								ZufallsGeneratorVerwacklung->SetzeC(atoi(tempString.c_str()));
								tempString = xmlReader->getAttributeValue(L"m");
								ZufallsGeneratorVerwacklung->SetzeM(atoi(tempString.c_str()));
							}
					}
				} // Ende VoxelRaum-Einstellungen lesen

				// Gaenge-Einstellungen lesen
				else if  ((irr::core::stringw("GaengeEinstellungen") == xmlReader->getNodeName()) && (Dateizugriff_Einstellungen_Gaenge) )
				{
					std::cout << "[Optionen]: Lese GaengeEinstellungen... " << std::endl;
					Architekt->QuerschnittGang_Normiert.Punkt.clear();
					Architekt->QuerschnittGang_Normiert.TextureX.clear();
					// bis End-Tag erreicht
					while(! ((xmlReader->getNodeType() == irr::io::EXML_NODE::EXN_ELEMENT_END) && (irr::core::stringw("GaengeEinstellungen") == xmlReader->getNodeName()) ) )
					{
						// Lesen
						xmlReader->read();
						// Verarbeiten
						if (xmlReader->getNodeType() == irr::io::EXML_NODE::EXN_ELEMENT)
							if (irr::core::stringw("GangParameter") == xmlReader->getNodeName())
							{
								tempString = xmlReader->getAttributeValue(L"GangBreite");
								Architekt->SetzeGangBreite(atoi(tempString.c_str()));
								tempString = xmlReader->getAttributeValue(L"GangsegmentAbstand");
								Architekt->SetzeAbstandSplineSegmente(atof(tempString.c_str()));
								tempString = xmlReader->getAttributeValue(L"TextureYproAbstand1");
								Architekt->SetzeAbstandTexturYProAbstand1(atof(tempString.c_str()));
								tempString = xmlReader->getAttributeValue(L"AnzahlDetailstufen");
								Architekt->SetzeAnzahlSplineDetailstufen(atoi(tempString.c_str()));
								tempString = xmlReader->getAttributeValue(L"GangTextur");
								Szene->SetzeGangTexturPfad(tempString);
							}
							else if (irr::core::stringw("GangQuerschnitt") == xmlReader->getNodeName())
							{
								// Speicher pre-allokieren
								irr::core::vector2d<double> tempPunkt;

								// bis End-Tag erreicht
								while(! ((xmlReader->getNodeType() == irr::io::EXML_NODE::EXN_ELEMENT_END) && (irr::core::stringw("GangQuerschnitt") == xmlReader->getNodeName()) ) )
								{
									// Lesen
									xmlReader->read();
									// Verarbeiten
									if ((xmlReader->getNodeType() == irr::io::EXML_NODE::EXN_ELEMENT) && (irr::core::stringw("QuerschnittsPunkt") == xmlReader->getNodeName()) )
									{
										// neuen Punkt hinzufuegen
										tempString = xmlReader->getAttributeValue(L"PositionX");
										tempPunkt.X = atof(tempString.c_str());
										tempString = xmlReader->getAttributeValue(L"PositionY");
										tempPunkt.Y = atof(tempString.c_str());
										Architekt->QuerschnittGang_Normiert.Punkt.push_back(tempPunkt);

										tempString = xmlReader->getAttributeValue(L"TexturkoordinateX");
										Architekt->QuerschnittGang_Normiert.TextureX.push_back(atof(tempString.c_str()));
									}
								}							
							}
					}
				}
				// Ende Gaenge-Einstellungen lesen

				// Detailobjekte-Einstellungen lesen
				else if  ((irr::core::stringw("DetailobjekteEinstellungen") == xmlReader->getNodeName()) && (Dateizugriff_Einstellungen_Detailobjekte) )
				{
					std::cout << "[Optionen]: Lese DetailobjekteEinstellungen... " << std::endl;
					for (unsigned int i = 0; i < Architekt->DetailobjektVorlagen.size(); i++)
						delete Architekt->DetailobjektVorlagen.at(i);
					Architekt->DetailobjektVorlagen.clear();
					// bis End-Tag erreicht
					while(! ((xmlReader->getNodeType() == irr::io::EXML_NODE::EXN_ELEMENT_END) && (irr::core::stringw("DetailobjekteEinstellungen") == xmlReader->getNodeName()) ) )
					{
						// Lesen
						xmlReader->read();
						// Verarbeiten
						if (xmlReader->getNodeType() == irr::io::EXML_NODE::EXN_ELEMENT)
							if (irr::core::stringw("Detailobjekt") == xmlReader->getNodeName())
							{
								// neues Detailobjekt erzeugen und Daten transferieren
								SSplineDetailobjektVorlage* neuesDetailobjekt = new SSplineDetailobjektVorlage();

								tempString = xmlReader->getAttributeValue(L"Benennung");
								neuesDetailobjekt->Name = tempString;
								tempString = xmlReader->getAttributeValue(L"MeshDateipfad");
								neuesDetailobjekt->MeshPfad = tempString;

								// bis End-Tag erreicht
								while(! ((xmlReader->getNodeType() == irr::io::EXML_NODE::EXN_ELEMENT_END) && (irr::core::stringw("Detailobjekt") == xmlReader->getNodeName()) ) )
								{
									// Lesen
									xmlReader->read();
									// Verarbeiten
									if ((xmlReader->getNodeType() == irr::io::EXML_NODE::EXN_ELEMENT) && (irr::core::stringw("Skalierung") == xmlReader->getNodeName()) )
									{
										tempString = xmlReader->getAttributeValue(L"X");
										neuesDetailobjekt->Skalierung.X = atof(tempString.c_str());
										tempString = xmlReader->getAttributeValue(L"Y");
										neuesDetailobjekt->Skalierung.Y = atof(tempString.c_str());
										tempString = xmlReader->getAttributeValue(L"Z");
										neuesDetailobjekt->Skalierung.Z = atof(tempString.c_str());
									}
									else if ((xmlReader->getNodeType() == irr::io::EXML_NODE::EXN_ELEMENT) && (irr::core::stringw("Rotation") == xmlReader->getNodeName()) )
									{
										tempString = xmlReader->getAttributeValue(L"X");
										neuesDetailobjekt->Rotation.X = atof(tempString.c_str());
										tempString = xmlReader->getAttributeValue(L"Y");
										neuesDetailobjekt->Rotation.Y = atof(tempString.c_str());
										tempString = xmlReader->getAttributeValue(L"Z");
										neuesDetailobjekt->Rotation.Z = atof(tempString.c_str());
									}
									else if ((xmlReader->getNodeType() == irr::io::EXML_NODE::EXN_ELEMENT) && (irr::core::stringw("Position") == xmlReader->getNodeName()) )
									{
										tempString = xmlReader->getAttributeValue(L"X");
										neuesDetailobjekt->Position.X = atof(tempString.c_str());
										tempString = xmlReader->getAttributeValue(L"Y");
										neuesDetailobjekt->Position.Y = atof(tempString.c_str());
									}
									else if ((xmlReader->getNodeType() == irr::io::EXML_NODE::EXN_ELEMENT) && (irr::core::stringw("NumAbstand") == xmlReader->getNodeName()) )
									{
										tempString = xmlReader->getAttributeValue(L"Faktor");
										neuesDetailobjekt->AbstandNumFaktor = atoi(tempString.c_str());
										tempString = xmlReader->getAttributeValue(L"Min");
										neuesDetailobjekt->AbstandNumMin = atoi(tempString.c_str());
										tempString = xmlReader->getAttributeValue(L"Max");
										neuesDetailobjekt->AbstandNumMax = atoi(tempString.c_str());
										tempString = xmlReader->getAttributeValue(L"MinErstesElement");
										neuesDetailobjekt->AbstandNumMinErstesElement = atoi(tempString.c_str());
										tempString = xmlReader->getAttributeValue(L"MaxErstesElement");
										neuesDetailobjekt->AbstandNumMaxErstesElement = atoi(tempString.c_str());
									}
									else if ((xmlReader->getNodeType() == irr::io::EXML_NODE::EXN_ELEMENT) && (irr::core::stringw("Weiteres") == xmlReader->getNodeName()) )
									{
										tempString = xmlReader->getAttributeValue(L"Abtastung");
										neuesDetailobjekt->AbstandsAbtastung = atof(tempString.c_str());
										tempString = xmlReader->getAttributeValue(L"ObjektAn1");
										neuesDetailobjekt->ObjektAn1 = atoi(tempString.c_str());
									}
								}

								// neues Detailobjekt speichern
								Architekt->DetailobjektVorlagen.push_back(neuesDetailobjekt);
							}							
					}
				}
				// Ende Detailobjekte-Einstellungen lesen

				// Architekt-Einstellungen lesen
				else if  ((irr::core::stringw("ArchitektEinstellungen") == xmlReader->getNodeName()) && (Dateizugriff_Einstellungen_Architekt) )
				{
					std::cout << "[Optionen]: Lese ArchitektEinstellungen... " << std::endl;
					// bis End-Tag erreicht
					while(! ((xmlReader->getNodeType() == irr::io::EXML_NODE::EXN_ELEMENT_END) && (irr::core::stringw("ArchitektEinstellungen") == xmlReader->getNodeName()) ) )
					{
						// Lesen
						xmlReader->read();
						// Verarbeiten
						if (xmlReader->getNodeType() == irr::io::EXML_NODE::EXN_ELEMENT)
							if (irr::core::stringw("Generelles") == xmlReader->getNodeName())
							{
								tempString = xmlReader->getAttributeValue(L"KontruktionsZufallsSeed");
								KonstruktionsZufallsSeed = atoi(tempString.c_str());
								tempString = xmlReader->getAttributeValue(L"VorgegebenenKontruktionsZufallsSeedNutzen");
								VorgegebenenKonstruktionsZufallsSeedNutzen = atoi(tempString.c_str());
								tempString = xmlReader->getAttributeValue(L"RaumsektorAusdehnung");
								Raumsektorausdehnung = atoi(tempString.c_str());
							}
							else if (irr::core::stringw("AndockEigenschaften") == xmlReader->getNodeName())
							{
								tempString = xmlReader->getAttributeValue(L"MinAbstandRaumHoehle");
								Architekt->SetzeAbstandAndockHoehleMin(atoi(tempString.c_str()));
								tempString = xmlReader->getAttributeValue(L"AbstandBoden");
								Architekt->SetzeAbstandVomBodenMax(atoi(tempString.c_str()));
								tempString = xmlReader->getAttributeValue(L"AbstandstestTiefe");
								Architekt->SetzeTiefeAbstandsTestMin(atoi(tempString.c_str()));
								tempString = xmlReader->getAttributeValue(L"AusfraesTiefe");
								Architekt->SetzeZusaetzlicheAusfraestiefe(atoi(tempString.c_str()));
								tempString = xmlReader->getAttributeValue(L"KollisionstestZusatzSicherheitsabstand");
								Architekt->SetzeZusaetzlicherSicherheitsabstand(atof(tempString.c_str()));
							}
							else if (irr::core::stringw("Wahrscheinlichkeiten") == xmlReader->getNodeName())
							{
								tempString = xmlReader->getAttributeValue(L"WahrscheinlichkeitZusatzgangRaumHoehle");
								Architekt->SetzeWahrscheinlichkeitZusaetzlicherGangRaumHoehle(atof(tempString.c_str()));
								tempString = xmlReader->getAttributeValue(L"WahrscheinlichkeitZusatzgangRaumRaum");
								Architekt->SetzeWahrscheinlichkeitZusaetzlicherGangRaumRaum(atof(tempString.c_str()));
								tempString = xmlReader->getAttributeValue(L"WahrscheinlichkeitsfaktorLeererRaumAnHoehle");
								Architekt->SetzePlatzierungsWahrscheinlichkeitsFaktorAnHoehleLeereSubSzene(atoi(tempString.c_str()));
								tempString = xmlReader->getAttributeValue(L"WahrscheinlichkeitsfaktorLeererRaumAnRaum");
								Architekt->SetzePlatzierungsWahrscheinlichkeitsFaktorAnRaumLeereSubSzene(atoi(tempString.c_str()));
							}
					}
				}
				// Ende Architekt-Einstellungen lesen

				// Subszenen-Einstellungen lesen
				else if  ((irr::core::stringw("SubszenenEinstellungen") == xmlReader->getNodeName()) && (Dateizugriff_Einstellungen_Subszenen) )
				{
					std::cout << "[Optionen]: Lese SubszenenEinstellungen... " << std::endl;
					for (unsigned int i = 0; i < Architekt->SubSzenen.size(); i++)
						delete Architekt->SubSzenen.at(i);
					Architekt->SubSzenen.clear();
					irr::core::vector3d<double> tempVec3D;

					// bis End-Tag erreicht
					while(! ((xmlReader->getNodeType() == irr::io::EXML_NODE::EXN_ELEMENT_END) && (irr::core::stringw("SubszenenEinstellungen") == xmlReader->getNodeName()) ) )
					{
						// Lesen
						xmlReader->read();
						// Verarbeiten
						if (xmlReader->getNodeType() == irr::io::EXML_NODE::EXN_ELEMENT)
							if (irr::core::stringw("Subszene") == xmlReader->getNodeName())
							{																
								// neue Subszene erzeugen und Daten transferieren
								tempString = xmlReader->getAttributeValue(L"Dateipfad");
								CSubSzene* neueSubszene = new CSubSzene(IrrDevice,tempString);
								
								tempString = xmlReader->getAttributeValue(L"Benennung");
								neueSubszene->SetzeName(tempString);	

								tempString = xmlReader->getAttributeValue(L"WahrscheinlichkeitsfaktorAnHoehle");
								neueSubszene->SetzePlatzierungsWahrscheinlichkeitsFaktorAnHoehle(atof(tempString.c_str()));
								tempString = xmlReader->getAttributeValue(L"WahrscheinlichkeitsfaktorAnRaum");
								neueSubszene->SetzePlatzierungsWahrscheinlichkeitsFaktorAnRaum(atof(tempString.c_str()));

								// bis End-Tag erreicht
								while(! ((xmlReader->getNodeType() == irr::io::EXML_NODE::EXN_ELEMENT_END) && (irr::core::stringw("Subszene") == xmlReader->getNodeName()) ) )
								{
									// Lesen
									xmlReader->read();
									// Verarbeiten
									if ((xmlReader->getNodeType() == irr::io::EXML_NODE::EXN_ELEMENT) && (irr::core::stringw("Skalierung") == xmlReader->getNodeName()) )
									{
										tempString = xmlReader->getAttributeValue(L"X");
										tempVec3D.X = atof(tempString.c_str());
										tempString = xmlReader->getAttributeValue(L"Y");
										tempVec3D.Y = atof(tempString.c_str());
										tempString = xmlReader->getAttributeValue(L"Z");
										tempVec3D.Z = atof(tempString.c_str());

										neueSubszene->SetzeSkalierung(tempVec3D);
									}
									else if ((xmlReader->getNodeType() == irr::io::EXML_NODE::EXN_ELEMENT) && (irr::core::stringw("SplineAbleitungsFaktoren") == xmlReader->getNodeName()) )
									{
										tempString = xmlReader->getAttributeValue(L"RaumZuRaum");
										neueSubszene->SetzeAbleitungsFaktorRaumRaum(atof(tempString.c_str()));
										tempString = xmlReader->getAttributeValue(L"RaumZuHoehle");
										neueSubszene->SetzeAbleitungsFaktorRaumHoehle(atof(tempString.c_str()));
										tempString = xmlReader->getAttributeValue(L"HoehleZuRaum");
										neueSubszene->SetzeAbleitungsFaktorHoehleRaum(atof(tempString.c_str()));
									}								
								}

								// neues Detailobjekt speichern
								Architekt->SubSzenen.push_back(neueSubszene);
							}
					}
				}
				// Ende Subszenen-Einstellungen lesen				
				
				// Erstellung-Einstellungen lesen
				else if  ((irr::core::stringw("ErstellungEinstellungen") == xmlReader->getNodeName()) && (Dateizugriff_Einstellungen_Erstellung) )
				{
					std::cout << "[Optionen]: Lese ErstellungEinstellungen... " << std::endl;
					// bis End-Tag erreicht
					while(! ((xmlReader->getNodeType() == irr::io::EXML_NODE::EXN_ELEMENT_END) && (irr::core::stringw("ErstellungEinstellungen") == xmlReader->getNodeName()) ) )
					{
						// Lesen
						xmlReader->read();
						// Verarbeiten
						if (xmlReader->getNodeType() == irr::io::EXML_NODE::EXN_ELEMENT)
							if (irr::core::stringw("Fraktalzeichenanweisung") == xmlReader->getNodeName())
							{
								tempString = xmlReader->getAttributeValue(L"aktiviert");
								Erstellung_Fraktalzeichenanweisung = atoi(tempString.c_str());
							}
							else if (irr::core::stringw("VoxelZeichnen_Erosion") == xmlReader->getNodeName())
							{
								tempString = xmlReader->getAttributeValue(L"aktiviert");
								Erstellung_VoxelZeichnen_Erosion = atoi(tempString.c_str());
							}
							else if (irr::core::stringw("Filterung1") == xmlReader->getNodeName())
							{
								tempString = xmlReader->getAttributeValue(L"aktiviert");
								Erstellung_Filterung1 = atoi(tempString.c_str());
							}
							else if (irr::core::stringw("Architekt") == xmlReader->getNodeName())
							{
								tempString = xmlReader->getAttributeValue(L"aktiviert");
								Erstellung_Architekt = atoi(tempString.c_str());
							}
							else if (irr::core::stringw("Filterung2") == xmlReader->getNodeName())
							{
								tempString = xmlReader->getAttributeValue(L"aktiviert");
								Erstellung_Filterung2 = atoi(tempString.c_str());
							}
							else if (irr::core::stringw("UmwandlungVoxel") == xmlReader->getNodeName())
							{
								tempString = xmlReader->getAttributeValue(L"aktiviert");
								Erstellung_UmwandlungVoxel = atoi(tempString.c_str());
							}
					}

				}
				// Ende Erstellung-Einstellungen lesen
				// Weiteres-Einstellungen lesen
				else if  ((irr::core::stringw("WeiteresEinstellungen") == xmlReader->getNodeName()) && (Dateizugriff_Einstellungen_Weiteres) )
				{
					std::cout << "[Optionen]: Lese WeiteresEinstellungen... " << std::endl;
					// bis End-Tag erreicht
					while(! ((xmlReader->getNodeType() == irr::io::EXML_NODE::EXN_ELEMENT_END) && (irr::core::stringw("WeiteresEinstellungen") == xmlReader->getNodeName()) ) )
					{
						// Lesen
						xmlReader->read();
						// Verarbeiten
						if (xmlReader->getNodeType() == irr::io::EXML_NODE::EXN_ELEMENT)
							if (irr::core::stringw("ZufallsparameterAllgemein") == xmlReader->getNodeName())
							{
								tempString = xmlReader->getAttributeValue(L"a");
								ZufallsGeneratorAllgemein->SetzeA(atoi(tempString.c_str()));
								tempString = xmlReader->getAttributeValue(L"c");
								ZufallsGeneratorAllgemein->SetzeC(atoi(tempString.c_str()));
								tempString = xmlReader->getAttributeValue(L"m");
								ZufallsGeneratorAllgemein->SetzeM(atoi(tempString.c_str()));
							}
							else if (irr::core::stringw("Hoehlenfarbe") == xmlReader->getNodeName())
							{
								irr::video::SColorf farbeHoehle;
								farbeHoehle.a = 1.0;

								tempString = xmlReader->getAttributeValue(L"Rot");
								farbeHoehle.r = atof(tempString.c_str());
								tempString = xmlReader->getAttributeValue(L"Gruen");
								farbeHoehle.g = atof(tempString.c_str());
								tempString = xmlReader->getAttributeValue(L"Blau");
								farbeHoehle.b = atof(tempString.c_str());

								Szene->LeseCallbackHoehlenShader()->SetzeFarbe(farbeHoehle);
							}
							else if (irr::core::stringw("Hintergrundfarbe") == xmlReader->getNodeName())
							{
								tempString = xmlReader->getAttributeValue(L"Rot");
								Hintergrundfarbe.setRed(atoi(tempString.c_str()));
								tempString = xmlReader->getAttributeValue(L"Gruen");
								Hintergrundfarbe.setGreen(atoi(tempString.c_str()));
								tempString = xmlReader->getAttributeValue(L"Blau");
								Hintergrundfarbe.setBlue(atoi(tempString.c_str()));
							}
					}

				}
				// Ende Weiteres-Einstellungen lesen
			}

		} // Parsen Ende
		
		// Datei schliessen
		xmlReader->drop();
		datei->drop();
		std::cout << "[Optionen]: Lesen abgeschlossen." << std::endl;

	}
	else
		std::cout << "Fehler beim Zugriff auf die Datei. Einstellungen wurden nicht geladen." << std::endl;
}
