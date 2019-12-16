
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

#include "LevelEditor.h"
#include "LevelData.h"

#include "core/Vector3D.h"
#include "core/Quaternion.h"
#include "core/FileSystem.h"

#include <Horde3D.h>
#include <Horde3DUtils.h>

#include <gtk/gtk.h>

LevelEditor::LevelEditor(Horde3DWidget *hordewidget, Glib::RefPtr<Gnome::Glade::Xml> ui) : InputReceiver(), hordewidget(hordewidget)
{
	this->ui = ui;
	
	((Gtk::Button*)ui->get_widget("newlevel"))->signal_clicked().connect(sigc::mem_fun(*this, &LevelEditor::onNewLevel));
	((Gtk::Button*)ui->get_widget("savelevel"))->signal_clicked().connect(sigc::mem_fun(*this, &LevelEditor::onSaveLevel));
	((Gtk::Button*)ui->get_widget("openlevel"))->signal_clicked().connect(sigc::mem_fun(*this, &LevelEditor::onOpenLevel));
	
	initialized = false;
	data = 0;
	camerapos[0] = 0;
	camerapos[1] = 0;
	camerapos[2] = 100;
	camerarot[0] = 0;
	camerarot[1] = 0;
	camerarot[2] = 0;
	
	hordewidget->setInputReceiver(this);
}
LevelEditor::~LevelEditor()
{
	if (initialized)
	{
		if (data)
		{
			delete data;
		}
	}
}

void LevelEditor::show(void)
{
	//ui->get_widget("matpreviewtype")->show();
	if (!initialized)
	{
		// Initialize scene
		pipeline = Horde3D::addResource(ResourceTypes::Pipeline, "forward.pipeline.xml", 0);
		int testoverlay = Horde3D::addResource(ResourceTypes::Material, "testoverlay.material.xml", 0);
		Horde3DWidget::loadResources();
		Horde3D::showOverlay( 0.75f, 0, 0, 0, 1, 0, 1, 0,
			1, 0.2f, 1, 1, 0.75f, 0.2f, 0, 1,
			7, testoverlay );
		camera = Horde3D::addCameraNode(RootNode, "Camera", pipeline);
		hordewidget->setCamera(camera);
		Horde3D::setNodeTransform(camera, camerapos[0], camerapos[1], camerapos[2], camerarot[0], camerarot[1], camerarot[2], 1, 1, 1);
		Horde3D::setupCameraView(camera, 90.0f, (float)640/480, 0.1f,1000.0f);
		Horde3DUtils::dumpMessages();
		initialized = true;
		
		// Load empty level
		newLevel("");
	}
	printf("Showing level editor.\n");
}
void LevelEditor::hide(void)
{
	printf("Hiding level editor.\n");
	//ui->get_widget("matpreviewtype")->hide();
}

bool LevelEditor::openLevel(std::string levelname)
{
	if (data) closeLevel();
	data = new LevelData;
	if (!data->load(levelname))
	{
		delete data;
		data = 0;
		return false;
	}
	else
	{
		this->levelname = levelname;
		Gtk::Label *label = (Gtk::Label*)ui->get_widget("currentlevel");
		label->set_text((std::string("Currently opened: ") + levelname).c_str());
		return true;
	}
}
bool LevelEditor::saveLevel(std::string levelname)
{
	if (!data) return false;
	if (levelname == "" && this->levelname == "") return false;
	if (levelname != "")
	{
		Gtk::Label *label = (Gtk::Label*)ui->get_widget("currentlevel");
		label->set_text((std::string("Currently opened: ") + levelname).c_str());
		this->levelname = levelname;
	}
	return data->save(levelname);
}
void LevelEditor::newLevel(std::string levelname)
{
	if (data) closeLevel();
	data = new LevelData;
	this->levelname = levelname;
	Gtk::Label *label = (Gtk::Label*)ui->get_widget("currentlevel");
	label->set_text((std::string("Currently opened: ") + levelname).c_str());
}
bool LevelEditor::closeLevel(void)
{
	levelname = "";
	delete data;
	data = 0;
	Gtk::Label *label = (Gtk::Label*)ui->get_widget("currentlevel");
	label->set_text((std::string("Currently opened: ") + levelname).c_str());
	return true;
}

void LevelEditor::onMouseMoved(int x, int y, int dx, int dy)
{
	printf("Mouse moved: %d/%d\n", dx, dy);
	if (getMouseButton(1))
	{
		// Rotate camera
		camerarot[0] += dy;
		if (camerarot[0] < -90) camerarot[0] = -90;
		if (camerarot[0] > 90) camerarot[0] = 90;
		camerarot[1] += dx;
		if (camerarot[1] < 0) camerarot[1] += 360;
		if (camerarot[1] > 360) camerarot[1] -= 360;
		Horde3D::setNodeTransform(camera, camerapos[0], camerapos[1], camerapos[2], camerarot[0], camerarot[1], camerarot[2], 1, 1, 1);
		hordewidget->update();
	}
	else if (getMouseButton(3))
	{
		// Move camera
		peak::Vector3D speed((float)dx / 4, 0, (float)dy / 4);
		speed.rotate(peak::Vector3D(camerarot[0] * M_PI / 180, camerarot[1] * M_PI / 180, camerarot[2] * M_PI / 180));
		camerapos[0] += speed.x;
		camerapos[1] -= speed.y;
		camerapos[2] -= speed.z;
		Horde3D::setNodeTransform(camera, camerapos[0], camerapos[1], camerapos[2], camerarot[0], camerarot[1], camerarot[2], 1, 1, 1);
		hordewidget->update();
	}
}
void LevelEditor::onMouseButton(int x, int y, int button, int state)
{
}

void LevelEditor::onOpenLevel(void)
{
	// Get levels
	Gtk::Dialog *dialog = (Gtk::Dialog*)ui->get_widget("openleveldialog");
	((Gtk::Button*)ui->get_widget("openlevelok"))->signal_clicked().connect(sigc::mem_fun(*this, &LevelEditor::onOpenLevelOk));
	((Gtk::Button*)ui->get_widget("openlevelcancel"))->signal_clicked().connect(sigc::mem_fun(*this, &LevelEditor::onOpenLevelCancel));
	Gtk::TreeView *list = (Gtk::TreeView*)ui->get_widget("openlevellist");
	Gtk::TreeModelColumnRecord levelcolumns;
	Gtk::TreeModelColumn<Glib::ustring> levelnamecolumn;
	levelcolumns.add(levelnamecolumn);
	Glib::RefPtr<Gtk::TreeStore> treestore = Gtk::TreeStore::create(levelcolumns);
	list->set_model(treestore);
	list->remove_all_columns();
	list->append_column("Name", levelnamecolumn);
	
	std::vector<std::string> levels = peak::FileSystem::get()->listFiles("levels", ".xml");
	for (unsigned int i = 0; i < levels.size(); i++)
	{
		levels[i] = levels[i].substr(0, levels[i].size() - 4);
		Gtk::TreeModel::iterator it = treestore->append();
		Gtk::TreeModel::Row row = *it;
		row[levelnamecolumn] = levels[i].c_str();
	}
	
	// Show dialog
	int result = dialog->run();
	dialog->hide();
	if (result == (int)GTK_RESPONSE_OK)
	{
		// Load level
		Gtk::TreeModel::Row row = *list->get_selection()->get_selected();
		std::string levelname = row.get_value(levelnamecolumn).raw();
		openLevel(levelname);
	}
}
void LevelEditor::onOpenLevelOk(void)
{
	Gtk::TreeView *list = (Gtk::TreeView*)ui->get_widget("openlevellist");
	if (list->get_selection()->count_selected_rows() == 0) return;
	Gtk::Dialog *dialog = (Gtk::Dialog*)ui->get_widget("openleveldialog");
	dialog->response((int)GTK_RESPONSE_OK);
	dialog->hide();
}
void LevelEditor::onOpenLevelCancel(void)
{
	Gtk::Dialog *dialog = (Gtk::Dialog*)ui->get_widget("openleveldialog");
	dialog->response((int)GTK_RESPONSE_CANCEL);
	dialog->hide();
}
void LevelEditor::onSaveLevel(void)
{
	Gtk::Dialog *dialog = (Gtk::Dialog*)ui->get_widget("saveleveldialog");
	Gtk::Entry *entry = (Gtk::Entry*)ui->get_widget("savelevelname");
	entry->set_text(levelname.c_str());
	((Gtk::Button*)ui->get_widget("savelevelok"))->signal_clicked().connect(sigc::mem_fun(*this, &LevelEditor::onSaveLevelOk));
	((Gtk::Button*)ui->get_widget("savelevelcancel"))->signal_clicked().connect(sigc::mem_fun(*this, &LevelEditor::onSaveLevelCancel));
	dialog->run();
	// Show dialog
	dialog->hide();
	int result = dialog->run();
	// FIXME: One has to click a button twice to close the dialog. Obviously I am
	// too stupid for gtkmm dialogs.
	dialog->hide();
	if (result == (int)GTK_RESPONSE_OK)
	{
		// Save level
		if (entry->get_text() != "")
		{
			levelname = entry->get_text().raw();
			if (levelname != "")
			{
				saveLevel(levelname);
			}
		}
	}
}
void LevelEditor::onSaveLevelOk(void)
{
	Gtk::Dialog *dialog = (Gtk::Dialog*)ui->get_widget("saveleveldialog");
	dialog->response((int)GTK_RESPONSE_OK);
	dialog->hide();
}
void LevelEditor::onSaveLevelCancel(void)
{
	Gtk::Dialog *dialog = (Gtk::Dialog*)ui->get_widget("saveleveldialog");
	dialog->response((int)GTK_RESPONSE_CANCEL);
	dialog->hide();
}
void LevelEditor::onNewLevel(void)
{
	newLevel("");
}

