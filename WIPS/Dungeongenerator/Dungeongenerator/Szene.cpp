// Implementierung der Klasse Optionen
#include "Szene.h"

// ======================================================
// Initialisierung
// ======================================================
DunGen::CSzene::CSzene(irr::IrrlichtDevice* _IrrDevice, CVoxelRaum* _VoxelRaum, CDreiecksMesh* _DreiecksMesh, CArchitekt* _Architekt)
{
	// Irrlicht-Objekte merken
	VideoDriver = _IrrDevice->getVideoDriver();
	SceneManager = _IrrDevice->getSceneManager();
	GuiEnvironment = _IrrDevice->getGUIEnvironment();

	// Generator-Objekte merken
	VoxelRaum = _VoxelRaum;
	DreiecksMesh = _DreiecksMesh;
	Architekt = _Architekt;

	// Startparameter setzen
	ZeichenModus = EZeichenModus::_ZEICHENMODUS_NICHTS_;
	KameraPositionGespeichert = irr::core::vector3df(0,0,0);
	KameraLookAtGespeichert = irr::core::vector3df(99999.f, 99999.f, 99999.f);

	DetailObjektMeshNode = NULL;
	RaumMeshNode = NULL;

	// Erstellung der Bilder und Texturen: DichteMaps
	// Bild Erstellen
	BildDichteXY = VideoDriver->createImage(irr::video::ECF_R8G8B8, irr::core::dimension2d<irr::u32>(VoxelRaum->VoxelraumX,VoxelRaum->VoxelraumY));
	// Bild Erstellen
	BildDichteXZ = VideoDriver->createImage(irr::video::ECF_R8G8B8, irr::core::dimension2d<irr::u32>(VoxelRaum->VoxelraumX,VoxelRaum->VoxelraumZ));
	// Bild Erstellen
	BildDichteYZ = VideoDriver->createImage(irr::video::ECF_R8G8B8, irr::core::dimension2d<irr::u32>(VoxelRaum->VoxelraumY,VoxelRaum->VoxelraumZ));
	BildDichteTextur = NULL;
	
	// Material fuer die Hoehle
	CallbackHoehlenShader = new CHoehlenShaderCallBack(_IrrDevice);
	irr::io::path vsFileName = "Shader/Hoehle_Basic.vert";
	irr::io::path psFileName = "Shader/Hoehle_Kristall_Innen.frag";

	HoehlenShaderMaterial.push_back(VideoDriver->getGPUProgrammingServices()->addHighLevelShaderMaterialFromFiles(
		vsFileName,
		"main",
		irr::video::E_VERTEX_SHADER_TYPE::EVST_VS_1_1,
		psFileName,
		"main",
		irr::video::E_PIXEL_SHADER_TYPE::EPST_PS_1_1,
		CallbackHoehlenShader,
		irr::video::EMT_SOLID));

	psFileName = "Shader/Hoehle_Kristall_Aussen.frag";
	HoehlenShaderMaterial.push_back(VideoDriver->getGPUProgrammingServices()->addHighLevelShaderMaterialFromFiles(
		vsFileName,
		"main",
		irr::video::E_VERTEX_SHADER_TYPE::EVST_VS_1_1,
		psFileName,
		"main",
		irr::video::E_PIXEL_SHADER_TYPE::EPST_PS_1_1,
		CallbackHoehlenShader,
		irr::video::EMT_SOLID));


	// Material fuer die Gaenge
	vsFileName = "Shader/Gang.vert";
	psFileName = "Shader/Gang.frag";
	CGangShaderCallBack* callbackGangShader = new CGangShaderCallBack(_IrrDevice);

	GangShaderMaterial = VideoDriver->getGPUProgrammingServices()->addHighLevelShaderMaterialFromFiles(
		vsFileName,
		"main",
		irr::video::E_VERTEX_SHADER_TYPE::EVST_VS_1_1,
		psFileName,
		"main",
		irr::video::E_PIXEL_SHADER_TYPE::EPST_PS_1_1,
		callbackGangShader,
		irr::video::EMT_SOLID);

	GangTexturPfad = "GangTexturen/Testtextur_0001.bmp";
}


// ======================================================
// Zeichenmodus aendern
// ======================================================
void DunGen::CSzene::SzeneLoeschen(bool _AllesLoeschen)
{
	std::cout << "[Szene]: Loesche Szene";

	// alle Grafiknodes loeschen
	SceneManager->getRootSceneNode()->removeAll();	
	
	// alles loeschen?
	if (_AllesLoeschen)
	{
		std::cout << ", Texturebuffer, Meshcache, Subszenen";

		// Testuren loeschen
		VideoDriver->removeAllTextures();
		BildDichteTextur = NULL;
		// Meshcache loeschen
		SceneManager->getMeshCache()->clear();

		// nach dem Loeschen des Texturebuffers sind die Subszenen invalide -> diese auch loeschen
		for (unsigned int i=0; i<Architekt->SubSzenen.size(); i++)
			delete Architekt->SubSzenen.at(i);
		Architekt->SubSzenen.clear();
		// vorhandene Raeume und Gaenge loeschen
		for (unsigned int i=0; i<Architekt->Raeume.size(); i++)
			delete Architekt->Raeume.at(i);
		Architekt->Raeume.clear();
		for (unsigned int i=0; i<Architekt->Gaenge.size(); i++)
			delete Architekt->Gaenge.at(i);
		Architekt->Gaenge.clear();
	}

	// Meshnodes sind geloescht -> Verweise loeschen
	VoxelMeshNodes.clear();
	VoxelMeshDetailstufen.clear();

	GangMeshNodes.clear();
	GangMeshDetailstufen.clear();
	AdapterMeshNodes.clear();

	GangTeileBlickdicht.clear();
	GangTeileDurchlaessig.clear();

	DetailObjektMeshNode = NULL;
	RaumMeshNode = NULL;

	std::cout << " ... fertig." << std::endl;
 }

void DunGen::CSzene::SzeneErstellen(EZeichenModus _ZeichenModus, bool _geaendert)
{
	std::cout << "[Szene]: Szene wird neu erstellt." << std::endl;

	irr::scene::IMeshSceneNode* meshSceneNode;
	irr::scene::ISceneNode* tempSceneNode;
	
	// neuen Modus herstellen
	switch(_ZeichenModus)
	{
		case EZeichenModus::_ZEICHENMODUS_EBENE_XY_:
			// Dichtebilder aktualisieren
			if (_geaendert)
			{
				ErstelleDichteBild(EEbene::_EBENE_XY_);
				ErstelleDichteBild(EEbene::_EBENE_XZ_);
				ErstelleDichteBild(EEbene::_EBENE_YZ_);
			}
			// alte Textur loeschen wenn im Texturebuffer
			if (BildDichteTextur!=NULL) VideoDriver->removeTexture(BildDichteTextur);
			// Bild in Textur umwandeln
			BildDichteTextur = VideoDriver->addTexture("Dichtekarte",BildDichteXY);
			break;

		case EZeichenModus::_ZEICHENMODUS_EBENE_XZ_:
			// Dichtebilder aktualisieren
			if (_geaendert)
			{
				ErstelleDichteBild(EEbene::_EBENE_XY_);
				ErstelleDichteBild(EEbene::_EBENE_XZ_);
				ErstelleDichteBild(EEbene::_EBENE_YZ_);
			}
			// alte Textur loeschen wenn im Texturebuffer
			if (BildDichteTextur!=NULL) VideoDriver->removeTexture(BildDichteTextur);
			// Bild in Textur umwandeln
			BildDichteTextur = VideoDriver->addTexture("Dichtekarte",BildDichteXZ);
			break;

		case EZeichenModus::_ZEICHENMODUS_EBENE_YZ_:
			// Dichtebilder aktualisieren
			if (_geaendert)
			{
				ErstelleDichteBild(EEbene::_EBENE_XY_);
				ErstelleDichteBild(EEbene::_EBENE_XZ_);
				ErstelleDichteBild(EEbene::_EBENE_YZ_);
			}
			// alte Textur loeschen wenn im Texturebuffer
			if (BildDichteTextur!=NULL) VideoDriver->removeTexture(BildDichteTextur);
			// Bild in Textur umwandeln
			BildDichteTextur = VideoDriver->addTexture("Dichtekarte",BildDichteYZ);
			break;

		case EZeichenModus::_ZEICHENMODUS_DUNGEON_:
			{
				GangMeshNodeDetailstufe = 0;
				AnsichtsmodusGangMeshDrahtgitterModell = false;
				AnsichtsmodusAdapterMeshDrahtgitterModell = false;
				AnsichtsmodusDetailObjekteZeigen = true;
				AnsichtsmodusRaeumeZeigen = true;
			
				SceneManager->getRootSceneNode()->setName("Dungeon");
				// ~~~~~~~~~~~~~~~~~~~~
				// Raeume hinzufuegen:
				// ~~~~~~~~~~~~~~~~~~~~

				// Elternknoten mit passendenr Benennung und ID hinzufuegen
				RaumMeshNode = SceneManager->addEmptySceneNode();
				RaumMeshNode->setName("Raueme");

				SDungeonRaum* raum;
				irr::scene::ISceneNode* ueberfluessigeGeoNode;
				
				for (unsigned int i=0; i<Architekt->Raeume.size(); i++)
				{
					raum = Architekt->Raeume.at(i);

					// Knoten mit passendem Namen und ID hinzufuegen
					tempSceneNode = SceneManager->addEmptySceneNode(RaumMeshNode, i);
					tempSceneNode->setName(raum->SubSzene->LeseName());
					tempSceneNode->setScale(irr::core::vector3df(raum->Skalierung.X, raum->Skalierung.Y, raum->Skalierung.Z));
					tempSceneNode->setRotation(irr::core::vector3df(raum->Rotation.X, raum->Rotation.Y, raum->Rotation.Z));
					tempSceneNode->setPosition(irr::core::vector3df(raum->Position.X, raum->Position.Y, raum->Position.Z));

					// SubSzene einfuegen
					raum->SubSzene->SzeneHinzufuegen(tempSceneNode,SceneManager);
					
					// Ueberfluessige Geometrien loeschen: je nach dem ob Gang angedockt wurde oder nicht
					if (raum->GangID[(0+raum->Offset)%4]==-1)
						ueberfluessigeGeoNode = SceneManager->getSceneNodeFromName("Nord_offen",tempSceneNode);
					else
						ueberfluessigeGeoNode = SceneManager->getSceneNodeFromName("Nord_geschlossen",tempSceneNode);
					if (ueberfluessigeGeoNode!=NULL) ueberfluessigeGeoNode->remove();

					if (raum->GangID[(1+raum->Offset)%4]==-1)
						ueberfluessigeGeoNode = SceneManager->getSceneNodeFromName("Ost_offen",tempSceneNode);
					else
						ueberfluessigeGeoNode = SceneManager->getSceneNodeFromName("Ost_geschlossen",tempSceneNode);
					if (ueberfluessigeGeoNode!=NULL) ueberfluessigeGeoNode->remove();

					if (raum->GangID[(2+raum->Offset)%4]==-1)
						ueberfluessigeGeoNode = SceneManager->getSceneNodeFromName("Sued_offen",tempSceneNode);
					else
						ueberfluessigeGeoNode = SceneManager->getSceneNodeFromName("Sued_geschlossen",tempSceneNode);
					if (ueberfluessigeGeoNode!=NULL) ueberfluessigeGeoNode->remove();

					if (raum->GangID[(3+raum->Offset)%4]==-1)
						ueberfluessigeGeoNode = SceneManager->getSceneNodeFromName("West_offen",tempSceneNode);
					else
						ueberfluessigeGeoNode = SceneManager->getSceneNodeFromName("West_geschlossen",tempSceneNode);
					if (ueberfluessigeGeoNode!=NULL) ueberfluessigeGeoNode->remove();

				}

				// ~~~~~~~~~~~~~~~~~~~~
				// Gaenge hinzufuegen:
				// ~~~~~~~~~~~~~~~~~~~~

				// Textur des Gangs auslesen
				irr::video::ITexture* gangTextur = VideoDriver->getTexture(GangTexturPfad);

				// fuer alle Detailstufen (alle Gaenge haben gleiche Detailstufenanzahl):
				if (!Architekt->Gaenge.empty())
				for (unsigned int i=0; i<Architekt->Gaenge.at(0)->MeshSpline.size(); i++)
				{
					// Elternknoten mit passendender Benennung und ID hinzufuegen
					tempSceneNode = SceneManager->addEmptySceneNode(0,i);
					tempSceneNode->setName("GangDetailstufe");
					if (i>0) tempSceneNode->setVisible(false); // am Anfang: Original-Detailstufe anzeigen
					GangMeshDetailstufen.push_back(tempSceneNode);

					// alle Gaenge hinzufuegen
					for (unsigned int j=0; j<Architekt->Gaenge.size(); j++)
					{
						// Mesh unter Elternknoten hinzufuegen mit passender Benennung und ID
						meshSceneNode = SceneManager -> addMeshSceneNode(Architekt->Gaenge.at(j)->MeshSpline.at(i), tempSceneNode, j);
						meshSceneNode->setName("Gang");

						meshSceneNode->setMaterialFlag(irr::video::EMF_BACK_FACE_CULLING, false);
						meshSceneNode->setMaterialType((irr::video::E_MATERIAL_TYPE)GangShaderMaterial);
						meshSceneNode->setMaterialTexture(0, gangTextur);

						// Verweis auf Knoten merken
						GangMeshNodes.push_back(meshSceneNode);
						if (Architekt->Gaenge.at(j)->LeseDefinitivBlickdicht())
							GangTeileBlickdicht.push_back(meshSceneNode);
						else
							GangTeileDurchlaessig.push_back(meshSceneNode);
					}
				}

				// Adapter hinzufuegen:
				// Elternknoten mit passendender Benennung und ID hinzufuegen
				tempSceneNode = SceneManager->addEmptySceneNode();
				tempSceneNode->setName("GangAdapter");

				// alle Adapter hinzufuegen
				for (unsigned int i=0; i<Architekt->Gaenge.size(); i++)
				{
					// Adapter 0 hinzufuegen
					meshSceneNode = SceneManager -> addMeshSceneNode(Architekt->Gaenge.at(i)->MeshAdapter[0], tempSceneNode, i);
					meshSceneNode->setName("Adapter0");

					meshSceneNode->setMaterialFlag(irr::video::EMF_BACK_FACE_CULLING, false);
					meshSceneNode->setMaterialType((irr::video::E_MATERIAL_TYPE)GangShaderMaterial);
					meshSceneNode->setMaterialTexture(0, gangTextur);
					
					// Verweis auf Knoten merken
					AdapterMeshNodes.push_back(meshSceneNode);
					if (Architekt->Gaenge.at(i)->LeseDefinitivBlickdicht())
						GangTeileBlickdicht.push_back(meshSceneNode);
					else
						GangTeileDurchlaessig.push_back(meshSceneNode);
		
					// Adapter 1 hinzufuegen
					meshSceneNode = SceneManager -> addMeshSceneNode(Architekt->Gaenge.at(i)->MeshAdapter[1], tempSceneNode, i);
					meshSceneNode->setName("Adapter1");

					meshSceneNode->setMaterialFlag(irr::video::EMF_BACK_FACE_CULLING, false);
					meshSceneNode->setMaterialType((irr::video::E_MATERIAL_TYPE)GangShaderMaterial);
					meshSceneNode->setMaterialTexture(0, gangTextur);
					
					// Verweis auf Knoten merken
					AdapterMeshNodes.push_back(meshSceneNode);
					if (Architekt->Gaenge.at(i)->LeseDefinitivBlickdicht())
						GangTeileBlickdicht.push_back(meshSceneNode);
					else
						GangTeileDurchlaessig.push_back(meshSceneNode);
				}

				// ~~~~~~~~~~~~~~~~~~~~~~~~~~
				// Detailobjekte hinzufuegen:
				// ~~~~~~~~~~~~~~~~~~~~~~~~~~
				
				// Elternknoten mit passendender Benennung und ID hinzufuegen
				DetailObjektMeshNode = SceneManager->addEmptySceneNode();
				DetailObjektMeshNode->setName("Detailobjekte");

				irr::core::vector3d<double> temp;
				SSplineDetailobjekt* detailObjekt;

				// fuer alle Gaenge
				for (unsigned i=0; i<Architekt->Gaenge.size(); i++)
				{
					// Sub-Elternknoten mit passendender Benennung und ID hinzufuegen
					tempSceneNode = SceneManager->addEmptySceneNode(DetailObjektMeshNode, i);
					tempSceneNode->setName("DetailobjektAnGang");

					// Detailobjekte hinzuefuegen:
					for (unsigned j=0; j<Architekt->Gaenge.at(i)->Detailobjekte.size(); j++)
					{
						detailObjekt = Architekt->Gaenge.at(i)->Detailobjekte.at(j);

						// alle Positionen des Detailobektes in den Szenegraphen laden
						for (unsigned k=0; k<detailObjekt->Position.size(); k++)
						{
							meshSceneNode = SceneManager -> addMeshSceneNode(detailObjekt->ObjektMesh, tempSceneNode, k);
							meshSceneNode->setName(detailObjekt->Name);

							temp = detailObjekt->Skalierung;
							meshSceneNode->setScale(irr::core::vector3df(temp.X,temp.Y,temp.Z));
							temp = detailObjekt->Position.at(k);
							meshSceneNode->setPosition(irr::core::vector3df(temp.X,temp.Y,temp.Z));
							temp = detailObjekt->Rotation.at(k);
							meshSceneNode->setRotation(irr::core::vector3df(temp.X,temp.Y,temp.Z));

							meshSceneNode->setMaterialType(irr::video::E_MATERIAL_TYPE::EMT_SOLID);
							meshSceneNode->setMaterialFlag(irr::video::E_MATERIAL_FLAG::EMF_LIGHTING,false);
						}
					}
				}

			}
			// break; // _ZEICHENMODUS_DREICKSMESH_ wird ebenfalls abgearbeitet

			case EZeichenModus::_ZEICHENMODUS_DREICKSMESH_:
			{
				VoxelMeshNodeDetailstufe = 0;
				AnsichtsmodusVoxelMeshDrahtgitterModell = false;
				VoxelMeshMaterial = 0;

				// fuer alle Detailstufen:
				for (unsigned int i=0; i<DreiecksMesh->MeshVoxelAufgeteilt.size(); i++)
				{
					// Elternknoten mit passendender Benennung und ID hinzufuegen
					tempSceneNode = SceneManager->addEmptySceneNode(0,i);
					tempSceneNode->setName("HoehlenDetailstufe");
					if (i>0) tempSceneNode->setVisible(false); // am Anfang: Original-Detailstufe anzeigen
					VoxelMeshDetailstufen.push_back(tempSceneNode);

					// alle Meshes hinzufuegen
					for (unsigned int j=0; j<DreiecksMesh->MeshVoxelAufgeteilt.at(i)->size(); j++)
					{
						// Mesh unter Elternknoten hinzufuegen mit passender Benennung und ID
						meshSceneNode = SceneManager->addMeshSceneNode(DreiecksMesh->MeshVoxelAufgeteilt.at(i)->at(j), tempSceneNode,j);
						meshSceneNode->setName("Hoehlenteil");
						
						meshSceneNode->setMaterialFlag(irr::video::EMF_BACK_FACE_CULLING, false);
						meshSceneNode->setMaterialType((irr::video::E_MATERIAL_TYPE)HoehlenShaderMaterial.at(VoxelMeshMaterial));

						VoxelMeshNodes.push_back(meshSceneNode);						
					}
				}
			}
			// break; // _ZEICHENMODUS_KAMERA_ wird ebenfalls abgearbeitet
			
			case EZeichenModus::_ZEICHENMODUS_KAMERA_:
				
				irr::scene::ICameraSceneNode* kamera = SceneManager->addCameraSceneNodeFPS(0,100.0f,0.05f);
				kamera->setFarValue(20000.0f);
				KameraZuruecksetzen();
				KameraEinstellenFPSModus(false);

			break;
		}
	ZeichenModus = _ZeichenModus;

}

// ======================================================
// Zeichenmodi anpassen
// ======================================================

// Detailstufen
void DunGen::CSzene::VoxelMeshDetailstufeAendern(unsigned int _Aenderung)
{
	if(VoxelMeshDetailstufen.size()==0)
		return;

	// alle Detailstufen ausblenden
	for (unsigned int i=0; i<VoxelMeshDetailstufen.size(); i++)
		VoxelMeshDetailstufen.at(i)->setVisible(false);

	// neue Detailstufe berechnen
	VoxelMeshNodeDetailstufe += _Aenderung;
	if (VoxelMeshNodeDetailstufe >= ((int)VoxelMeshDetailstufen.size()) )
		VoxelMeshNodeDetailstufe = ((int)VoxelMeshDetailstufen.size())-1;
	if (VoxelMeshNodeDetailstufe < 0)
		VoxelMeshNodeDetailstufe = 0;

	// neue Detailstufe anzeigen
	VoxelMeshDetailstufen.at(VoxelMeshNodeDetailstufe)->setVisible(true);
}

void DunGen::CSzene::GangMeshDetailstufeAendern(unsigned int _Aenderung)
{
	if(GangMeshDetailstufen.size()==0)
		return;

	// alle Detailstufen ausblenden
	for (unsigned int i=0; i<GangMeshDetailstufen.size(); i++)
		GangMeshDetailstufen.at(i)->setVisible(false);

	// neue Detailstufe berechnen
	GangMeshNodeDetailstufe += _Aenderung;
	if (GangMeshNodeDetailstufe >= ((int)GangMeshDetailstufen.size()) )
		GangMeshNodeDetailstufe = ((int)GangMeshDetailstufen.size())-1;
	if (GangMeshNodeDetailstufe < 0)
		GangMeshNodeDetailstufe = 0;

	// neue Detailstufe anzeigen
	GangMeshDetailstufen.at(GangMeshNodeDetailstufe)->setVisible(true);
}

// Ansichtsmodi aendern

// Ansichtsmodus fuer Voxel-Mesh umschalten
void DunGen::CSzene::VoxelMeshDrahtgitterModell(bool _An)
{
	AnsichtsmodusVoxelMeshDrahtgitterModell = _An;

	// fuer alle Hoehlenteile und alle Detailstufen
	for (unsigned int i=0; i<VoxelMeshNodes.size(); i++)
		// Drahtgittermodus an/aus
		VoxelMeshNodes.at(i)->setMaterialFlag(irr::video::EMF_WIREFRAME, _An);
}

// Ansichtsmodus fuer Gang-Mesh umschalten
void DunGen::CSzene::GangMeshDrahtgitterModell(bool _An)
{
	AnsichtsmodusGangMeshDrahtgitterModell = _An;

	// fuer alle Gaenge und alle Detailstufen
	for (unsigned int i=0; i<GangMeshNodes.size(); i++)
		// Drahtgittermodus an/aus
		GangMeshNodes.at(i)->setMaterialFlag(irr::video::EMF_WIREFRAME, _An);
}

// Ansichtsmodus fuer Adapter-Mesh umschalten
void DunGen::CSzene::AdapterMeshDrahtgitterModell(bool _An)
{
	AnsichtsmodusAdapterMeshDrahtgitterModell = _An;

	// fuer alle Adapter
	for (unsigned int i=0; i<AdapterMeshNodes.size(); i++)
		AdapterMeshNodes.at(i)->setMaterialFlag(irr::video::EMF_WIREFRAME, _An);
}

// Detailobjekte anzeige bzw. unsichtbar schalten
void DunGen::CSzene::DetailObjekteZeigen(bool _An)
{
	AnsichtsmodusDetailObjekteZeigen = _An;
	if (DetailObjektMeshNode)
		DetailObjektMeshNode->setVisible(_An);
}

// Raeume anzeige bzw. unsichtbar schalten
void DunGen::CSzene::RaeumeZeigen(bool _An)
{
	AnsichtsmodusRaeumeZeigen = _An;
	if (RaumMeshNode)
		RaumMeshNode->setVisible(_An);
}

// Modi umschalten
void DunGen::CSzene::VoxelMeshDrahtgitterModellUmschalten()
{
	VoxelMeshDrahtgitterModell(!AnsichtsmodusVoxelMeshDrahtgitterModell);
}
void DunGen::CSzene::GangMeshDrahtgitterModellUmschalten()
{
	GangMeshDrahtgitterModell(!AnsichtsmodusGangMeshDrahtgitterModell);
}
void DunGen::CSzene::AdapterMeshDrahtgitterModellUmschalten()
{
	AdapterMeshDrahtgitterModell(!AnsichtsmodusAdapterMeshDrahtgitterModell);
}
void DunGen::CSzene::DetailObjekteZeigenUmschalten()
{
	DetailObjekteZeigen(!AnsichtsmodusDetailObjekteZeigen);
}
void DunGen::CSzene::RaeumeZeigenUmschalten()
{
	RaeumeZeigen(!AnsichtsmodusRaeumeZeigen);
}

// Material umschalten
void DunGen::CSzene::VoxelMeshMaterialAendern()
{
	// das naechste Material verwenden
	VoxelMeshMaterial++;
	// wieder bei 0 anfangen, wenn am Ende
	if (VoxelMeshMaterial>=HoehlenShaderMaterial.size())
		VoxelMeshMaterial = 0;

	// fuer alle Hoehlenteile und alle Detailstufen
	for (unsigned int i=0; i<VoxelMeshNodes.size(); i++)
		VoxelMeshNodes.at(i)->setMaterialType((irr::video::E_MATERIAL_TYPE)HoehlenShaderMaterial.at(VoxelMeshMaterial));
}

// Visualisierung der Blickdichtigkeit
void DunGen::CSzene::GangeBlickdichtigkeitVisualisieren(bool _Modus)
{
	// Modus true ... Blickdicht ist Drahtgittermodell & Nicht-Blickdicht normal
	// Modus false ... Nicht-Blickdicht ist Drahtgittermodell & Blickdicht normal
	
	// fuer alle Gangteile
	for (unsigned int i=0; i<GangTeileBlickdicht.size(); i++)
		GangTeileBlickdicht.at(i)->setMaterialFlag(irr::video::EMF_WIREFRAME, _Modus);
	for (unsigned int i=0; i<GangTeileDurchlaessig.size(); i++)
		GangTeileDurchlaessig.at(i)->setMaterialFlag(irr::video::EMF_WIREFRAME, !_Modus);
}

// ======================================================
// Kamera-Einstellungen
// ======================================================

void DunGen::CSzene::KameraEinstellenFPSModus(bool _FPSKamera)
{
	// FPS-Kamerasteuerung ein-/ausschalten
	irr::scene::ICameraSceneNode* kamera = SceneManager->getActiveCamera();
	if (kamera)
		kamera->setInputReceiverEnabled(_FPSKamera);
}

void DunGen::CSzene::KameraZuruecksetzen()
{
	// Kamerawerte auf Standard
	irr::scene::ICameraSceneNode* kamera = SceneManager->getActiveCamera();
	if (kamera)
	{
		// Position und Ausrichtung
		kamera->setPosition(irr::core::vector3df(0,0,0));
		kamera->setTarget(irr::core::vector3df(99999.f, 99999.f, 99999.f)); // in die Szene blickend

		// Geschwindigkeit
		KameraGeschwindigkeitAendern(0.0, 0.05);
	}
}

void DunGen::CSzene::KameraGeschwindigkeitAendern(double _Geschwindigkeitsaenderung, double _AbsoluteGeschwindigkeit)
{
	irr::scene::ICameraSceneNode* kamera = SceneManager->getActiveCamera();
	if (kamera)
	{
		// Animatoren heraussuchen
		irr::core::list<irr::scene::ISceneNodeAnimator*> const & animatoren = kamera->getAnimators();
		for (irr::core::list<irr::scene::ISceneNodeAnimator*>::ConstIterator it = animatoren.begin(); it != animatoren.end(); it++)
			// wenn FPS-Kamera-Animator gefunden
			if ((*it)->getType() == irr::scene::ESCENE_NODE_ANIMATOR_TYPE::ESNAT_CAMERA_FPS)
			{
				// neue Geschwindigkeit einstellen
				double geschwindigkeit;

				if (_AbsoluteGeschwindigkeit == 0.0)
				{
					geschwindigkeit = ((irr::scene::ISceneNodeAnimatorCameraFPS*)(*it))->getMoveSpeed();
					geschwindigkeit *= _Geschwindigkeitsaenderung; // prozentuale Veraenderung
				}
				else
					geschwindigkeit = _AbsoluteGeschwindigkeit;	// fester Wert

				((irr::scene::ISceneNodeAnimatorCameraFPS*)(*it))->setMoveSpeed(geschwindigkeit);
			}
	}
}

// Position & Ausrichtung merken
void DunGen::CSzene::KameraDatenMerken()
{
	irr::scene::ICameraSceneNode* kamera = SceneManager->getActiveCamera();
	if (kamera)
	{
		KameraPositionGespeichert = kamera->getPosition();
		KameraLookAtGespeichert = kamera->getTarget();
		std::cout << "[Szene]: Kameradaten wurden gespeichert." << std::endl;
	}
}

// Position & Ausrichtung wiederherstellen
void DunGen::CSzene::KameraDatenWiederherstellen()
{
	irr::scene::ICameraSceneNode* kamera = SceneManager->getActiveCamera();
	if (kamera)
	{
		kamera->setPosition(KameraPositionGespeichert);
		kamera->setTarget(KameraLookAtGespeichert);
		std::cout << "[Szene]: Kameradaten wurden wiederhergestellt." << std::endl;
	}	
}

// ======================================================
// Zeichenroutine
// ======================================================

void DunGen::CSzene::SzeneZeichnen()
{
	// Szene/Bild darstellen
	switch(ZeichenModus)
	{
		case EZeichenModus::_ZEICHENMODUS_EBENE_XY_:
			VideoDriver->draw2DRectangleOutline(irr::core::recti(irr::core::vector2d<irr::s32>(24,49),
				irr::core::dimension2d<irr::s32>(VoxelRaum->VoxelraumX+1,VoxelRaum->VoxelraumY+1)),
				irr::video::SColor(255,128,128,255));
			VideoDriver->draw2DImage(BildDichteTextur, irr::core::position2d<irr::s32>(25,50) );
			break;

		case EZeichenModus::_ZEICHENMODUS_EBENE_XZ_:
			VideoDriver->draw2DRectangleOutline(irr::core::recti(irr::core::vector2d<irr::s32>(24,49),
				irr::core::dimension2d<irr::s32>(VoxelRaum->VoxelraumX+1,VoxelRaum->VoxelraumZ+1)),
				irr::video::SColor(255,128,128,255));
			VideoDriver->draw2DImage(BildDichteTextur, irr::core::position2d<irr::s32>(25,50) );
			break;

		case EZeichenModus::_ZEICHENMODUS_EBENE_YZ_:
			VideoDriver->draw2DRectangleOutline(irr::core::recti(irr::core::vector2d<irr::s32>(24,49),
				irr::core::dimension2d<irr::s32>(VoxelRaum->VoxelraumZ+1,VoxelRaum->VoxelraumY+1)),
				irr::video::SColor(255,128,128,255));
			VideoDriver->draw2DImage(BildDichteTextur, irr::core::position2d<irr::s32>(25,50) );
			break;

		default:
			SceneManager->drawAll();
			break;
	}
	
	// GUI darstellen
	GuiEnvironment->drawAll();
}

// ======================================================
// Dichte-Bilder erstellen
// ======================================================
void DunGen::CSzene::ErstelleDichteBild(EEbene _Ebene)
{
	unsigned int grauwert;
	irr::video::SColor farbe(255,0,0,0);

	// Passende Ebene umwandeln
	switch(_Ebene)
	{
		// Topansicht (X,Y)
		case EEbene::_EBENE_XY_:	
			for (unsigned int i = 0; i < VoxelRaum->VoxelraumX; i++)
				for (unsigned int j = 0; j < VoxelRaum->VoxelraumY; j++)
				{
					grauwert = 255 * saveDiv(VoxelRaum->LeseDichteAn(i,j,EEbene::_EBENE_XY_),VoxelRaum->LeseDichteMaximum(EEbene::_EBENE_XY_));
					farbe.setRed(grauwert);
					farbe.setGreen(grauwert);
					farbe.setBlue(grauwert);
					BildDichteXY->setPixel(i, VoxelRaum->VoxelraumY-1-j,farbe);
				}
			break;

		// Seitenansicht (X,Z)
		case EEbene::_EBENE_XZ_:	
			for (unsigned int i = 0; i < VoxelRaum->VoxelraumX; i++)
				for (unsigned int j = 0; j < VoxelRaum->VoxelraumZ; j++)
				{
					grauwert = 255 * saveDiv(VoxelRaum->LeseDichteAn(i,j,EEbene::_EBENE_XZ_),VoxelRaum->LeseDichteMaximum(EEbene::_EBENE_XZ_));
					farbe.setRed(grauwert);
					farbe.setGreen(grauwert);
					farbe.setBlue(grauwert);
					BildDichteXZ->setPixel(i, VoxelRaum->VoxelraumZ-1-j,farbe);
				}
			break;

		// Frontansicht (-Z,Y)
		case EEbene::_EBENE_YZ_:	
			for (unsigned int i = 0; i < VoxelRaum->VoxelraumY; i++)
				for (unsigned int j = 0; j < VoxelRaum->VoxelraumZ; j++)
				{
					grauwert = 255 * saveDiv(VoxelRaum->LeseDichteAn(i,j,EEbene::_EBENE_YZ_),VoxelRaum->LeseDichteMaximum(EEbene::_EBENE_YZ_));
					farbe.setRed(grauwert);
					farbe.setGreen(grauwert);
					farbe.setBlue(grauwert);
					BildDichteYZ->setPixel(VoxelRaum->VoxelraumZ-1-j, VoxelRaum->VoxelraumY-1-i,farbe);
				}
			break;
	}
}
// ======================================================
// Auslesen
// ======================================================
irr::video::IImage* DunGen::CSzene::LeseDichteBild(EEbene _Ebene)
{
	switch(_Ebene)
	{
		case EEbene::_EBENE_XY_:
			return BildDichteXY;
		case EEbene::_EBENE_XZ_:
			return BildDichteXZ;
		case EEbene::_EBENE_YZ_:
			return BildDichteYZ;
	}
	return NULL;
}

CHoehlenShaderCallBack* DunGen::CSzene::LeseCallbackHoehlenShader()
{
	return CallbackHoehlenShader;
}

irr::io::path DunGen::CSzene::LeseGangTexturPfad()
{
	return GangTexturPfad;
}

// ======================================================
// Setzen
// ======================================================

void DunGen::CSzene::SetzeGangTexturPfad(irr::io::path _GangTexturPfad)
{
	GangTexturPfad = _GangTexturPfad;
}

// ======================================================
// Reparsen der Szene (nach Neu-Einladen)
// ======================================================

void DunGen::CSzene::SzeneReparsen()
{
	// Grundwerte (Standard-Werte beim Dungeon-Export = Werte beim Import)
	GangMeshNodeDetailstufe = 0;
	AnsichtsmodusGangMeshDrahtgitterModell = false;
	AnsichtsmodusAdapterMeshDrahtgitterModell = false;
	AnsichtsmodusDetailObjekteZeigen = true;
	AnsichtsmodusRaeumeZeigen = true;
	VoxelMeshNodeDetailstufe = 0;
	AnsichtsmodusVoxelMeshDrahtgitterModell = false;
	VoxelMeshMaterial = 0;
	
	// Detailobjekte
	DetailObjektMeshNode = SceneManager->getSceneNodeFromName("Detailobjekte");

	// Raeume
	RaumMeshNode = SceneManager->getSceneNodeFromName("Raueme");

	// ~~~~~~~~~
	// Adapter
	// ~~~~~~~~~
	irr::scene::ISceneNode* tempSceneNode = SceneManager->getSceneNodeFromName("GangAdapter");
	irr::scene::IMeshSceneNode* tempMeshSceneNode;
	
	unsigned int num = 0;
	tempMeshSceneNode = (irr::scene::IMeshSceneNode*) getSceneNodeFromNameAndID("Adapter0",num, tempSceneNode);
	while (tempMeshSceneNode!=NULL)
	{
		AdapterMeshNodes.push_back(tempMeshSceneNode);
		tempMeshSceneNode->setMaterialType((irr::video::E_MATERIAL_TYPE)GangShaderMaterial);
		num++;
		tempMeshSceneNode = (irr::scene::IMeshSceneNode*) getSceneNodeFromNameAndID("Adapter0",num, tempSceneNode);
	}
	
	num = 0;
	tempMeshSceneNode = (irr::scene::IMeshSceneNode*) getSceneNodeFromNameAndID("Adapter1",num, tempSceneNode);
	while (tempMeshSceneNode!=NULL)
	{
		AdapterMeshNodes.push_back(tempMeshSceneNode);
		tempMeshSceneNode->setMaterialType((irr::video::E_MATERIAL_TYPE)GangShaderMaterial);
		num++;
		tempMeshSceneNode = (irr::scene::IMeshSceneNode*) getSceneNodeFromNameAndID("Adapter1",num, tempSceneNode);
	}
	
	// ~~~~~~~~~
	// Gaenge:
	// ~~~~~~~~~

	// Detailstufen
	num = 0;
	tempSceneNode = getSceneNodeFromNameAndID("GangDetailstufe",num, 0);
	while (tempSceneNode!=NULL)
	{
		GangMeshDetailstufen.push_back(tempSceneNode);
		num++;
		tempSceneNode = getSceneNodeFromNameAndID("GangDetailstufe",num, 0);
	}

	// Gangmeshes
	for (unsigned int i=0; i<GangMeshDetailstufen.size(); i++)
	{
		tempSceneNode = GangMeshDetailstufen.at(i);

		num = 0;
		tempMeshSceneNode = (irr::scene::IMeshSceneNode*) getSceneNodeFromNameAndID("Gang",num, tempSceneNode);
		while (tempMeshSceneNode!=NULL)
		{
			GangMeshNodes.push_back(tempMeshSceneNode);
			tempMeshSceneNode->setMaterialType((irr::video::E_MATERIAL_TYPE)GangShaderMaterial);

			num++;
			tempMeshSceneNode = (irr::scene::IMeshSceneNode*) getSceneNodeFromNameAndID("Gang",num, tempSceneNode);
		}

	}

	// ~~~~~~~~~
	// Hoehle:
	// ~~~~~~~~~

	// Detailstufen
	num = 0;
	tempSceneNode = getSceneNodeFromNameAndID("HoehlenDetailstufe",num, 0);
	while (tempSceneNode!=NULL)
	{
		VoxelMeshDetailstufen.push_back(tempSceneNode);
		num++;
		tempSceneNode = getSceneNodeFromNameAndID("HoehlenDetailstufe",num, 0);
	}

	// Hoehlenmeshes
	for (unsigned int i=0; i<VoxelMeshDetailstufen.size(); i++)
	{
		tempSceneNode = VoxelMeshDetailstufen.at(i);
		
		num = 0;
		tempMeshSceneNode = (irr::scene::IMeshSceneNode*) getSceneNodeFromNameAndID("Hoehlenteil",num, tempSceneNode);
		while (tempMeshSceneNode!=NULL)
		{
			VoxelMeshNodes.push_back(tempMeshSceneNode);
			tempMeshSceneNode->setMaterialType((irr::video::E_MATERIAL_TYPE)HoehlenShaderMaterial.at(VoxelMeshMaterial));

			num++;
			tempMeshSceneNode = (irr::scene::IMeshSceneNode*) getSceneNodeFromNameAndID("Hoehlenteil",num, tempSceneNode);
		}
	}
}

// Gibt den ersten gefundenen Knoten mit passender ID und Benennung zurueck
// modifiziert von Irrlicht: CSceneManager::getSceneNodeFromName und CSceneManager::::getSceneNodeFromName
irr::scene::ISceneNode* DunGen::CSzene::getSceneNodeFromNameAndID(const char* _name, irr::s32 _id, irr::scene::ISceneNode* _start)
{
	if (_start == 0)
		_start = SceneManager->getRootSceneNode();

	// Gefunden?
	if ((!strcmp(_start->getName(),_name)) && (_start->getID() == _id))
		return _start;

	irr::scene::ISceneNode* node = 0;

	// Kinder durchsuchen
	const irr::scene::ISceneNodeList& list = _start->getChildren();
	irr::scene::ISceneNodeList::ConstIterator it = list.begin();
	for (; it!=list.end(); ++it)
	{
		node = getSceneNodeFromNameAndID(_name, _id,  *it);
		if (node)
			return node;
	}

	return 0;
}