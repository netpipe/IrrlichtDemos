// Implementierung der Klasse EventReceiver
#include "EventReceiver.h"

// Initialisierung
DunGen::CEventReceiver::CEventReceiver()
{
	Initialisiert = false;
}
void DunGen::CEventReceiver::Initialisiere(irr::gui::IGUIEnvironment* _GuiEnvironment, CGUI* _GUI, COptionen* _Optionen, CSzene* _Szene)
{
	GuiEnvironment = _GuiEnvironment;
	GUI = _GUI;
	Optionen = _Optionen;
	Szene = _Szene;
	Initialisiert = true;
}

// Eventbearbeitung
bool DunGen::CEventReceiver::OnEvent(const irr::SEvent& _Event)
{
	// andere Objekte schon zugewiesen?
	if (!Initialisiert)
		return false;

	// GUI-Event
	//if ((_Event.EventType == irr::EET_GUI_EVENT) && Szene->GUIan())
	if (_Event.EventType == irr::EET_GUI_EVENT)
	{		
		// ID des Elements bestimmen
		irr::s32 id = _Event.GUIEvent.Caller->getID();
		// Aktualisierungs-Flag
		bool aktualisieren = true;

		// Typ des Elements
		switch(_Event.GUIEvent.EventType)
		{
			// Fenster schliessen
			case irr::gui::EGDT_WINDOW_CLOSE:
				// Fensterinhalte auslesen (Daten des zu schliessenden Festers merken, Daten fuer alle anderen Fenster zwecks Eingabefilterung ebenfalls)
				Optionen->MerkeFenster();
				// Fensterinhalte aktualisieren (fuer alle anderen Fenster interessant, da hiermit alle Eingaben gefiltert werden)
				Optionen->AktualisiereFenster();
				break;

			// Button
			case irr::gui::EGET_BUTTON_CLICKED:
				// Fensterinhalte auslesen
				Optionen->MerkeFenster();
				// Arbeitsverzeichnis wieder auf Ausgang (Standard) setzen
				Optionen->Arbeitsverzeichnis_wiederherstellen();

				switch(id)
				{
					// Toolbar-Buttons
					case _GUI_ID_BUTTON_DATEIZUGRIFF_EINSTELLUNGEN_:
						GUI->ErstelleFenster_Dateizugriff_Einstellungen();
						break;
					case _GUI_ID_BUTTON_DATEIZUGRIFF_ERGEBNISSE_:
						GUI->ErstelleFenster_Dateizugriff_Ergebnisse();
						break;
					case _GUI_ID_BUTTON_FRAKTALEDITOR_:
						GUI->ErstelleFenster_FraktalEditor();
						break;
					case _GUI_ID_BUTTON_VOXELHOEHLE_:
						GUI->ErstelleFenster_VoxelHoehle();
						break;
					case _GUI_ID_BUTTON_GAENGE_:
						GUI->ErstelleFenster_Gaenge();
						break;
					case _GUI_ID_BUTTON_ARCHITEKT_:
						GUI->ErstelleFenster_Architekt();
						break;
					case _GUI_ID_BUTTON_ERSTELLUNG_:
						GUI->ErstelleFenster_Erstellung();
						break;
					case _GUI_ID_BUTTON_WEITERES_:
						GUI->ErstelleFenster_Weiteres();
						break;

					// Einstellungen-Laden/Speichern-Buttons
					case _GUI_ID_BUTTON_SPEICHERN_EINSTELLUNGEN_:
						GUI->ErstelleFenster_FileOpenDialog(_GUI_ID_FILEOPENDIALOG_SPEICHERN_EINSTELLUNGEN_,true);
						break;
					case _GUI_ID_BUTTON_LADEN_EINSTELLUNGEN_:
						GUI->ErstelleFenster_FileOpenDialog(_GUI_ID_FILEOPENDIALOG_LADEN_EINSTELLUNGEN_,false);
						break;

					// Ergebnisse-Laden/Speichern-Buttons
					case _GUI_ID_BUTTON_SPEICHERN_ERGEBNISSE_FRAKTALSTRINGS_:
						GUI->ErstelleFenster_FileOpenDialog(_GUI_ID_FILEOPENDIALOG_SPEICHERN_ERGEBNISSE_FRAKTALSTRINGS_,true);
						break;
					case _GUI_ID_BUTTON_SPEICHERN_ERGEBNISSE_DICHTE_XY_:
						GUI->ErstelleFenster_FileOpenDialog(_GUI_ID_FILEOPENDIALOG_SPEICHERN_ERGEBNISSE_DICHTE_XY_,true);
						break;
					case _GUI_ID_BUTTON_SPEICHERN_ERGEBNISSE_DICHTE_XZ_:
						GUI->ErstelleFenster_FileOpenDialog(_GUI_ID_FILEOPENDIALOG_SPEICHERN_ERGEBNISSE_DICHTE_XZ_,true);
						break;
					case _GUI_ID_BUTTON_SPEICHERN_ERGEBNISSE_DICHTE_YZ_:
						GUI->ErstelleFenster_FileOpenDialog(_GUI_ID_FILEOPENDIALOG_SPEICHERN_ERGEBNISSE_DICHTE_YZ_,true);
						break;
					case _GUI_ID_BUTTON_SPEICHERN_ERGEBNISSE_DREIECKSMESH_:
						GUI->ErstelleFenster_FileOpenDialog(_GUI_ID_FILEOPENDIALOG_SPEICHERN_ERGEBNISSE_DREIECKSMESH_HOEHLE_,true);
						break;
					case _GUI_ID_BUTTON_SPEICHERN_ERGEBNISSE_DUNGEON_:
						GUI->ErstelleFenster_FileOpenDialog(_GUI_ID_FILEOPENDIALOG_SPEICHERN_ERGEBNISSE_DUNGEON_,true);
						break;
					case _GUI_ID_BUTTON_LADEN_ERGEBNISSE_DUNGEON_:
						GUI->ErstelleFenster_FileOpenDialog(_GUI_ID_FILEOPENDIALOG_LADEN_ERGEBNISSE_DUNGEON_,true);
						break;

					// FraktalEditor-Buttons
					case _GUI_ID_BUTTON_ZUFALLSPARAMETER_:
						Optionen->Button_ZufallsParameter();
						break;
					case _GUI_ID_BUTTON_ZUFALLSSTART_:
						Optionen->Button_ZufallsStart();
						break;
					case _GUI_ID_BUTTON_ZUFALLSREGEL_:
						Optionen->Button_ZufallsRegel();
						break;
					case _GUI_ID_BUTTON_REGELHINZUFUEGEN_:
						Optionen->Button_RegelHinzufuegen();
						break;
					case _GUI_ID_BUTTON_REGELNLOESCHEN_:
						Optionen->Button_RegelnLoeschen();
						break;
					case _GUI_ID_BUTTON_FRAKTALGENERIEREN_:
						Optionen->Button_FraktalGenerieren();
						break;
					case _GUI_ID_BUTTON_ZUFALLSFRAKTALGENERIEREN_:
						Optionen->Button_ZufallsFraktalGenerieren();
						break;						

					// VoxelHoehle-Buttons
					case _GUI_ID_BUTTON_EROSIONSSCHRITT_HINZUFUEGEN_:
						Optionen->Button_Erosionsschritt_hinzufuegen();
						break;
					case _GUI_ID_BUTTON_EROSIONSSCHRITTE_LOESCHEN_:
						Optionen->Button_Erosionsschritte_loeschen();
						break;
					case _GUI_ID_BUTTON_VOXEL_BERECHNEN_:
						Optionen->Button_Voxel_berechnen();
						break;
					case _GUI_ID_BUTTON_ZEIGE_DICHTE_XY_:
						Optionen->Button_Zeige_Dichte(EEbene::_EBENE_XY_);
						break;
					case _GUI_ID_BUTTON_ZEIGE_DICHTE_XZ_:
						Optionen->Button_Zeige_Dichte(EEbene::_EBENE_XZ_);
						break;
					case _GUI_ID_BUTTON_ZEIGE_DICHTE_YZ_:
						Optionen->Button_Zeige_Dichte(EEbene::_EBENE_YZ_);
						break;
					case _GUI_ID_BUTTON_VOXEL_ZU_DREIECKSNETZ_BERECHNEN_:
						Optionen->Button_Voxel_Zu_Dreiecksnetz_Berechnen();
						break;
					case _GUI_ID_BUTTON_ZEIGE_DRAHTGITTERMODELL_:
						Optionen->Button_Zeige_Drahtgittermodell();
						break;

					// Gaenge-Buttons
					case _GUI_ID_BUTTON_DATEI_WAEHLEN_GANGTEXTUR_:
						GUI->ErstelleFenster_FileOpenDialog(_GUI_ID_FILEOPENDIALOG_GANGTEXTUR_,false);
						break;
					case _GUI_ID_BUTTON_NEU_GANGPUNKT_:
						Optionen->Button_Neu_Gangpunkt();
						break;
					case _GUI_ID_BUTTON_UEBERNEHMEN_GANGPUNKT_:
						Optionen->Button_Uebernehmen_Gangpunkt();
						break;
					case _GUI_ID_BUTTON_LOESCHEN_GANGPUNKT_:
						Optionen->Button_Loeschen_Gangpunkt();
						break;
					case _GUI_ID_BUTTON_NEU_DETAILOBJEKT_:
						GUI->ErstelleFenster_FileOpenDialog(_GUI_ID_FILEOPENDIALOG_DETAILOBJEKTMESH_,false);
						aktualisieren = false; // erst nach Auswahl der Datei aktualisieren
						break;
					case _GUI_ID_BUTTON_UEBERNEHMEN_DETAILOBJEKT_:
						Optionen->Button_Uebernehmen_Detailobjekt();
						break;
					case _GUI_ID_BUTTON_LOESCHEN_DETAILOBJEKT_:
						Optionen->Button_Loeschen_Detailobjekt();
						break;

					// Architekt-Buttons
					case _GUI_ID_BUTTON_NEU_SUBSZENE_:
						GUI->ErstelleFenster_FileOpenDialog(_GUI_ID_FILEOPENDIALOG_SUBSZENE_,false);
						aktualisieren = false; // erst nach Auswahl der Datei aktualisieren
						break;
					case _GUI_ID_BUTTON_UEBERNEHMEN_SUBSZENE_:
						Optionen->Button_Uebernehmen_Subszene();
						break;
					case _GUI_ID_BUTTON_LOESCHEN_SUBSZENE_:
						Optionen->Button_Loeschen_Subszene();
						break;

					// Erstellung-Buttons
					case _GUI_ID_BUTTON_ERSTELLE_DUNGEON_:
						Optionen->Button_Erstelle_Dungeon();
						break;

					case _GUI_ID_BUTTON_ZEIGE_DUNGEON_:
						Optionen->Button_Zeige_Dungeon();
						break;

					// Weiteres-Buttons
					case _GUI_ID_BUTTON_FARBE_HOEHLE_UEBERNEHMEN_:
						Optionen->Button_Farbe_Hoehle_uebernehmen();
						break;
					case _GUI_ID_BUTTON_FARBE_HINTERGRUND_UEBERNEHMEN_:
						Optionen->Button_Farbe_Hintergrund_uebernehmen();
						break;
					case _GUI_ID_BUTTON_MESHCACHE_TEXTUREBUFFER_LOESCHEN_:
						Optionen->Button_Meshcache_Texturebuffer_loeschen();
						break;
				}
				// Fensterinhalte aktualisieren
				if (aktualisieren)
					Optionen->AktualisiereFenster();
				break;

			// Listboxfeld neu ausgewaehlt
			case irr::gui::EGET_LISTBOX_CHANGED:
				// Fensterinhalte auslesen
				Optionen->MerkeFenster();
				// Fensterinhalte aktualisieren
				Optionen->AktualisiereFenster();
				break;

			// Datei ausgewaehlt (im FileOpen-Dialog)
			case irr::gui::EGET_FILE_SELECTED:
				// Fensterinhalte auslesen
				Optionen->MerkeFenster();
				// Arbeitsverzeichnis wieder auf Ausgang (Standard) setzen
				Optionen->Arbeitsverzeichnis_wiederherstellen();
				// verantwortlichen Dialog ermitteln
				irr::gui::IGUIFileOpenDialog* dialog =  (irr::gui::IGUIFileOpenDialog*)_Event.GUIEvent.Caller;

				// Dateinamen an entsprechende Funktion weitergeben
                switch(id)
				{
					case _GUI_ID_FILEOPENDIALOG_SPEICHERN_EINSTELLUNGEN_:						
						Optionen->Speichere_Einstellungen(dialog->getFileName());
						break;
					case _GUI_ID_FILEOPENDIALOG_LADEN_EINSTELLUNGEN_:
						Optionen->Lade_Einstellungen(dialog->getFileName());
						break;
					case _GUI_ID_FILEOPENDIALOG_SPEICHERN_ERGEBNISSE_FRAKTALSTRINGS_:
						Optionen->Speichere_Ergebnisse_Fraktalstrings(dialog->getFileName());
						break;
					case _GUI_ID_FILEOPENDIALOG_SPEICHERN_ERGEBNISSE_DICHTE_XY_:
						Optionen->Speichere_Ergebnisse_Dichtekarte(EEbene::_EBENE_XY_, dialog->getFileName());
						break;
					case _GUI_ID_FILEOPENDIALOG_SPEICHERN_ERGEBNISSE_DICHTE_XZ_:
						Optionen->Speichere_Ergebnisse_Dichtekarte(EEbene::_EBENE_XZ_, dialog->getFileName());
						break;
					case _GUI_ID_FILEOPENDIALOG_SPEICHERN_ERGEBNISSE_DICHTE_YZ_:
						Optionen->Speichere_Ergebnisse_Dichtekarte(EEbene::_EBENE_YZ_, dialog->getFileName());
						break;
					case _GUI_ID_FILEOPENDIALOG_SPEICHERN_ERGEBNISSE_DREIECKSMESH_HOEHLE_:
						Optionen->Speichere_Ergebnisse_Dreicksmesh_Hoehle(dialog->getFileName());
						break;
					case _GUI_ID_FILEOPENDIALOG_SPEICHERN_ERGEBNISSE_DUNGEON_:
						Optionen->Speichere_Ergebnisse_Dungeon(dialog->getFileName());
						break;
					case _GUI_ID_FILEOPENDIALOG_LADEN_ERGEBNISSE_DUNGEON_:
						Optionen->Lade_Ergebnisse_Dungeon(dialog->getFileName());
						break;

					case _GUI_ID_FILEOPENDIALOG_GANGTEXTUR_:
						Optionen->Dateizugriff_Gangtextur_Auswahl(dialog->getFileName());
						break;
					case _GUI_ID_FILEOPENDIALOG_DETAILOBJEKTMESH_:
						Optionen->Dateizugriff_Detailobjekt_Auswahl(dialog->getFileName());
						break;
					case _GUI_ID_FILEOPENDIALOG_SUBSZENE_:
						Optionen->Dateizugriff_Subszene_Auswahl(dialog->getFileName());
						break;

                }
				// Fensterinhalte aktualisieren
				Optionen->AktualisiereFenster();				
				break;
		}
	}

	// Tasten-Event
	if (_Event.EventType == irr::EET_KEY_INPUT_EVENT &&
                    _Event.KeyInput.PressedDown == false &&					
					GuiEnvironment->getFocus() == 0 // kein Menuefenster im Fokus, sondern das Renderfenster
					)
    {
		// Welche Taste
		switch(_Event.KeyInput.Key)
		{
			case irr::KEY_F1:
				Szene->VoxelMeshDetailstufeAendern(-1);			// mehr Details
				break;
			case irr::KEY_F2 :
				Szene->VoxelMeshDetailstufeAendern(1);			// weniger Details
				break;
			case irr::KEY_F3 :
				Szene->GangMeshDetailstufeAendern(-1);			// mehr Details
				break;
			case irr::KEY_F4 :
				Szene->GangMeshDetailstufeAendern(1);			// weniger Details
				break;

			case irr::KEY_F5 :
				Szene->KameraEinstellenFPSModus(true);
				break;
			case irr::KEY_F6 :
				Szene->KameraEinstellenFPSModus(false);
				break;
			case irr::KEY_F7 :
				Szene->KameraGeschwindigkeitAendern(2.0,0.0);	// verdoppeln
				break;
			case irr::KEY_F8 :
				Szene->KameraGeschwindigkeitAendern(0.5,0.0);	// halbieren
				break;
			case irr::KEY_F9:
				Szene->KameraZuruecksetzen();
				break;
			case irr::KEY_F11:
				Szene->KameraDatenMerken();
				break;
			case irr::KEY_F12:
				Szene->KameraDatenWiederherstellen();
				break;

			case irr::KEY_KEY_1:
				Szene->VoxelMeshDrahtgitterModellUmschalten();
				break;
			case irr::KEY_KEY_2 :
				Szene->GangMeshDrahtgitterModellUmschalten();
				break;
			case irr::KEY_KEY_3 :
				Szene->AdapterMeshDrahtgitterModellUmschalten();
				break;
			case irr::KEY_KEY_4 :
				Szene->DetailObjekteZeigenUmschalten();
				break;
			case irr::KEY_KEY_5 :
				Szene->RaeumeZeigenUmschalten();
				break;

			case irr::KEY_KEY_6 :
				Szene->GangeBlickdichtigkeitVisualisieren(true);	// blickdichte Gaenge auf Drahtgittermodus
				break;
			case irr::KEY_KEY_7 :
				Szene->GangeBlickdichtigkeitVisualisieren(false);	// nicht-blickdichte Gaenge auf Drahtgittermodus
				break;

			case irr::KEY_KEY_8 :
				Szene->VoxelMeshMaterialAendern();
				break;
				
			case irr::KEY_KEY_9 :
				Optionen->GUIumschalten();
				break;

			case irr::KEY_KEY_0 :
				// Arbeitsverzeichnis wieder auf Ausgang (Standard) setzen
				Optionen->Arbeitsverzeichnis_wiederherstellen();
				Optionen->ScreenshotAnfertigen();	// Screenshot
				break;

			break;
		}
    }

	// Event noch nicht abgearbeitet: andere Irrlicht-Objekte verwenden das Event noch (FPS-Kamera, Fenstersteuerung, ...)
	return false;
}
