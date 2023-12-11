// zur Verhinderung doppelter Definition
#ifndef OPTIONEN_H
#define OPTIONEN_H

#include <vector>
#include <string>
#include <iostream>

#include "GUI.h"
#include "Szene.h"
#include "FraktalGenerator.h"
#include "VoxelRaum.h"
#include "DreiecksMesh.h"
#include "Architekt.h"
#include "Zeitmesser.h"

#include "COBJMeshWriter_Modified.h"

// Namespace DunGen : DungeonGenerator
namespace DunGen
{
	// Vorabdeklaration
	class CGUI;

	// Klasse Optionen
	class COptionen
	{
	public:
		void Initialisiere(irr::IrrlichtDevice* _IrrDevice, CGUI* _GUI, CSzene* _Szene,
			CFraktalGenerator* _FraktalGenerator, CVoxelRaum* _VoxelRaum, CDreiecksMesh* _DreiecksMesh, CArchitekt* _Architekt,
			CZufallsGenerator* _ZufallsGeneratorAllgemein, CZufallsGenerator* _ZufallsGeneratorVerwacklung, CZeitmesser* _Zeitmesser);

		// Werte auslesen
		irr::video::SColor LeseHintergrundfarbe();

		// Alle Fenster aktualisieren
		void AktualisiereFenster();	
		// Fensterinhalt aller Fenster merken
		void MerkeFenster();
		// GUI ein/aus
		void GUIumschalten();
		
		// Arbeitsverzeichnis wieder auf den Ausgangswert setzen
		void Arbeitsverzeichnis_wiederherstellen();
		
		// Buttons Fraktaleditor
		void Button_ZufallsParameter();
		void Button_ZufallsStart();
		void Button_ZufallsRegel();
		void Button_RegelHinzufuegen();
		void Button_RegelnLoeschen();
		void Button_FraktalGenerieren();
		void Button_ZufallsFraktalGenerieren();

		// Buttons Voxelraum
		void Button_Erosionsschritt_hinzufuegen();
		void Button_Erosionsschritte_loeschen();
		void Button_Voxel_berechnen();
		void Button_Zeige_Dichte(EEbene _Ebene);
		void Button_Voxel_Zu_Dreiecksnetz_Berechnen();
		void Button_Zeige_Drahtgittermodell();

		// Buttons Gaenge
		void Button_Neu_Gangpunkt();
		void Button_Uebernehmen_Gangpunkt();
		void Button_Loeschen_Gangpunkt();
		void Button_Uebernehmen_Detailobjekt();
		void Button_Loeschen_Detailobjekt();

		// Buttons Architekt
		void Button_Uebernehmen_Subszene();
		void Button_Loeschen_Subszene();

		// Buttons Erstellung
		void Button_Erstelle_Dungeon();
		void Button_Zeige_Dungeon();

		// Buttons Weiteres
		void Button_Farbe_Hoehle_uebernehmen();
		void Button_Farbe_Hintergrund_uebernehmen();
		void Button_Meshcache_Texturebuffer_loeschen();

		// Laden und Speichern
		void Speichere_Einstellungen(const irr::io::path& _filename);
		void Lade_Einstellungen(const irr::io::path& _filename);

		void Speichere_Ergebnisse_Fraktalstrings(const irr::io::path& _filename);
		void Speichere_Ergebnisse_Dichtekarte(EEbene _Ebene, const irr::io::path& _filename);
		void Speichere_Ergebnisse_Dreicksmesh_Hoehle(const irr::io::path& _filename);
		void Speichere_Ergebnisse_Dungeon(const irr::io::path& _filename);
		void Lade_Ergebnisse_Dungeon(const irr::io::path& _filename);

		// Diverse weitere Dateizugriffe
		void Dateizugriff_Gangtextur_Auswahl(const irr::io::path& _filename);
		void Dateizugriff_Detailobjekt_Auswahl(const irr::io::path& _filename);
		void Dateizugriff_Subszene_Auswahl(const irr::io::path& _filename);

		// Screenshot anfertigen
		void ScreenshotAnfertigen();

	private:
		// Fensterinhalt aktualisieren
		void AktualisiereFenster_Dateizugriff_Einstellungen();
		void AktualisiereFenster_FraktalEditor();
		void AktualisiereFenster_VoxelRaum();
		void AktualisiereFenster_Gaenge();
		void AktualisiereFenster_Architekt();
		void AktualisiereFenster_Erstellung();
		void AktualisiereFenster_Weiteres();
		// Fensterinhalt merken
		void MerkeFenster_Dateizugriff_Einstellungen();
		void MerkeFenster_FraktalEditor();
		void MerkeFenster_VoxelRaum();
		void MerkeFenster_Gaenge();
		void MerkeFenster_Architekt();
		void MerkeFenster_Erstellung();
		void MerkeFenster_Weiteres();
		// GUI ausblenden
		void GUIaus();
		// GUI einblenden
		void GUIan();

		// Diverse weitere Dateizugriffe
		// Zusatzinformationen zum Dunegon speichern
		void Speichere_Ergebnisse_Zusatzinformationen(const irr::io::path& _filename);

		// Irrlicht-Objekte
		irr::IrrlichtDevice* IrrDevice;
		irr::io::IFileSystem* FileSystem;
		irr::video::IVideoDriver* VideoDriver;
		irr::scene::ISceneManager* SceneManager;
		// Arbeitsverzeichnis
		irr::io::path ArbeitsVerzeichnis;

		// Zeiger auf andere Objekte
		CGUI* GUI;
		CSzene* Szene;
		CFraktalGenerator* FraktalGenerator;
		CVoxelRaum* VoxelRaum;
		CDreiecksMesh* DreiecksMesh;
		CArchitekt* Architekt;
		// Zeitmessung
		CZeitmesser* Zeitmesser;
		// Zufallsgeneratoren
		CZufallsGenerator* ZufallsGeneratorAllgemein;
		CZufallsGenerator* ZufallsGeneratorVerwacklung;

		// Ergebnisse
		SLSystemAbleitungen* Fraktal;

		// Ist die GUI eingeblendet oder ausgeblendet?
		bool GUIaktiv; 

		// in dieser Klasse vermerkte Optionen -> andere Optionen sind direkt in den verantwortlichen Klassen gespeichert:
				
		// Dateizugriff Einstellungen
		bool Dateizugriff_Einstellungen_Fraktalparameter;
		bool Dateizugriff_Einstellungen_Fraktaleinstellungen_Weitere;
		bool Dateizugriff_Einstellungen_Voxelraum;
		bool Dateizugriff_Einstellungen_Gaenge;
		bool Dateizugriff_Einstellungen_Detailobjekte;
		bool Dateizugriff_Einstellungen_Architekt;
		bool Dateizugriff_Einstellungen_Subszenen;
		bool Dateizugriff_Einstellungen_Erstellung;
		bool Dateizugriff_Einstellungen_Weiteres;

		// ID des ausgewaehlten Listbox-Feldes (-1 ... keines)
		int ListBox_Ersetzungsregeln_ID;
		int ListBox_Erosionschritte_ID;		
		int ListBox_Gangpunkte_ID;			
		int ListBox_Detailobjekte_ID;
		int ListBox_Subszenen_ID;

		// FraktalEditor
		double WinkelGier_Min, WinkelGier_Max;
		int WinkelGier_Kommastellen;
		double WinkelNick_Min, WinkelNick_Max;
		int WinkelNick_Kommastellen;
		double WinkelRoll_Min, WinkelRoll_Max;
		int WinkelRoll_Kommastellen;

		double StartRadius_Min, StartRadius_Max;
		int StartRadius_Kommastellen;
		double RadiusFaktor_Min, RadiusFaktor_Max;
		int RadiusFaktor_Kommastellen;
		double RadiusDekrementor_Min, RadiusDekrementor_Max;
		int RadiusDekrementor_Kommastellen;

		std::string Start_ErlaubteSymbole;
		unsigned int Start_MinLaenge, Start_MaxLaenge;

		char Symbol;
		std::string Symbolersetzung;
		std::string Ersetzung_ErlaubteSymbole;
		unsigned int Ersetzung_MinLaenge, Ersetzung_MaxLaenge;

		unsigned int Iterationsstufe;

		// VoxelRaum
		double Erosionswahrscheinlichkeit;
		std::vector<double> ErosionsSchritte;
		bool SchwebendeFragmenteFiltern;
		unsigned int ErosionsZufallsSeed;
		bool VorgegebenenErosionsZufallsSeedNutzen;
		unsigned int DreiecksMesh_Detailstufen;
		// Flag, ob Ergebnis schon vorliegt
		bool Dichte_berechnet;

		// Architekt
		unsigned int KonstruktionsZufallsSeed;
		unsigned int Raumsektorausdehnung;
		bool VorgegebenenKonstruktionsZufallsSeedNutzen;		

		// Erstellung
		bool Erstellung_Fraktalzeichenanweisung;
		bool Erstellung_VoxelZeichnen_Erosion;
		bool Erstellung_Filterung1;
		bool Erstellung_Architekt;
		bool Erstellung_Filterung2;
		bool Erstellung_UmwandlungVoxel;

		// Weiteres
		irr::video::SColor Hintergrundfarbe;
	};
	
} // END NAMESPACE DunGen

#endif