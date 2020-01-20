// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#ifndef OSRPG_GAME_MAP_SKY_H
#define OSRPG_GAME_MAP_SKY_H

#include "../registry.h"
#include "format.h"

class MapSky {
	public:
		MapSky (Registry *rg);
		~MapSky ();
		void Clear ();
		void CreateNode ();
		OMFSkyBox GetSkyBoxCopy () const;
		OMFSkyDome GetSkyDomeCopy () const;
		void SetSkyBox (OMFSkyBox sb);
		void SetSkyDome (OMFSkyDome sd);
		bool SkyDomeIsUsed () const;
		void SetSkyDomeUsed (bool state);

	private:
		Registry *reg;
		bool skydome_is_used;
		OMFSkyBox sb;
		OMFSkyDome sd;
		irr::scene::ISceneNode *n;
		irr::core::stringc tex_dir;
};

#endif // OSRPG_GAME_MAP_SKY_H

