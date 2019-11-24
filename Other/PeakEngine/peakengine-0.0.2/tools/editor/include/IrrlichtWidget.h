
#ifndef _IRRLICHTWIDGET_H_
#define _IRRLICHTWIDGET_H_

#include <gtkmm.h>
#include <libglademm/xml.h>

#include <irrlicht/irrlicht.h>
#include <GL/glx.h>

class IrrlichtWidget : public Gtk::DrawingArea
{
	public:
		IrrlichtWidget();
		virtual ~IrrlichtWidget();
		
		void beginScene(void);
		void endScene(void);
	private:
		bool OnExpose(GdkEventExpose *event);
		bool OnConfigure(GdkEventConfigure *event);
	
		static irr::IrrlichtDevice *device;
		static int usecount;
		GLXContext context;
		unsigned long window;
		GLXWindow glxwin;
		bool createdwindow;
		static GLXContext irrcontext;
};

#endif

