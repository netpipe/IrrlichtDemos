// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#include "properties.h"
#include "../misc/events.h"
using namespace irr;
using namespace gui;
using namespace core;
using namespace io;

Properties::Properties (Registry *reg, MapScene *ms) {
	Properties::reg = reg;
	Properties::ms = ms;
	tex_dir = reg->settings->GetDataDir () + "textures/";
	IGUIEnvironment *env = reg->env;
	const int dx = 400;
	const int dy = 500;
	const int x = 100;
	const int y = 100;
	wn = env->addWindow (rect<s32> (x, y, x + dx, y + dy), false,
			L"Properties");
	wn->getCloseButton ()->setVisible (false);
	name_text = env->addStaticText (L"Name:", rect<s32> (10, 30,
				dx - 10, 50), false, false, wn);
	name = env->addEditBox (L"", rect<s32> (10, 50, dx - 10, 70), true,
			wn);
	script_text = env->addStaticText (L"Script name:", rect<s32> (10, 80,
				dx - 10, 100), false, false, wn);
	script = env->addEditBox (L"", rect<s32> (10, 100, dx - 10, 120), true,
			wn);
	gravity_text = env->addStaticText (L"Gravity:", rect<s32> (10, 130,
				dx - 10, 150), false, false, wn);
	gr_x = env->addEditBox (L"", rect<s32> (10, 150, 135, 170), true,
			wn);
	gr_y = env->addEditBox (L"", rect<s32> (135, 150, 260, 170), true,
			wn);
	gr_z = env->addEditBox (L"", rect<s32> (260, 150, 380, 170), true,
			wn);
	list_text = env->addStaticText (L"Texture:", rect<s32> (10, 180,
				dx - 10, 200), false, false, wn);
	list = env->addListBox (rect<s32> (10, 200, dx - 10, 290), wn);
	b1 = env->addButton (rect<s32> (10, 300, 30, 320), wn, EV_USE1, L"->");
	t1 = env->addEditBox (L"", rect<s32> (30, 300, dx - 10, 320), true, wn);
	b2 = env->addButton (rect<s32> (10, 320, 30, 340), wn, EV_USE2, L"->");
	t2 = env->addEditBox (L"", rect<s32> (30, 320, dx - 10, 340), true, wn);
	b3 = env->addButton (rect<s32> (10, 340, 30, 360), wn, EV_USE3, L"->");
	t3 = env->addEditBox (L"", rect<s32> (30, 340, dx - 10, 360), true, wn);
	b4 = env->addButton (rect<s32> (10, 360, 30, 380), wn, EV_USE4, L"->");
	t4 = env->addEditBox (L"", rect<s32> (30, 360, dx - 10, 380), true, wn);
	b5 = env->addButton (rect<s32> (10, 380, 30, 400), wn, EV_USE5, L"->");
	t5 = env->addEditBox (L"", rect<s32> (30, 380, dx - 10, 400), true, wn);
	b6 = env->addButton (rect<s32> (10, 400, 30, 420), wn, EV_USE6, L"->");
	t6 = env->addEditBox (L"", rect<s32> (30, 400, dx - 10, 420), true, wn);

	ok_btn = env->addButton (rect<s32> (dx - 120, dy - 30, dx - 70,
				dy - 10), wn, EV_PROPERTIES_OK, L"OK");
	cancel_btn = env->addButton (rect<s32> (dx - 60, dy - 30, dx - 10,
				dy - 10), wn, EV_PROPERTIES_CANCEL, L"Cancel");
	skydome = env->addCheckBox (true, rect<s32> (10, dy - 30, 100, dy - 10),
			wn, EV_SKY_DOME, L"SkyDome");
	Show (false);
}

Properties::~Properties () {

}

void Properties::OnCancel () {
	Show (false);
}

bool Properties::OnEvent (const irr::SEvent &e) {
	if (e.EventType == EET_GUI_EVENT) {
		s32 id = e.GUIEvent.Caller->getID ();
		switch (e.GUIEvent.EventType) {
			case EGET_BUTTON_CLICKED:
				switch (id) {
					case EV_PROPERTIES_CANCEL:
						OnCancel ();
						return true;
						break;
					case EV_PROPERTIES_OK:
						OnOk ();
						return true;
						break;
					case EV_USE1:
						UseTexture (0);
						return true;
						break;
					case EV_USE2:
						UseTexture (1);
						return true;
						break;
					case EV_USE3:
						UseTexture (2);
						return true;
						break;
					case EV_USE4:
						UseTexture (3);
						return true;
						break;
					case EV_USE5:
						UseTexture (4);
						return true;
						break;
					case EV_USE6:
						UseTexture (5);
						return true;
						break;
					default:
						break;
				}
				break;
			case EGET_CHECKBOX_CHANGED:
				switch (id) {
					case EV_SKY_DOME:
						SetSkyDomeOn (!ms->GetMapSky ()->SkyDomeIsUsed ());
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
void Properties::OnOk () {
	ms->SetName (stringc (name->getText ()).c_str ());
	ms->SetScriptName (stringc (script->getText ()).c_str ());
	char buf[64];
	float fmp;
	vector3df grav;
	strcpy (buf, stringc (gr_x->getText ()).c_str ());
	if (sscanf (buf, "%f", &fmp))
		grav.X = fmp;
	else
		grav.X = 0;
	strcpy (buf, stringc (gr_y->getText ()).c_str ());
	if (sscanf (buf, "%f", &fmp))
		grav.Y = fmp;
	else
		grav.Y = 0;
	strcpy (buf, stringc (gr_z->getText ()).c_str ());
	if (sscanf (buf, "%f", &fmp))
		grav.Z = fmp;
	else
		grav.Z = 0;
	ms->SetGravity (grav);
	if (skydome->isChecked ()) {
		OMFSkyDome sd;
		strcpy (sd.texture, stringc (t1->getText ()).c_str ());
		int imp;
		strcpy (buf, stringc (t2->getText ()).c_str ());
		if (sscanf (buf, "%i", &imp))
			sd.horiz_res = imp;
		strcpy (buf, stringc (t3->getText ()).c_str ());
		if (sscanf (buf, "%i", &imp))
			sd.vert_res = imp;
		strcpy (buf, stringc (t4->getText ()).c_str ());
		if (sscanf (buf, "%f", &fmp))
			sd.tpercentage = fmp;
		strcpy (buf, stringc (t5->getText ()).c_str ());
		if (sscanf (buf, "%f", &fmp))
			sd.spercentage = fmp;
		ms->GetMapSky ()->SetSkyDome (sd);
	}
	else {
		OMFSkyBox sb;
		strcpy (sb.top, stringc (t1->getText ()).c_str ());
		strcpy (sb.bottom, stringc (t2->getText ()).c_str ());
		strcpy (sb.left, stringc (t3->getText ()).c_str ());
		strcpy (sb.right, stringc (t4->getText ()).c_str ());
		strcpy (sb.front, stringc (t5->getText ()).c_str ());
		strcpy (sb.back, stringc (t6->getText ()).c_str ());
		ms->GetMapSky ()->SetSkyBox (sb);
	}
	Show (false);
}

void Properties::Show (bool state) {
	wn->setVisible (state);
	// Refresh
	if (state) {
		name->setText (stringw (ms->GetName ().c_str ()).c_str ());
		script->setText (stringw (ms->GetScriptName ().c_str ()).c_str ());
		vector3df grav = ms->GetGravity ();
		gr_x->setText (stringw (grav.X).c_str ());
		gr_y->setText (stringw (grav.Y).c_str ());
		gr_z->setText (stringw (grav.Z).c_str ());
		skydome->setChecked (ms->GetMapSky ()->SkyDomeIsUsed ());
		DisplaySkyDomeOn (skydome->isChecked ());
		reg->dev->getFileSystem ()->changeWorkingDirectoryTo (
				tex_dir.c_str ());
		IFileList *fl = reg->dev->getFileSystem ()->createFileList ();
		list->clear ();
		for (unsigned short i = 0; i < fl->getFileCount (); i++)
			if (!fl->isDirectory (i))
				list->addItem (stringw (fl->getFileName (i)).c_str ());
	}
}

void Properties::DisplaySkyDomeOn (bool state) {
	b2->setVisible (!state);
	b3->setVisible (!state);
	b4->setVisible (!state);
	b5->setVisible (!state);
	b6->setVisible (!state);
	t6->setVisible (!state);
	// skydome
	if (ms->GetMapSky ()->SkyDomeIsUsed ()) {
		OMFSkyDome sd = ms->GetMapSky ()->GetSkyDomeCopy ();
		t1->setText (stringw (sd.texture).c_str ());
		t2->setText (stringw ((int)sd.horiz_res).c_str ());
		t3->setText (stringw ((int)sd.vert_res).c_str ());
		t4->setText (stringw (sd.tpercentage).c_str ());
		t5->setText (stringw (sd.spercentage).c_str ());
	}
	else {
		OMFSkyBox sb = ms->GetMapSky ()->GetSkyBoxCopy ();
		t1->setText (stringw (sb.top).c_str ());
		t2->setText (stringw (sb.bottom).c_str ());
		t3->setText (stringw (sb.left).c_str ());
		t4->setText (stringw (sb.right).c_str ());
		t5->setText (stringw (sb.front).c_str ());
		t6->setText (stringw (sb.back).c_str ());
	}
}

void Properties::SetSkyDomeOn (bool state) {
	ms->GetMapSky ()->SetSkyDomeUsed (state);
	DisplaySkyDomeOn (state);
}

void Properties::UseTexture (char i) {
	int k = list->getSelected ();
	if (k == -1)
		return;
	IGUIEditBox *bp = 0;
	switch (i) {
		case 0:
			bp = t1;
			break;
		case 1:
			bp = t2;
			break;
		case 2:
			bp = t3;
			break;
		case 3:
			bp = t4;
			break;
		case 4:
			bp = t5;
			break;
		case 5:
			bp = t6;
			break;
	}
	bp->setText (list->getListItem (k));
}

