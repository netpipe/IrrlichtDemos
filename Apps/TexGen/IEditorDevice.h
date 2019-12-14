#pragma once

#include "TextureFactory.h"
#include "GUIEditorControl.h"

using namespace irr;
using namespace TexGen;

class CWorkspace;

/// The editor device class
/// Handles the editor side of image process editing
class IEditorDevice :
	public io::IAttributeExchangingObject
{
public:
	IEditorDevice( CWorkspace* workspace, IImageProcess* process );
	virtual ~IEditorDevice(void);

	u32 getID( void );
	PROCESS_TYPES getType( void );
	const c8* getName( void );

	IImageProcess* getImgProcess( void );

	video::IImage* getImage( u32 width, u32 height );

	virtual bool createGUIItem( core::vector2d<s32> pos ) = 0;
	virtual gui::IGUIWindow* createGUIParamWindow( void ) = 0;

	virtual core::vector2d<s32> getInputLinkPoint( u32 idx );
	virtual core::vector2d<s32> getOutputLinkPoint( u32 idx );

	virtual bool OnGUIEvent( const SEvent &event ) = 0;

	u32		getListIndex( void );
	void	setListIndex( u32 index );

	CGUIEditorControl* getEditorControl( void ) const;

	virtual void serializeAttributes( io::IAttributes* out, io::SAttributeReadWriteOptions* options = 0 ) const;
	virtual void deserializeAttributes( io::IAttributes* in, io::SAttributeReadWriteOptions* options = 0 );

protected:

	CWorkspace*			m_Workspace;		///< The workspace this device belongs to

	CGUIEditorControl*	m_EditorControl;	///< The GUI editor item

	gui::IGUIWindow*	m_PropWindow;		///< The property window

	u32					m_ListIndex;			///< The index in the device list

	IImageProcess*		m_pImageProc;		///< The image process associated with this device

	// Previous settings for when we cancel the property window
	io::IAttributes*	m_PrevSettings;
	
};
