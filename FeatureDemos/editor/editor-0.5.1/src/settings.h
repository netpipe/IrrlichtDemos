// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#ifndef OSRPG_EDITOR_SETTINGS_H
#define OSRPG_EDITOR_SETTINGS_H

#include <irrlicht.h>

class Settings {
	public:
		Settings (const char *fn);
		~Settings ();
		irr::core::stringc GetDataDir () const;
		Settings& SetDataDir (irr::core::stringc data_dir);

	private:
		void Load ();
		void Save ();

		char *filename;
		irr::core::stringc data_dir;
};

#endif // OSRPG_EDITOR_SETTINGS_H

