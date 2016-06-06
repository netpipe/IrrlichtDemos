// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#ifndef OSRPG_SETTINGS_H
#define OSRPG_SETTINGS_H

/// \ingroup CoreGame
///@{

/// Settings

#include <irrlicht.h>

class Settings {
	public:
		/// Constructor

		/// \param fn File name
		///
		Settings (const char *fn);
		/// Destructor
		~Settings ();
		/// Get bits per pixel

		/// \return Bits per pixel
		///
		unsigned short GetBPP () const;
		/// Get bits per pixel index

		/// \return Bits per pixel index
		/// - 0: 16 bpp
		/// - 1: 32 bpp
		unsigned short GetBPPIndex () const;
		/// Get driver index

		/// \return Driver index
		/// - 0: OpenGL
		/// - 1: Software
		unsigned short GetDriverIndex () const;
		/// Get driver type

		/// \return Driver type
		///
		irr::video::E_DRIVER_TYPE GetDriverType () const;
		/// Get gravity

		/// \return Gravity
		///
		float GetGravity () const;
		/// Get amount of iterations

		/// \return Amount of iterations
		///
		unsigned short GetIters () const;
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
		/// Get step

		/// \return Step
		///
		float GetStep () const;
		/// Is full screen?

		/// \return FullScreen state
		///
		bool IsFullScreen () const;
		/// Set bits per pixel

		/// \param bpp Bits per pixel
		/// \return Settings
		Settings& SetBPP (const unsigned short bpp);
		/// Set bits per pixel index

		/// \param i Bits per pixel index
		/// - 0: 16 bpp
		/// - 1: 32 bpp
		/// \return Settings
		Settings& SetBPPIndex (const unsigned short i);
		/// Set driver index

		/// \param i Driver index
		/// - 0: OpenGL
		/// - 1: Software
		/// \return Settings
		Settings& SetDriverIndex (const unsigned short i);
		/// Set driver type

		/// \param dt Driver type
		/// \return Settings
		Settings& SetDriverType (const irr::video::E_DRIVER_TYPE dt);
		/// Set gravity

		/// \param g Gravity
		/// \return Settings
		Settings& SetGravity (const float g);
		/// Set full screen or not

		/// \param st FullScreen state
		/// \return Settings
		Settings& SetFullScreen (const bool st);
		/// Set amount of iterations

		/// \param i Amount of iterations
		/// \return Settings
		Settings& SetIters (const unsigned short i);
		/// Set screen resolution index

		/// \param i Screen resolution index
		/// - 0: 640x480
		/// - 1: 800x600
		/// - 2: 1024x768
		/// \return Settings
		Settings& SetScreenRes (const unsigned short i);
		/// Set step

		/// \param s Step
		/// \return Settings
		Settings& SetStep (const float s);

	private:
		void Load ();
		void Save ();

		char *filename;
		// Graphics
		bool fs;
		irr::video::E_DRIVER_TYPE dt;
		unsigned short sh,
					   sw,
					   bpp;
		// Physics
		// We have 2 copies for each physics parameter so that
		// the first ones will be used within Get functions,
		// the second ones will be used within Set functions
		// This is necessary because we can't change physics parameters at
		// run time, but we need to keep this parameters changed somewhere
		// so that it doesn't affect application
		float gr1,
			  gr2,
			  step1,
			  step2;
		unsigned short iters1,
					   iters2;
};

///@}

#endif // OSRPG_SETTINGS_H

