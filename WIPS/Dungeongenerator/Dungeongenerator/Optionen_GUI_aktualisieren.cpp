// Implementierung der Klasse Optionen
#include "Optionen.h"

// ======================================================
// Fensterinhalt aktualisieren
// ======================================================

void DunGen::COptionen::AktualisiereFenster()
{
	AktualisiereFenster_Dateizugriff_Einstellungen();
	AktualisiereFenster_FraktalEditor();
	AktualisiereFenster_VoxelRaum();
	AktualisiereFenster_Gaenge();
	AktualisiereFenster_Architekt();
	AktualisiereFenster_Erstellung();
	AktualisiereFenster_Weiteres();
}

void DunGen::COptionen::AktualisiereFenster_Dateizugriff_Einstellungen()
{
	// Rootelement bestimmen
	irr::gui::IGUIElement* root = GUI->GUIEnvironment->getRootGUIElement();
	// Test ob Fenster vorhanden
	irr::gui::IGUIElement* fenster = root->getElementFromId(_GUI_ID_WINDOW_DATEIZUGRIFF_EINSTELLUNGEN_, true);
    if (!fenster)
		return;
	
	irr::gui::IGUICheckBox* checkBox;

	checkBox = (irr::gui::IGUICheckBox*) root->getElementFromId(_GUI_ID_CHECKBOX_DATEI_EINSTELLUNGEN_FRAKTALPARAMETER_, true);
	checkBox->setChecked(Dateizugriff_Einstellungen_Fraktalparameter);
	checkBox = (irr::gui::IGUICheckBox*) root->getElementFromId(_GUI_ID_CHECKBOX_DATEI_EINSTELLUNGEN_WEITERE_FRAKTALEINSTELLUNGEN_, true);
	checkBox->setChecked(Dateizugriff_Einstellungen_Fraktaleinstellungen_Weitere);
	checkBox = (irr::gui::IGUICheckBox*) root->getElementFromId(_GUI_ID_CHECKBOX_DATEI_EINSTELLUNGEN_VOXELRAUM_, true);
	checkBox->setChecked(Dateizugriff_Einstellungen_Voxelraum);

	checkBox = (irr::gui::IGUICheckBox*) root->getElementFromId(_GUI_ID_CHECKBOX_DATEI_EINSTELLUNGEN_GAENGE_, true);
	checkBox->setChecked(Dateizugriff_Einstellungen_Gaenge);
	checkBox = (irr::gui::IGUICheckBox*) root->getElementFromId(_GUI_ID_CHECKBOX_DATEI_EINSTELLUNGEN_DETAILOBJEKTE_, true);
	checkBox->setChecked(Dateizugriff_Einstellungen_Detailobjekte);
	checkBox = (irr::gui::IGUICheckBox*) root->getElementFromId(_GUI_ID_CHECKBOX_DATEI_EINSTELLUNGEN_ARCHITEKT_, true);
	checkBox->setChecked(Dateizugriff_Einstellungen_Architekt);

	checkBox = (irr::gui::IGUICheckBox*) root->getElementFromId(_GUI_ID_CHECKBOX_DATEI_EINSTELLUNGEN_SUBSZENEN_, true);
	checkBox->setChecked(Dateizugriff_Einstellungen_Subszenen);
	checkBox = (irr::gui::IGUICheckBox*) root->getElementFromId(_GUI_ID_CHECKBOX_DATEI_EINSTELLUNGEN_ERSTELLUNG_, true);
	checkBox->setChecked(Dateizugriff_Einstellungen_Erstellung);
	checkBox = (irr::gui::IGUICheckBox*) root->getElementFromId(_GUI_ID_CHECKBOX_DATEI_EINSTELLUNGEN_WEITERES_, true);
	checkBox->setChecked(Dateizugriff_Einstellungen_Weiteres);
}

void DunGen::COptionen::AktualisiereFenster_FraktalEditor()
{
	// Rootelement bestimmen
	irr::gui::IGUIElement* root = GUI->GUIEnvironment->getRootGUIElement();
	// Test ob Fenster vorhanden
	irr::gui::IGUIElement* fenster = root->getElementFromId(_GUI_ID_WINDOW_FRAKTALEDITOR_, true);
    if (!fenster)
		return;
	
	irr::gui::IGUIEditBox* editBox;
	
	// Parameter aktualisieren
    editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_WINKELGIER_WERT_, true);
	editBox->setText(irr::core::stringw(FraktalGenerator->LeseParameter(ELSystemEigenschaft::_LSYSTEMEIGENSCHAFT_WINKEL_GIER_)).c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_WINKELGIER_MIN_, true);
	editBox->setText(irr::core::stringw(WinkelGier_Min).c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_WINKELGIER_MAX_, true);
	editBox->setText(irr::core::stringw(WinkelGier_Max).c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_WINKELGIER_KOMMASTELLEN_, true);
	editBox->setText(irr::core::stringw(WinkelGier_Kommastellen).c_str());

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_WINKELNICK_WERT_, true);
	editBox->setText(irr::core::stringw(FraktalGenerator->LeseParameter(ELSystemEigenschaft::_LSYSTEMEIGENSCHAFT_WINKEL_NICK_)).c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_WINKELNICK_MIN_, true);
	editBox->setText(irr::core::stringw(WinkelNick_Min).c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_WINKELNICK_MAX_, true);
	editBox->setText(irr::core::stringw(WinkelNick_Max).c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_WINKELNICK_KOMMASTELLEN_, true);
	editBox->setText(irr::core::stringw(WinkelNick_Kommastellen).c_str());

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_WINKELROLL_WERT_, true);
	editBox->setText(irr::core::stringw(FraktalGenerator->LeseParameter(ELSystemEigenschaft::_LSYSTEMEIGENSCHAFT_WINKEL_ROLL_)).c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_WINKELROLL_MIN_, true);
	editBox->setText(irr::core::stringw(WinkelRoll_Min).c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_WINKELROLL_MAX_, true);
	editBox->setText(irr::core::stringw(WinkelRoll_Max).c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_WINKELROLL_KOMMASTELLEN_, true);
	editBox->setText(irr::core::stringw(WinkelRoll_Kommastellen).c_str());

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_RADIUSSTART_WERT_, true);
	editBox->setText(irr::core::stringw(FraktalGenerator->LeseParameter(ELSystemEigenschaft::_LSYSTEMEIGENSCHAFT_START_RADIUS_)).c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_RADIUSSTART_MIN_, true);
	editBox->setText(irr::core::stringw(StartRadius_Min).c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_RADIUSSTART_MAX_, true);
	editBox->setText(irr::core::stringw(StartRadius_Max).c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_RADIUSSTART_KOMMASTELLEN_, true);
	editBox->setText(irr::core::stringw(StartRadius_Kommastellen).c_str());

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_RADIUSFAKTOR_WERT_, true);
	editBox->setText(irr::core::stringw(FraktalGenerator->LeseParameter(ELSystemEigenschaft::_LSYSTEMEIGENSCHAFT_RADIUS_FAKTOR_)).c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_RADIUSFAKTOR_MIN_, true);
	editBox->setText(irr::core::stringw(RadiusFaktor_Min).c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_RADIUSFAKTOR_MAX_, true);
	editBox->setText(irr::core::stringw(RadiusFaktor_Max).c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_RADIUSFAKTOR_KOMMASTELLEN_, true);
	editBox->setText(irr::core::stringw(RadiusFaktor_Kommastellen).c_str());

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_RADIUSDEKREMENTOR_WERT_, true);
	editBox->setText(irr::core::stringw(FraktalGenerator->LeseParameter(ELSystemEigenschaft::_LSYSTEMEIGENSCHAFT_RADIUS_DEKREMENTOR_)).c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_RADIUSDEKREMENTOR_MIN_, true);
	editBox->setText(irr::core::stringw(RadiusDekrementor_Min).c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_RADIUSDEKREMENTOR_MAX_, true);
	editBox->setText(irr::core::stringw(RadiusDekrementor_Max).c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_RADIUSDEKREMENTOR_KOMMASTELLEN_, true);
	editBox->setText(irr::core::stringw(RadiusDekrementor_Kommastellen).c_str());

	// Weitere EditBoxes
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_STARTSTRING_, true);
	editBox->setText(irr::core::stringw(FraktalGenerator->LeseStart().c_str()).c_str());

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_STARTSTRING_ERLAUBTE_SYMBOLE_, true);
	editBox->setText(irr::core::stringw(Start_ErlaubteSymbole.c_str()).c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_STARTSTRING_MINLAENGE_, true);
	editBox->setText(irr::core::stringw(Start_MinLaenge).c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_STARTSTRING_MAXLAENGE_, true);
	editBox->setText(irr::core::stringw(Start_MaxLaenge).c_str());

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_SYMBOL_, true);
	std::string tempString = "0"; 
	tempString.at(0) = Symbol;	// char -> string
	editBox->setText(irr::core::stringw(tempString.c_str()).c_str());

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_SYMBOLERSETZUNG_, true);
	editBox->setText(irr::core::stringw(Symbolersetzung.c_str()).c_str());

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_SYMBOLERSETZUNG_ERLAUBTE_SYMBOLE_, true);
	editBox->setText(irr::core::stringw(Ersetzung_ErlaubteSymbole.c_str()).c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_SYMBOLERSETZUNG_MINLAENGE_, true);
	editBox->setText(irr::core::stringw(Ersetzung_MinLaenge).c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_SYMBOLERSETZUNG_MAXLAENGE_, true);
	editBox->setText(irr::core::stringw(Ersetzung_MaxLaenge).c_str());

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_ITERATIONSSTUFE_, true);
	editBox->setText(irr::core::stringw(Iterationsstufe).c_str());

	// ListBox mit Regeln
	irr::gui::IGUIListBox* listBox = (irr::gui::IGUIListBox*) root->getElementFromId(_GUI_ID_LISTBOX_ERSETZUNGSREGELN_, true);
	listBox->clear();
	for (unsigned int i = 0; i < FraktalGenerator->LeseRegelAnzahl(); i++)
	{
		tempString = "0";
		tempString.at(0) = FraktalGenerator->LeseRegel(i).Symbol;
		tempString += " -> ";
		tempString += FraktalGenerator->LeseRegel(i).Ersetzung;

		listBox->addItem(irr::core::stringw(tempString.c_str()).c_str());
	}
	if (ListBox_Ersetzungsregeln_ID < listBox->getItemCount())
		listBox->setSelected(ListBox_Ersetzungsregeln_ID);
	else
		ListBox_Ersetzungsregeln_ID = -1;
	
	// Editbox: Regel-Ersetzungsinfo
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_ERSETZUNGSINFO_, true);
	if (ListBox_Ersetzungsregeln_ID >= 0)
		editBox->setText(irr::core::stringw(FraktalGenerator->LeseRegel(ListBox_Ersetzungsregeln_ID).Ersetzung.c_str()).c_str());
	else
		editBox->setText(L"");

}
void DunGen::COptionen::AktualisiereFenster_VoxelRaum()
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

	// Werte aktualisieren: Edit Boxes
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_RAND_, true);
	editBox->setText(irr::core::stringw(VoxelRaum->LeseRand()).c_str());

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_MINIMALRADIUS_, true);
	editBox->setText(irr::core::stringw(VoxelRaum->LeseMinimalenStrichradius()).c_str());

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_EROSIONSWAHRSCHEINLICHKEIT_, true);
	editBox->setText(irr::core::stringw(Erosionswahrscheinlichkeit).c_str());

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_EROSION_ZUFALLSSEED_, true);
	editBox->setText(irr::core::stringw(ErosionsZufallsSeed).c_str());
	
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_VERTICES_VERWACKLUNGSTAERKE_, true);
	editBox->setText(irr::core::stringw(DreiecksMesh->LeseVerwacklungsStaerke()).c_str());
	
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_VERWACKLUNG_ZUFALLSSEED_, true);
	editBox->setText(irr::core::stringw(DreiecksMesh->LeseZufallsSeedVerwackeln()).c_str());

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_VERWACKLUNG_ZUFALLSPARAMETER_A_, true);
	editBox->setText(irr::core::stringw(ZufallsGeneratorVerwacklung->LeseA()).c_str());

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_VERWACKLUNG_ZUFALLSPARAMETER_C_, true);
	editBox->setText(irr::core::stringw(ZufallsGeneratorVerwacklung->LeseC()).c_str());

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_VERWACKLUNG_ZUFALLSPARAMETER_M_, true);
	editBox->setText(irr::core::stringw(ZufallsGeneratorVerwacklung->LeseM()).c_str());

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_DETAILSTUFEN_VOXELMESH_, true);
	editBox->setText(irr::core::stringw(DreiecksMesh_Detailstufen).c_str());

	// Combobox
	comboBox = (irr::gui::IGUIComboBox*) root->getElementFromId(_GUI_ID_COMBOBOX_NORMALEN_BERECHNUNGSMETHODE_, true);
	comboBox->setSelected(DreiecksMesh->LeseNormalenwichtungsmethode());

	// CheckBox: vogegebenen Zufallsseed benutzen
	checkBox = (irr::gui::IGUICheckBox*) root->getElementFromId(_GUI_ID_CHECKBOX_EROSION_VORGEGEBENEN_RANDSEED_BENUTZEN_, true);
	checkBox->setChecked(VorgegebenenErosionsZufallsSeedNutzen);

	// CheckBox: schwebende Fragmente entfernen
	checkBox = (irr::gui::IGUICheckBox*) root->getElementFromId(_GUI_ID_CHECKBOX_SCHWEBENDE_FRAGMENTE_ENTFERNEN_, true);
	checkBox->setChecked(SchwebendeFragmenteFiltern);

	// CheckBox: Vertices verwackeln
	checkBox = (irr::gui::IGUICheckBox*) root->getElementFromId(_GUI_ID_CHECKBOX_VERTICES_VERWACKELN_, true);
	checkBox->setChecked(DreiecksMesh->LeseOptionVerwackeln());

	// CheckBox: Mesh Glaetten
	checkBox = (irr::gui::IGUICheckBox*) root->getElementFromId(_GUI_ID_CHECKBOX_MESH_GLAETTEN_, true);
	checkBox->setChecked(DreiecksMesh->LeseOptionGlaetten());

	// ListBox: Erosionschritte
	irr::gui::IGUIListBox* listBox = (irr::gui::IGUIListBox*) root->getElementFromId(_GUI_ID_LISTBOX_EROSIONSSCHRITTE_, true);
	listBox->clear();

	irr::core::stringw tempString;
	for (unsigned int i = 0; i < ErosionsSchritte.size(); i++)
	{
		tempString = irr::core::stringw(i+1);
		tempString.append(". Schritt mit Erosionswahrscheinlichkeit = ");
		tempString.append(irr::core::stringw(ErosionsSchritte.at(i)));
		listBox->addItem(tempString.c_str());
	}
	if (ListBox_Erosionschritte_ID < listBox->getItemCount())
		listBox->setSelected(ListBox_Erosionschritte_ID);
	else
		ListBox_Erosionschritte_ID = -1;
}

void DunGen::COptionen::AktualisiereFenster_Gaenge()
{
	irr::gui::IGUIElement* root = GUI->GUIEnvironment->getRootGUIElement();
	// Test ob Fenster vorhanden
	irr::gui::IGUIElement* fenster = root->getElementFromId(_GUI_ID_WINDOW_GAENGE_, true);
    if (!fenster)
		return;

	irr::gui::IGUIEditBox* editBox;
	irr::gui::IGUICheckBox* checkBox;
	irr::gui::IGUIListBox* listBox;
	irr::core::stringw tempString;

	// GAENGE:	

	// ListBox: Gangpunkte
	listBox = (irr::gui::IGUIListBox*) root->getElementFromId(_GUI_ID_LISTBOX_GANGPUNKTE_, true);
	listBox->clear();
	for (unsigned int i = 0; i < Architekt->QuerschnittGang_Normiert.Punkt.size(); i++)
	{
		tempString = irr::core::stringw(i+1);
		tempString.append(".) Pos:(");
		tempString.append(irr::core::stringw(Architekt->QuerschnittGang_Normiert.Punkt.at(i).X));
		tempString.append(" , ");
		tempString.append(irr::core::stringw(Architekt->QuerschnittGang_Normiert.Punkt.at(i).Y));
		tempString.append(") T.X: ");
		tempString.append(irr::core::stringw(Architekt->QuerschnittGang_Normiert.TextureX.at(i)));
		listBox->addItem(tempString.c_str());
	}
	if (ListBox_Gangpunkte_ID < listBox->getItemCount())
		listBox->setSelected(ListBox_Gangpunkte_ID);
	else
		ListBox_Gangpunkte_ID = -1;
	
	// Werte aktualisieren: Edit Boxes
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_GANGBREITE_, true);
	editBox->setText(irr::core::stringw(Architekt->LeseGangBreite()).c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_PUNKTABSTAND_, true);
	editBox->setText(irr::core::stringw(Architekt->LeseAbstandSplineSegmente()).c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_TEXTUREY_ERHOEHUNG_, true);
	editBox->setText(irr::core::stringw(Architekt->LeseAbstandTexturYProAbstand1()).c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_DETAILSTUFEN_GANGMESH_, true);
	editBox->setText(irr::core::stringw(Architekt->LeseAnzahlSplineDetailstufen()).c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_GANGTEXTUR_, true);
	editBox->setText(irr::core::stringw(Szene->LeseGangTexturPfad()).c_str());
		
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_PUNKT_X_, true);
	if (ListBox_Gangpunkte_ID >= 0)
		editBox->setText(irr::core::stringw(Architekt->QuerschnittGang_Normiert.Punkt.at(ListBox_Gangpunkte_ID).X).c_str());
	else
		editBox->setText(L"");
	
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_PUNKT_Y_, true);
	if (ListBox_Gangpunkte_ID >= 0)
		editBox->setText(irr::core::stringw(Architekt->QuerschnittGang_Normiert.Punkt.at(ListBox_Gangpunkte_ID).Y).c_str());
	else
		editBox->setText(L"");

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_PUNKT_TEXTUREX_, true);
	if (ListBox_Gangpunkte_ID >= 0)
		editBox->setText(irr::core::stringw(Architekt->QuerschnittGang_Normiert.TextureX.at(ListBox_Gangpunkte_ID)).c_str());
	else
		editBox->setText(L"");

	// DETAILOBJEKTE:

	// ListBox: Detailobjekte
	listBox = (irr::gui::IGUIListBox*) root->getElementFromId(_GUI_ID_LISTBOX_DETAILOBJEKTE_, true);
	listBox->clear();	
	for (unsigned int i = 0; i < Architekt->DetailobjektVorlagen.size(); i++)
	{
		tempString = irr::core::stringw(i+1);
		tempString.append(".) Pos:(");
		tempString.append(irr::core::stringw( Architekt->DetailobjektVorlagen.at(i)->Position.X));
		tempString.append(" , ");
		tempString.append(irr::core::stringw( Architekt->DetailobjektVorlagen.at(i)->Position.Y));
		tempString.append(") '");
		tempString.append(irr::core::stringw( Architekt->DetailobjektVorlagen.at(i)->Name));
		tempString.append("' -> '");
		tempString.append(irr::core::stringw( FileSystem->getFileBasename(Architekt->DetailobjektVorlagen.at(i)->MeshPfad) ));
		tempString.append("'");
		listBox->addItem(tempString.c_str());
	}
	if (ListBox_Detailobjekte_ID < listBox->getItemCount())
		listBox->setSelected(ListBox_Detailobjekte_ID);
	else
		ListBox_Detailobjekte_ID = -1;
	
	// Werte aktualisieren: Edit Boxes
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_DETAILOBJEKT_BENENNUNG_, true);
	if (ListBox_Detailobjekte_ID >= 0)
		editBox->setText(irr::core::stringw(Architekt->DetailobjektVorlagen.at(ListBox_Detailobjekte_ID)->Name).c_str());
	else
		editBox->setText(L"");

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_DETAILOBJEKT_SKALIERUNG_X_, true);
	if (ListBox_Detailobjekte_ID >= 0)
		editBox->setText(irr::core::stringw(Architekt->DetailobjektVorlagen.at(ListBox_Detailobjekte_ID)->Skalierung.X).c_str());
	else
		editBox->setText(L"");
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_DETAILOBJEKT_SKALIERUNG_Y_, true);
	if (ListBox_Detailobjekte_ID >= 0)
		editBox->setText(irr::core::stringw(Architekt->DetailobjektVorlagen.at(ListBox_Detailobjekte_ID)->Skalierung.Y).c_str());
	else
		editBox->setText(L"");
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_DETAILOBJEKT_SKALIERUNG_Z_, true);
	if (ListBox_Detailobjekte_ID >= 0)
		editBox->setText(irr::core::stringw(Architekt->DetailobjektVorlagen.at(ListBox_Detailobjekte_ID)->Skalierung.Z).c_str());
	else
		editBox->setText(L"");

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_DETAILOBJEKT_ROTATION_X_, true);
	if (ListBox_Detailobjekte_ID >= 0)
		editBox->setText(irr::core::stringw(Architekt->DetailobjektVorlagen.at(ListBox_Detailobjekte_ID)->Rotation.X).c_str());
	else
		editBox->setText(L"");
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_DETAILOBJEKT_ROTATION_Y_, true);
	if (ListBox_Detailobjekte_ID >= 0)
		editBox->setText(irr::core::stringw(Architekt->DetailobjektVorlagen.at(ListBox_Detailobjekte_ID)->Rotation.Y).c_str());
	else
		editBox->setText(L"");
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_DETAILOBJEKT_ROTATION_Z_, true);
	if (ListBox_Detailobjekte_ID >= 0)
		editBox->setText(irr::core::stringw(Architekt->DetailobjektVorlagen.at(ListBox_Detailobjekte_ID)->Rotation.Z).c_str());
	else
		editBox->setText(L"");

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_DETAILOBJEKT_POSITION_X_, true);
	if (ListBox_Detailobjekte_ID >= 0)
		editBox->setText(irr::core::stringw(Architekt->DetailobjektVorlagen.at(ListBox_Detailobjekte_ID)->Position.X).c_str());
	else
		editBox->setText(L"");
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_DETAILOBJEKT_POSITION_Y_, true);
	if (ListBox_Detailobjekte_ID >= 0)
		editBox->setText(irr::core::stringw(Architekt->DetailobjektVorlagen.at(ListBox_Detailobjekte_ID)->Position.Y).c_str());
	else
		editBox->setText(L"");

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_DETAILOBJEKT_ABTASTUNG_, true);
	if (ListBox_Detailobjekte_ID >= 0)
		editBox->setText(irr::core::stringw(Architekt->DetailobjektVorlagen.at(ListBox_Detailobjekte_ID)->AbstandsAbtastung).c_str());
	else
		editBox->setText(L"");
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_DETAILOBJEKT_FAKTOR_NUMABSTAND_, true);
	if (ListBox_Detailobjekte_ID >= 0)
		editBox->setText(irr::core::stringw(Architekt->DetailobjektVorlagen.at(ListBox_Detailobjekte_ID)->AbstandNumFaktor).c_str());
	else
		editBox->setText(L"");
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_DETAILOBJEKT_NUMABSTAND_MIN_, true);
	if (ListBox_Detailobjekte_ID >= 0)
		editBox->setText(irr::core::stringw(Architekt->DetailobjektVorlagen.at(ListBox_Detailobjekte_ID)->AbstandNumMin).c_str());
	else
		editBox->setText(L"");
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_DETAILOBJEKT_NUMABSTAND_MAX_, true);
	if (ListBox_Detailobjekte_ID >= 0)
		editBox->setText(irr::core::stringw(Architekt->DetailobjektVorlagen.at(ListBox_Detailobjekte_ID)->AbstandNumMax).c_str());
	else
		editBox->setText(L"");
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_DETAILOBJEKT_NUMABSTAND_MIN_ANFANG_, true);
	if (ListBox_Detailobjekte_ID >= 0)
		editBox->setText(irr::core::stringw(Architekt->DetailobjektVorlagen.at(ListBox_Detailobjekte_ID)->AbstandNumMinErstesElement).c_str());
	else
		editBox->setText(L"");
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_DETAILOBJEKT_NUMABSTAND_MAX_ANFANG_, true);
	if (ListBox_Detailobjekte_ID >= 0)
		editBox->setText(irr::core::stringw(Architekt->DetailobjektVorlagen.at(ListBox_Detailobjekte_ID)->AbstandNumMaxErstesElement).c_str());
	else
		editBox->setText(L"");

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_DETAILOBJEKT_PFADINFO_, true);
	if (ListBox_Detailobjekte_ID >= 0)
		editBox->setText(irr::core::stringw(Architekt->DetailobjektVorlagen.at(ListBox_Detailobjekte_ID)->MeshPfad).c_str());
	else
		editBox->setText(L"");
	
	// Checkboxes aktualisieren
	checkBox = (irr::gui::IGUICheckBox*) root->getElementFromId(_GUI_ID_CHECKBOX_OBJEKT_GANGENDE_, true);
	if (ListBox_Detailobjekte_ID >= 0)
		checkBox->setChecked(Architekt->DetailobjektVorlagen.at(ListBox_Detailobjekte_ID)->ObjektAn1);
	else
		checkBox->setChecked(false);

}

void DunGen::COptionen::AktualisiereFenster_Architekt()
{
	irr::gui::IGUIElement* root = GUI->GUIEnvironment->getRootGUIElement();
	// Test ob Fenster vorhanden
	irr::gui::IGUIElement* fenster = root->getElementFromId(_GUI_ID_WINDOW_ARCHITEKT_, true);
    if (!fenster)
		return;

	irr::gui::IGUIEditBox* editBox;
	irr::gui::IGUICheckBox* checkBox;
	irr::gui::IGUIListBox* listBox;
	irr::core::stringw tempString;

	// ARCHITEKT:

	// Werte aktualisieren: Edit Boxes
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_ARCHITEKT_KONTRUKTIONS_ZUFALLSSEED_, true);
	editBox->setText(irr::core::stringw(KonstruktionsZufallsSeed).c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_ARCHITEKT_RAUMSEKTORAUSDEHNUNG_, true);
	editBox->setText(irr::core::stringw(Raumsektorausdehnung).c_str());

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_ARCHITEKT_MINABSTAND_RAUM_HOEHLE_, true);
	editBox->setText(irr::core::stringw(Architekt->LeseAbstandAndockHoehleMin()).c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_ARCHITEKT_ABSTAND_BODEN_, true);
	editBox->setText(irr::core::stringw(Architekt->LeseAbstandVomBodenMax()).c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_ARCHITEKT_ABSTANDSTEST_TIEFE_, true);
	editBox->setText(irr::core::stringw(Architekt->LeseTiefeAbstandsTestMin()).c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_ARCHITEKT_AUSFRAESTIEFE_, true);
	editBox->setText(irr::core::stringw(Architekt->LeseZusaetzlicheAusfraestiefe()).c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_ARCHITEKT_SICHERHEITSABSTAND_, true);
	editBox->setText(irr::core::stringw(Architekt->LeseZusaetzlicherSicherheitsabstand()).c_str());

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_ARCHITEKT_WAHRSCHEINLICHKEIT_ZUSATZGANG_HOEHLE_, true);
	editBox->setText(irr::core::stringw(Architekt->LeseWahrscheinlichkeitZusaetzlicherGangRaumHoehle()).c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_ARCHITEKT_WAHRSCHEINLICHKEIT_ZUSATZGANG_RAUM_, true);
	editBox->setText(irr::core::stringw(Architekt->LeseWahrscheinlichkeitZusaetzlicherGangRaumRaum()).c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_ARCHITEKT_WAHRSCHEINLICHKEIT_LEERERRAUM_HOEHLE_, true);
	editBox->setText(irr::core::stringw(Architekt->LesePlatzierungsWahrscheinlichkeitsFaktorAnHoehleLeereSubSzene()).c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_ARCHITEKT_WAHRSCHEINLICHKEIT_LEERERRAUM_RAUM_, true);
	editBox->setText(irr::core::stringw(Architekt->LesePlatzierungsWahrscheinlichkeitsFaktorAnRaumLeereSubSzene()).c_str());

	// Checkboxes aktualisieren
	checkBox = (irr::gui::IGUICheckBox*) root->getElementFromId(_GUI_ID_CHECKBOX_ARCHITEKT_VORGEGEBENEN_ZUFALLSSEED_NUTZEN_, true);
	checkBox->setChecked(VorgegebenenKonstruktionsZufallsSeedNutzen);

	// SUBSZENEN:

	// ListBox: Subszenen
	listBox = (irr::gui::IGUIListBox*) root->getElementFromId(_GUI_ID_LISTBOX_SUBSZENEN_, true);
	listBox->clear();	
	for (unsigned int i = 0; i < Architekt->SubSzenen.size(); i++)
	{
		tempString = irr::core::stringw(i+1);
		tempString.append(".) '");
		tempString.append(irr::core::stringw(Architekt->SubSzenen.at(i)->LeseName() ));
		tempString.append("' -> '");
		tempString.append(irr::core::stringw(FileSystem->getFileBasename(Architekt->SubSzenen.at(i)->LesePfad() )));
		tempString.append("' : WF.Hoehle: ");
		tempString.append(irr::core::stringw(Architekt->SubSzenen.at(i)->LesePlatzierungsWahrscheinlichkeitsFaktorAnHoehle() ));
		tempString.append(" , WF.Raum: ");
		tempString.append(irr::core::stringw(Architekt->SubSzenen.at(i)->LesePlatzierungsWahrscheinlichkeitsFaktorAnRaum() ));
		listBox->addItem(tempString.c_str());
	}
	if (ListBox_Subszenen_ID < listBox->getItemCount())
		listBox->setSelected(ListBox_Subszenen_ID);
	else
		ListBox_Subszenen_ID = -1;
	
	// Werte aktualisieren: Edit Boxes
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_SUBSZENE_BENENNUNG_, true);
	if (ListBox_Subszenen_ID >= 0)
		editBox->setText(irr::core::stringw(Architekt->SubSzenen.at(ListBox_Subszenen_ID)->LeseName()).c_str());
	else
		editBox->setText(L"");

	// Skalierung
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_SUBSZENE_SKALIERUNG_X_, true);
	if (ListBox_Subszenen_ID >= 0)
		editBox->setText(irr::core::stringw(Architekt->SubSzenen.at(ListBox_Subszenen_ID)->LeseSkalierung().X).c_str());
	else
		editBox->setText(L"");
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_SUBSZENE_SKALIERUNG_Y_, true);
	if (ListBox_Subszenen_ID >= 0)
		editBox->setText(irr::core::stringw(Architekt->SubSzenen.at(ListBox_Subszenen_ID)->LeseSkalierung().Y).c_str());
	else
		editBox->setText(L"");
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_SUBSZENE_SKALIERUNG_Z_, true);
	if (ListBox_Subszenen_ID >= 0)
		editBox->setText(irr::core::stringw(Architekt->SubSzenen.at(ListBox_Subszenen_ID)->LeseSkalierung().Z).c_str());
	else
		editBox->setText(L"");

	// Platzierungswahrscheinlichkeitsfaktoren
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_SUBSZENE_WAHRSCHEINLICHKEIT_HOEHLE_, true);
	if (ListBox_Subszenen_ID >= 0)
		editBox->setText(irr::core::stringw(Architekt->SubSzenen.at(ListBox_Subszenen_ID)->LesePlatzierungsWahrscheinlichkeitsFaktorAnHoehle()).c_str());
	else
		editBox->setText(L"");
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_SUBSZENE_WAHRSCHEINLICHKEIT_RAUM_, true);
	if (ListBox_Subszenen_ID >= 0)
		editBox->setText(irr::core::stringw(Architekt->SubSzenen.at(ListBox_Subszenen_ID)->LesePlatzierungsWahrscheinlichkeitsFaktorAnRaum()).c_str());
	else
		editBox->setText(L"");

	// Gangsplineableitungsstaerken
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_SUBSZENE_GANGSPLINEABLEITUNG_RAUM_RAUM_, true);
	if (ListBox_Subszenen_ID >= 0)
		editBox->setText(irr::core::stringw(Architekt->SubSzenen.at(ListBox_Subszenen_ID)->LeseAbleitungsFaktorRaumRaum()).c_str());
	else
		editBox->setText(L"");
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_SUBSZENE_GANGSPLINEABLEITUNG_RAUM_HOEHLE_, true);
	if (ListBox_Subszenen_ID >= 0)
		editBox->setText(irr::core::stringw(Architekt->SubSzenen.at(ListBox_Subszenen_ID)->LeseAbleitungsFaktorRaumHoehle()).c_str());
	else
		editBox->setText(L"");
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_SUBSZENE_GANGSPLINEABLEITUNG_HOEHLE_RAUM_, true);
	if (ListBox_Subszenen_ID >= 0)
		editBox->setText(irr::core::stringw(Architekt->SubSzenen.at(ListBox_Subszenen_ID)->LeseAbleitungsFaktorHoehleRaum()).c_str());
	else
		editBox->setText(L"");

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_SUBSZENE_PFADINFO_, true);
	if (ListBox_Subszenen_ID >= 0)
		editBox->setText(irr::core::stringw(Architekt->SubSzenen.at(ListBox_Subszenen_ID)->LesePfad()).c_str());
	else
		editBox->setText(L"");
}

void DunGen::COptionen::AktualisiereFenster_Erstellung()
{
	irr::gui::IGUIElement* root = GUI->GUIEnvironment->getRootGUIElement();
	// Test ob Fenster vorhanden
	irr::gui::IGUIElement* fenster = root->getElementFromId(_GUI_ID_WINDOW_ERSTELLUNG_, true);
    if (!fenster)
		return;

	irr::gui::IGUICheckBox* checkBox;

	// Checkboxes aktualisieren
	checkBox = (irr::gui::IGUICheckBox*) root->getElementFromId(_GUI_ID_CHECKBOX_ERSTELLUNG_FRAKTALZEICHENANWEISUNG_, true);
	checkBox->setChecked(Erstellung_Fraktalzeichenanweisung);
	checkBox = (irr::gui::IGUICheckBox*) root->getElementFromId(_GUI_ID_CHECKBOX_ERSTELLUNG_VOXELZEICHNEN_EROSION_, true);
	checkBox->setChecked(Erstellung_VoxelZeichnen_Erosion);
	checkBox = (irr::gui::IGUICheckBox*) root->getElementFromId(_GUI_ID_CHECKBOX_ERSTELLUNG_FILTERUNG1_, true);
	checkBox->setChecked(Erstellung_Filterung1);

	checkBox = (irr::gui::IGUICheckBox*) root->getElementFromId(_GUI_ID_CHECKBOX_ERSTELLUNG_ARCHITEKT_, true);
	checkBox->setChecked(Erstellung_Architekt);
	checkBox = (irr::gui::IGUICheckBox*) root->getElementFromId(_GUI_ID_CHECKBOX_ERSTELLUNG_FILTERUNG2_, true);
	checkBox->setChecked(Erstellung_Filterung2);
	checkBox = (irr::gui::IGUICheckBox*) root->getElementFromId(_GUI_ID_CHECKBOX_ERSTELLUNG_UMWANDLUNGVOXEL_, true);
	checkBox->setChecked(Erstellung_UmwandlungVoxel);
}

void DunGen::COptionen::AktualisiereFenster_Weiteres()
{
	// Rootelement bestimmen
	irr::gui::IGUIElement* root = GUI->GUIEnvironment->getRootGUIElement();
	// Test ob Fenster vorhanden
	irr::gui::IGUIElement* fenster = root->getElementFromId(_GUI_ID_WINDOW_WEITERES_, true);
    if (!fenster)
		return;

	irr::gui::IGUIEditBox* editBox;
	irr::video::SColorf farbeHoehle = Szene->LeseCallbackHoehlenShader()->LeseFarbe();

	// Editboxes aktualisieren
	
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_ALLGEMEIN_ZUFALLSPARAMETER_A_, true);
	editBox->setText(irr::core::stringw(ZufallsGeneratorAllgemein->LeseA()).c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_ALLGEMEIN_ZUFALLSPARAMETER_C_, true);
	editBox->setText(irr::core::stringw(ZufallsGeneratorAllgemein->LeseC()).c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_ALLGEMEIN_ZUFALLSPARAMETER_M_, true);
	editBox->setText(irr::core::stringw(ZufallsGeneratorAllgemein->LeseM()).c_str());

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_FARBE_HOEHLE_R_, true);
	editBox->setText(irr::core::stringw(farbeHoehle.r).c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_FARBE_HOEHLE_G_, true);
	editBox->setText(irr::core::stringw(farbeHoehle.g).c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_FARBE_HOEHLE_B_, true);
	editBox->setText(irr::core::stringw(farbeHoehle.b).c_str());

	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_FARBE_HINTERGRUND_R_, true);
	editBox->setText(irr::core::stringw(Hintergrundfarbe.getRed()).c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_FARBE_HINTERGRUND_G_, true);
	editBox->setText(irr::core::stringw(Hintergrundfarbe.getGreen()).c_str());
	editBox = (irr::gui::IGUIEditBox*) root->getElementFromId(_GUI_ID_EDITBOX_FARBE_HINTERGRUND_B_, true);
	editBox->setText(irr::core::stringw(Hintergrundfarbe.getBlue()).c_str());
}

// ======================================================
// GUI ein-/ausblenden
// ======================================================

void DunGen::COptionen::GUIaus()
{
	MerkeFenster();
	GUI->LoescheGUI();
	GUIaktiv = false;
}

void DunGen::COptionen::GUIan()
{
	GUI->ErstelleMenue();
	GUIaktiv = true;
}

void DunGen::COptionen::GUIumschalten()
{
	if (GUIaktiv)
		GUIaus();
	else
		GUIan();
}