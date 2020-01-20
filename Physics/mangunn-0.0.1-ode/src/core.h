// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#ifndef OSRPG_GAME_CORE_H
#define OSRPG_GAME_CORE_H

#include "gui/main_menu.h"
#include "map/scene.h"

/// Core

class Core:	public irr::IEventReceiver {
	public:
		/// Constructor
		Core ();
		/// Destructor
		~Core ();
		/// Static NearCallback

		/// Gets called when two geoms may potentially collide. You
		/// must not call this function!
		/// \param data "this" pointer
		/// \param o1 First geom
		/// \param o2 Second geom
		//static void NearCallback (void *data, dGeomID o1, dGeomID o2);
		/// Member function NearCallback.

		/// Gets called by static NearCallback. You must not call this
		/// function!
		/// \param o1 First geom
		/// \param o2 Second geom
		//void NearCallback (dGeomID o1, dGeomID o2);
		/// Gets called when event occurs.

		/// You must not call this function!
		/// \param ev Event
		/// \return Tells if event has been handled
		bool OnEvent (const irr::SEvent &e);
		/// Quit

		/// \return Whether we should restart renderer or simply quit app
		///
		bool Quit ();
		/// Main loop
		void Run ();
		/// Physics simulation

		/// \param dt Step
		/// \param iters Amount of iterations to perform
		//void Simulate (float dt, int iters);
		/// Initialize graphics and physics (if necessary) objects
		void Start ();
		/// Some necessary updates
		//void Update ();

	private:
		irr::IrrlichtDevice *dev;
		irr::scene::ICameraSceneNode *cam;
		Settings *settings;
		Registry *reg;
		MainMenu *mm;
		MapScene *ms;
		//bool first_run; // NO NEED IN IT! WE HAVE reg POINTER!
};

#endif // OSRPG_GAME_CORE_H

