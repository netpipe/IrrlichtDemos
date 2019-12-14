#pragma once

#include "IEditorDevice.h"

using namespace irr;

class CEditor;

/// Class handling the workspace for TexGen.
class CWorkspace :
	public io::IAttributeExchangingObject
{
public:
	CWorkspace( CEditor* Editor, IrrlichtDevice* Device, CTextureFactory* TexFactory );
	virtual ~CWorkspace(void);

	io::IAttributes* createAttributes( void ) const;
	CGUIEditorControl* createEditorControl( void ) const;
	CTextureFactory* getTextureFactory( void ) const;
	gui::IGUIEnvironment* getGUIEnvironment( void ) const;

	void addDevice( IEditorDevice* device, core::vector2d<s32> pos );
	IEditorDevice* createDevice( PROCESS_TYPES type, u32 ID = 0 );

	IEditorDevice* getDevice( u32 id );

	bool removeDevice( u32 DeviceID );
	bool removeDevice( IEditorDevice* Device );

	void clearWorkspace( void );

	void addLink( u32 SourceDevID, u32 SourceDevIdx, u32 DestDevID, u32 DestDevIdx );
	void addLink( IEditorDevice* SourceDev, u32 SourceDevIdx, IEditorDevice* DestDev, u32 DestDevIdx );

	void drawLinks( void );
	void showPreview( IEditorDevice* device );

	void setAddDeviceMode( TexGen::PROCESS_TYPES DeviceType );
	void savePreviewImage( void );

	// Mouse events
	bool OnLMouseDown( const core::vector2d<s32> &Pos );
	bool OnLMouseUp( const core::vector2d<s32> &Pos );
	bool OnLMouseDblClick( const core::vector2d<s32> &Pos );
	bool OnMouseMove( const core::vector2d<s32> &Pos, bool LButtonDown, bool RButtonDown );

	// GUI Events
	bool OnGUIEvent( const SEvent &event );
	bool OnResize( core::rect<s32> &rect );
	bool OnDevicePropWindowClose( IEditorDevice* Device );

	// Key events
	bool OnDelKeyPressed( void );

	bool isPointInside( const core::vector2d<s32> &Pos );

	bool saveWorkspace( const io::path& filename );
	bool saveWorkspace( io::IWriteFile* file );
	bool loadWorkspace( const io::path& filename );
	bool loadWorkspace( io::IReadFile* file );

	virtual void serializeAttributes( io::IAttributes* out, io::SAttributeReadWriteOptions* options = 0 ) const;
	virtual void deserializeAttributes( io::IAttributes* in, io::SAttributeReadWriteOptions* options = 0 );

private:

	void readDevices( io::IXMLReader* reader );
	void readDevice( io::IXMLReader* reader );
	void readLinks( io::IXMLReader* reader );
	void readLink( io::IXMLReader* reader );

	typedef struct SDeviceLink
	{
		// Constructor
		SDeviceLink() : Source(NULL), SourceIdx(0), Dest(NULL), DestIdx(0) {};
		void reset() { Source = NULL; SourceIdx = 0; Dest = NULL; DestIdx = 0; };

		IEditorDevice*	Source;		///< The source device
		u32				SourceIdx;	///< The source index
		IEditorDevice*	Dest;		///< The destination device
		u32				DestIdx;	///< The destination index
	} SDeviceLink;

	void addLink( SDeviceLink* Link );

	CEditor*					m_Editor;

	gui::IGUIEnvironment*		m_GUIEnv;			///< The GUI environment
	io::IFileSystem*			m_FileSystem;		///< The Filesystem pointer
	video::IVideoDriver*		m_Driver;			///< The video driver

	CTextureFactory*			m_TextureFactory;	///< The Texture Factory pointer

	core::array<IEditorDevice*>	m_Devices;			///< The list of devices
	core::array<SDeviceLink>	m_DeviceLinks;		///< The links between the devices

	IEditorDevice*				m_SelectedDevice;	///< The currently selected device
	IEditorDevice*				m_Dragging;			///< Are we dragging a device's control around?

	core::rect<s32>				m_WorkspaceRect;	///< The rect used for the workspace

	enum EDITOR_MODE
	{
		EM_NORMAL = 0,
		EM_ADD_ELEMENT = 1,
		EM_EDIT_ELEMENT = 2,
		EM_LINK_ELEMENT = 3
	};

	typedef struct SEditorMode
	{
		/// Constructor
		SEditorMode() : Mode(EM_NORMAL), PType(PT_NONE), MousePos(0,0) {};
		void reset() { Mode = EM_NORMAL; PType = PT_NONE; Link.reset(); MousePos.set(0,0); };
		/// operators
		bool operator== (const EDITOR_MODE &mode) const { return (Mode == mode); };
		bool operator!= (const EDITOR_MODE &mode) const { return (Mode != mode); };
		void operator= (const EDITOR_MODE &mode) { Mode = mode; };

		EDITOR_MODE	Mode;
		PROCESS_TYPES PType;

		SDeviceLink Link;

		core::vector2d<s32> MousePos;	///< The mouse position for drawing the connecting link
	} SEditorMode;

	SEditorMode					m_EditorMode;		///< The editor's mode
};

