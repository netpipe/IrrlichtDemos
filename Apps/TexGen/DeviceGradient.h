#pragma once
#include "IEditorDevice.h"

class CDeviceGradient :
	public IEditorDevice
{
public:
	CDeviceGradient( CWorkspace* workspace, IImageProcess* proc );
	virtual ~CDeviceGradient(void);

	virtual bool createGUIItem( core::vector2d<s32> pos );
	virtual gui::IGUIWindow* createGUIParamWindow( void );

	virtual bool OnGUIEvent( const SEvent &event );
};
