// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#ifndef OSRPG_GAME_SETTINGS_H
#define OSRPG_GAME_SETTINGS_H

/// Settings

#include <irrlicht.h>
#include <ode/ode.h>

class Settings {
	public:
		/// Constructor

		/// \param fn File name
		///
		Settings (const char *fn);
		/// Destructor
		~Settings ();
		irr::core::stringc GetDataDir () const;
		irr::core::stringc GetMapFile () const;
		/// Get screen height

		/// \return Screen height
		///
		unsigned short GetScreenHeight () const;
		/// Get screen resolution index

		/// \return Screen resolution index
		/// - 0: 640x480
		/// - 1: 800x600
		/// - 2: 1024x768
		unsigned short GetScreenRes () const;
		/// Get screen width

		/// \return Screen width
		///
		unsigned short GetScreenWidth () const;
		/// Is full screen?

		/// \return FullScreen state
		///
		bool IsFullScreen () const;
		Settings& SetDataDir (irr::core::stringc data_dir);
		Settings& SetFullScreen (const bool state);
		Settings& SetMapFile (irr::core::stringc mf);
		/// Set screen resolution index

		/// \param i Screen resolution index
		/// - 0: 640x480
		/// - 1: 800x600
		/// - 2: 1024x768
		/// \return Settings
		Settings& SetScreenRes (const unsigned short i);

	private:
		void Load ();
		void Save ();

		char *filename;
		// Graphics
		bool fs;
		unsigned short sh,
					   sw;
		irr::core::stringc data_dir,
			map_file;
};

#endif // OSRPG_GAME_SETTINGS_H

