#pragma once
#include "IEditorDevice.h"
#include "GUIColourSelect.h"

class CDeviceColourize :
	public IEditorDevice
{
public:
	CDeviceColourize( CWorkspace* workspace, IImageProcess* proc );
	virtual ~CDeviceColourize(void);

	virtual bool createGUIItem( core::vector2d<s32> pos );
	virtual gui::IGUIWindow* createGUIParamWindow( void );

	virtual bool OnGUIEvent( const SEvent &event );

private:

	CGUIColourSelect*	m_CS_Start;
	CGUIColourSelect*	m_CS_End;

};
