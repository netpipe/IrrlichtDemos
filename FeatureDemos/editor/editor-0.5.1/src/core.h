// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#ifndef OSRPG_EDITOR_CORE_H
#define OSRPG_EDITOR_CORE_H

#include "gui/main_menu.h"

class Core: public irr::IEventReceiver {
	public:
		Core ();
		~Core ();
		virtual bool OnEvent (const irr::SEvent &e);
		void Run ();
		void Quit ();
		void Update ();

	private:
		Registry *reg;
		Settings *settings;
		irr::IrrlichtDevice *dev;
		MainMenu *mm;
		irr::scene::ILightSceneNode *ln;
		int fps;
};

#endif // OSRPG_EDITOR_CORE_H

