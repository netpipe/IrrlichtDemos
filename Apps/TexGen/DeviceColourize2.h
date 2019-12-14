#pragma once
#include "IEditorDevice.h"
#include "GUIColourSelect.h"

class CDeviceColourize2 :
	public IEditorDevice
{
public:
	CDeviceColourize2( CWorkspace* workspace, IImageProcess* proc );
	virtual ~CDeviceColourize2(void);

	virtual bool createGUIItem( core::vector2d<s32> pos );
	virtual gui::IGUIWindow* createGUIParamWindow( void );

	virtual bool OnGUIEvent( const SEvent &event );

private:

	CImgGradient* m_Grad;	///< Pointer to a gradient function so we can display the colourize item in the property window
	CImgColourize2* m_Col2;	///< Pointer to a colourize funtion so we can display the colourize item in the property window

	// Colour point listbox
	gui::IGUIListBox*	m_PointList;
	// Colour sliders
	CGUIColourSelect* m_CS;

	// Delta slider
	gui::IGUIScrollBar* m_DS;
	// The Delta text box
	gui::IGUIStaticText* m_DST;
	// Add point and remove point buttons
	gui::IGUIButton* m_AddP;
	gui::IGUIButton* m_RemP;
	// The selected point index
	s32 m_SelPoint;


};
