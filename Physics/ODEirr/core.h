// The code is distributed under the terms of the GNU General Public License
// Part of the OSRPG project (http://osrpgcreation.sf.net)
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

/// Core (it's really overwhelmed, we should make it clearer)

#ifndef GAME_CORE_H
#define GAME_CORE_H

#include "settings.h"
#include "menu.h"
#include "physics/basic.h"
#include "base/rag_doll.h"
#include "base/map.h"
#include "base/box.h"
#include "base/player.h"
#include "camera.h"
#include "misc/player_direction.h"
#include "base/cam_attachment.h"

//#define DEBUG

class GameCore {
	public:
		/// Constructor
		GameCore ();
		/// Destructor
		~GameCore ();
		/// Run application (loop)
		RunMsg Run ();
		/// Start application (init)
		void Start ();
		/// Request renderer restart
		void Restart ();
		/// Request application exit
		void Quit ();
		/// This tells menu which window to show after renderer restart

		/// Be default no window is shown
		/// \param index Window index
		void LeaveWindowOpen (int index = -1);
		/// Show/hide GUI

		/// \param state Show/hide state
		///
		void ShowGUI (bool state);
		/// Toggle GUI show state
		void ToggleGUI ();
		/// Is GUI shown?
		bool GUIIsShown ();
		/// Shoot the box
		void Shoot ();
		/// Update ray length

		/// This updates ray length displayed on the screen
		/// and requests Player capsule height to be updated
		/// \param ray Ray length
		void UpdateRayLength (float ray);
		/// Get player
		BasePlayer* GetPlayer ();
		/// Request Player capsule update

		/// \param ray Ray length
		///
		void SubmitRay (float ray);
		/// Request Player move

		/// \param index Direction
		/// \n 0: forward
		/// \n 1: backward
		/// \n 2: left
		/// \n 3: right
		void SubmitPlayerMove (unsigned short index);

		/// Update camera position
		void UpdateCamPos ();
		void UpdateSpeedTexts ();
		void Jump ();

		// test
		/*
		void RotX (bool state);
		void RotY (bool state);
		void RotZ (bool state);
		void PrintRot ();
		*/

		/// Menu pointer (we should make it private)
		MainMenu *menu;
		/// Camera pointer (we should make it private)
		Camera camera;

	private:
		void CommitRay ();
		void CommitPlayerMove ();
		void CreateRenderer ();
		void CreateInterface ();
		void Simulate (float dt);
		void DrawAim ();

		//void LoadHandModel ();

		void UpdateNumOfEnabledBoxes ();

		void CreateNOEBText ();
		void CreateRayText ();
		void CreateSpeedTexts ();

		bool quit;
		bool restart;
		bool first_run;
		bool show_gui;
		bool submit_ray;
		bool submit_player_move;
		IrrlichtDevice *dev;
		IVideoDriver *drv;
		ISceneManager *smgr;
		IGUIEnvironment *env;
		int leave_window_open;
		BaseRagDoll *rag_doll;
		BaseMap *map;
		BaseBox **wall_box;
		BaseBox *shot;
		BasePlayer *player;
		float submitted_ray;
		bool *wbon;
		IGUIStaticText *noeb_text;
		IGUIStaticText *ray_text;
		IGUIStaticText *add_force;
		IGUIStaticText *linear_vel;

		float accum,
			  cur_time;
		ITimer *timer;

		int wbam,
			iters;
		float wbsize,
			  wbmass,
			  step,
			  capsule_mass,
			  gravity,
			  ray_height;
		core::vector3df lv, af;
		// We don't have float gravity, because it's initialized
		// in CreateWorld, which runs only when app starts
		// Now we have it since we need it in player positioning
		MiscPlayerDirection *pd;

		// For hand and gun
		BaseCamAttachment *ca[2];
		MeshNode *hand_node;

		/*
		int rot_x,
			rot_y,
			rot_z;
		*/
};
/// This must be global to be accessible for EvRec class
GameCore game_core;

/// Event receiver
class EvRec: public IEventReceiver {
	public:
		virtual bool OnEvent (const SEvent& ev);
};
/// This must be global to be accessible for GameCore class :P
EvRec receiver;

/// This functions unfortunately here as well, because we need to access the
/// ray. May be there's another way so that we can place it in
/// physics/basic.h?
void NearCallback (void *data, dGeomID o1, dGeomID o2);

#endif // GAME_CORE_H

