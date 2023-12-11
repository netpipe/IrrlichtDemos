// Implementierung der Klasse GUI
#include "GUI.h"

// Konstruktor
DunGen::CGUI::CGUI(irr::gui::IGUIEnvironment* _GUIEnvironment)
{
	// Werte uebernehmen
	GUIEnvironment = _GUIEnvironment;

	// Transparenzen der GUI aendern
	for (irr::u32 i=0; i<irr::gui::EGDC_COUNT ; ++i)
	{
		irr::video::SColor color = GUIEnvironment->getSkin()->getColor((irr::gui::EGUI_DEFAULT_COLOR)i);
		color.setAlpha(255);
		GUIEnvironment->getSkin()->setColor((irr::gui::EGUI_DEFAULT_COLOR)i, color);
	}

}

// Hauptmenue erstellen
void DunGen::CGUI::ErstelleMenue()
{
	irr::gui::IGUIToolBar* toolbar = GUIEnvironment->addToolBar();
	toolbar->addButton(_GUI_ID_BUTTON_DATEIZUGRIFF_EINSTELLUNGEN_, L"Dateizugriff: Einstellungen");
	toolbar->addButton(_GUI_ID_BUTTON_DATEIZUGRIFF_ERGEBNISSE_, L"Dateizugriff: Ergebnisse");
	toolbar->addButton(_GUI_ID_BUTTON_FRAKTALEDITOR_, L"   FraktalEditor   ");
	toolbar->addButton(_GUI_ID_BUTTON_VOXELHOEHLE_, L"   VoxelRaum   ");
	toolbar->addButton(_GUI_ID_BUTTON_GAENGE_, L"   Gange   ");
	toolbar->addButton(_GUI_ID_BUTTON_ARCHITEKT_, L"   Architekt   ");
	toolbar->addButton(_GUI_ID_BUTTON_ERSTELLUNG_, L"   Erstellung   ");
	toolbar->addButton(_GUI_ID_BUTTON_WEITERES_, L"   Weiteres   ");
}


// Dateizugriff: Einstellungen - Fenster erstellen
void DunGen::CGUI::ErstelleFenster_Dateizugriff_Einstellungen()
{
	// Fenster schliessen, falls schon vorhanden
	irr::gui::IGUIElement* root = GUIEnvironment->getRootGUIElement();
    irr::gui::IGUIElement* element = root->getElementFromId(_GUI_ID_WINDOW_DATEIZUGRIFF_EINSTELLUNGEN_, true);
    if (element)
	{
		// entfernen
		element->remove();
		return;
	}

	// Fenster erstellen
    irr::gui::IGUIWindow* fenster = GUIEnvironment->addWindow(irr::core::rect<irr::s32>(100,50,360,320),
                false, L"Einstellungen laden/speichern", 0, _GUI_ID_WINDOW_DATEIZUGRIFF_EINSTELLUNGEN_);

	GUIEnvironment->addStaticText(L"Welche Einstellungen sollen gespeichert bzw. geladen werden?", irr::core::rect<irr::s32>(10,25,999,999), false, false, fenster);
	GUIEnvironment->addCheckBox(true,irr::core::rect<irr::s32>(10,40,240,60),fenster,_GUI_ID_CHECKBOX_DATEI_EINSTELLUNGEN_FRAKTALPARAMETER_,L"L-System inklusive zugehoriger Parameter");
	GUIEnvironment->addCheckBox(true,irr::core::rect<irr::s32>(10,60,240,80),fenster,_GUI_ID_CHECKBOX_DATEI_EINSTELLUNGEN_WEITERE_FRAKTALEINSTELLUNGEN_,L"Weitere FraktalEditor-Einstellungen");
	GUIEnvironment->addCheckBox(true,irr::core::rect<irr::s32>(10,80,240,100),fenster,_GUI_ID_CHECKBOX_DATEI_EINSTELLUNGEN_VOXELRAUM_,L"VoxelRaum-Einstellungen");
	GUIEnvironment->addCheckBox(true,irr::core::rect<irr::s32>(10,100,240,120),fenster,_GUI_ID_CHECKBOX_DATEI_EINSTELLUNGEN_GAENGE_,L"Gange-Einstellungen");
	GUIEnvironment->addCheckBox(true,irr::core::rect<irr::s32>(10,120,240,140),fenster,_GUI_ID_CHECKBOX_DATEI_EINSTELLUNGEN_DETAILOBJEKTE_,L"Detailobjekte-Einstellungen");
	GUIEnvironment->addCheckBox(true,irr::core::rect<irr::s32>(10,140,240,160),fenster,_GUI_ID_CHECKBOX_DATEI_EINSTELLUNGEN_ARCHITEKT_,L"Architekt-Einstellungen");
	GUIEnvironment->addCheckBox(true,irr::core::rect<irr::s32>(10,160,240,180),fenster,_GUI_ID_CHECKBOX_DATEI_EINSTELLUNGEN_SUBSZENEN_,L"Subszenen-Einstellungen");
	GUIEnvironment->addCheckBox(true,irr::core::rect<irr::s32>(10,180,240,200),fenster,_GUI_ID_CHECKBOX_DATEI_EINSTELLUNGEN_ERSTELLUNG_,L"Erstellung-Einstellungen");
	GUIEnvironment->addCheckBox(true,irr::core::rect<irr::s32>(10,200,240,220),fenster,_GUI_ID_CHECKBOX_DATEI_EINSTELLUNGEN_WEITERES_,L"Weiteres-Einstellungen");

	GUIEnvironment->addButton(irr::core::rect<irr::s32>(20,240,120,260),fenster,_GUI_ID_BUTTON_SPEICHERN_EINSTELLUNGEN_,L"Speichern");
	GUIEnvironment->addButton(irr::core::rect<irr::s32>(140,240,240,260),fenster,_GUI_ID_BUTTON_LADEN_EINSTELLUNGEN_,L"Laden");
}

// Dateizugriff: Ergebnisse - Fenster erstellen
void DunGen::CGUI::ErstelleFenster_Dateizugriff_Ergebnisse()
{
	// Fenster schliessen, falls schon vorhanden
	irr::gui::IGUIElement* root = GUIEnvironment->getRootGUIElement();
    irr::gui::IGUIElement* element = root->getElementFromId(_GUI_ID_WINDOW_DATEIZUGRIFF_ERGEBNISSE_, true);
    if (element)
	{
		// entfernen
		element->remove();
		return;
	}

	// Fenster erstellen
    irr::gui::IGUIWindow* fenster = GUIEnvironment->addWindow(irr::core::rect<irr::s32>(100,50,360,300),
                false, L"Ergebnisse laden/speichern", 0, _GUI_ID_WINDOW_DATEIZUGRIFF_ERGEBNISSE_);

	GUIEnvironment->addStaticText(L"Welche Ergebnisse sollen gespeichert bzw. geladen werden?", irr::core::rect<irr::s32>(10,25,999,999), false, false, fenster);

	GUIEnvironment->addStaticText(L"Generierte Ableitungen des L-Systems + Parameter:", irr::core::rect<irr::s32>(10,45,999,999), false, false, fenster);
	GUIEnvironment->addButton(irr::core::rect<irr::s32>(200,40,250,60),fenster,_GUI_ID_BUTTON_SPEICHERN_ERGEBNISSE_FRAKTALSTRINGS_,L"Speichern");

	GUIEnvironment->addStaticText(L"Voxelraum-Dichtekarte Top-Ansicht (X,Z):", irr::core::rect<irr::s32>(10,75,999,999), false, false, fenster);
	GUIEnvironment->addButton(irr::core::rect<irr::s32>(200,70,250,90),fenster,_GUI_ID_BUTTON_SPEICHERN_ERGEBNISSE_DICHTE_XZ_,L"Speichern");

	GUIEnvironment->addStaticText(L"Voxelraum-Dichtekarte Seiten-Ansicht (X,Y):", irr::core::rect<irr::s32>(10,105,999,999), false, false, fenster);
	GUIEnvironment->addButton(irr::core::rect<irr::s32>(200,100,250,120),fenster,_GUI_ID_BUTTON_SPEICHERN_ERGEBNISSE_DICHTE_XY_,L"Speichern");

	GUIEnvironment->addStaticText(L"Voxelraum-Dichtekarte Front-Ansicht (-Z,Y):", irr::core::rect<irr::s32>(10,135,999,999), false, false, fenster);
	GUIEnvironment->addButton(irr::core::rect<irr::s32>(200,130,250,150),fenster,_GUI_ID_BUTTON_SPEICHERN_ERGEBNISSE_DICHTE_YZ_,L"Speichern");

	GUIEnvironment->addStaticText(L"Generiertes Dreiecksnetz der Hohle:", irr::core::rect<irr::s32>(10,165,999,999), false, false, fenster);
	GUIEnvironment->addButton(irr::core::rect<irr::s32>(200,160,250,180),fenster,_GUI_ID_BUTTON_SPEICHERN_ERGEBNISSE_DREIECKSMESH_,L"Speichern");

	GUIEnvironment->addStaticText(L"Generierter Dungeon:", irr::core::rect<irr::s32>(10,195,999,999), false, false, fenster);
	GUIEnvironment->addButton(irr::core::rect<irr::s32>(200,190,250,210),fenster,_GUI_ID_BUTTON_SPEICHERN_ERGEBNISSE_DUNGEON_,L"Speichern");

	GUIEnvironment->addStaticText(L"Test auf korrekten Export: .irr-Szene einladen", irr::core::rect<irr::s32>(10,225,999,999), false, false, fenster);
	GUIEnvironment->addButton(irr::core::rect<irr::s32>(200,220,250,240),fenster,_GUI_ID_BUTTON_LADEN_ERGEBNISSE_DUNGEON_,L"Laden");

}

// FraktalEditor-Fenster erstellen
void DunGen::CGUI::ErstelleFenster_FraktalEditor()
{
	// Fenster schliessen, falls schon vorhanden
	irr::gui::IGUIElement* root = GUIEnvironment->getRootGUIElement();
    irr::gui::IGUIElement* element = root->getElementFromId(_GUI_ID_WINDOW_FRAKTALEDITOR_, true);
    if (element)
	{
		// entfernen
		element->remove();
		return;
	}

	// Fenster erstellen
    irr::gui::IGUIWindow* fenster = GUIEnvironment->addWindow(irr::core::rect<irr::s32>(100,50,420,680),
                false, L"FraktalEditor", 0, _GUI_ID_WINDOW_FRAKTALEDITOR_);

	// Parameter
	GUIEnvironment->addStaticText(L"PARAMETER:", irr::core::rect<irr::s32>(10,20,999,999), false, false, fenster);
	GUIEnvironment->addStaticText(L"Wert", irr::core::rect<irr::s32>(80,28,999,999), false, false, fenster);
	GUIEnvironment->addStaticText(L"Min", irr::core::rect<irr::s32>(140,28,999,999), false, false, fenster);
	GUIEnvironment->addStaticText(L"Max", irr::core::rect<irr::s32>(200,28,999,999), false, false, fenster);
	GUIEnvironment->addStaticText(L"Kommastellen", irr::core::rect<irr::s32>(260,28,999,999), false, false, fenster);

	GUIEnvironment->addStaticText(L"Winkel Gier:", irr::core::rect<irr::s32>(10,45,999,999), false, false, fenster);
	GUIEnvironment->addStaticText(L"Winkel Nick:", irr::core::rect<irr::s32>(10,65,999,999), false, false, fenster);
	GUIEnvironment->addStaticText(L"Winkel Roll:", irr::core::rect<irr::s32>(10,85,999,999), false, false, fenster);
	GUIEnvironment->addStaticText(L"Radius Faktor:", irr::core::rect<irr::s32>(10,105,999,999), false, false, fenster);
	GUIEnvironment->addStaticText(L"Radius Dekr.:", irr::core::rect<irr::s32>(10,125,999,999), false, false, fenster);
	GUIEnvironment->addStaticText(L"Startradius:", irr::core::rect<irr::s32>(10,145,999,999), false, false, fenster);

	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(80,40,120,60), true, fenster, _GUI_ID_EDITBOX_WINKELGIER_WERT_);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(140,40,180,60), true, fenster, _GUI_ID_EDITBOX_WINKELGIER_MIN_);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(200,40,240,60), true, fenster, _GUI_ID_EDITBOX_WINKELGIER_MAX_);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(260,40,300,60), true, fenster, _GUI_ID_EDITBOX_WINKELGIER_KOMMASTELLEN_);

	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(80,60,120,80), true, fenster, _GUI_ID_EDITBOX_WINKELNICK_WERT_);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(140,60,180,80), true, fenster, _GUI_ID_EDITBOX_WINKELNICK_MIN_);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(200,60,240,80), true, fenster, _GUI_ID_EDITBOX_WINKELNICK_MAX_);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(260,60,300,80), true, fenster, _GUI_ID_EDITBOX_WINKELNICK_KOMMASTELLEN_);

	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(80,80,120,100), true, fenster, _GUI_ID_EDITBOX_WINKELROLL_WERT_);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(140,80,180,100), true, fenster, _GUI_ID_EDITBOX_WINKELROLL_MIN_);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(200,80,240,100), true, fenster, _GUI_ID_EDITBOX_WINKELROLL_MAX_);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(260,80,300,100), true, fenster, _GUI_ID_EDITBOX_WINKELROLL_KOMMASTELLEN_);

	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(80,100,120,120), true, fenster, _GUI_ID_EDITBOX_RADIUSFAKTOR_WERT_);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(140,100,180,120), true, fenster, _GUI_ID_EDITBOX_RADIUSFAKTOR_MIN_);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(200,100,240,120), true, fenster, _GUI_ID_EDITBOX_RADIUSFAKTOR_MAX_);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(260,100,300,120), true, fenster, _GUI_ID_EDITBOX_RADIUSFAKTOR_KOMMASTELLEN_);

	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(80,120,120,140), true, fenster, _GUI_ID_EDITBOX_RADIUSDEKREMENTOR_WERT_);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(140,120,180,140), true, fenster, _GUI_ID_EDITBOX_RADIUSDEKREMENTOR_MIN_);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(200,120,240,140), true, fenster, _GUI_ID_EDITBOX_RADIUSDEKREMENTOR_MAX_);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(260,120,300,140), true, fenster, _GUI_ID_EDITBOX_RADIUSDEKREMENTOR_KOMMASTELLEN_);

	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(80,140,120,160), true, fenster, _GUI_ID_EDITBOX_RADIUSSTART_WERT_);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(140,140,180,160), true, fenster, _GUI_ID_EDITBOX_RADIUSSTART_MIN_);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(200,140,240,160), true, fenster, _GUI_ID_EDITBOX_RADIUSSTART_MAX_);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(260,140,300,160), true, fenster, _GUI_ID_EDITBOX_RADIUSSTART_KOMMASTELLEN_);

	GUIEnvironment->addButton(irr::core::rect<irr::s32>(140,165,300,180),fenster,_GUI_ID_BUTTON_ZUFALLSPARAMETER_,L"Zufallsparameter");

	// StartString
	GUIEnvironment->addStaticText(L"Startstring:", irr::core::rect<irr::s32>(10,205,999,999), false, false, fenster);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(80,200,300,220), true, fenster, _GUI_ID_EDITBOX_STARTSTRING_);

	GUIEnvironment->addStaticText(L"Erlaubte Symbole:", irr::core::rect<irr::s32>(10,225,999,999), false, false, fenster);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(80,220,300,240), true, fenster, _GUI_ID_EDITBOX_STARTSTRING_ERLAUBTE_SYMBOLE_);
	GUIEnvironment->addStaticText(L"Min. Lange:", irr::core::rect<irr::s32>(10,245,999,999), false, false, fenster);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(80,240,120,260), true, fenster, _GUI_ID_EDITBOX_STARTSTRING_MINLAENGE_);
	GUIEnvironment->addStaticText(L"Max. Lange:", irr::core::rect<irr::s32>(130,245,999,999), false, false, fenster);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(200,240,240,260), true, fenster, _GUI_ID_EDITBOX_STARTSTRING_MAXLAENGE_);
	GUIEnvironment->addButton(irr::core::rect<irr::s32>(80,265,240,280),fenster,_GUI_ID_BUTTON_ZUFALLSSTART_,L"Zufalliger Startstring");

	// Regeln
	GUIEnvironment->addStaticText(L"Symbol:", irr::core::rect<irr::s32>(10,305,999,999), false, false, fenster);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(80,300,120,320), true, fenster, _GUI_ID_EDITBOX_SYMBOL_);
	GUIEnvironment->addStaticText(L"Ersetzung:", irr::core::rect<irr::s32>(10,325,999,999), false, false, fenster);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(80,320,300,340), true, fenster, _GUI_ID_EDITBOX_SYMBOLERSETZUNG_);
	GUIEnvironment->addStaticText(L"Erlaubte Symbole:", irr::core::rect<irr::s32>(10,345,999,999), false, false, fenster);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(80,340,300,360), true, fenster, _GUI_ID_EDITBOX_SYMBOLERSETZUNG_ERLAUBTE_SYMBOLE_);

	GUIEnvironment->addStaticText(L"Min. Lange:", irr::core::rect<irr::s32>(10,365,999,999), false, false, fenster);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(80,360,120,380), true, fenster, _GUI_ID_EDITBOX_SYMBOLERSETZUNG_MINLAENGE_);
	GUIEnvironment->addStaticText(L"Max. Lange:", irr::core::rect<irr::s32>(130,365,999,999), false, false, fenster);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(200,360,240,380), true, fenster, _GUI_ID_EDITBOX_SYMBOLERSETZUNG_MAXLAENGE_);
	GUIEnvironment->addButton(irr::core::rect<irr::s32>(10,385,155,400),fenster,_GUI_ID_BUTTON_REGELHINZUFUEGEN_,L"Regel hinzufugen");
	GUIEnvironment->addButton(irr::core::rect<irr::s32>(155,385,300,400),fenster,_GUI_ID_BUTTON_ZUFALLSREGEL_,L"Zufallige Regel hinzufugen");

	GUIEnvironment->addStaticText(L"Aktuelle Regeln:", irr::core::rect<irr::s32>(10,405,999,999), false, false, fenster);
	GUIEnvironment->addListBox(irr::core::rect<irr::s32>(10,420,300,500),fenster,_GUI_ID_LISTBOX_ERSETZUNGSREGELN_,true);
	GUIEnvironment->addButton(irr::core::rect<irr::s32>(10,505,300,520),fenster,_GUI_ID_BUTTON_REGELNLOESCHEN_,L"Regeln loschen");

	GUIEnvironment->addStaticText(L"Ersetzungsinfo:", irr::core::rect<irr::s32>(10,530,999,999), false, false, fenster);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(80,525,300,545), true, fenster, _GUI_ID_EDITBOX_ERSETZUNGSINFO_);

	// Erstellung
	GUIEnvironment->addStaticText(L"Gewunschte Iterationsstufe:", irr::core::rect<irr::s32>(10,555,999,999), false, false, fenster);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(200,550,240,570), true, fenster, _GUI_ID_EDITBOX_ITERATIONSSTUFE_);
	GUIEnvironment->addButton(irr::core::rect<irr::s32>(10,575,300,595),fenster,_GUI_ID_BUTTON_FRAKTALGENERIEREN_,L"Generiere Zeichenanweisungen");

	GUIEnvironment->addButton(irr::core::rect<irr::s32>(10,600,300,620),fenster,_GUI_ID_BUTTON_ZUFALLSFRAKTALGENERIEREN_,L"Generiere Zufallsparameter und Zeichenanweisungen");
}

// VoxelRaum-Fenster erstellen
void DunGen::CGUI::ErstelleFenster_VoxelHoehle()
{
	// Fenster schliessen, falls schon vorhanden
	irr::gui::IGUIElement* root = GUIEnvironment->getRootGUIElement();
    irr::gui::IGUIElement* element = root->getElementFromId(_GUI_ID_WINDOW_VOXELRAUM_, true);
    if (element)
	{
		// entfernen
		element->remove();
		return;
	}

	// Fenster erstellen
    irr::gui::IGUIWindow* fenster = GUIEnvironment->addWindow(irr::core::rect<irr::s32>(100,50,600,500),
                false, L"VoxelRaum", 0, _GUI_ID_WINDOW_VOXELRAUM_);

	// VOXEL:

	// Einstellungen
	GUIEnvironment->addStaticText(L"Voxel-Rand:", irr::core::rect<irr::s32>(10,35,999,999), false, false, fenster);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(200,30,240,50), true, fenster, _GUI_ID_EDITBOX_RAND_);

	GUIEnvironment->addStaticText(L"Minimaler Strichradius:", irr::core::rect<irr::s32>(10,55,999,999), false, false, fenster);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(200,50,240,70), true, fenster, _GUI_ID_EDITBOX_MINIMALRADIUS_);

	GUIEnvironment->addStaticText(L"Erosions-Zufallsseed:", irr::core::rect<irr::s32>(10,95,999,999), false, false, fenster);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(200,90,240,110), true, fenster, _GUI_ID_EDITBOX_EROSION_ZUFALLSSEED_);
	GUIEnvironment->addCheckBox(true,irr::core::rect<irr::s32>(10,115,240,135),fenster,_GUI_ID_CHECKBOX_EROSION_VORGEGEBENEN_RANDSEED_BENUTZEN_,L"Erosion: vorgegebenen Zufallsseed benutzen");

	GUIEnvironment->addStaticText(L"Erosionswahrscheinlichkeit:", irr::core::rect<irr::s32>(10,145,999,999), false, false, fenster);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(200,140,240,160), true, fenster, _GUI_ID_EDITBOX_EROSIONSWAHRSCHEINLICHKEIT_);
	GUIEnvironment->addButton(irr::core::rect<irr::s32>(10,165,240,180),fenster,_GUI_ID_BUTTON_EROSIONSSCHRITT_HINZUFUEGEN_,L"Fuge Erosionsschritt hinzu");

	GUIEnvironment->addStaticText(L"Aktuelle Erosionsschritte:", irr::core::rect<irr::s32>(10,185,999,999), false, false, fenster);
	GUIEnvironment->addListBox(irr::core::rect<irr::s32>(10,195,240,275),fenster,_GUI_ID_LISTBOX_EROSIONSSCHRITTE_,true);
	GUIEnvironment->addButton(irr::core::rect<irr::s32>(10,280,240,295),fenster,_GUI_ID_BUTTON_EROSIONSSCHRITTE_LOESCHEN_,L"Erosionsschritte loschen");

	GUIEnvironment->addCheckBox(true,irr::core::rect<irr::s32>(10,305,240,325),fenster,_GUI_ID_CHECKBOX_SCHWEBENDE_FRAGMENTE_ENTFERNEN_,L"Schwebende Fragmente entfernen");

	// Berechnen
	GUIEnvironment->addButton(irr::core::rect<irr::s32>(10,335,240,355),fenster,_GUI_ID_BUTTON_VOXEL_BERECHNEN_,L"Berechne Voxel");

	// Berechnen und Anzeigen der Dichte
	GUIEnvironment->addButton(irr::core::rect<irr::s32>(10,375,240,395),fenster,_GUI_ID_BUTTON_ZEIGE_DICHTE_XZ_,L"Dichte berechnen und zeigen: Top-Ansicht (X,Z)");
	GUIEnvironment->addButton(irr::core::rect<irr::s32>(10,395,240,415),fenster,_GUI_ID_BUTTON_ZEIGE_DICHTE_XY_,L"Dichte berechnen und zeigen: Seiten-Ansicht (X,Y)");
	GUIEnvironment->addButton(irr::core::rect<irr::s32>(10,415,240,435),fenster,_GUI_ID_BUTTON_ZEIGE_DICHTE_YZ_,L"Dichte berechnen und zeigen: Front-Ansicht (-Z,Y)");

	// DREIECKSNETZ:

	// Umwandlung ins Dreiecksnetz
	GUIEnvironment->addCheckBox(true,irr::core::rect<irr::s32>(260,35,490,55),fenster,_GUI_ID_CHECKBOX_VERTICES_VERWACKELN_,L"Verwackle Vertices beim Umwandeln Voxel->Dreicksnetz");
	GUIEnvironment->addStaticText(L"Verwacklungstaerke:", irr::core::rect<irr::s32>(260,60,999,999), false, false, fenster);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(450,55,490,75), true, fenster, _GUI_ID_EDITBOX_VERTICES_VERWACKLUNGSTAERKE_);

	GUIEnvironment->addStaticText(L"Verwacklungs-Zufallsgenerator [x = (ax+c) % m] - Parameter:", irr::core::rect<irr::s32>(260,85,999,999), false, false, fenster);

	GUIEnvironment->addStaticText(L"Verwacklungs-Zufallsseed:", irr::core::rect<irr::s32>(260,105,999,999), false, false, fenster);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(450,100,490,120), true, fenster, _GUI_ID_EDITBOX_VERWACKLUNG_ZUFALLSSEED_);

	GUIEnvironment->addStaticText(L"a:", irr::core::rect<irr::s32>(260,125,999,999), false, false, fenster);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(275,120,330,140), true, fenster, _GUI_ID_EDITBOX_VERWACKLUNG_ZUFALLSPARAMETER_A_);
	GUIEnvironment->addStaticText(L"c:", irr::core::rect<irr::s32>(340,125,999,999), false, false, fenster);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(355,120,410,140), true, fenster, _GUI_ID_EDITBOX_VERWACKLUNG_ZUFALLSPARAMETER_C_);
	GUIEnvironment->addStaticText(L"m:", irr::core::rect<irr::s32>(420,125,999,999), false, false, fenster);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(435,120,490,140), true, fenster, _GUI_ID_EDITBOX_VERWACKLUNG_ZUFALLSPARAMETER_M_);

	GUIEnvironment->addCheckBox(true,irr::core::rect<irr::s32>(260,150,490,170),fenster,_GUI_ID_CHECKBOX_MESH_GLAETTEN_,L"Glaette Dreicksmesh beim Verwackeln");

	GUIEnvironment->addStaticText(L"Normalen-Berechnung Wichtungsmethode:", irr::core::rect<irr::s32>(260,185,999,999), false, false, fenster);
	irr::gui::IGUIComboBox* comboBox = GUIEnvironment->addComboBox(irr::core::rect<irr::s32>(260,200,490,220),fenster,_GUI_ID_COMBOBOX_NORMALEN_BERECHNUNGSMETHODE_);
	comboBox->addItem(L"Wichtung per Flacheninhalt",0);
	comboBox->addItem(L"Wichtung per Winkel",0);
	comboBox->addItem(L"Uniforme Wichtung",0);

	GUIEnvironment->addStaticText(L"Anzahl zusatzlicher Detailstufen:", irr::core::rect<irr::s32>(260,235,999,999), false, false, fenster);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(450,230,490,250), true, fenster, _GUI_ID_EDITBOX_DETAILSTUFEN_VOXELMESH_);

	GUIEnvironment->addButton(irr::core::rect<irr::s32>(260,260,490,280),fenster,_GUI_ID_BUTTON_VOXEL_ZU_DREIECKSNETZ_BERECHNEN_,L"Berechne Dreicksnetz aus Voxeln");
	GUIEnvironment->addButton(irr::core::rect<irr::s32>(260,280,490,300),fenster,_GUI_ID_BUTTON_ZEIGE_DRAHTGITTERMODELL_,L"Zeige berechnetes Dreiecksnetz");
}

// Gang-Fenster erstellen
void DunGen::CGUI::ErstelleFenster_Gaenge()
{
	// Fenster schliessen, falls schon vorhanden
	irr::gui::IGUIElement* root = GUIEnvironment->getRootGUIElement();
    irr::gui::IGUIElement* element = root->getElementFromId(_GUI_ID_WINDOW_GAENGE_, true);
    if (element)
	{
		// entfernen
		element->remove();
		return;
	}

	// Fenster erstellen
    irr::gui::IGUIWindow* fenster = GUIEnvironment->addWindow(irr::core::rect<irr::s32>(100,50,700,615),
                false, L"Gange", 0, _GUI_ID_WINDOW_GAENGE_);

	// GANG:

	// Einstellungen
	GUIEnvironment->addStaticText(L"Gangbreite:", irr::core::rect<irr::s32>(10,35,999,999), false, false, fenster);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(200,30,240,50), true, fenster, _GUI_ID_EDITBOX_GANGBREITE_);

	GUIEnvironment->addStaticText(L"GANGQUERSCHNITT:", irr::core::rect<irr::s32>(10,60,999,999), false, false, fenster);
	GUIEnvironment->addListBox(irr::core::rect<irr::s32>(10,70,240,225),fenster,_GUI_ID_LISTBOX_GANGPUNKTE_,true);

	GUIEnvironment->addButton(irr::core::rect<irr::s32>(10,230,125,245),fenster,_GUI_ID_BUTTON_NEU_GANGPUNKT_,L"Punkt hinzufugen");
	GUIEnvironment->addButton(irr::core::rect<irr::s32>(125,230,240,245),fenster,_GUI_ID_BUTTON_LOESCHEN_GANGPUNKT_,L"Punkt loschen");

	GUIEnvironment->addStaticText(L"Punkt editieren:", irr::core::rect<irr::s32>(10,250,999,999), false, false, fenster);
	GUIEnvironment->addStaticText(L"Position (lok.):", irr::core::rect<irr::s32>(10,270,999,999), false, false, fenster);
	GUIEnvironment->addStaticText(L"X:", irr::core::rect<irr::s32>(125,270,999,999), false, false, fenster);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(140,265,180,285), true, fenster, _GUI_ID_EDITBOX_PUNKT_X_);
	GUIEnvironment->addStaticText(L"Y:", irr::core::rect<irr::s32>(185,270,999,999), false, false, fenster);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(200,265,240,285), true, fenster, _GUI_ID_EDITBOX_PUNKT_Y_);
	GUIEnvironment->addStaticText(L"Texturkoordinate X:", irr::core::rect<irr::s32>(10,290,999,999), false, false, fenster);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(200,285,240,305), true, fenster, _GUI_ID_EDITBOX_PUNKT_TEXTUREX_);

	GUIEnvironment->addButton(irr::core::rect<irr::s32>(10,310,240,330),fenster,_GUI_ID_BUTTON_UEBERNEHMEN_GANGPUNKT_,L"Anderungen ubernehmen");

	GUIEnvironment->addStaticText(L"Gangsegmentabstand:", irr::core::rect<irr::s32>(10,345,999,999), false, false, fenster);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(200,340,240,360), true, fenster, _GUI_ID_EDITBOX_PUNKTABSTAND_);
	GUIEnvironment->addStaticText(L"Texturkoordinate Y Erhohung pro Abstand 1 :", irr::core::rect<irr::s32>(10,365,999,999), false, false, fenster);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(200,360,240,380), true, fenster, _GUI_ID_EDITBOX_TEXTUREY_ERHOEHUNG_);

	GUIEnvironment->addStaticText(L"Anzahl zusatzlicher Detailstufen:", irr::core::rect<irr::s32>(10,395,999,999), false, false, fenster);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(200,390,240,410), true, fenster, _GUI_ID_EDITBOX_DETAILSTUFEN_GANGMESH_);

	GUIEnvironment->addStaticText(L"Gangtextur:", irr::core::rect<irr::s32>(10,425,999,999), false, false, fenster);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(10,440,240,460), true, fenster, _GUI_ID_EDITBOX_GANGTEXTUR_);
	GUIEnvironment->addButton(irr::core::rect<irr::s32>(10,465,240,480),fenster,_GUI_ID_BUTTON_DATEI_WAEHLEN_GANGTEXTUR_,L"Texturdatei auswahlen");

	// DETAILOBJEKTE:
	GUIEnvironment->addStaticText(L"DETAILOBJEKTE:", irr::core::rect<irr::s32>(260,30,999,999), false, false, fenster);
	GUIEnvironment->addListBox(irr::core::rect<irr::s32>(260,40,590,225),fenster,_GUI_ID_LISTBOX_DETAILOBJEKTE_,true);
	GUIEnvironment->addButton(irr::core::rect<irr::s32>(260,230,425,245),fenster,_GUI_ID_BUTTON_NEU_DETAILOBJEKT_,L"Detailobjekt hinzufugen");
	GUIEnvironment->addButton(irr::core::rect<irr::s32>(425,230,590,245),fenster,_GUI_ID_BUTTON_LOESCHEN_DETAILOBJEKT_,L"Detailobjekt loschen");

	GUIEnvironment->addStaticText(L"Detailobjekt editieren:", irr::core::rect<irr::s32>(260,250,999,999), false, false, fenster);
	GUIEnvironment->addStaticText(L"Benennung:", irr::core::rect<irr::s32>(260,270,999,999), false, false, fenster);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(330,265,490,285), true, fenster, _GUI_ID_EDITBOX_DETAILOBJEKT_BENENNUNG_);

	GUIEnvironment->addStaticText(L"Skalierung:", irr::core::rect<irr::s32>(260,295,999,999), false, false, fenster);
	GUIEnvironment->addStaticText(L"X:", irr::core::rect<irr::s32>(315,295,999,999), false, false, fenster);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(330,290,370,310), true, fenster, _GUI_ID_EDITBOX_DETAILOBJEKT_SKALIERUNG_X_);
	GUIEnvironment->addStaticText(L"Y:", irr::core::rect<irr::s32>(375,295,999,999), false, false, fenster);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(390,290,430,310), true, fenster, _GUI_ID_EDITBOX_DETAILOBJEKT_SKALIERUNG_Y_);
	GUIEnvironment->addStaticText(L"Z:", irr::core::rect<irr::s32>(435,295,999,999), false, false, fenster);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(450,290,490,310), true, fenster, _GUI_ID_EDITBOX_DETAILOBJEKT_SKALIERUNG_Z_);

	GUIEnvironment->addStaticText(L"Rotation:", irr::core::rect<irr::s32>(260,315,999,999), false, false, fenster);
	GUIEnvironment->addStaticText(L"X:", irr::core::rect<irr::s32>(315,315,999,999), false, false, fenster);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(330,310,370,330), true, fenster, _GUI_ID_EDITBOX_DETAILOBJEKT_ROTATION_X_);
	GUIEnvironment->addStaticText(L"Y:", irr::core::rect<irr::s32>(375,315,999,999), false, false, fenster);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(390,310,430,330), true, fenster, _GUI_ID_EDITBOX_DETAILOBJEKT_ROTATION_Y_);
	GUIEnvironment->addStaticText(L"Z:", irr::core::rect<irr::s32>(435,315,999,999), false, false, fenster);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(450,310,490,330), true, fenster, _GUI_ID_EDITBOX_DETAILOBJEKT_ROTATION_Z_);

	GUIEnvironment->addStaticText(L"Position (lok.):", irr::core::rect<irr::s32>(260,335,999,999), false, false, fenster);
	GUIEnvironment->addStaticText(L"X:", irr::core::rect<irr::s32>(375,335,999,999), false, false, fenster);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(390,330,430,350), true, fenster, _GUI_ID_EDITBOX_DETAILOBJEKT_POSITION_X_);
	GUIEnvironment->addStaticText(L"Y:", irr::core::rect<irr::s32>(435,335,999,999), false, false, fenster);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(450,330,490,350), true, fenster, _GUI_ID_EDITBOX_DETAILOBJEKT_POSITION_Y_);

	GUIEnvironment->addStaticText(L"Abtastung:", irr::core::rect<irr::s32>(260,360,999,999), false, false, fenster);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(450,355,490,375), true, fenster, _GUI_ID_EDITBOX_DETAILOBJEKT_ABTASTUNG_);

	GUIEnvironment->addStaticText(L"Faktor fur Num Abstand:", irr::core::rect<irr::s32>(260,385,999,999), false, false, fenster);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(450,380,490,400), true, fenster, _GUI_ID_EDITBOX_DETAILOBJEKT_FAKTOR_NUMABSTAND_);
	GUIEnvironment->addStaticText(L"Num Abstand Min:", irr::core::rect<irr::s32>(260,405,999,999), false, false, fenster);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(450,400,490,420), true, fenster, _GUI_ID_EDITBOX_DETAILOBJEKT_NUMABSTAND_MIN_);
	GUIEnvironment->addStaticText(L"Num Abstand Max:", irr::core::rect<irr::s32>(260,425,999,999), false, false, fenster);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(450,420,490,440), true, fenster, _GUI_ID_EDITBOX_DETAILOBJEKT_NUMABSTAND_MAX_);
	GUIEnvironment->addStaticText(L"Num Abstand Min (erstes Element):", irr::core::rect<irr::s32>(260,445,999,999), false, false, fenster);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(450,440,490,460), true, fenster, _GUI_ID_EDITBOX_DETAILOBJEKT_NUMABSTAND_MIN_ANFANG_);
	GUIEnvironment->addStaticText(L"Num Abstand Max (erstes Element):", irr::core::rect<irr::s32>(260,465,999,999), false, false, fenster);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(450,460,490,480), true, fenster, _GUI_ID_EDITBOX_DETAILOBJEKT_NUMABSTAND_MAX_ANFANG_);

	GUIEnvironment->addCheckBox(false,irr::core::rect<irr::s32>(260,480,490,500),fenster,_GUI_ID_CHECKBOX_OBJEKT_GANGENDE_,L"Objekt am Ende des Gangs definitiv platzieren");

	GUIEnvironment->addButton(irr::core::rect<irr::s32>(260,505,590,525),fenster,_GUI_ID_BUTTON_UEBERNEHMEN_DETAILOBJEKT_,L"Anderungen ubernehmen");

	GUIEnvironment->addStaticText(L"Pfadinfo: ", irr::core::rect<irr::s32>(260,540,999,999), false, false, fenster);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(310,535,590,555), true, fenster, _GUI_ID_EDITBOX_DETAILOBJEKT_PFADINFO_);

}

// Architekt-Fenster erstellen
void DunGen::CGUI::ErstelleFenster_Architekt()
{
	// Fenster schliessen, falls schon vorhanden
	irr::gui::IGUIElement* root = GUIEnvironment->getRootGUIElement();
    irr::gui::IGUIElement* element = root->getElementFromId(_GUI_ID_WINDOW_ARCHITEKT_, true);
    if (element)
	{
		// entfernen
		element->remove();
		return;
	}

	// Fenster erstellen
    irr::gui::IGUIWindow* fenster = GUIEnvironment->addWindow(irr::core::rect<irr::s32>(100,50,700,525),
                false, L"Architekt", 0, _GUI_ID_WINDOW_ARCHITEKT_);

	// ARCHITEKT:
	GUIEnvironment->addStaticText(L"Konstruktions-Zufallsseed:", irr::core::rect<irr::s32>(10,35,999,999), false, false, fenster);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(200,30,240,50), true, fenster, _GUI_ID_EDITBOX_ARCHITEKT_KONTRUKTIONS_ZUFALLSSEED_);
	GUIEnvironment->addCheckBox(true,irr::core::rect<irr::s32>(10,50,240,70),fenster,_GUI_ID_CHECKBOX_ARCHITEKT_VORGEGEBENEN_ZUFALLSSEED_NUTZEN_,L"Konstruktion: vorgegebenen Zufallsseed benutzen");

	GUIEnvironment->addStaticText(L"Raumsektorausdehung:", irr::core::rect<irr::s32>(10,80,999,999), false, false, fenster);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(200,75,240,95), true, fenster, _GUI_ID_EDITBOX_ARCHITEKT_RAUMSEKTORAUSDEHNUNG_);

	GUIEnvironment->addStaticText(L"Minimalabstand Raum<->Hohle (fur Verbindung):", irr::core::rect<irr::s32>(10,105,999,999), false, false, fenster);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(200,100,240,120), true, fenster, _GUI_ID_EDITBOX_ARCHITEKT_MINABSTAND_RAUM_HOEHLE_);

	GUIEnvironment->addStaticText(L"Andockabstand Gang zum Boden:", irr::core::rect<irr::s32>(10,130,999,999), false, false, fenster);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(200,125,240,145), true, fenster, _GUI_ID_EDITBOX_ARCHITEKT_ABSTAND_BODEN_);
	GUIEnvironment->addStaticText(L"Tiefe fur Abstandstest:", irr::core::rect<irr::s32>(10,150,999,999), false, false, fenster);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(200,145,240,165), true, fenster, _GUI_ID_EDITBOX_ARCHITEKT_ABSTANDSTEST_TIEFE_);

	GUIEnvironment->addStaticText(L"zusatzliche Ausfrastiefe:", irr::core::rect<irr::s32>(10,175,999,999), false, false, fenster);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(200,170,240,190), true, fenster, _GUI_ID_EDITBOX_ARCHITEKT_AUSFRAESTIEFE_);

	GUIEnvironment->addStaticText(L"Zusatz.Sicherheitsabst. Kollisionstest Gang<->Hohle:", irr::core::rect<irr::s32>(10,200,999,999), false, false, fenster);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(200,195,240,215), true, fenster, _GUI_ID_EDITBOX_ARCHITEKT_SICHERHEITSABSTAND_);

	GUIEnvironment->addStaticText(L"Wahrscheinlichkeit fur zusatzlichen Gang:", irr::core::rect<irr::s32>(10,230,999,999), false, false, fenster);
	GUIEnvironment->addStaticText(L"Raum<->Hohle:", irr::core::rect<irr::s32>(20,250,999,999), false, false, fenster);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(85,245,125,265), true, fenster, _GUI_ID_EDITBOX_ARCHITEKT_WAHRSCHEINLICHKEIT_ZUSATZGANG_HOEHLE_);
	GUIEnvironment->addStaticText(L"Raum<->Raum:", irr::core::rect<irr::s32>(135,250,999,999), false, false, fenster);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(200,245,240,265), true, fenster, _GUI_ID_EDITBOX_ARCHITEKT_WAHRSCHEINLICHKEIT_ZUSATZGANG_RAUM_);

	GUIEnvironment->addStaticText(L"Platzierungswahrscheinlichkeitsfaktor fur leere Subszene:", irr::core::rect<irr::s32>(10,275,999,999), false, false, fenster);
	GUIEnvironment->addStaticText(L"an Hohle:", irr::core::rect<irr::s32>(40,295,999,999), false, false, fenster);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(85,290,125,310), true, fenster, _GUI_ID_EDITBOX_ARCHITEKT_WAHRSCHEINLICHKEIT_LEERERRAUM_HOEHLE_);
	GUIEnvironment->addStaticText(L"an Raum:", irr::core::rect<irr::s32>(155,295,999,999), false, false, fenster);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(200,290,240,310), true, fenster, _GUI_ID_EDITBOX_ARCHITEKT_WAHRSCHEINLICHKEIT_LEERERRAUM_RAUM_);

	// SUBSZENEN:
	GUIEnvironment->addStaticText(L"SUBSZENEN:", irr::core::rect<irr::s32>(260,30,999,999), false, false, fenster);
	GUIEnvironment->addListBox(irr::core::rect<irr::s32>(260,40,590,225),fenster,_GUI_ID_LISTBOX_SUBSZENEN_,true);
	GUIEnvironment->addButton(irr::core::rect<irr::s32>(260,230,425,245),fenster,_GUI_ID_BUTTON_NEU_SUBSZENE_,L"Subszene hinzufugen");
	GUIEnvironment->addButton(irr::core::rect<irr::s32>(425,230,590,245),fenster,_GUI_ID_BUTTON_LOESCHEN_SUBSZENE_,L"Subszene loschen");

	GUIEnvironment->addStaticText(L"Subszene editieren:", irr::core::rect<irr::s32>(260,250,999,999), false, false, fenster);
	GUIEnvironment->addStaticText(L"Benennung:", irr::core::rect<irr::s32>(260,270,999,999), false, false, fenster);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(330,265,490,285), true, fenster, _GUI_ID_EDITBOX_SUBSZENE_BENENNUNG_);

	GUIEnvironment->addStaticText(L"Skalierung:", irr::core::rect<irr::s32>(260,295,999,999), false, false, fenster);
	GUIEnvironment->addStaticText(L"X:", irr::core::rect<irr::s32>(315,295,999,999), false, false, fenster);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(330,290,370,310), true, fenster, _GUI_ID_EDITBOX_SUBSZENE_SKALIERUNG_X_);
	GUIEnvironment->addStaticText(L"Y:", irr::core::rect<irr::s32>(375,295,999,999), false, false, fenster);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(390,290,430,310), true, fenster, _GUI_ID_EDITBOX_SUBSZENE_SKALIERUNG_Y_);
	GUIEnvironment->addStaticText(L"Z:", irr::core::rect<irr::s32>(435,295,999,999), false, false, fenster);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(450,290,490,310), true, fenster, _GUI_ID_EDITBOX_SUBSZENE_SKALIERUNG_Z_);

	GUIEnvironment->addStaticText(L"Platzierungswahrscheinlichkeitsfaktor:", irr::core::rect<irr::s32>(260,320,999,999), false, false, fenster);
	GUIEnvironment->addStaticText(L"an Hohle:", irr::core::rect<irr::s32>(290,340,999,999), false, false, fenster);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(330,335,370,355), true, fenster, _GUI_ID_EDITBOX_SUBSZENE_WAHRSCHEINLICHKEIT_HOEHLE_);
	GUIEnvironment->addStaticText(L"an Raum:", irr::core::rect<irr::s32>(405,340,999,999), false, false, fenster);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(440,335,480,355), true, fenster, _GUI_ID_EDITBOX_SUBSZENE_WAHRSCHEINLICHKEIT_RAUM_);

	GUIEnvironment->addStaticText(L"Starke der Gangsplineableitungen:", irr::core::rect<irr::s32>(260,365,999,999), false, false, fenster);

	GUIEnvironment->addStaticText(L"Raum<->Raum:", irr::core::rect<irr::s32>(270,385,999,999), false, false, fenster);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(330,380,370,400), true, fenster, _GUI_ID_EDITBOX_SUBSZENE_GANGSPLINEABLEITUNG_RAUM_RAUM_);
	GUIEnvironment->addStaticText(L"Raum->Hohle:", irr::core::rect<irr::s32>(380,385,999,999), false, false, fenster);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(440,380,480,400), true, fenster, _GUI_ID_EDITBOX_SUBSZENE_GANGSPLINEABLEITUNG_RAUM_HOEHLE_);
	GUIEnvironment->addStaticText(L"Hohle->Raum:", irr::core::rect<irr::s32>(490,385,999,999), false, false, fenster);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(550,380,590,400), true, fenster, _GUI_ID_EDITBOX_SUBSZENE_GANGSPLINEABLEITUNG_HOEHLE_RAUM_);


	GUIEnvironment->addButton(irr::core::rect<irr::s32>(260,415,590,435),fenster,_GUI_ID_BUTTON_UEBERNEHMEN_SUBSZENE_,L"Anderungen ubernehmen");

	GUIEnvironment->addStaticText(L"Pfadinfo: ", irr::core::rect<irr::s32>(260,450,999,999), false, false, fenster);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(310,445,590,465), true, fenster, _GUI_ID_EDITBOX_SUBSZENE_PFADINFO_);
}

// Erstellungs-Fenster erstellen
void DunGen::CGUI::ErstelleFenster_Erstellung()
{
	// Fenster schliessen, falls schon vorhanden
	irr::gui::IGUIElement* root = GUIEnvironment->getRootGUIElement();
    irr::gui::IGUIElement* element = root->getElementFromId(_GUI_ID_WINDOW_ERSTELLUNG_, true);
    if (element)
	{
		// entfernen
		element->remove();
		return;
	}

	// Fenster erstellen
    irr::gui::IGUIWindow* fenster = GUIEnvironment->addWindow(irr::core::rect<irr::s32>(100,50,350,270),
                false, L"Erstellung", 0, _GUI_ID_WINDOW_ERSTELLUNG_);

	GUIEnvironment->addStaticText(L"Welche Dungeon-Erstellungschritte sollen ausgefuhrt werden?", irr::core::rect<irr::s32>(10,25,999,999), false, false, fenster);
	GUIEnvironment->addCheckBox(true,irr::core::rect<irr::s32>(10,40,240,60),fenster,_GUI_ID_CHECKBOX_ERSTELLUNG_FRAKTALZEICHENANWEISUNG_,L"1.) Hohlen-Zeichenanweisung erstellen");
	GUIEnvironment->addCheckBox(true,irr::core::rect<irr::s32>(10,60,240,80),fenster,_GUI_ID_CHECKBOX_ERSTELLUNG_VOXELZEICHNEN_EROSION_,L"2.) Voxelhohle zeichnen & evntl. Erosion durchfuhren");
	GUIEnvironment->addCheckBox(true,irr::core::rect<irr::s32>(10,80,240,100),fenster,_GUI_ID_CHECKBOX_ERSTELLUNG_FILTERUNG1_,L"3.) Filterung schwebender Fragmente 1");
	GUIEnvironment->addCheckBox(true,irr::core::rect<irr::s32>(10,100,240,120),fenster,_GUI_ID_CHECKBOX_ERSTELLUNG_ARCHITEKT_,L"4.) Architekt: Gange und Raume platzieren");
	GUIEnvironment->addCheckBox(true,irr::core::rect<irr::s32>(10,120,240,140),fenster,_GUI_ID_CHECKBOX_ERSTELLUNG_FILTERUNG2_,L"5.) Filterung schwebender Fragmente 2");
	GUIEnvironment->addCheckBox(true,irr::core::rect<irr::s32>(10,140,240,160),fenster,_GUI_ID_CHECKBOX_ERSTELLUNG_UMWANDLUNGVOXEL_,L"6.) Umwandlung Voxelhohle -> Dreicksmesh");

	GUIEnvironment->addButton(irr::core::rect<irr::s32>(10,170,240,190),fenster,_GUI_ID_BUTTON_ERSTELLE_DUNGEON_,L"Dungeon erstellen");
	GUIEnvironment->addButton(irr::core::rect<irr::s32>(10,190,240,210),fenster,_GUI_ID_BUTTON_ZEIGE_DUNGEON_,L"Dungeon anzeigen");
}

// Weiteres-Fenster erstellen
void DunGen::CGUI::ErstelleFenster_Weiteres()
{
	// Fenster schliessen, falls schon vorhanden
	irr::gui::IGUIElement* root = GUIEnvironment->getRootGUIElement();
    irr::gui::IGUIElement* element = root->getElementFromId(_GUI_ID_WINDOW_WEITERES_, true);
    if (element)
	{
		// entfernen
		element->remove();
		return;
	}

	// Fenster erstellen
    irr::gui::IGUIWindow* fenster = GUIEnvironment->addWindow(irr::core::rect<irr::s32>(100,50,350,330),
                false, L"Weiteres", 0, _GUI_ID_WINDOW_WEITERES_);

	GUIEnvironment->addStaticText(L"Zufallsgenerator [x = (ax+c) % m] - Parameter:", irr::core::rect<irr::s32>(10,25,999,999), false, false, fenster);
	GUIEnvironment->addStaticText(L"(Zufallsgenerator fur FraktalEditor, Erosion, Architekt)", irr::core::rect<irr::s32>(10,45,999,999), false, false, fenster);

	GUIEnvironment->addStaticText(L"a:", irr::core::rect<irr::s32>(10,65,999,999), false, false, fenster);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(25,60,80,80), true, fenster, _GUI_ID_EDITBOX_ALLGEMEIN_ZUFALLSPARAMETER_A_);
	GUIEnvironment->addStaticText(L"c:", irr::core::rect<irr::s32>(90,65,999,999), false, false, fenster);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(105,60,160,80), true, fenster, _GUI_ID_EDITBOX_ALLGEMEIN_ZUFALLSPARAMETER_C_);
	GUIEnvironment->addStaticText(L"m:", irr::core::rect<irr::s32>(170,65,999,999), false, false, fenster);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(185,60,240,80), true, fenster, _GUI_ID_EDITBOX_ALLGEMEIN_ZUFALLSPARAMETER_M_);

	GUIEnvironment->addStaticText(L"Farbe der Hohle: (0.0 bis 1.0)", irr::core::rect<irr::s32>(10,100,999,999), false, false, fenster);
	GUIEnvironment->addStaticText(L"Rot:", irr::core::rect<irr::s32>(10,120,999,999), false, false, fenster);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(40,115,80,135), true, fenster, _GUI_ID_EDITBOX_FARBE_HOEHLE_R_);
	GUIEnvironment->addStaticText(L"Grun:", irr::core::rect<irr::s32>(90,120,999,999), false, false, fenster);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(120,115,160,135), true, fenster, _GUI_ID_EDITBOX_FARBE_HOEHLE_G_);
	GUIEnvironment->addStaticText(L"Blau:", irr::core::rect<irr::s32>(170,120,999,999), false, false, fenster);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(200,115,240,135), true, fenster, _GUI_ID_EDITBOX_FARBE_HOEHLE_B_);

	GUIEnvironment->addButton(irr::core::rect<irr::s32>(10,145,240,160),fenster,_GUI_ID_BUTTON_FARBE_HOEHLE_UEBERNEHMEN_,L"Anderungen ubernehmen");

	GUIEnvironment->addStaticText(L"Hintergrundfarbe des Renderfensters: (0 bis 255)", irr::core::rect<irr::s32>(10,175,999,999), false, false, fenster);
	GUIEnvironment->addStaticText(L"Rot:", irr::core::rect<irr::s32>(10,195,999,999), false, false, fenster);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(40,190,80,210), true, fenster, _GUI_ID_EDITBOX_FARBE_HINTERGRUND_R_);
	GUIEnvironment->addStaticText(L"Grun:", irr::core::rect<irr::s32>(90,195,999,999), false, false, fenster);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(120,190,160,210), true, fenster, _GUI_ID_EDITBOX_FARBE_HINTERGRUND_G_);
	GUIEnvironment->addStaticText(L"Blau:", irr::core::rect<irr::s32>(170,195,999,999), false, false, fenster);
	GUIEnvironment->addEditBox(L"", irr::core::rect<irr::s32>(200,190,240,210), true, fenster, _GUI_ID_EDITBOX_FARBE_HINTERGRUND_B_);

	GUIEnvironment->addButton(irr::core::rect<irr::s32>(10,220,240,235),fenster,_GUI_ID_BUTTON_FARBE_HINTERGRUND_UEBERNEHMEN_,L"Anderungen ubernehmen");

	GUIEnvironment->addButton(irr::core::rect<irr::s32>(10,250,240,270),fenster,_GUI_ID_BUTTON_MESHCACHE_TEXTUREBUFFER_LOESCHEN_,L"Meshcache & Texturebuffer (incl. Subszenen) loschen");

}

void DunGen::CGUI::ErstelleFenster_FileOpenDialog(irr::s32 _ID, bool Speichern)
{
	// Eigenen FileOpen-Dialog erstellen
	irr::gui::IGUIFileOpenDialog* fileOpenDialog;
	if (Speichern)
		fileOpenDialog = new irr::gui::CGUIFileOpenDialog_Modified(L"Bitte Datei zum Speichern auswahlen:", GUIEnvironment, GUIEnvironment->getRootGUIElement(), _ID);
	else
		fileOpenDialog = new irr::gui::CGUIFileOpenDialog_Modified(L"Bitte Datei zum Laden auswahlen:", GUIEnvironment, GUIEnvironment->getRootGUIElement(), _ID);
	fileOpenDialog->drop();

	// Modal = immer im Vordergrund
	//irr::gui::IGUIElement* modalScreen = GUIEnvironment->addModalScreen(NULL);
	//modalScreen->addChild(fileOpenDialog);
}

// alle GUI-Elemente entfernen
void DunGen::CGUI::LoescheGUI()
{
	GUIEnvironment->clear();
}
