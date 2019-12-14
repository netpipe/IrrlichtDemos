#pragma once
#include "IEditorDevice.h"

class CDevicePerlin :
	public IEditorDevice
{
public:
	CDevicePerlin( CWorkspace* workspace, IImageProcess* proc );
	virtual ~CDevicePerlin(void);

	virtual bool createGUIItem( core::vector2d<s32> pos );
	virtual gui::IGUIWindow* createGUIParamWindow( void );

	virtual bool OnGUIEvent( const SEvent &event );

protected:

	gui::IGUIScrollBar*		m_ThresholdScroll;	///< Threshold scrollbar
	gui::IGUIStaticText*	m_ThresholdText;	///< Threshold text box
};
