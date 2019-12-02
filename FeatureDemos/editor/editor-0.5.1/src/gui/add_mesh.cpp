// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#include "add_mesh.h"
#include "../misc/events.h"
using namespace irr;
using namespace gui;
using namespace io;
using namespace core;

AddMesh::AddMesh (Registry *reg, MapScene *ms) {
	AddMesh::reg = reg;
	AddMesh::ms = ms;
	model_dir = reg->settings->GetDataDir () + "models/";
	const int dx = 400;
	const int dy = 300;
	const int x = 100;
	const int y = 100;
	wn = reg->env->addWindow (rect<s32> (x, y, x + dx, y + dy), false,
			L"Add mesh");
	wn->getCloseButton ()->setVisible (false);
	Show (false);
	ok_btn = reg->env->addButton (rect<s32> (dx - 120, dy - 30, dx - 70,
				dy - 10), wn, EV_ADD_MESH_ITEM_OK, L"OK");
	cancel_btn = reg->env->addButton (rect<s32> (dx - 60, dy - 30, dx - 10,
				dy - 10), wn, EV_ADD_MESH_ITEM_CANCEL, L"Cancel");
	list = reg->env->addListBox (rect<s32> (10, 30, dx - 10, dy - 40), wn);
}

AddMesh::~AddMesh () {

}

void AddMesh::OnCancel () {
	Show (false);
}

bool AddMesh::OnEvent (const irr::SEvent &e) {
	if (e.EventType == EET_GUI_EVENT) {
		s32 id = e.GUIEvent.Caller->getID ();
		switch (e.GUIEvent.EventType) {
			case EGET_BUTTON_CLICKED:
				switch (id) {
					case EV_ADD_MESH_ITEM_CANCEL:
						OnCancel ();
						return true;
						break;
					case EV_ADD_MESH_ITEM_OK:
						OnOk ();
						return true;
						break;
					default:
						break;
				}
				break;
			default:
				break;
		}
	}
	return false;
}

void AddMesh::OnOk () {
	if (list->getSelected () != -1) {
		stringc s = stringc (list->getListItem (list->getSelected ()));
		ms->AddMesh ("", "", s.c_str (), "", reg->cam->getPosition (),
				vector3df (0, 0, 0), 1, 0, false);
	}
	Show (false);
}

void AddMesh::Show (bool state) {
	wn->setVisible (state);
	// Refresh
	if (state) {
		reg->dev->getFileSystem ()->changeWorkingDirectoryTo (
				model_dir.c_str ());
		IFileList *fl = reg->dev->getFileSystem ()->createFileList ();
		list->clear ();
		for (unsigned short i = 0; i < fl->getFileCount (); i++)
			if (!fl->isDirectory (i))
				if (strstr (fl->getFileName (i), ".obj") != 0)
					list->addItem (stringw (fl->getFileName (i)).c_str ());
	}
}

