
#include "MainWindow.h"

#include <gdk/gdk.h>
#include <gdk/gdkx.h>
#include <GL/glx.h>

MainWindow::MainWindow(Glib::RefPtr<Gnome::Glade::Xml> ui) : Gtk::Window()
{
	this->ui = ui;
	// Create window
	ui->reparent_widget("mainwindowbox", *this);
	Gtk::Widget *drawingarea = ui->get_widget("drawingarea1");
	Gtk::Widget *drawingarea2 = ui->get_widget("drawingarea2");
	
	((Gtk::HPaned*)ui->get_widget("hpaned1"))->pack2(irrlicht1);
	irrlicht1.show();
	((Gtk::HPaned*)ui->get_widget("hpaned2"))->pack2(irrlicht2);
	irrlicht2.show();
	
	window = this;
	
	// FIXME: Variable size
	resize(800, 600);
}

MainWindow::~MainWindow()
{
}

MainWindow *MainWindow::get(void)
{
	return window;
}
Glib::RefPtr<Gnome::Glade::Xml> MainWindow::getUI(void)
{
	return ui;
}

MainWindow *MainWindow::window;

