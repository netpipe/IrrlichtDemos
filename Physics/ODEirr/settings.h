// The code is distributed under the terms of the GNU General Public License
// Part of the OSRPG project (http://osrpgcreation.sf.net)
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

/// Game settings container
#ifndef GAME_SETTINGS_H
#define GAME_SETTINGS_H

#include "globals.h"

class GameSettings {
	public:
		/// Constructor

		/// \param filename Path to settings file
		///
		GameSettings (string filename);
		/// Load settings
		bool Load ();
		/// Save settings
		bool Save ();
		/// Fullscreen?
		bool IsFullscreen ();
		/// Set fullscreen or non-fullscreen

		/// \param fs Fullscreen or non-fullscreen
		///
		void SetFullscreen (bool fs);
		/// Get screen width
		unsigned short GetScreenWidth ();
		/// Get screen height
		unsigned short GetScreenHeight ();
		/// Set screen resolution index

		/// \param index Index of standard resolutions
		/// \n 0: 640x480
		/// \n 1: 800x600
		/// \n 2: 1024x768
		void SetScreenResolution (unsigned short index);
		/// Get screen resolution index
		unsigned short GetScreenResolution ();
		/// Get driver type
		E_DRIVER_TYPE GetDriverType ();
		/// Get driver type index
		unsigned short GetDriverTypeIdx ();
		/// Set driver type

		/// \param dt Driver type (irr::video::E_DRIVER_TYPE)
		///
		void SetDriverType (E_DRIVER_TYPE dt);
		/// Set driver type index
		
		/// \param Set driver type index of standard drivers
		/// \n 0: OpenGL
		/// \n 1: Software
		void SetDriverType (unsigned short index);
		/// Get amount of wall boxes
		unsigned short GetBoxesAmount ();
		/// Set amount of wall boxes

		/// \param am Amount of boxes
		///
		void SetBoxesAmount (unsigned short am);
		/// Get box mass
		float GetBoxMass ();
		/// Set box mass

		/// \param m Box mass
		///
		void SetBoxMass (float m);
		/// Get box size
		float GetBoxSize ();
		/// Set box size

		/// \param s Box size
		///
		void SetBoxSize (float s);
		/// Get amount of iterations
		unsigned short GetIters ();
		/// Set amount of iterations
		
		/// \param i Amount of interations
		///
		void SetIters (unsigned short i);
		/// Get step
		float GetStep ();
		/// Set step

		/// \param st Set step
		///
		void SetStep (float st);
		/// Get gravity
		float GetGravity ();
		/// Set gravity

		/// \param gr Gravity (must be negative to act in down direction)
		///
		void SetGravity (float gr);
		/// Get capsule mass
		float GetCapsuleMass ();
		/// Set capsule mass
		
		/// \param m Capsule mass
		void SetCapsuleMass (float m);
	
	private:
		bool fullscreen;
		E_DRIVER_TYPE driver_type;
		string settings_file;
		unsigned short screen_width,
					 screen_height;
		unsigned short boxes_amount,
					   iters;
		float step,
			  gravity,
			  box_mass,
			  box_size,
			  capsule_mass;
};

//=====================================
// Global settings
//=====================================

extern GameSettings *settings;

#endif // GAME_SETTINGS_H

