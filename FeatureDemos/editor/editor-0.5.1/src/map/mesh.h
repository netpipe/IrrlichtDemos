// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#ifndef OSRPG_EDITOR_MAP_MESH_H
#define OSRPG_EDITOR_MAP_MESH_H

#include "format.h"
#include "item.h"

class MapMesh: public MapItem {
	public:
		MapMesh (Registry *reg, const char *name, const char *script_name,
				const char *model_file, const char *collision_model_file,
				const irr::core::vector3df pos,
				const irr::core::vector3df rot, const char maggun_effect,
				const float mass, bool collidable, bool optimize);
		MapMesh (Registry *reg, const OMFMesh m);
		void Edit (const char *name, const char *script_name,
				const char *model_file, const char *collision_model_file,
				const irr::core::vector3df pos,
				const irr::core::vector3df rot, const char maggun_effect,
				const float mass, bool collidable, bool optimize);
		OMFMesh GetCopy () const;
		irr::core::vector3df GetPosition () const;
		char GetType () const;
		void SetCollidable (bool state);
		void SetCollisionModelFile (const char *cmf);
		void SetMaggunEffect (const char maggun_effect);
		void SetMass (const float mass);
		void SetModelFile (const char *model_file, bool optimize = false);
		void SetName (const char *name);
		void SetPosition (const irr::core::vector3df pos);
		void SetRotation (const irr::core::vector3df rot);
		void SetScriptName (const char *s);

	private:
		OMFMesh mesh;
};

#endif // OSRPG_EDITOR_MAP_MESH_H

