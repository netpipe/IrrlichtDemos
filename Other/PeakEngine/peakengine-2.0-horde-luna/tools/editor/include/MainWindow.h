
#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

#include <gtkmm.h>
#include <libglademm/xml.h>

#include "Horde3DWidget.h"

class MainWindow : public Gtk::Window
{
	public:
		MainWindow(Glib::RefPtr<Gnome::Glade::Xml> ui);
		virtual ~MainWindow();
		
		static MainWindow *get(void);
		Glib::RefPtr<Gnome::Glade::Xml> getUI(void);
		
		bool OnExpose(GdkEventExpose *event);
		bool OnExpose2(GdkEventExpose *event);
	private:
		Glib::RefPtr<Gnome::Glade::Xml> ui;
		
		Horde3DWidget horde1;
		
		static MainWindow *window;
};

#endif

