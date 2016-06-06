// The code is distributed under the terms of the GNU General Public License
// Part of the OSRPG project (http://osrpgcreation.sf.net)
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#include "player_direction.h"

MiscPlayerDirection::MiscPlayerDirection () {
	Reset ();
}

void MiscPlayerDirection::Reset () {
	forward = false;
	backward = false;
	left = false;
	right = false;
}

