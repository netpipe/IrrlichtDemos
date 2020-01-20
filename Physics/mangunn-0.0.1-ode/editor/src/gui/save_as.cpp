// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#include "save_as.h"
#include "../misc/events.h"
using namespace irr;
using namespace gui;
using namespace core;
using namespace io;

SaveAs::SaveAs (Registry *reg, MapScene *ms) {
	SaveAs::reg = reg;
	SaveAs::ms = ms;
	map_dir = reg->settings->GetDataDir () + L"maps/";
	const int dx = 400;
	const int dy = 300;
	const int x = 100;
	const int y = 100;
	wn = reg->env->addWindow (rect<s32> (x, y, x + dx, y + dy), false,
			L"Save as");
	wn->getCloseButton ()->setVisible (false);
	Show (false);
	ok_btn = reg->env->addButton (rect<s32> (dx - 120, dy - 30, dx - 70,
				dy - 10), wn, EV_SAVE_AS_OK, L"OK");
	cancel_btn = reg->env->addButton (rect<s32> (dx - 60, dy - 30, dx - 10,
				dy - 10), wn, EV_SAVE_AS_CANCEL, L"Cancel");
	list = reg->env->addListBox (rect<s32> (10, 30, dx - 10, dy - 70), wn);
	edit = reg->env->addEditBox (L"", rect<s32> (10, dy - 60, dx - 10,
				dy - 40), true, wn);
}

SaveAs::~SaveAs () {

}

void SaveAs::OnCancel () {
	Show (false);
}

bool SaveAs::OnEvent (const irr::SEvent &e) {
	if (e.EventType == EET_GUI_EVENT) {
		s32 id = e.GUIEvent.Caller->getID ();
		switch (e.GUIEvent.EventType) {
			case EGET_BUTTON_CLICKED:
				switch (id) {
					case EV_SAVE_AS_CANCEL:
						OnCancel ();
						return true;
						break;
					case EV_SAVE_AS_OK:
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

void SaveAs::OnOk () {
	stringc s (edit->getText ());
	if (s.size () > 4)
		ms->Save (s.c_str ());
	Show (false);
}

void SaveAs::Show (bool state) {
	wn->setVisible (state);
	// Refresh
	if (state) {
		reg->dev->getFileSystem ()->changeWorkingDirectoryTo (map_dir.c_str ());
		IFileList *fl = reg->dev->getFileSystem ()->createFileList ();
		list->clear ();
		for (unsigned short i = 0; i < fl->getFileCount (); i++)
			if (!fl->isDirectory (i))
				if (strstr (fl->getFileName (i), ".omf") != 0)
					list->addItem (stringw (fl->getFileName (i)).c_str ());
	}
}

