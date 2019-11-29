
#include "MainWindow.h"

#include <gdk/gdk.h>

MainWindow::MainWindow(Glib::RefPtr<Gnome::Glade::Xml> ui)
{
	this->ui = ui;
	// Create window
	ui->reparent_widget("mainwindowbox", *this);
	
	// FIXME: Variable size
	resize(800, 600);
}

MainWindow::~MainWindow()
{
}

