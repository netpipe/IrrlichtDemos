
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

#include "MainWindow.h"
#include "MaterialEditor.h"
#include "LevelEditor.h"

#include <gdk/gdk.h>
#include <gdk/gdkx.h>
#include <GL/glx.h>

MainWindow::MainWindow(Glib::RefPtr<Gnome::Glade::Xml> ui) : Gtk::Window()
{
	this->ui = ui;
	// Create window
	ui->reparent_widget("mainwindowbox", *this);
	
	notebook = (Gtk::Notebook*)ui->get_widget("notebook");
	
	notebook->signal_switch_page().connect(sigc::mem_fun(*this, &MainWindow::changeNotebookPage));
	
	// Create material editor
	hordematerial = new Horde3DWidget;
	((Gtk::HPaned*)ui->get_widget("hpaned1"))->pack2(*hordematerial);
	hordematerial->realize();
	hordematerial->show();
	materialeditor = new MaterialEditor(hordematerial, ui);
	
	// Create level editor
	hordelevel = new Horde3DWidget;
	((Gtk::HPaned*)ui->get_widget("hpaned3"))->pack2(*hordelevel);
	hordelevel->realize();
	hordelevel->show();
	leveleditor = new LevelEditor(hordelevel, ui);
	
	set_reallocate_redraws(true);
	
	window = this;
	activepage = -1;
	
	// FIXME: Variable size
	resize(800, 600);
}

MainWindow::~MainWindow()
{
	delete materialeditor;
}

MainWindow *MainWindow::get(void)
{
	return window;
}
Glib::RefPtr<Gnome::Glade::Xml> MainWindow::getUI(void)
{
	return ui;
}

void MainWindow::changeNotebookPage(GtkNotebookPage *page, int index)
{
	printf("Changed notebook page (%d).\n", index);
	switch (activepage)
	{
		case 3:
			materialeditor->hide();
			break;
		case 5:
			leveleditor->hide();
			break;
	}
	switch (index)
	{
		case 3:
			materialeditor->show();
			break;
		case 5:
			leveleditor->show();
			break;
	}
	activepage = index;
}

bool MainWindow::on_motion_notify_event(GdkEventMotion* event)
{
	return true;
}

MainWindow *MainWindow::window;

