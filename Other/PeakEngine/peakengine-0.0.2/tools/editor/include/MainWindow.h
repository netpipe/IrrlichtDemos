
#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

#include <gtkmm.h>
#include <libglademm/xml.h>

#include "IrrlichtWidget.h"

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
		
		IrrlichtWidget irrlicht1;
		IrrlichtWidget irrlicht2;
		
		static MainWindow *window;
};

#endif

