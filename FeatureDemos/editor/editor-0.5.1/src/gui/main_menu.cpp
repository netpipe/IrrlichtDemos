// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#include "main_menu.h"
#include "../misc/events.h"
#include "../misc/help.h"
using namespace irr;
using namespace gui;
using namespace video;
using namespace core;
using namespace scene;

#define DRAW_AXES
//#define SHOW_CROSS

MainMenu::MainMenu (Registry *reg) {
	ms = 0;
	cam_dir = -1;
	cam_is_moving = false;
	mat.Lighting = false;
	m.makeIdentity ();
	MainMenu::reg = reg;
	IGUIEnvironment *env = reg->env;
	IGUISkin *skin = env->createSkin (EGST_WINDOWS_METALLIC);
	env->setSkin (skin);
	stringc data_dir = reg->settings->GetDataDir ();
	stringc smp = data_dir + L"textures/fontlucida.png";
	IGUIFont *font = env->getFont (smp.c_str ());
	if (font)
		skin->setFont (font);
	else
		Report ("Couldn't load font. Using defaults", "MainMenu::MainMenu");
	skin->setColor (EGDC_3D_FACE, SColor (255, 205, 205, 255));

	menu = env->addMenu ();
	menu->addItem (L"File", -1, true, true);
	menu->addItem (L"Edit", -1, true, true);
	menu->addItem (L"Add", -1, true, true);
	//menu->addItem (L"Help", -1, true, true);
	IGUIContextMenu *sub;

	sub = menu->getSubMenu (0);
	sub->addItem (L"New     (Ctrl + N)", EV_NEW);
	sub->addItem (L"Open    (Ctrl + O)", EV_OPEN);
	sub->addItem (L"Save    (Ctrl + S)", EV_SAVE);
	sub->addItem (L"Save as", EV_SAVE_AS);
	sub->addItem (L"Quit    (Ctrl + Q)", EV_QUIT);

	sub = menu->getSubMenu (1);
	sub->addItem (L"Item properties (Ctrl + E)", EV_EDIT_ITEM);
	sub->addItem (L"Remove item     (Ctrl + R)", EV_REMOVE_ITEM);
	sub->addItem (L"Map properties", EV_PROPERTIES);
	sub->addItem (L"Deselect item   (Space)", EV_DESELECT_ITEM);
	/*
	sub->addItem (L"Undo", EV_UNDO);
	sub->addItem (L"Redo", EV_REDO);
	sub->addItem (L"Duplicate item", EV_DUPLICATE_ITEM);
	*/

	sub = menu->getSubMenu (2);
	sub->addItem (L"Mesh", EV_ADD_MESH_ITEM);
	sub->addItem (L"Light", EV_ADD_LIGHT_ITEM);
	sub->addItem (L"Zone", EV_ADD_ZONE_ITEM);
	sub->addItem (L"Placetag", EV_ADD_PLACE_TAG_ITEM);
	sub->addItem (L"Ball-socket joint", EV_ADD_JOINT_BALL_SOCKET);
	sub->addItem (L"Hinge joint", EV_ADD_JOINT_HINGE);

	/*
	sub = menu->getSubMenu (3);
	sub->addItem (L"About", EV_ABOUT);
	*/

	ms = new MapScene (reg);
	add_mesh = new AddMesh (reg, ms);
	edit_mesh = new EditMesh (reg, ms);
	edit_light = new EditLight (reg, ms);
	edit_zone = new EditZone (reg, ms);
	edit_place_tag = new EditPlaceTag (reg, ms);
	edit_joint_bs = new EditJointBS (reg, ms);
	edit_joint_hinge = new EditJointHinge (reg, ms);
	save_as = new SaveAs (reg, ms);
	open = new Open (reg, ms);
	properties = new Properties (reg, ms);

	text = env->addStaticText (L"Selected object name" , rect<s32> (10, 50, 300, 70));

	Show (false);
	Toggle ();
}

MainMenu::~MainMenu () {
	delete properties;
	delete open;
	delete save_as;
	delete edit_joint_bs;
	delete edit_joint_hinge;
	delete edit_place_tag;
	delete edit_zone;
	delete edit_light;
	delete edit_mesh;
	delete add_mesh;
	delete ms;
}

bool MainMenu::EditItem () {
	switch (ms->GetSelectedType ()) {
		// Mesh
		case 0:
			edit_mesh->Show ();
			return true;
			break;
		// Light
		case 1:
			edit_light->Show ();
			return true;
			break;
		// Zone
		case 2:
			edit_zone->Show ();
			return true;
			break;
		// Place tag
		case 3:
			edit_place_tag->Show ();
			return true;
			break;
		// Ball-socket joint
		case 4:
			edit_joint_bs->Show ();
			return true;
			break;
		// Hinge joint
		case 5:
			edit_joint_hinge->Show ();
			return true;
			break;
	}
	return false;
}

void MainMenu::MoveCam (char dir) {
	cam_is_moving = true;
	cam_dir = dir;
}

bool MainMenu::OnEvent (const irr::SEvent &e) {
	/*
	EKEY_CODE hotkey_toggle_gui;
	hotkey_toggle_gui = KEY_ESCAPE;
	*/
	if (e.EventType == EET_KEY_INPUT_EVENT &&
			e.KeyInput.PressedDown == false && !e.KeyInput.Control)
		switch (e.KeyInput.Key) {
			case KEY_ESCAPE:
			//case hotkey_toggle_gui:
				Toggle ();
				return true;
				break;
			case KEY_KEY_W:
			case KEY_KEY_S:
			case KEY_KEY_A:
		    case KEY_KEY_D:
				cam_is_moving = false;
				return true;
				break;
			case KEY_SPACE:
				if (!shown)
					ms->DeselectItem ();
				return true;
				break;
			default:
				break;
		}
	if (!shown && e.EventType == EET_KEY_INPUT_EVENT &&
			e.KeyInput.PressedDown == true && !e.KeyInput.Control) {
		switch (e.KeyInput.Key) {
			case KEY_KEY_W:
				MoveCam (0);
				return true;
				break;
			case KEY_KEY_S:
				MoveCam (1);
				return true;
				break;
			case KEY_KEY_A:
				MoveCam (2);
				return true;
				break;
			case KEY_KEY_D:
				MoveCam (3);
				return true;
				break;
			default:
				break;
		}
	}
	// Ctrl + button
	if (e.EventType == EET_KEY_INPUT_EVENT &&
			e.KeyInput.PressedDown == false &&
			e.KeyInput.Control == true)
		switch (e.KeyInput.Key) {
			case KEY_KEY_E:
				if (!shown) {
					shown = false;
					Toggle ();
				}
				return EditItem ();
				break;
			case KEY_KEY_N:
				ms->Clear ();
				return true;
				break;
			case KEY_KEY_O:
				if (!shown) {
					shown = false;
					Toggle ();
				}
				open->Show ();
				return true;
				break;
			case KEY_KEY_Q:
				reg->quit = true;
				return true;
				break;
			case KEY_KEY_R:
				ms->RemoveSelected ();
				return true;
				break;
			case KEY_KEY_S:
				OnSave ();
				return true;
				break;
			default:
				break;
		}
	
	if (e.EventType == EET_GUI_EVENT) {
		// WARNING: Watch out for that id ;)
		//s32 id = e.GUIEvent.Caller->getID ();
		switch (e.GUIEvent.EventType) {
			case EGET_MENU_ITEM_SELECTED: {
					IGUIContextMenu *menu = (IGUIContextMenu*)e.GUIEvent.Caller;
					s32 id = menu->getItemCommandId (menu->getSelectedItem ());
					switch (id) {
						case EV_ADD_JOINT_BALL_SOCKET:
							edit_joint_bs->Show (true, false);
							return true;
							break;
						case EV_ADD_JOINT_HINGE:
							edit_joint_hinge->Show (true, false);
							return true;
							break;
						case EV_ADD_LIGHT_ITEM:
							ms->AddLight ("", reg->cam->getPosition (), 10,
									SColorf (1.0f, 1.0f, 1.0f, 1.0f));
							return true;
							break;
						case EV_ADD_MESH_ITEM:
							add_mesh->Show ();
							return true;
							break;
						case EV_ADD_ZONE_ITEM:
							ms->AddZone ("", "box.obj", "",
									reg->cam->getPosition (),
									vector3df (0, 0, 0));
							return true;
							break;
						case EV_ADD_PLACE_TAG_ITEM:
							ms->AddPlaceTag ("", reg->cam->getPosition ());
							return true;
							break;
						case EV_DESELECT_ITEM:
							ms->DeselectItem ();
							return true;
							break;
						case EV_EDIT_ITEM:
							return EditItem ();
							break;
						case EV_NEW:
							ms->Clear ();
							return true;
							break;
						case EV_OPEN:
							open->Show ();
							return true;
							break;
						case EV_PROPERTIES:
							properties->Show ();
							return true;
							break;
						case EV_REMOVE_ITEM:
							ms->RemoveSelected ();
							return true;
							break;
						case EV_SAVE:
							OnSave ();
							return true;
							break;
						case EV_SAVE_AS:
							save_as->Show ();
							return true;
							break;
						case EV_QUIT:
							reg->quit = true;
							return true;
							break;
						default:
							break;
					}
				} // EGET_MENU_ITEM_SELECTED
				break;
			default:
				break;
		}
	}
	if (!shown && e.EventType == EET_MOUSE_INPUT_EVENT) {
		switch (e.MouseInput.Event) {
			case EMIE_LMOUSE_LEFT_UP: {
					//
					ISceneNode *sn =
						reg->smgr->getSceneCollisionManager (
								)->getSceneNodeFromCameraBB (reg->cam);
					if (sn && sn->getID () != -1) {
						ms->SelectItem (sn->getID (), true);
						//text->setText (stringw (sn->getID ()).c_str ());
						text->setText (stringw (sn->getName ()).c_str ());
					}
					if (sn && sn->getID () == -1)
						Report ("node id: -1", "MainMenu::OnEvent");
				}
				return true;
				break;
			default:
				break;
		}
	}
	if (add_mesh && add_mesh->OnEvent (e))
		return true;
	if (edit_mesh && edit_mesh->OnEvent (e))
		return true;
	if (edit_light && edit_light->OnEvent (e))
		return true;
	if (edit_zone && edit_zone->OnEvent (e))
		return true;
	if (edit_place_tag && edit_place_tag->OnEvent (e))
		return true;
	if (edit_joint_bs && edit_joint_bs->OnEvent (e))
		return true;
	if (edit_joint_hinge && edit_joint_hinge->OnEvent (e))
		return true;
	if (save_as && save_as->OnEvent (e))
		return true;
	if (open && open->OnEvent (e))
		return true;
	if (properties && properties->OnEvent (e))
		return true;
	return false;
}

void MainMenu::OnSave () {
	stringc s = ms->GetCurFile ();
	if (s.size () > 4)
		ms->Save (s.c_str ());
	else {
		if (!shown) {
			shown = false;
			Toggle ();
		}
		save_as->Show ();
	}
}

void MainMenu::Show (bool state) {
	shown = state;
	menu->setVisible (shown);
	// Hide all
	if (!shown) {
		add_mesh->Show (false);
		edit_mesh->Show (false);
		edit_light->Show (false);
		edit_zone->Show (false);
		edit_place_tag->Show (false);
		edit_joint_bs->Show (false);
		edit_joint_hinge->Show (false);
		properties->Show (false);
		open->Show (false);
		save_as->Show (false);
	}
}

void MainMenu::Toggle () {
	Show (shown = !shown);
	reg->dev->getCursorControl ()->setVisible (shown);
	reg->cam->setInputReceiverEnabled (!shown);
	if (!shown) {
		rect<s32> vp = reg->drv->getViewPort ();
	    reg->dev->getCursorControl ()->setPosition (
				vp.getWidth () / 2, vp.getHeight () / 2);
	}
}

void MainMenu::Update (float elapsed_time) {
#ifdef SHOW_CROSS
	IVideoDriver *drv = reg->drv;
	rect<s32> vp = drv->getViewPort ();
	if (!shown) {
		SColor cl = SColor (255, 200, 100, 50);
		// Draw aim cross
		drv->draw2DLine (position2d<s32> (
					vp.getWidth () / 2 - 10,
					vp.getHeight () / 2),
				position2d<s32> (vp.getWidth () / 2 - 4,
					vp.getHeight () / 2), cl);
		drv->draw2DLine (core::position2d<s32> (
					vp.getWidth () / 2 + 3,
					vp.getHeight () / 2),
				core::position2d<s32> (
					vp.getWidth () / 2 + 10,
					vp.getHeight () / 2), cl);
		drv->draw2DLine (core::position2d<s32> (
					vp.getWidth () / 2,
					vp.getHeight () / 2 + 9),
				core::position2d<s32> (
					vp.getWidth () / 2,
					vp.getHeight () / 2 + 3), cl);
		drv->draw2DLine (core::position2d<s32> (
					vp.getWidth () / 2,
					vp.getHeight () / 2 - 10),
				core::position2d<s32> (
					vp.getWidth () / 2,
					vp.getHeight () / 2 - 3), cl);
	}
#endif
	ms->DrawAddons ();
#ifdef DRAW_AXES
	DrawAxes ();
#endif
	// It doesn't fit well into MainMenu idea, but... where else?
	// Move camera
	if (cam_is_moving) {
		core::vector3df dir = - reg->cam->getPosition () + reg->cam->getTarget ();
		dir.normalize ();
		dir *= 0.5 * elapsed_time;
		switch (cam_dir) {
			case 0:
				reg->cam->setPosition (reg->cam->getPosition () + dir);
				break;
			case 1:
				reg->cam->setPosition (reg->cam->getPosition () - dir);
				break;
			case 2:
				reg->cam->setPosition (reg->cam->getPosition () +
						vector3df (-dir.Z, 0, dir.X));
				break;
			case 3:
				reg->cam->setPosition (reg->cam->getPosition () -
						vector3df (-dir.Z, 0, dir.X));
				break;
			default:
				break;
		}
	}
}

void MainMenu::DrawAxes () {
	vector3df dir = reg->cam->getTarget () - reg->cam->getPosition ();
	dir.normalize ();
	vector3df pos = reg->cam->getPosition () + dir * 1;
	reg->drv->setTransform (ETS_WORLD, m);
	reg->drv->setMaterial (mat);
	reg->drv->draw3DLine (pos, pos + vector3df (0.1, 0, 0),
			SColor (255, 255, 0, 0));
	reg->drv->draw3DLine (pos, pos + vector3df (0, 0.1, 0),
			SColor (255, 0, 255, 0));
	reg->drv->draw3DLine (pos, pos + vector3df (0, 0, 0.1),
			SColor (255, 0, 0, 255));
	/*
	reg->drv->draw3DLine (pos + vector3df (0.01, 0, 0), pos +
			vector3df (0.1, 0, 0), SColor (255, 255, 0, 0));
	reg->drv->draw3DLine (pos + vector3df (0, 0.01, 0), pos +
			vector3df (0, 0.1, 0), SColor (255, 0, 255, 0));
	reg->drv->draw3DLine (pos + vector3df (0, 0, 0.01), pos +
			vector3df (0, 0, 0.1), SColor (255, 0, 0, 255));
	*/
}

