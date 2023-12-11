// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#ifndef OSRPG_CORE_H
#define OSRPG_CORE_H

/// \defgroup CoreGame
///@{

/// CoreGame

#include "gui/main_menu.h"
#include "phys/world.h"
#include "phys/sphere.h"
#include "phys/box.h"
#include "wrap/sphere.h"
#include "wrap/box.h"
#include "wrap/map.h"
#include "help.h"
#include "settings.h"
#include <vector>

class CoreGame:	public irr::IEventReceiver {
	public:
		/// Constructor
		CoreGame ();
		/// Destructor
		~CoreGame ();
		/// Static NearCallback

		/// Gets called when two geoms may potentially collide. You
		/// must not call this function!
		/// \param data "this" pointer
		/// \param o1 First geom
		/// \param o2 Second geom
		static void NearCallback (void *data, dGeomID o1, dGeomID o2);
		/// Member function NearCallback.

		/// Gets called by static NearCallback. You must not call this
		/// function!
		/// \param o1 First geom
		/// \param o2 Second geom
		void NearCallback (dGeomID o1, dGeomID o2);
		/// Gets called when event occurs.

		/// You must not call this function!
		/// \param ev Event
		/// \return I don't know :P
		bool OnEvent (const irr::SEvent& ev);
		/// Quit

		/// \return Whether we should restart renderer
		///
		bool Quit ();
		/// Main loop
		void Run ();
		/// Shoot
		void Shoot ();
		/// Physics simulation

		/// \param dt Step
		/// \param iters Amount of iterations to perform
		void Simulate (float dt, int iters);
		/// Initialize graphics and physics (if necessary) objects
		void Start ();
		/// Toggle GUI show state
		void ToggleGUI ();
		/// Update score
		void UpdateScore ();

	private:
		// Environment stuff
		irr::IrrlichtDevice *dev;
		irr::video::IVideoDriver *drv;
		irr::scene::ISceneManager *smgr;
		irr::gui::IGUIEnvironment *env;
		Settings *settings;
		MainMenu *mm;
		PhysWorld phys_world;
		unsigned short open_window;
		bool quit,
			 restart,
			 gui_shown,
			 first_run;
		// Gameplay stuff
		WrapMap *map;
		// Each cam_pos and cam_target is used by the red and the blue
		// teams respectively
		irr::core::vector3df cam_pos,
			cam_pos2;
		irr::core::vector3df cam_target,
			cam_target2;
		WrapSphere* sphere;
		std::vector<WrapBox*> box;
		unsigned short amount,
					   prev_bs,
					   prev_rs;
		char *blue_data,
			 *red_data;
		bool red_play,
			 shooted,
			 game_over;
};

///@}

#endif // OSRPG_CORE_H

