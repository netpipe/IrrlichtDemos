
#include <gtkmm/main.h>

#include "MainWindow.h"
#include "Horde3DWidget.h"

#include "core/FileSystem.h"

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		printf("Excepted game root directory as first parameter.\n");
		return -1;
	}
	Horde3DWidget::setRootDirectory(argv[1]);
	peak::FileSystem::get()->init();
	peak::FileSystem::get()->setRoot(argv[1]);
	
	Glib::thread_init();
	// Create window
	Gtk::Main gtkmain(argc, argv);
	Gtk::GL::init(argc, argv);

	Glib::RefPtr<Gnome::Glade::Xml> ui = Gnome::Glade::Xml::create("tools/editor/ui2.glade");
	MainWindow window(ui);
	
	gtkmain.run(window);
	return 0;
}

