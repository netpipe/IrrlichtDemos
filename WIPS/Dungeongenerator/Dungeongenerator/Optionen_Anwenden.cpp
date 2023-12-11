// Implementierung der Klasse Optionen
#include "Optionen.h"

// ======================================================
// Initialisierung
// ======================================================
void DunGen::COptionen::Initialisiere(irr::IrrlichtDevice* _IrrDevice, CGUI* _GUI, CSzene* _Szene,
	CFraktalGenerator* _FraktalGenerator, CVoxelRaum* _VoxelRaum, CDreiecksMesh* _DreiecksMesh, CArchitekt* _Architekt,
			CZufallsGenerator* _ZufallsGeneratorAllgemein, CZufallsGenerator* _ZufallsGeneratorVerwacklung, CZeitmesser* _Zeitmesser)
{
	// Objekte merken
	IrrDevice = _IrrDevice;
	FileSystem = _IrrDevice->getFileSystem();
	VideoDriver = _IrrDevice->getVideoDriver();
	SceneManager = _IrrDevice->getSceneManager();
	GUI = _GUI;
	Szene = _Szene;
	FraktalGenerator = _FraktalGenerator;
	VoxelRaum = _VoxelRaum;
	DreiecksMesh = _DreiecksMesh;
	Architekt = _Architekt;
	ZufallsGeneratorAllgemein = _ZufallsGeneratorAllgemein;
	ZufallsGeneratorVerwacklung = _ZufallsGeneratorVerwacklung;
	Zeitmesser = _Zeitmesser;

	// jetziges Arbeitsverzeichnis merken
	ArbeitsVerzeichnis = FileSystem->getWorkingDirectory();

	// Startwerte einstellen

	// Einstellungen Laden/Speichern
	Dateizugriff_Einstellungen_Fraktalparameter = true;
	Dateizugriff_Einstellungen_Fraktaleinstellungen_Weitere = true;
	Dateizugriff_Einstellungen_Voxelraum = true;
	Dateizugriff_Einstellungen_Gaenge = true;
	Dateizugriff_Einstellungen_Detailobjekte = true;
	Dateizugriff_Einstellungen_Architekt = true;
	Dateizugriff_Einstellungen_Subszenen = true;
	Dateizugriff_Einstellungen_Erstellung = true;
	Dateizugriff_Einstellungen_Weiteres = true;

	// ID des ausgewaehlten Listbox-Feldes (-1 ... keines)
	ListBox_Ersetzungsregeln_ID = -1;
	ListBox_Erosionschritte_ID = -1;
	ListBox_Gangpunkte_ID = -1;
	ListBox_Detailobjekte_ID = -1;
	ListBox_Subszenen_ID = -1;
	
	// FraktalEditor
	WinkelGier_Min = 0.0;
	WinkelGier_Max = 360.0;
	WinkelGier_Kommastellen = 0;

	WinkelNick_Min = 0.0;
	WinkelNick_Max = 360.0;
	WinkelNick_Kommastellen = 0;

	WinkelRoll_Min = 0.0;
	WinkelRoll_Max = 360.0;
	WinkelRoll_Kommastellen = 0;

	StartRadius_Min = 10.0;
	StartRadius_Max = 30.0;
	StartRadius_Kommastellen = 0;

	RadiusFaktor_Min = 0.5;
	RadiusFaktor_Max = 0.99;
	RadiusFaktor_Kommastellen = 2;

	RadiusDekrementor_Min = 1.0;
	RadiusDekrementor_Max = 2.0;
	RadiusDekrementor_Kommastellen = 0;

	Start_ErlaubteSymbole = "FFFF![]+-ouzg|$";
	Start_MinLaenge = 5;
	Start_MaxLaenge = 10;

	Symbol = 'F';
	Symbolersetzung = "FXY";
	Ersetzung_ErlaubteSymbole = "FFXY![]+-ouzg|$";
	Ersetzung_MinLaenge = 5;
	Ersetzung_MaxLaenge = 10;

	Iterationsstufe = 7;
	
	// Fraktal
	Fraktal = FraktalGenerator->GeneriereAbleitungen(0);

	// VoxelRaum
	ErosionsZufallsSeed = 0;
	VorgegebenenErosionsZufallsSeedNutzen = true; 
	Erosionswahrscheinlichkeit = 0.5;
	SchwebendeFragmenteFiltern = false;
	DreiecksMesh_Detailstufen = 4;	
	// Flags
	Dichte_berechnet = false;

	// Architekt
	KonstruktionsZufallsSeed = 0;
	Raumsektorausdehnung = 160;
	VorgegebenenKonstruktionsZufallsSeedNutzen = false;		

	// Erstellung
	Erstellung_Fraktalzeichenanweisung = true;
	Erstellung_VoxelZeichnen_Erosion = true;
	Erstellung_Filterung1 = true;
	Erstellung_Architekt = true;
	Erstellung_Filterung2 = true;
	Erstellung_UmwandlungVoxel = true;	

	// Weiters
	Hintergrundfarbe = irr::video::SColor(255,50,50,50);

	// Standard-Einstellungen laden
	// ueberschreibt obige Einstellungen, soweit enthalten	
	Lade_Einstellungen("Einstellungen_Standard.xml");

	// GUI anschalten
	GUIan();
}

// Werte auslesen
irr::video::SColor DunGen::COptionen::LeseHintergrundfarbe()
{
	return Hintergrundfarbe;
}


// ======================================================
// Buttons Fraktaleditor
// ======================================================

void DunGen::COptionen::Button_ZufallsParameter()
{	
	// Zufallsparameter bestimmen
	FraktalGenerator->SetzeZufallsParameter(ELSystemEigenschaft::_LSYSTEMEIGENSCHAFT_WINKEL_GIER_, WinkelGier_Min, WinkelGier_Max, WinkelGier_Kommastellen);
	FraktalGenerator->SetzeZufallsParameter(ELSystemEigenschaft::_LSYSTEMEIGENSCHAFT_WINKEL_NICK_, WinkelNick_Min, WinkelNick_Max, WinkelNick_Kommastellen);
	FraktalGenerator->SetzeZufallsParameter(ELSystemEigenschaft::_LSYSTEMEIGENSCHAFT_WINKEL_ROLL_, WinkelRoll_Min, WinkelRoll_Max, WinkelRoll_Kommastellen);
	FraktalGenerator->SetzeZufallsParameter(ELSystemEigenschaft::_LSYSTEMEIGENSCHAFT_START_RADIUS_, StartRadius_Min, StartRadius_Max, StartRadius_Kommastellen);
	FraktalGenerator->SetzeZufallsParameter(ELSystemEigenschaft::_LSYSTEMEIGENSCHAFT_RADIUS_FAKTOR_, RadiusFaktor_Min, RadiusFaktor_Max, RadiusFaktor_Kommastellen);
	FraktalGenerator->SetzeZufallsParameter(ELSystemEigenschaft::_LSYSTEMEIGENSCHAFT_RADIUS_DEKREMENTOR_, RadiusDekrementor_Min, RadiusDekrementor_Max, RadiusDekrementor_Kommastellen);
}
void DunGen::COptionen::Button_ZufallsStart()
{	
	// Zufallswert setzen
	FraktalGenerator->SetzeZufallsStart(Start_ErlaubteSymbole,Start_MinLaenge,Start_MaxLaenge);
}
void DunGen::COptionen::Button_ZufallsRegel()
{	
	// Zufallswert setzen
	FraktalGenerator->FuegeZufallsRegelHinzu(Symbol,Ersetzung_ErlaubteSymbole,Ersetzung_MinLaenge,Ersetzung_MaxLaenge);
}
void DunGen::COptionen::Button_RegelHinzufuegen()
{	
	FraktalGenerator->FuegeRegelHinzu(Symbol,Symbolersetzung);
}
void DunGen::COptionen::Button_RegelnLoeschen()
{	
	FraktalGenerator->LoescheRegeln();
}
void DunGen::COptionen::Button_FraktalGenerieren()
{
	// letztes Fraktal loeschen
	delete Fraktal;
	// neues Fraktal generieren
	Fraktal = FraktalGenerator->GeneriereAbleitungen(Iterationsstufe);
	// Iterationstufe aktualisieren: eventuell niedriger als gewuenscht, falls String zu lang wird
	Iterationsstufe = Fraktal->FraktalIteration.size()-1;
}

void DunGen::COptionen::Button_ZufallsFraktalGenerieren()
{
	// Parameter auswuerfeln
	Button_ZufallsParameter();
	Button_ZufallsStart();
	Button_RegelnLoeschen();

	// Bestimme zu ersetzende Symbole
	std::string verwendeteSymbole = Start_ErlaubteSymbole + Ersetzung_ErlaubteSymbole;
	std::string zuErsetzendeSymbole = "F";
	char aktZeichen;
	bool vorhanden;

	for (unsigned i = 0; i<verwendeteSymbole.size();i++)
	{
		aktZeichen = verwendeteSymbole.at(i);
		// kein erlaubtes Symbol?
		if (aktZeichen != '[' && aktZeichen != ']' && aktZeichen != '!'
			&& aktZeichen != '+' && aktZeichen != '-'
			&& aktZeichen != 'u' && aktZeichen != 'o' 
			&& aktZeichen != 'g' && aktZeichen != 'z' 
			&& aktZeichen != '|' && aktZeichen != '$')
		{
			//std::cout << "zuErsetzendeSymbole: " << zuErsetzendeSymbole << std::endl;

			vorhanden = false;
			// kein schon vorhandenes Symbol?
			for (unsigned j = 0; j<zuErsetzendeSymbole.size(); j++)
				if (aktZeichen == zuErsetzendeSymbole.at(j))
					vorhanden = true;

			if (!vorhanden)
				zuErsetzendeSymbole.append(1,aktZeichen);
		}
	}	

	// Regeln erstellen
	for (unsigned i = 0; i<zuErsetzendeSymbole.size(); i++)
		FraktalGenerator->FuegeZufallsRegelHinzu(zuErsetzendeSymbole.at(i),Ersetzung_ErlaubteSymbole,Ersetzung_MinLaenge,Ersetzung_MaxLaenge);

	// Generierung
	Button_FraktalGenerieren();
}


// ======================================================
// Buttons Voxelhoehle
// ======================================================

void DunGen::COptionen::Button_Erosionsschritt_hinzufuegen()
{
	// Schritt hinzufuegen
	ErosionsSchritte.push_back(Erosionswahrscheinlichkeit);
}
void DunGen::COptionen::Button_Erosionsschritte_loeschen()
{
	// Schritt loeschen
	ErosionsSchritte.clear();
}

void DunGen::COptionen::Button_Voxel_berechnen()
{
	// Fraktal im Voxelraum zeichnen
	VoxelRaum->ZeichneGrafik(Fraktal->FraktalIteration.at(Fraktal->FraktalIteration.size()-1),
		Fraktal->WinkelGier,Fraktal->WinkelNick,Fraktal->WinkelRoll,
		Fraktal->StartRadius,Fraktal->RadiusFaktor,Fraktal->RadiusDekrementor);

	// Nachbearbeitung: Erosion
	if (ErosionsSchritte.size()>0)
	{
		if (VorgegebenenErosionsZufallsSeedNutzen)
			ZufallsGeneratorAllgemein->SetzeZufallsSeed(ErosionsZufallsSeed);
		else
			ErosionsZufallsSeed = ZufallsGeneratorAllgemein->GeneriereZufallsWertBasis();
	
		// Erosionsschritte durchfuehren
		for (unsigned int i = 0; i < ErosionsSchritte.size(); i++)
			VoxelRaum->Erodiere(ErosionsSchritte.at(i));

		// Zufallsseed wieder randomisieren
		ZufallsGeneratorAllgemein->SetzeZufallsSeed(time(NULL));
	}

	// Nachbearbeitung: Schwebende Fragmente entfernen
	if (SchwebendeFragmenteFiltern)
		VoxelRaum->FilterAnwenden();

	// Flags setzen
	Dichte_berechnet = false;

	// Polygone bei Umwandlung schaetzen
	VoxelRaum->ErmittleDreiecksNetzDaten();
}
void DunGen::COptionen::Button_Zeige_Dichte(EEbene _Ebene)
{
	// Berechnung
	if (!Dichte_berechnet)
		// Dichte des Voxelraums berechnen
		VoxelRaum->BerechneDichte();

	// Zeichenmodus aktualisieren
	Szene->SzeneLoeschen(false);
	switch (_Ebene)
	{
	case EEbene::_EBENE_XY_:
		Szene->SzeneErstellen(EZeichenModus::_ZEICHENMODUS_EBENE_XY_, !Dichte_berechnet);
		break;
	case EEbene::_EBENE_XZ_:
		Szene->SzeneErstellen(EZeichenModus::_ZEICHENMODUS_EBENE_XZ_, !Dichte_berechnet);
		break;
	case EEbene::_EBENE_YZ_:
		Szene->SzeneErstellen(EZeichenModus::_ZEICHENMODUS_EBENE_YZ_, !Dichte_berechnet);
		break;
	}

	// Flag setzen
	Dichte_berechnet = true;
}

void DunGen::COptionen::Button_Voxel_Zu_Dreiecksnetz_Berechnen()
{
	// Umwandlung
	DreiecksMesh->WandleVoxelInMesh(DreiecksMesh_Detailstufen);
}

void DunGen::COptionen::Button_Zeige_Drahtgittermodell()
{
	// Zeichenmodus aktualisieren
	Szene->SzeneLoeschen(false);
	Szene->SzeneErstellen(EZeichenModus::_ZEICHENMODUS_DREICKSMESH_, true);
}

// ======================================================
// Buttons Gaenge
// ======================================================
void DunGen::COptionen::Button_Neu_Gangpunkt()
{
	// neues letztes Element auswaehlen
	ListBox_Gangpunkte_ID = Architekt->QuerschnittGang_Normiert.Punkt.size();

	// neuen Punkt hinzufuegen
	irr::core::vector2d<double> neuerPunkt;
	double neuesTextureX;
	Architekt->QuerschnittGang_Normiert.Punkt.push_back(neuerPunkt);
	Architekt->QuerschnittGang_Normiert.TextureX.push_back(neuesTextureX);

	// Werte einstellen
	Button_Uebernehmen_Gangpunkt();
}

void DunGen::COptionen::Button_Uebernehmen_Gangpunkt()
{
	// gueltiger Index?
	if (ListBox_Gangpunkte_ID<0 || ListBox_Gangpunkte_ID>=Architekt->QuerschnittGang_Normiert.Punkt.size())
		return;

	// Rootelement bestimmen
	irr::gui::IGUIElement* root = GUI->GUIEnvironment->getRootGUIElement();
	// Test ob Fenster vorhanden nicht noetig, da sich der Button in diesem befindet

	irr::gui::IGUIEditBox* editBox;
	irr::core::stringc tempString;
	
	// Werte auslesen und uebergeben
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_PUNKT_X_, true);
	tempString = editBox->getText();
	Architekt->QuerschnittGang_Normiert.Punkt.at(ListBox_Gangpunkte_ID).X = atof(tempString.c_str());

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_PUNKT_Y_, true);
	tempString = editBox->getText();
	Architekt->QuerschnittGang_Normiert.Punkt.at(ListBox_Gangpunkte_ID).Y = atof(tempString.c_str());

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_PUNKT_TEXTUREX_, true);
	tempString = editBox->getText();
	Architekt->QuerschnittGang_Normiert.TextureX.at(ListBox_Gangpunkte_ID) = atof(tempString.c_str());

}
void DunGen::COptionen::Button_Loeschen_Gangpunkt()
{
	// Punkt aus Gangquerschnitt entfernen
	if (ListBox_Gangpunkte_ID>=0 && ListBox_Gangpunkte_ID<Architekt->QuerschnittGang_Normiert.Punkt.size())
	{
		Architekt->QuerschnittGang_Normiert.Punkt.erase(Architekt->QuerschnittGang_Normiert.Punkt.begin()+ListBox_Gangpunkte_ID);
		Architekt->QuerschnittGang_Normiert.TextureX.erase(Architekt->QuerschnittGang_Normiert.TextureX.begin()+ListBox_Gangpunkte_ID);
	}
}

void DunGen::COptionen::Button_Uebernehmen_Detailobjekt()
{
	// gueltiger Index?
	if (ListBox_Detailobjekte_ID<0 || ListBox_Detailobjekte_ID>=Architekt->DetailobjektVorlagen.size())
		return;

	// Rootelement bestimmen
	irr::gui::IGUIElement* root = GUI->GUIEnvironment->getRootGUIElement();
	// Test ob Fenster vorhanden nicht noetig, da sich der Button in diesem befindet

	irr::gui::IGUIEditBox* editBox;
	irr::gui::IGUICheckBox* checkBox;
	irr::core::stringc tempString;
	
	// Werte auslesen und uebergeben
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_DETAILOBJEKT_BENENNUNG_, true);
	tempString = editBox->getText();
	Architekt->DetailobjektVorlagen.at(ListBox_Detailobjekte_ID)->Name = tempString;

	// Skalierung
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_DETAILOBJEKT_SKALIERUNG_X_, true);
	tempString = editBox->getText();
	Architekt->DetailobjektVorlagen.at(ListBox_Detailobjekte_ID)->Skalierung.X = atof(tempString.c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_DETAILOBJEKT_SKALIERUNG_Y_, true);
	tempString = editBox->getText();
	Architekt->DetailobjektVorlagen.at(ListBox_Detailobjekte_ID)->Skalierung.Y = atof(tempString.c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_DETAILOBJEKT_SKALIERUNG_Z_, true);
	tempString = editBox->getText();
	Architekt->DetailobjektVorlagen.at(ListBox_Detailobjekte_ID)->Skalierung.Z = atof(tempString.c_str());

	// Rotation
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_DETAILOBJEKT_ROTATION_X_, true);
	tempString = editBox->getText();
	Architekt->DetailobjektVorlagen.at(ListBox_Detailobjekte_ID)->Rotation.X = atof(tempString.c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_DETAILOBJEKT_ROTATION_Y_, true);
	tempString = editBox->getText();
	Architekt->DetailobjektVorlagen.at(ListBox_Detailobjekte_ID)->Rotation.Y = atof(tempString.c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_DETAILOBJEKT_ROTATION_Z_, true);
	tempString = editBox->getText();
	Architekt->DetailobjektVorlagen.at(ListBox_Detailobjekte_ID)->Rotation.Z = atof(tempString.c_str());

	// Position
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_DETAILOBJEKT_POSITION_X_, true);
	tempString = editBox->getText();
	Architekt->DetailobjektVorlagen.at(ListBox_Detailobjekte_ID)->Position.X = atof(tempString.c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_DETAILOBJEKT_POSITION_Y_, true);
	tempString = editBox->getText();
	Architekt->DetailobjektVorlagen.at(ListBox_Detailobjekte_ID)->Position.Y = atof(tempString.c_str());

	// Platzierung
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_DETAILOBJEKT_ABTASTUNG_, true);
	tempString = editBox->getText();
	double tempDouble = atof(tempString.c_str());
	if (tempDouble < 10*DunGen::CSpline::Genauigkeit) tempDouble = 10*DunGen::CSpline::Genauigkeit;
	Architekt->DetailobjektVorlagen.at(ListBox_Detailobjekte_ID)->AbstandsAbtastung = tempDouble;
	
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_DETAILOBJEKT_FAKTOR_NUMABSTAND_, true);
	tempString = editBox->getText();
	int tempInt = atoi(tempString.c_str());
	if (tempInt<1) tempInt=1;
	Architekt->DetailobjektVorlagen.at(ListBox_Detailobjekte_ID)->AbstandNumFaktor = tempInt;

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_DETAILOBJEKT_NUMABSTAND_MIN_, true);
	tempString = editBox->getText();
	tempInt = atoi(tempString.c_str());
	if (tempInt<1) tempInt=1;
	Architekt->DetailobjektVorlagen.at(ListBox_Detailobjekte_ID)->AbstandNumMin = tempInt;

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_DETAILOBJEKT_NUMABSTAND_MAX_, true);
	tempString = editBox->getText();
	int tempInt2 = atoi(tempString.c_str());
	if (tempInt2<tempInt) tempInt2=tempInt;
	Architekt->DetailobjektVorlagen.at(ListBox_Detailobjekte_ID)->AbstandNumMax = tempInt2;

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_DETAILOBJEKT_NUMABSTAND_MIN_ANFANG_, true);
	tempString = editBox->getText();
	tempInt = atoi(tempString.c_str());
	if (tempInt<0) tempInt=0;
	Architekt->DetailobjektVorlagen.at(ListBox_Detailobjekte_ID)->AbstandNumMinErstesElement = tempInt;

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_DETAILOBJEKT_NUMABSTAND_MAX_ANFANG_, true);
	tempString = editBox->getText();
	tempInt2 = atoi(tempString.c_str());
	if (tempInt2<tempInt) tempInt2=tempInt;
	Architekt->DetailobjektVorlagen.at(ListBox_Detailobjekte_ID)->AbstandNumMaxErstesElement = tempInt2;

	// Checkboxes auslesen
	checkBox = (irr::gui::IGUICheckBox*) root->getElementFromId(_GUI_ID_CHECKBOX_OBJEKT_GANGENDE_, true);
	Architekt->DetailobjektVorlagen.at(ListBox_Detailobjekte_ID)->ObjektAn1 = checkBox->isChecked();

}
void DunGen::COptionen::Button_Loeschen_Detailobjekt()
{
	// Detailobjekt entfernen
	if (ListBox_Detailobjekte_ID>=0 && ListBox_Detailobjekte_ID<Architekt->DetailobjektVorlagen.size())
	{
		delete Architekt->DetailobjektVorlagen.at(ListBox_Detailobjekte_ID);
		Architekt->DetailobjektVorlagen.erase(Architekt->DetailobjektVorlagen.begin()+ListBox_Detailobjekte_ID);
	}
}

// ======================================================
// Buttons Architekt
// ======================================================

void DunGen::COptionen::Button_Uebernehmen_Subszene()
{
	// gueltiger Index?
	if (ListBox_Subszenen_ID<0 || ListBox_Subszenen_ID>=Architekt->SubSzenen.size())
		return;

	// Rootelement bestimmen
	irr::gui::IGUIElement* root = GUI->GUIEnvironment->getRootGUIElement();
	// Test ob Fenster vorhanden nicht noetig, da sich der Button in diesem befindet

	irr::gui::IGUIEditBox* editBox;
	irr::core::stringc tempString;
	irr::core::vector3d<double> tempVec3D;
	
	// Werte auslesen und uebergeben
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_SUBSZENE_BENENNUNG_, true);
	tempString = editBox->getText();
	Architekt->SubSzenen.at(ListBox_Subszenen_ID)->SetzeName(tempString);

	// Skalierung
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_SUBSZENE_SKALIERUNG_X_, true);
	tempString = editBox->getText();
	tempVec3D.X = atof(tempString.c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_SUBSZENE_SKALIERUNG_Y_, true);
	tempString = editBox->getText();
	tempVec3D.Y = atof(tempString.c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_SUBSZENE_SKALIERUNG_Z_, true);
	tempString = editBox->getText();
	tempVec3D.Z = atof(tempString.c_str());

	Architekt->SubSzenen.at(ListBox_Subszenen_ID)->SetzeSkalierung(tempVec3D);

	// Platzierungswahrscheinlichkeitsfaktoren
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_SUBSZENE_WAHRSCHEINLICHKEIT_HOEHLE_, true);
	tempString = editBox->getText();
	Architekt->SubSzenen.at(ListBox_Subszenen_ID)->SetzePlatzierungsWahrscheinlichkeitsFaktorAnHoehle(atoi(tempString.c_str()));
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_SUBSZENE_WAHRSCHEINLICHKEIT_RAUM_, true);
	tempString = editBox->getText();
	Architekt->SubSzenen.at(ListBox_Subszenen_ID)->SetzePlatzierungsWahrscheinlichkeitsFaktorAnRaum(atoi(tempString.c_str()));

	// Gangsplineableitungsstaerken
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_SUBSZENE_GANGSPLINEABLEITUNG_RAUM_RAUM_, true);
	tempString = editBox->getText();
	Architekt->SubSzenen.at(ListBox_Subszenen_ID)->SetzeAbleitungsFaktorRaumRaum(atof(tempString.c_str()));
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_SUBSZENE_GANGSPLINEABLEITUNG_RAUM_HOEHLE_, true);
	tempString = editBox->getText();
	Architekt->SubSzenen.at(ListBox_Subszenen_ID)->SetzeAbleitungsFaktorRaumHoehle(atof(tempString.c_str()));
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_SUBSZENE_GANGSPLINEABLEITUNG_HOEHLE_RAUM_, true);
	tempString = editBox->getText();
	Architekt->SubSzenen.at(ListBox_Subszenen_ID)->SetzeAbleitungsFaktorHoehleRaum(atof(tempString.c_str()));
}
void DunGen::COptionen::Button_Loeschen_Subszene()
{
	// Subszene entfernen
	if (ListBox_Subszenen_ID>=0 && ListBox_Subszenen_ID<Architekt->SubSzenen.size())
	{
		delete Architekt->SubSzenen.at(ListBox_Subszenen_ID);
		Architekt->SubSzenen.erase(Architekt->SubSzenen.begin()+ListBox_Subszenen_ID);
	}
}


// ======================================================
// Buttons Erstellung
// ======================================================
void DunGen::COptionen::Button_Erstelle_Dungeon()
{
	std::cout << "[Optionen]: Beginne mit der Dungeongenerierung ..." << std::endl;
	Zeitmesser->Start(3);

	// SCHRITT 1: Fraktalzeichenanweisung erstellen
	if (Erstellung_Fraktalzeichenanweisung)
		Button_FraktalGenerieren();

	// SCHRITT 2: Zeichnen im Voxelraum + Erosion
	if (Erstellung_VoxelZeichnen_Erosion)
	{
		// Fraktal im Voxelraum zeichnen
		VoxelRaum->ZeichneGrafik(Fraktal->FraktalIteration.at(Fraktal->FraktalIteration.size()-1),
			Fraktal->WinkelGier,Fraktal->WinkelNick,Fraktal->WinkelRoll,
			Fraktal->StartRadius,Fraktal->RadiusFaktor,Fraktal->RadiusDekrementor);

		// Nachbearbeitung: Erosion
		if (ErosionsSchritte.size()>0)
		{		
			if (VorgegebenenErosionsZufallsSeedNutzen)
				ZufallsGeneratorAllgemein->SetzeZufallsSeed(ErosionsZufallsSeed);
			else
				ErosionsZufallsSeed = ZufallsGeneratorAllgemein->GeneriereZufallsWertBasis();
			std::cout << "[Optionen]: Setze Zufallsseed auf: " << ErosionsZufallsSeed << std::endl;
	
			// Erosionsschritte durchfuehren
			for (unsigned int i = 0; i < ErosionsSchritte.size(); i++)
				VoxelRaum->Erodiere(ErosionsSchritte.at(i));

			// Zufallsseed wieder randomisieren
			ZufallsGeneratorAllgemein->SetzeZufallsSeed(time(NULL));
		}

		// Flags setzen
		Dichte_berechnet = false;

		// Polygone bei Umwandlung schaetzen
		VoxelRaum->ErmittleDreiecksNetzDaten();
	}

	// SCHRITT 3: Filterung schwebender Fragmente 1
	if (Erstellung_Filterung1)
		VoxelRaum->FilterAnwenden();

	// SCHRITT 4: Konstruktion des Dungeons: Gaenge und Raeume
	if (Erstellung_Architekt)
	{
		// Vorbereitung:
		// Gangquerschnitt auf Korrektheit pruefen
		if (Architekt->QuerschnittGang_Normiert.Punkt.size() == 0)
		{
				std::cout << "[Optionen]: Fehler: Gangquerschnitt besitzt keine Punkte -> Erstellung wird abgebrochen." << std::endl;
				return;
		}
		// Meshes der Detailobjekte einladen
		for (unsigned int i=0; i<Architekt->DetailobjektVorlagen.size(); i++)
		{
			Architekt->DetailobjektVorlagen.at(i)->ObjektMesh = IrrDevice->getSceneManager()->getMesh(Architekt->DetailobjektVorlagen.at(i)->MeshPfad);
			if (Architekt->DetailobjektVorlagen.at(i)->ObjektMesh == NULL)
			{
				std::cout << "[Optionen]: Fehler beim Einladen von Detailobjekt-Mesh " << i+1 << " -> Erstellung wird abgebrochen." << std::endl;
				return;
			}
		}
		// Subszenen auf Korrektheit pruefen
		for (unsigned int i=0; i<Architekt->SubSzenen.size(); i++)
			for (unsigned int j=0; j<4; j++)
				if (Architekt->SubSzenen.at(i)->LeseAndockstelle(j).Punkt.size() == 0)
				{
					std::cout << "[Optionen]: Fehler bei SubSzene " << i+1 << ": Andockstelle " << j << " besitzt keine Punkte -> Erstellung wird abgebrochen." << std::endl;
					return;
				}

		// Ausfuehrung:
		if (VorgegebenenKonstruktionsZufallsSeedNutzen)
			ZufallsGeneratorAllgemein->SetzeZufallsSeed(KonstruktionsZufallsSeed);
		else
			KonstruktionsZufallsSeed = ZufallsGeneratorAllgemein->GeneriereZufallsWertBasis();
		std::cout << "[Optionen]: Setze Zufallsseed auf: " << KonstruktionsZufallsSeed << std::endl;

		Architekt->KonstruiereDungeon(Raumsektorausdehnung);

		// Zufallsseed wieder randomisieren
		ZufallsGeneratorAllgemein->SetzeZufallsSeed(time(NULL));
	}
	
	// SCHRITT 5: Filterung schwebender Fragmente 1
	if (Erstellung_Filterung2)
		VoxelRaum->FilterAnwenden();

	// SCHRITT 6: Umwandlung Voxelhoehle in Dreiecksnetz
	if (Erstellung_UmwandlungVoxel)
		Button_Voxel_Zu_Dreiecksnetz_Berechnen();

	std::cout << "[Optionen]: Dungeongenerierung abgeschlossen. Benoetigte Zeit: ";
	Zeitmesser->Stop(3);
}

void DunGen::COptionen::Button_Zeige_Dungeon()
{
	// Zeichenmodus aktualisieren
	Szene->SzeneLoeschen(false);	
	Szene->SzeneErstellen(EZeichenModus::_ZEICHENMODUS_DUNGEON_, true);
}

// ======================================================
// Buttons Weiteres
// ======================================================
		
void DunGen::COptionen::Button_Farbe_Hoehle_uebernehmen()
{
	// Rootelement bestimmen
	irr::gui::IGUIElement* root = GUI->GUIEnvironment->getRootGUIElement();
	// Test ob Fenster vorhanden nicht noetig, da sich der Button in diesem befindet
		
	irr::gui::IGUIEditBox* editBox;
	irr::core::stringc tempString;
	irr::video::SColorf farbeHoehle;
	farbeHoehle.a = 1.0;

	// Farbe bestimmen
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_FARBE_HOEHLE_R_, true);
	tempString = editBox->getText();
	farbeHoehle.r = atof(tempString.c_str());

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_FARBE_HOEHLE_G_, true);
	tempString = editBox->getText();
	farbeHoehle.g = atof(tempString.c_str());

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_FARBE_HOEHLE_B_, true);
	tempString = editBox->getText();
	farbeHoehle.b = atof(tempString.c_str());
	
	// Farbe uebergeben
	Szene->LeseCallbackHoehlenShader()->SetzeFarbe(farbeHoehle);
}

void DunGen::COptionen::Button_Farbe_Hintergrund_uebernehmen()
{
	// Rootelement bestimmen
	irr::gui::IGUIElement* root = GUI->GUIEnvironment->getRootGUIElement();
	// Test ob Fenster vorhanden nicht noetig, da sich der Button in diesem befindet

	irr::gui::IGUIEditBox* editBox;
	irr::core::stringc tempString;

	// Farbe auslesen und uebergeben
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_FARBE_HINTERGRUND_R_, true);
	tempString = editBox->getText();
	Hintergrundfarbe.setRed(atoi(tempString.c_str()));

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_FARBE_HINTERGRUND_G_, true);
	tempString = editBox->getText();
	Hintergrundfarbe.setGreen(atoi(tempString.c_str()));

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_FARBE_HINTERGRUND_B_, true);
	tempString = editBox->getText();
	Hintergrundfarbe.setBlue(atoi(tempString.c_str()));
}

void DunGen::COptionen::Button_Meshcache_Texturebuffer_loeschen()
{	
	// Szene, Meshcache und Texturebuffer loeschen
	Szene->SzeneLoeschen(true);
	Szene->SzeneErstellen(EZeichenModus::_ZEICHENMODUS_NICHTS_, true);
}
