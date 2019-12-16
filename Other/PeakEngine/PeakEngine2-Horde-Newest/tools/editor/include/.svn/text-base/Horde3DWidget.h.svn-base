
/*
Copyright (C) 2008   Mathias Gottschlag

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in the
Software without restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the
Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef _HORDE3DWIDGET_H_
#define _HORDE3DWIDGET_H_

#include <gtkmm.h>
#include <libglademm/xml.h>
#include <gtkglmm.h>

class InputReceiver;

class Horde3DWidget : public Gtk::GL::DrawingArea
{
	public:
		Horde3DWidget();
		virtual ~Horde3DWidget();
		
		void setCamera(int camera);
		void setInputReceiver(InputReceiver *receiver) { input = receiver; };
		
		static void setRootDirectory(std::string rootdir);
		static void loadResources(void);
		
		void realize() { Gtk::GL::DrawingArea::realize(); }
		
		void update(void);
	private:
		virtual void on_realize(void);
		virtual bool on_configure_event(GdkEventConfigure* event);
		virtual bool on_expose_event(GdkEventExpose* event);
		virtual bool on_motion_notify_event(GdkEventMotion* event);
		virtual bool on_drag_motion(const Glib::RefPtr<Gdk::DragContext>& context, int x, int y, guint time);
		virtual bool on_button_press_event(GdkEventButton *button);
		
		bool initialize(void);
		
		volatile static bool initialized;
		static Horde3DWidget *firstwidget;
		static std::string rootdir;
		
		int camera;
		InputReceiver *input;
		int state;
};

#endif

