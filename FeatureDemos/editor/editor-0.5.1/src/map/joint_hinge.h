// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#ifndef OSRPG_EDITOR_MAP_JOINT_HINGE_H
#define OSRPG_EDITOR_MAP_JOINT_HINGE_H

#include "mesh.h"

class MapJointHinge: public MapItem {
	public:
		MapJointHinge (Registry *reg, MapMesh *m1, MapMesh *m2,
				const irr::core::vector3df anchor,
				const irr::core::vector3df axis,
				const bool constrain, const short angle_low,
				const short angle_high);
		void DrawAddons ();
		void Edit (MapMesh *m1, MapMesh *m2,
				const irr::core::vector3df anchor,
				const irr::core::vector3df axis,
				const bool constrain, const short angle_low,
				const short angle_high);
		OMFJointHinge GetCopy () const;
		char GetType () const;
		void Select (bool state);
		void GetVectorFromAngles (const irr::core::vector3df angles, irr::core::vector3df S);

	private:
		OMFJointHinge joint;
		MapMesh *m1,
				*m2;
		bool selected;
		irr::video::SMaterial mat;
		irr::core::matrix4 m;
};

#endif // OSRPG_EDITOR_MAP_JOINT_HINGE_H

