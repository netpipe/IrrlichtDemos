#pragma once
#include "IEditorDevice.h"
class CDeviceBlend :
	public IEditorDevice
{
public:
	CDeviceBlend( CWorkspace* workspace, IImageProcess* proc );
	virtual ~CDeviceBlend(void);

	virtual bool createGUIItem( core::vector2d<s32> pos );
	virtual gui::IGUIWindow* createGUIParamWindow( void );

	virtual bool OnGUIEvent( const SEvent &event );

private:

	gui::IGUIComboBox*	m_BlendMode;
};

