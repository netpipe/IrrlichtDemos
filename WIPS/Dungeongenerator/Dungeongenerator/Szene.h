// zur Verhinderung doppelter Definition
#ifndef SZENE_H
#define SZENE_H

#include <irrlicht.h>
#include <vector>
#include "Hilfsfunktionen.h"
#include "VoxelRaum.h"
#include "DreiecksMesh.h"
#include "Architekt.h"
#include "HoehlenShaderCallback.h"
#include "GangShaderCallback.h"

// Namespace DunGen : DungeonGenerator
namespace DunGen
{
	enum EZeichenModus {
		_ZEICHENMODUS_NICHTS_,
		_ZEICHENMODUS_EBENE_XY_,
		_ZEICHENMODUS_EBENE_XZ_,
		_ZEICHENMODUS_EBENE_YZ_,
		_ZEICHENMODUS_DREICKSMESH_,	
		_ZEICHENMODUS_DUNGEON_,
		_ZEICHENMODUS_KAMERA_
	};
	
	// Klasse fuer Verwaltung der Szene
	class CSzene
	{
	public:
		CSzene(irr::IrrlichtDevice* _IrrDevice, CVoxelRaum* _VoxelRaum, CDreiecksMesh* _DreiecksMesh, CArchitekt* _Architekt);

		// Szene loeschen und erstellen
		void SzeneLoeschen(bool _AllesLoeschen);
		void SzeneErstellen(EZeichenModus _ZeichenModus, bool _geaendert);
		void SzeneZeichnen();

		void ErstelleDichteBild(EEbene _Ebene);

		// Kamera einstellen
		void KameraEinstellenFPSModus(bool _FPSKamera);
		void KameraZuruecksetzen();
		void KameraGeschwindigkeitAendern(double _Geschwindigkeitsaenderung, double _AbsoluteGeschwindigkeit);
		void KameraDatenMerken();				// Position & Ausrichtung merken
		void KameraDatenWiederherstellen();		// Position & Ausrichtung wiederherstellen

		// Level-Of-Detail festlegen
		void VoxelMeshDetailstufeAendern(unsigned int _Aenderung);	// Detailstufe justieren
		void GangMeshDetailstufeAendern(unsigned int _Aenderung);	// Detailstufe justieren
		
		// Drahtgittermodi
		void VoxelMeshDrahtgitterModell(bool _An);					// Drahtgittermodell An/Aus
		void GangMeshDrahtgitterModell(bool _An);					// Drahtgittermodell An/Aus
		void AdapterMeshDrahtgitterModell(bool _An);				// Drahtgittermodell An/Aus
		void VoxelMeshDrahtgitterModellUmschalten();
		void GangMeshDrahtgitterModellUmschalten();
		void AdapterMeshDrahtgitterModellUmschalten();
		
		// Sichtbarkeitsmodi
		void DetailObjekteZeigen(bool _An);							// DetailObjekte An/Aus
		void RaeumeZeigen(bool _An);								// Raeume An/Aus
		void DetailObjekteZeigenUmschalten();
		void RaeumeZeigenUmschalten();

		void GangeBlickdichtigkeitVisualisieren(bool _Modus);		// Modus true ... Blickdicht ist Drahtgittermodell & Nicht-Blickdicht normal, Modus false ... umgekehrt

		// Material umschalten
		void VoxelMeshMaterialAendern();

		// Setzen
		void SetzeGangTexturPfad(irr::io::path _GangTexturPfad);
		// Auslesen
		irr::video::IImage* LeseDichteBild(EEbene _Ebene);
		CHoehlenShaderCallBack* LeseCallbackHoehlenShader();
		irr::io::path LeseGangTexturPfad();

		// Szene reparsen (nach Testeinladen einer Dungeon-Szene als .irr)
		void SzeneReparsen();

	private:

		// Gibt den ersten gefundenen Knoten mit passender ID und Benennung zurueck
		// modifiziert von Irrlicht: CSceneManager::getSceneNodeFromName und  CSceneManager::::getSceneNodeFromName
		irr::scene::ISceneNode* DunGen::CSzene::getSceneNodeFromNameAndID(const char* _name, irr::s32 _id, irr::scene::ISceneNode* _start);

		// Irrlicht-Objekte
		irr::video::IVideoDriver* VideoDriver;
		irr::scene::ISceneManager* SceneManager;
		irr::gui::IGUIEnvironment* GuiEnvironment;

		// Zeiger auf andere Objekte
		CVoxelRaum* VoxelRaum;
		CDreiecksMesh* DreiecksMesh;
		CArchitekt* Architekt;

		// Kamera
		irr::core::vector3df KameraPositionGespeichert;
		irr::core::vector3df KameraLookAtGespeichert;

		// Bilder
		irr::video::IImage* BildDichteXY;
		irr::video::IImage* BildDichteXZ;
		irr::video::IImage* BildDichteYZ;
		// Dichte-Bild als Textur
		irr::video::ITexture* BildDichteTextur;

		// Hoehlenmeshteile
		std::vector<irr::scene::IMeshSceneNode*> VoxelMeshNodes;
		std::vector<irr::scene::ISceneNode*> VoxelMeshDetailstufen;
		// Gaenge
		std::vector<irr::scene::IMeshSceneNode*> GangMeshNodes;
		std::vector<irr::scene::ISceneNode*> GangMeshDetailstufen;
		std::vector<irr::scene::IMeshSceneNode*> AdapterMeshNodes;
		// fuer Visualisierung der Blickdichtigkeit
		std::vector<irr::scene::IMeshSceneNode*> GangTeileBlickdicht;
		std::vector<irr::scene::IMeshSceneNode*> GangTeileDurchlaessig;

		// Detailobjekte
		irr::scene::ISceneNode* DetailObjektMeshNode;
		// Raeume
		irr::scene::ISceneNode* RaumMeshNode;

		// Materialien
		std::vector<irr::s32> HoehlenShaderMaterial;
		irr::s32 GangShaderMaterial;
		CHoehlenShaderCallBack* CallbackHoehlenShader;
		irr::io::path GangTexturPfad;

		// Aktueller Zeichenmodus
		EZeichenModus ZeichenModus;

		// Ansichtsmodi
		unsigned int VoxelMeshMaterial;	// aktuelles Material der Hoehle
		int VoxelMeshNodeDetailstufe;
		int GangMeshNodeDetailstufe;
		bool AnsichtsmodusVoxelMeshDrahtgitterModell;
		bool AnsichtsmodusGangMeshDrahtgitterModell;
		bool AnsichtsmodusAdapterMeshDrahtgitterModell;
		bool AnsichtsmodusDetailObjekteZeigen;
		bool AnsichtsmodusRaeumeZeigen;
	};
}
#endif

