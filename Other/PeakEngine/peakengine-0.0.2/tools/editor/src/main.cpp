
#include <gtkmm/main.h>

#include "MainWindow.h"

int main(int argc, char **argv)
{
	Glib::thread_init();
	// Create window
	Gtk::Main gtkmain(argc, argv);

	Glib::RefPtr<Gnome::Glade::Xml> ui = Gnome::Glade::Xml::create("tools/editor/ui2.glade");
	MainWindow window(ui);
	
	gtkmain.run(window);
	return 0;
}

