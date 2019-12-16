
#ifndef _HORDE3DWIDGET_H_
#define _HORDE3DWIDGET_H_

#include <gtkmm.h>
#include <libglademm/xml.h>
#include <gtkglmm.h>

class Horde3DWidget : public Gtk::GL::DrawingArea
{
	public:
		Horde3DWidget();
		virtual ~Horde3DWidget();
	private:
		virtual void on_realize(void);
		virtual bool on_configure_event(GdkEventConfigure* event);
		virtual bool on_expose_event(GdkEventExpose* event);
		virtual bool on_motion_notify_event(GdkEventMotion* event);
		
		bool initialize(void);
		
		volatile static bool initialized;
		static Horde3DWidget *firstwidget;
};

#endif

