// Implementierung der Klasse Optionen
#include "Optionen.h"

// ======================================================
// Fensterinhalt merken
// ======================================================

void DunGen::COptionen::MerkeFenster()
{
	MerkeFenster_Dateizugriff_Einstellungen();
	MerkeFenster_FraktalEditor();
	MerkeFenster_VoxelRaum();
	MerkeFenster_Gaenge();
	MerkeFenster_Architekt();
	MerkeFenster_Erstellung();
	MerkeFenster_Weiteres();
}

void DunGen::COptionen::MerkeFenster_Dateizugriff_Einstellungen()
{
	// Rootelement bestimmen
	irr::gui::IGUIElement* root = GUI->GUIEnvironment->getRootGUIElement();
	// Test ob Fenster vorhanden
	irr::gui::IGUIElement* fenster = root->getElementFromId(_GUI_ID_WINDOW_DATEIZUGRIFF_EINSTELLUNGEN_, true);
    if (!fenster)
		return;

	irr::gui::IGUICheckBox* checkBox;
	
	checkBox = (irr::gui::IGUICheckBox*) root->getElementFromId(_GUI_ID_CHECKBOX_DATEI_EINSTELLUNGEN_FRAKTALPARAMETER_, true);
	Dateizugriff_Einstellungen_Fraktalparameter = checkBox->isChecked();
	checkBox = (irr::gui::IGUICheckBox*) root->getElementFromId(_GUI_ID_CHECKBOX_DATEI_EINSTELLUNGEN_WEITERE_FRAKTALEINSTELLUNGEN_, true);
	Dateizugriff_Einstellungen_Fraktaleinstellungen_Weitere = checkBox->isChecked();
	checkBox = (irr::gui::IGUICheckBox*) root->getElementFromId(_GUI_ID_CHECKBOX_DATEI_EINSTELLUNGEN_VOXELRAUM_, true);
	Dateizugriff_Einstellungen_Voxelraum = checkBox->isChecked();

	checkBox = (irr::gui::IGUICheckBox*) root->getElementFromId(_GUI_ID_CHECKBOX_DATEI_EINSTELLUNGEN_GAENGE_, true);
	Dateizugriff_Einstellungen_Gaenge = checkBox->isChecked();
	checkBox = (irr::gui::IGUICheckBox*) root->getElementFromId(_GUI_ID_CHECKBOX_DATEI_EINSTELLUNGEN_DETAILOBJEKTE_, true);
	Dateizugriff_Einstellungen_Detailobjekte = checkBox->isChecked();
	checkBox = (irr::gui::IGUICheckBox*) root->getElementFromId(_GUI_ID_CHECKBOX_DATEI_EINSTELLUNGEN_ARCHITEKT_, true);
	Dateizugriff_Einstellungen_Architekt = checkBox->isChecked();

	checkBox = (irr::gui::IGUICheckBox*) root->getElementFromId(_GUI_ID_CHECKBOX_DATEI_EINSTELLUNGEN_SUBSZENEN_, true);
	Dateizugriff_Einstellungen_Subszenen = checkBox->isChecked();
	checkBox = (irr::gui::IGUICheckBox*) root->getElementFromId(_GUI_ID_CHECKBOX_DATEI_EINSTELLUNGEN_ERSTELLUNG_, true);
	Dateizugriff_Einstellungen_Erstellung = checkBox->isChecked();
	checkBox = (irr::gui::IGUICheckBox*) root->getElementFromId(_GUI_ID_CHECKBOX_DATEI_EINSTELLUNGEN_WEITERES_, true);
	Dateizugriff_Einstellungen_Weiteres = checkBox->isChecked();
}

void DunGen::COptionen::MerkeFenster_FraktalEditor()
{
	// Rootelement bestimmen
	irr::gui::IGUIElement* root = GUI->GUIEnvironment->getRootGUIElement();
	// Test ob Fenster vorhanden
	irr::gui::IGUIElement* fenster = root->getElementFromId(_GUI_ID_WINDOW_FRAKTALEDITOR_, true);
    if (!fenster)
		return;

	irr::gui::IGUIListBox* listBox;

	// ListBox: Ersetzungsregeln
	listBox = (irr::gui::IGUIListBox*) root->getElementFromId(_GUI_ID_LISTBOX_ERSETZUNGSREGELN_, true);
	ListBox_Ersetzungsregeln_ID = listBox->getSelected();

	// Werte bestimmen: WinkelGier
	irr::gui::IGUIEditBox* editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_WINKELGIER_WERT_, true);
	irr::core::stringc tempString = editBox->getText();
	FraktalGenerator->SetzeParameter(ELSystemEigenschaft::_LSYSTEMEIGENSCHAFT_WINKEL_GIER_,atof(tempString.c_str()));

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_WINKELGIER_MIN_, true);
	tempString = editBox->getText();
	WinkelGier_Min = atof(tempString.c_str());

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_WINKELGIER_MAX_, true);
	tempString = editBox->getText();
	WinkelGier_Max = atof(tempString.c_str());
	if (WinkelGier_Max < WinkelGier_Min) WinkelGier_Max = WinkelGier_Min;

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_WINKELGIER_KOMMASTELLEN_, true);
	tempString = editBox->getText();
	WinkelGier_Kommastellen = atoi(tempString.c_str());

	// Werte bestimmen: WinkelNick
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_WINKELNICK_WERT_, true);
	tempString = editBox->getText();
	FraktalGenerator->SetzeParameter(ELSystemEigenschaft::_LSYSTEMEIGENSCHAFT_WINKEL_NICK_,atof(tempString.c_str()));

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_WINKELNICK_MIN_, true);
	tempString = editBox->getText();
	WinkelNick_Min = atof(tempString.c_str());

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_WINKELNICK_MAX_, true);
	tempString = editBox->getText();
	WinkelNick_Max = atof(tempString.c_str());
	if (WinkelNick_Max < WinkelNick_Min) WinkelNick_Max = WinkelNick_Min;

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_WINKELNICK_KOMMASTELLEN_, true);
	tempString = editBox->getText();
	WinkelNick_Kommastellen = atoi(tempString.c_str());

	// Werte bestimmen: WinkelRoll
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_WINKELROLL_WERT_, true);
	tempString = editBox->getText();
	FraktalGenerator->SetzeParameter(ELSystemEigenschaft::_LSYSTEMEIGENSCHAFT_WINKEL_ROLL_,atof(tempString.c_str()));

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_WINKELROLL_MIN_, true);
	tempString = editBox->getText();
	WinkelRoll_Min = atof(tempString.c_str());

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_WINKELROLL_MAX_, true);
	tempString = editBox->getText();
	WinkelRoll_Max = atof(tempString.c_str());
	if (WinkelRoll_Max < WinkelRoll_Min) WinkelRoll_Max = WinkelRoll_Min;

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_WINKELROLL_KOMMASTELLEN_, true);
	tempString = editBox->getText();
	WinkelRoll_Kommastellen = atoi(tempString.c_str());

	// Werte bestimmen: StartRadius
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_RADIUSSTART_WERT_, true);
	tempString = editBox->getText();
	double tempDouble = atof(tempString.c_str());
	if (tempDouble < 0.0) tempDouble = 0.0;
	if (tempDouble > 128.0) tempDouble = 128.0;
	FraktalGenerator->SetzeParameter(ELSystemEigenschaft::_LSYSTEMEIGENSCHAFT_START_RADIUS_,tempDouble);
	
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_RADIUSSTART_MIN_, true);
	tempString = editBox->getText();
	StartRadius_Min = atof(tempString.c_str());
	if (StartRadius_Min < 0.0) StartRadius_Min = 0.0;
	if (StartRadius_Min > 128.0) StartRadius_Min = 128.0;
		
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_RADIUSSTART_MAX_, true);
	tempString = editBox->getText();
	StartRadius_Max = atof(tempString.c_str());
	if (StartRadius_Max < StartRadius_Min) StartRadius_Max = StartRadius_Min;
	if (StartRadius_Max > 128.0) StartRadius_Max = 128.0;

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_RADIUSSTART_KOMMASTELLEN_, true);
	tempString = editBox->getText();
	StartRadius_Kommastellen = atoi(tempString.c_str());

	// Werte bestimmen: RadiusFaktor
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_RADIUSFAKTOR_WERT_, true);
	tempString = editBox->getText();
	tempDouble = atof(tempString.c_str());
	if (tempDouble < 0.0) tempDouble = 0.0;
	if (tempDouble > 1.0) tempDouble = 1.0;
	FraktalGenerator->SetzeParameter(ELSystemEigenschaft::_LSYSTEMEIGENSCHAFT_RADIUS_FAKTOR_,tempDouble);

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_RADIUSFAKTOR_MIN_, true);
	tempString = editBox->getText();
	RadiusFaktor_Min = atof(tempString.c_str());
	if (RadiusFaktor_Min < 0) RadiusFaktor_Min = 0;
	if (RadiusFaktor_Min > 1.0) RadiusFaktor_Min = 1.0;

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_RADIUSFAKTOR_MAX_, true);
	tempString = editBox->getText();
	RadiusFaktor_Max = atof(tempString.c_str());
	if (RadiusFaktor_Max < RadiusFaktor_Min) RadiusFaktor_Max = RadiusFaktor_Min;
	if (RadiusFaktor_Max > 1.0) RadiusFaktor_Max = 1.0;

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_RADIUSFAKTOR_KOMMASTELLEN_, true);
	tempString = editBox->getText();
	RadiusFaktor_Kommastellen = atoi(tempString.c_str());

	// Werte bestimmen: RadiusDekrementor
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_RADIUSDEKREMENTOR_WERT_, true);
	tempString = editBox->getText();
	tempDouble = atof(tempString.c_str());
	if (tempDouble < 0.0) tempDouble = 0;
	FraktalGenerator->SetzeParameter(ELSystemEigenschaft::_LSYSTEMEIGENSCHAFT_RADIUS_DEKREMENTOR_,tempDouble);

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_RADIUSDEKREMENTOR_MIN_, true);
	tempString = editBox->getText();
	RadiusDekrementor_Min = atof(tempString.c_str());
	if (RadiusDekrementor_Min < 0) RadiusDekrementor_Min = 0;

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_RADIUSDEKREMENTOR_MAX_, true);
	tempString = editBox->getText();
	RadiusDekrementor_Max = atof(tempString.c_str());
	if (RadiusDekrementor_Max < RadiusDekrementor_Min) RadiusDekrementor_Max = RadiusDekrementor_Min;

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_RADIUSDEKREMENTOR_KOMMASTELLEN_, true);
	tempString = editBox->getText();
	RadiusDekrementor_Kommastellen = atoi(tempString.c_str());

	// Werte bestimmen: Start
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_STARTSTRING_, true);
	tempString = editBox->getText();
	FraktalGenerator->SetzeStart(tempString.c_str());

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_STARTSTRING_ERLAUBTE_SYMBOLE_, true);
	tempString = editBox->getText();
	Start_ErlaubteSymbole = tempString.c_str();

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_STARTSTRING_MINLAENGE_, true);
	tempString = editBox->getText();
	Start_MinLaenge = atoi(tempString.c_str());
	if (Start_MinLaenge<1) Start_MinLaenge = 1;

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_STARTSTRING_MAXLAENGE_, true);
	tempString = editBox->getText();
	Start_MaxLaenge = atoi(tempString.c_str());
	if (Start_MaxLaenge<Start_MinLaenge) Start_MaxLaenge = Start_MinLaenge;

	// Werte bestimmen: Regeln
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_SYMBOL_, true);
	tempString = editBox->getText();
	if (tempString.size()>0)
		Symbol = tempString.c_str()[0];
	else
		Symbol = 'F';

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_SYMBOLERSETZUNG_, true);
	tempString = editBox->getText();
	Symbolersetzung = tempString.c_str();

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_SYMBOLERSETZUNG_ERLAUBTE_SYMBOLE_, true);
	tempString = editBox->getText();
	Ersetzung_ErlaubteSymbole = tempString.c_str();

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_SYMBOLERSETZUNG_MINLAENGE_, true);
	tempString = editBox->getText();
	Ersetzung_MinLaenge = atoi(tempString.c_str());
	if (Ersetzung_MinLaenge<1) Ersetzung_MinLaenge = 1;

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_SYMBOLERSETZUNG_MAXLAENGE_, true);
	tempString = editBox->getText();
	Ersetzung_MaxLaenge = atoi(tempString.c_str());
	if (Ersetzung_MaxLaenge<Ersetzung_MinLaenge) Ersetzung_MaxLaenge = Ersetzung_MinLaenge;

	// Wert bestimmen: Iterationsstufe
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_ITERATIONSSTUFE_, true);
	tempString = editBox->getText();
	Iterationsstufe = abs(atoi(tempString.c_str())); // Vorzeichen entfernen, da unsigned int gefordert

}

void DunGen::COptionen::MerkeFenster_VoxelRaum()
{
	// Rootelement bestimmen
	irr::gui::IGUIElement* root = GUI->GUIEnvironment->getRootGUIElement();
	// Test ob Fenster vorhanden
	irr::gui::IGUIElement* fenster = root->getElementFromId(_GUI_ID_WINDOW_VOXELRAUM_, true);
    if (!fenster)
		return;
	
	irr::gui::IGUIEditBox* editBox;
	irr::gui::IGUICheckBox* checkBox;
	irr::gui::IGUIComboBox* comboBox;
	irr::gui::IGUIListBox* listBox;
	irr::core::stringc tempString;	

	// ListBox: Erosionschritte
	listBox = (irr::gui::IGUIListBox*) root->getElementFromId(_GUI_ID_LISTBOX_EROSIONSSCHRITTE_, true);
	ListBox_Erosionschritte_ID = listBox->getSelected();

	// Wert bestimmen: VoxelRand
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_RAND_, true);
	tempString = editBox->getText();
	int tempInt = atoi(tempString.c_str());
	if (tempInt > 96) tempInt = 96;
	VoxelRaum->SetzeRand(tempInt);

	// Wert bestimmen: MinimalRadius
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_MINIMALRADIUS_, true);
	tempString = editBox->getText();
	tempInt = atoi(tempString.c_str());
	if (tempInt < 1) tempInt = 1;	// minimaler Radius = 1
	if (tempInt >  128) tempInt =  128;
	VoxelRaum->SetzeMinimalenStrichradius(tempInt);

	// Wert bestimmen: vorgebenen Erosions-Zufalls-Seed benutzen
	checkBox = (irr::gui::IGUICheckBox*) root->getElementFromId(_GUI_ID_CHECKBOX_EROSION_VORGEGEBENEN_RANDSEED_BENUTZEN_, true);
	VorgegebenenErosionsZufallsSeedNutzen = checkBox->isChecked();

	// Wert bestimmen: ErosionsZufallsSeed
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_EROSION_ZUFALLSSEED_, true);
	tempString = editBox->getText();
	ErosionsZufallsSeed = abs(atoi(tempString.c_str())); // Vorzeichen entfernen, da unsigned int gefordert
	
	// Wert bestimmen: Erosionswahrscheinlichkeit
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_EROSIONSWAHRSCHEINLICHKEIT_, true);
	tempString = editBox->getText();
	Erosionswahrscheinlichkeit = atof(tempString.c_str());
	if (Erosionswahrscheinlichkeit < 0.0) Erosionswahrscheinlichkeit = 0.0;
	if (Erosionswahrscheinlichkeit > 1.0) Erosionswahrscheinlichkeit = 1.0;
		
	// Wert bestimmen: schwebende Fragmente filtern
	checkBox = (irr::gui::IGUICheckBox*) root->getElementFromId(_GUI_ID_CHECKBOX_SCHWEBENDE_FRAGMENTE_ENTFERNEN_, true);
	SchwebendeFragmenteFiltern = checkBox->isChecked();

	// Werte bestimmen: Verwackeln
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_VERWACKLUNG_ZUFALLSSEED_, true);
	tempString = editBox->getText();
	tempInt = atoi(tempString.c_str());
	tempInt = abs(tempInt);	// Vorzeichen entfernen, da unsigned int gefordert
	DreiecksMesh->SetzeZufallsSeedVerwackeln(tempInt);

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_VERWACKLUNG_ZUFALLSPARAMETER_A_, true);
	tempString = editBox->getText();
	tempInt = atoi(tempString.c_str());
	tempInt = abs(tempInt);	// Vorzeichen entfernen, da unsigned int gefordert
	ZufallsGeneratorVerwacklung->SetzeA(tempInt);

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_VERWACKLUNG_ZUFALLSPARAMETER_C_, true);
	tempString = editBox->getText();
	tempInt = atoi(tempString.c_str());
	tempInt = abs(tempInt);	// Vorzeichen entfernen, da unsigned int gefordert
	ZufallsGeneratorVerwacklung->SetzeC(tempInt);

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_VERWACKLUNG_ZUFALLSPARAMETER_M_, true);
	tempString = editBox->getText();
	tempInt = atoi(tempString.c_str());
	tempInt = abs(tempInt);	// Vorzeichen entfernen, da unsigned int gefordert
	ZufallsGeneratorVerwacklung->SetzeM(tempInt);

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_VERTICES_VERWACKLUNGSTAERKE_, true);
	tempString = editBox->getText();
	double tempDouble = atof(tempString.c_str());
	DreiecksMesh->SetzeVerwacklungsStaerke(tempDouble);

	checkBox = (irr::gui::IGUICheckBox*) root->getElementFromId(_GUI_ID_CHECKBOX_VERTICES_VERWACKELN_, true);
	DreiecksMesh->SetzeOptionVerwackeln(checkBox->isChecked());
	checkBox = (irr::gui::IGUICheckBox*) root->getElementFromId(_GUI_ID_CHECKBOX_MESH_GLAETTEN_, true);
	DreiecksMesh->SetzeOptionGlaetten(checkBox->isChecked());
	
	// Normalenberechnungsmethode
	comboBox = (irr::gui::IGUIComboBox*) root->getElementFromId(_GUI_ID_COMBOBOX_NORMALEN_BERECHNUNGSMETHODE_, true);
	DreiecksMesh->SetzeNormalenwichtungsmethode((ENormalenWichtungsmethode)comboBox->getSelected());

	// Detailstufen
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_DETAILSTUFEN_VOXELMESH_, true);
	tempString = editBox->getText();
	DreiecksMesh_Detailstufen = atoi(tempString.c_str());
	tempInt = abs(tempInt);	// Vorzeichen entfernen, da unsigned int gefordert
	if (DreiecksMesh_Detailstufen>20)
		DreiecksMesh_Detailstufen = 20;
}

void DunGen::COptionen::MerkeFenster_Gaenge()
{
	irr::gui::IGUIElement* root = GUI->GUIEnvironment->getRootGUIElement();
	// Test ob Fenster vorhanden
	irr::gui::IGUIElement* fenster = root->getElementFromId(_GUI_ID_WINDOW_GAENGE_, true);
    if (!fenster)
		return;

	irr::gui::IGUIListBox* listBox;
	irr::gui::IGUIEditBox* editBox;
	irr::core::stringc tempString;
	int tempInt;

	// ListBox: Gangpunkte
	listBox = (irr::gui::IGUIListBox*) root->getElementFromId(_GUI_ID_LISTBOX_GANGPUNKTE_, true);
	ListBox_Gangpunkte_ID = listBox->getSelected();

	// ListBox: Detailobjekte
	listBox = (irr::gui::IGUIListBox*) root->getElementFromId(_GUI_ID_LISTBOX_DETAILOBJEKTE_, true);
	ListBox_Detailobjekte_ID = listBox->getSelected();

	// Editboxes auslesen
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_GANGBREITE_, true);
	tempString = editBox->getText();
	tempInt = atoi(tempString.c_str());	
	if (tempInt<1) tempInt = 1;
	if (tempInt>64) tempInt = 64;
	Architekt->SetzeGangBreite(tempInt); 

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_PUNKTABSTAND_, true);
	tempString = editBox->getText();
	double tempDouble = atof(tempString.c_str());
	if (tempDouble < 10*DunGen::CSpline::Genauigkeit) tempDouble = 10*DunGen::CSpline::Genauigkeit;
	Architekt->SetzeAbstandSplineSegmente(tempDouble);
	
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_TEXTUREY_ERHOEHUNG_, true);
	tempString = editBox->getText();
	Architekt->SetzeAbstandTexturYProAbstand1(atof(tempString.c_str()));
	
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_DETAILSTUFEN_GANGMESH_, true);
	tempString = editBox->getText();
	tempInt = atoi(tempString.c_str());
	tempInt = abs(tempInt);	// Vorzeichen entfernen, da unsigned int gefordert
	if (tempInt>20)
		tempInt = 20;
	Architekt->SetzeAnzahlSplineDetailstufen(tempInt);

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_GANGTEXTUR_, true);
	tempString = editBox->getText();
	Szene->SetzeGangTexturPfad(tempString);
}

void DunGen::COptionen::MerkeFenster_Architekt()
{
	irr::gui::IGUIElement* root = GUI->GUIEnvironment->getRootGUIElement();
	// Test ob Fenster vorhanden
	irr::gui::IGUIElement* fenster = root->getElementFromId(_GUI_ID_WINDOW_ARCHITEKT_, true);
    if (!fenster)
		return;

	irr::gui::IGUIListBox* listBox;
	irr::gui::IGUIEditBox* editBox;
	irr::gui::IGUICheckBox* checkBox;
	irr::core::stringc tempString;
	int tempInt;
	double tempDouble;

	// ListBox: Subszenen
	listBox = (irr::gui::IGUIListBox*) root->getElementFromId(_GUI_ID_LISTBOX_SUBSZENEN_, true);
	ListBox_Subszenen_ID = listBox->getSelected();

	// Editboxes auslesen
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_ARCHITEKT_KONTRUKTIONS_ZUFALLSSEED_, true);
	tempString = editBox->getText();
	tempInt = atoi(tempString.c_str());
	tempInt = abs(tempInt);	// Vorzeichen entfernen, da unsigned int gefordert
	KonstruktionsZufallsSeed = tempInt;

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_ARCHITEKT_RAUMSEKTORAUSDEHNUNG_, true);
	tempString = editBox->getText();
	tempInt = atoi(tempString.c_str());
	tempInt = abs(tempInt);	// Vorzeichen entfernen, da unsigned int gefordert
	Raumsektorausdehnung = tempInt;
	unsigned int maxVoxelDim =  std::max(std::max(CVoxelRaum::VoxelraumX,CVoxelRaum::VoxelraumY),CVoxelRaum::VoxelraumZ);
	if (Raumsektorausdehnung < maxVoxelDim/16)
		Raumsektorausdehnung = maxVoxelDim/16;
	if (Raumsektorausdehnung > maxVoxelDim)
		Raumsektorausdehnung = maxVoxelDim;
	
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_ARCHITEKT_MINABSTAND_RAUM_HOEHLE_, true);
	tempString = editBox->getText();
	tempInt = atoi(tempString.c_str());
	tempInt = abs(tempInt);	// Vorzeichen entfernen, da unsigned int gefordert
	Architekt->SetzeAbstandAndockHoehleMin(tempInt);

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_ARCHITEKT_ABSTAND_BODEN_, true);
	tempString = editBox->getText();
	tempInt = atoi(tempString.c_str());
	tempInt = abs(tempInt);	// Vorzeichen entfernen, da unsigned int gefordert
	Architekt->SetzeAbstandVomBodenMax(tempInt);

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_ARCHITEKT_ABSTANDSTEST_TIEFE_, true);
	tempString = editBox->getText();
	tempInt = atoi(tempString.c_str());
	tempInt = abs(tempInt);	// Vorzeichen entfernen, da unsigned int gefordert
	Architekt->SetzeTiefeAbstandsTestMin(tempInt);

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_ARCHITEKT_AUSFRAESTIEFE_, true);
	tempString = editBox->getText();
	tempInt = atoi(tempString.c_str());
	tempInt = abs(tempInt);	// Vorzeichen entfernen, da unsigned int gefordert
	Architekt->SetzeZusaetzlicheAusfraestiefe(tempInt);

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_ARCHITEKT_SICHERHEITSABSTAND_, true);
	tempString = editBox->getText();
	tempDouble = atof(tempString.c_str());
	if (tempDouble < 1.0)	// mindestens Faktor 1.0
		tempDouble = 1.0;
	Architekt->SetzeZusaetzlicherSicherheitsabstand(tempDouble);	

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_ARCHITEKT_WAHRSCHEINLICHKEIT_ZUSATZGANG_HOEHLE_, true);
	tempString = editBox->getText();
	tempDouble = atof(tempString.c_str());
	if (tempDouble < 0.0)	// keine negativen Wahrscheinlichkeiten moeglich
		tempDouble = 0.0;
	if (tempDouble > 1.0)	// keine Wahrscheinlichkeiten > 1.0 moeglich
		tempDouble = 1.0;
	Architekt->SetzeWahrscheinlichkeitZusaetzlicherGangRaumHoehle(tempDouble);	

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_ARCHITEKT_WAHRSCHEINLICHKEIT_ZUSATZGANG_RAUM_, true);
	tempString = editBox->getText();
	tempDouble = atof(tempString.c_str());
	if (tempDouble < 0.0)	// keine negativen Wahrscheinlichkeiten moeglich
		tempDouble = 0.0;
	if (tempDouble > 1.0)	// keine Wahrscheinlichkeiten > 1.0 moeglich
		tempDouble = 1.0;
	Architekt->SetzeWahrscheinlichkeitZusaetzlicherGangRaumRaum(tempDouble);	

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_ARCHITEKT_WAHRSCHEINLICHKEIT_LEERERRAUM_HOEHLE_, true);
	tempString = editBox->getText();
	tempInt = atoi(tempString.c_str());
	tempInt = abs(tempInt);	// Vorzeichen entfernen, da unsigned int gefordert
	Architekt->SetzePlatzierungsWahrscheinlichkeitsFaktorAnHoehleLeereSubSzene(tempInt);

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_ARCHITEKT_WAHRSCHEINLICHKEIT_LEERERRAUM_RAUM_, true);
	tempString = editBox->getText();
	tempInt = atoi(tempString.c_str());
	tempInt = abs(tempInt);	// Vorzeichen entfernen, da unsigned int gefordert
	Architekt->SetzePlatzierungsWahrscheinlichkeitsFaktorAnRaumLeereSubSzene(tempInt);
	
	// Checkboxes auslesen
	checkBox = (irr::gui::IGUICheckBox*) root->getElementFromId(_GUI_ID_CHECKBOX_ARCHITEKT_VORGEGEBENEN_ZUFALLSSEED_NUTZEN_, true);
	VorgegebenenKonstruktionsZufallsSeedNutzen = checkBox->isChecked();
}

void DunGen::COptionen::MerkeFenster_Erstellung()
{
	irr::gui::IGUIElement* root = GUI->GUIEnvironment->getRootGUIElement();
	// Test ob Fenster vorhanden
	irr::gui::IGUIElement* fenster = root->getElementFromId(_GUI_ID_WINDOW_ERSTELLUNG_, true);
    if (!fenster)
		return;

	irr::gui::IGUICheckBox* checkBox;

	// Checkboxes auslesen
	checkBox = (irr::gui::IGUICheckBox*) root->getElementFromId(_GUI_ID_CHECKBOX_ERSTELLUNG_FRAKTALZEICHENANWEISUNG_, true);
	Erstellung_Fraktalzeichenanweisung = checkBox->isChecked();
	checkBox = (irr::gui::IGUICheckBox*) root->getElementFromId(_GUI_ID_CHECKBOX_ERSTELLUNG_VOXELZEICHNEN_EROSION_, true);
	Erstellung_VoxelZeichnen_Erosion = checkBox->isChecked();
	checkBox = (irr::gui::IGUICheckBox*) root->getElementFromId(_GUI_ID_CHECKBOX_ERSTELLUNG_FILTERUNG1_, true);
	Erstellung_Filterung1 = checkBox->isChecked();

	checkBox = (irr::gui::IGUICheckBox*) root->getElementFromId(_GUI_ID_CHECKBOX_ERSTELLUNG_ARCHITEKT_, true);
	Erstellung_Architekt = checkBox->isChecked();
	checkBox = (irr::gui::IGUICheckBox*) root->getElementFromId(_GUI_ID_CHECKBOX_ERSTELLUNG_FILTERUNG2_, true);
	Erstellung_Filterung2 = checkBox->isChecked();
	checkBox = (irr::gui::IGUICheckBox*) root->getElementFromId(_GUI_ID_CHECKBOX_ERSTELLUNG_UMWANDLUNGVOXEL_, true);
	Erstellung_UmwandlungVoxel = checkBox->isChecked();
}

void DunGen::COptionen::MerkeFenster_Weiteres()
{
	irr::gui::IGUIElement* root = GUI->GUIEnvironment->getRootGUIElement();
	// Test ob Fenster vorhanden
	irr::gui::IGUIElement* fenster = root->getElementFromId(_GUI_ID_WINDOW_WEITERES_, true);
    if (!fenster)
		return;
	
	irr::gui::IGUIEditBox* editBox;
	irr::core::stringc tempString;	
	int tempInt;

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_ALLGEMEIN_ZUFALLSPARAMETER_A_, true);
	tempString = editBox->getText();
	tempInt = atoi(tempString.c_str());
	tempInt = abs(tempInt);	// Vorzeichen entfernen, da unsigned int gefordert
	ZufallsGeneratorAllgemein->SetzeA(tempInt);

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_ALLGEMEIN_ZUFALLSPARAMETER_C_, true);
	tempString = editBox->getText();
	tempInt = atoi(tempString.c_str());
	tempInt = abs(tempInt); // Vorzeichen entfernen, da unsigned int gefordert
	ZufallsGeneratorAllgemein->SetzeC(tempInt);

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_ALLGEMEIN_ZUFALLSPARAMETER_M_, true);
	tempString = editBox->getText();
	tempInt = atoi(tempString.c_str());
	tempInt = abs(tempInt); // Vorzeichen entfernen, da unsigned int gefordert
	ZufallsGeneratorAllgemein->SetzeM(tempInt);
}
