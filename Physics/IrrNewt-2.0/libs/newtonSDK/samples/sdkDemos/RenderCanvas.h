#ifndef __RENDER_CANVAS__
#define __RENDER_CANVAS__

#include "OpenGlUtil.h"

class DGCamera;
class RenderCanvas: public wxGLCanvas
{
public:

	RenderCanvas(wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, int* attributes);
	virtual ~RenderCanvas(void);

	void InitGl ();
	void ResetProjectionMode();

	DGCamera* GetDGCamera () {return m_camera;}

	private:
	void OnIdle(wxIdleEvent &event);
	void OnSize(wxSizeEvent& event);
	void OnPaint(wxPaintEvent& event);
	void OnKeyDown(wxKeyEvent &event);
	void OnKeyUp(wxKeyEvent &event);
	void OnMouseEvent(wxMouseEvent &event);
	void OnEraseBackground(wxEraseEvent& event);

	void RenderFrame ();

	DECLARE_EVENT_TABLE()

	bool m_showCollision;
	DGCamera* m_camera; 
	bool m_glIsInit;


//	bool m_dragging0;
//	bool m_dragging1;
	dMOUSE_POINT m_mousePos;

	friend class NewtonFrame;
};


#endif