#pragma once

#include "gui_freetype_font.h"
#include "texturefactory.h"

using namespace irr;
using namespace TexGen;



class IEditorDevice;

class CGUIData
{
public:

	CGUIData(void);
	~CGUIData(void);

	CGUIFreetypeFont*		guifont;	///< The font for the GUI
	CGUITTFace*				guiface;	///< The font face

	CTextureFactory*		TextureFactory;	///< Pointer to the texture factory

	IEditorDevice*			Selected_Device;	///< The currently selected device			

	gui::IGUIEnvironment*	guienv;		///< The Gui Environment pointer
	video::IVideoDriver*	driver;		///< The video driver pointer
	IrrlichtDevice*			device;		///< The irrlict device

	gui::IGUIImage*			preview;	///< Pointer to the preview window
	gui::IGUIListBox*		list;		///< List box for displaying names of image processes

	gui::IGUIWindow*		properties;	///< The properties window
	gui::IGUITable*			prop_table;	///< The properties table

	u32						prop_sel;	///< ID of the selected property

	gui::IGUIWindow*		dev_modal;	///< The devices property window

	void addDevice( IEditorDevice* device, core::vector2d<s32> pos );
	IEditorDevice* createDevice( PROCESS_TYPES type, u32 ID = 0 );

	IEditorDevice* getDevice( u32 id );
	IEditorDevice* getDeviceByIndex( u32 idx );
	u32 getNumDevices( void );

	void clearDevices( void );
	void rebuildDevices( void );

	void addLink( u32 SourceDevID, u32 SourceDevIdx, u32 DestDevID, u32 DestDevIdx );
	void addLink( IEditorDevice* SourceDev, u32 SourceDevIdx, IEditorDevice* DestDev, u32 DestDevIdx );
	void drawLinks( void );

	void showPreview( IEditorDevice* device );

	bool saveDevices( const io::path& filename );
	bool saveDevices( io::IWriteFile* file );
	bool loadDevices( const io::path& filename );
	bool loadDevices( io::IReadFile* file );

	bool removeDevice( u32 DeviceID );
	bool removeDevice( IEditorDevice* Device );

	typedef struct SDeviceLink
	{
		IEditorDevice*	Source;		///< The source device
		u32				SourceIdx;	///< The source index
		IEditorDevice*	Dest;		///< The destination device
		u32				DestIdx;	///< The destination index
	} SDeviceLink;

	enum EDITOR_MODE
	{
		EM_ADD_ELEMENT = 1,
		EM_EDIT_ELEMENT = 2,
		EM_LINK_ELEMENT = 3
	};

	typedef struct SEditorMode
	{
		EDITOR_MODE	Mode;
		PROCESS_TYPES PType;

		SDeviceLink Link;
	} SEditorMode;

	SEditorMode	EditorMode;			///< The current mode the editor is in. Either editing devices or adding them
	
private:

	core::array<IEditorDevice*> m_Devices;		///< The array of editor devices
	core::array<SDeviceLink>	m_DeviceLinks;	///< The links between the devices

	void readDevice( io::IXMLReader* reader );
	void readLink( io::IXMLReader* reader );

};
