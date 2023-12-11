// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#ifndef OSRPG_EDITOR_MAP_SCENE_H
#define OSRPG_EDITOR_MAP_SCENE_H

#include "mesh.h"
#include "light.h"
#include "zone.h"
#include "place_tag.h"
#include "sky.h"
#include "joint_ball_socket.h"
#include "joint_hinge.h"

class MapScene {
	public:
		MapScene (Registry *reg);
		~MapScene ();
		bool AddJointBallSocket (const char *name1, const char *name2,
				const irr::core::vector3df anchor,
				const bool constrain_x = false,
				const bool constrain_y = false,
				const bool constrain_z = false,
				const short x_low = 0, const short x_high = 0,
				const short y_low = 0, const short y_high = 0,
				const short z_low = 0, const short z_high = 0);
		bool AddJointBallSocket (const OMFJointBallSocket b);
		bool AddJointHinge (const char *name1, const char *name2,
				const irr::core::vector3df anchor,
				const irr::core::vector3df axis,
				const bool constrain, const short angle_low = 0,
				const short angle_high = 0);
		bool AddJointHinge (const OMFJointHinge h);
		void AddLight (const char *name, const irr::core::vector3df pos,
				const float rad, const irr::video::SColorf col);
		void AddLight (const OMFLight l);
		void AddMesh (const char *name, const char *script_name,
				const char *model_file, const char *collision_model_file,
				const irr::core::vector3df pos,
				const irr::core::vector3df rot, const char maggun_effect,
				const float mass, bool collidable = true,
				bool optimize = false);
		void AddMesh (const OMFMesh m);
		void AddPlaceTag (const char *name, const irr::core::vector3df pos);
		void AddPlaceTag (const OMFPlaceTag pt);
		void AddZone (const char *name, const char *model_file,
				const char *script_name, const irr::core::vector3df pos,
				const irr::core::vector3df rot);
		void AddZone (const OMFZone z);
		// This can be called in constructor
		void Clean ();
		// This cannot be called in constructor
		void Clear ();
		void DeselectItem ();
		void DrawAddons ();
		bool EditJointBallSocket (const unsigned int id, const char *name1,
				const char *name2, const irr::core::vector3df anchor,
				const bool constrain_x = false,
				const bool constrain_y = false,
				const bool constrain_z = false,
				const short x_low = 0, const short x_high = 0,
				const short y_low = 0, const short y_high = 0,
				const short z_low = 0, const short z_high = 0);
		bool EditJointHinge (const unsigned int id, const char *name1,
				const char *name2, 	const irr::core::vector3df anchor,
				const irr::core::vector3df axis, const bool constrain,
				const short angle_low = 0, const short angle_high = 0);
		void EditLight (const unsigned int id, const char *name,
				const irr::core::vector3df pos, const float rad,
				const irr::video::SColorf col);
		void EditMesh (const unsigned int id, const char *name,
				const char *script_name, const char *model_file,
				const char *collision_model_file,
				const irr::core::vector3df pos,
				const irr::core::vector3df rot, const char maggun_effect,
				const float mass, bool collidable = true,
				bool optimize = false);
		void EditPlaceTag (const unsigned int id, const char *name,
				const irr::core::vector3df pos);
		void EditZone (const unsigned int id, const char *name,
				const char *model_file, const char *script_name,
				const irr::core::vector3df pos,
				const irr::core::vector3df rot);
		void EditSelectedJointBallSocket (const char *name1,
				const char *name2, const irr::core::vector3df anchor,
				const bool constrain_x = false,
				const bool constrain_y = false,
				const bool constrain_z = false,
				const short x_low = 0, const short x_high = 0,
				const short y_low = 0, const short y_high = 0,
				const short z_low = 0, const short z_high = 0);
		void EditSelectedJointBallSocket (const OMFJointBallSocket b);
		void EditSelectedJointHinge (const char *name1, const char *name2,
				const irr::core::vector3df anchor,
				const irr::core::vector3df axis,
				const bool constrain, const short angle_low = 0,
				const short angle_high = 0);
		void EditSelectedJointHinge (const OMFJointHinge h);
		void EditSelectedLight (const char *name,
				const irr::core::vector3df pos, const float rad,
				const irr::video::SColorf col);
		void EditSelectedLight (const OMFLight l);
		void EditSelectedMesh (const char *name, const char *script_name,
				const char *model_file, const char *collision_model_file,
				const irr::core::vector3df pos,
				const irr::core::vector3df rot, const char maggun_effect,
				const float mass, bool collidable = true,
				bool optimize = false);
		void EditSelectedMesh (const OMFMesh m);
		void EditSelectedPlaceTag (const char *name,
				const irr::core::vector3df pos);
		void EditSelectedPlaceTag (const OMFPlaceTag pt);
		void EditSelectedZone (const char *name, const char *model_file,
				const char *script_name, const irr::core::vector3df pos,
				const irr::core::vector3df rot);
		void EditSelectedZone (const OMFZone z);
		irr::core::stringc GetCurFile () const;
		irr::core::vector3df GetGravity () const;
		irr::core::stringc GetName () const;
		irr::core::stringc GetScriptName () const;
		OMFJointBallSocket GetSelectedJointBallSocketCopy () const;
		OMFJointHinge GetSelectedJointHingeCopy () const;
		OMFLight GetSelectedLightCopy () const;
		OMFMesh GetSelectedMeshCopy () const;
		OMFPlaceTag GetSelectedPlaceTagCopy () const;
		OMFZone GetSelectedZoneCopy () const;
		char GetSelectedType () const;
		MapSky* GetMapSky () const;
		bool Import (FILE *f, const unsigned char major,
				const unsigned char minor);
		void Open (const char *fn);
		void RemoveItem (const unsigned int id);
		void RemoveSelected ();
		void Save (const char *fn);
		void SelectItem (const unsigned int id, bool state);
		void SetGravity (irr::core::vector3df gr);
		void SetName (const char *name);
		void SetScriptName (const char *name);
		bool SkyDomeIsUsed () const;

	private:
		Registry *reg;
		std::vector<MapMesh*> map_meshes;
		std::vector<MapLight*> map_lights;
		std::vector<MapZone*> map_zones;
		std::vector<MapPlaceTag*> map_place_tags;
		std::vector<MapJointBallSocket*> jbs;
		std::vector<MapJointHinge*> jhs;
		MapSky *sky;
		MapItem *selected_item;
		irr::core::stringc cur_file,
			name,
			script_name;
		irr::core::vector3df gravity;
};

#endif // OSRPG_EDITOR_MAP_SCENE_H

