
#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

#include <gtkmm.h>
#include <libglademm/xml.h>

class MainWindow : public Gtk::Window
{
	public:
		MainWindow(Glib::RefPtr<Gnome::Glade::Xml> ui);
		virtual ~MainWindow();
		
	private:
		Glib::RefPtr<Gnome::Glade::Xml> ui;
};

#endif

