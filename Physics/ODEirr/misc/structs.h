// The code is distributed under the terms of the GNU General Public License
// Part of the OSRPG project (http://osrpgcreation.sf.net)
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

/// \file structs.h
/// \brief Some structs

#ifndef MISC_STRUCTS_H
#define MISC_STRUCTS_H

typedef struct {
	bool forward,
		 backward,
		 left,
		 right;
	void Reset () {
		forward = false;
		backward = false;
		left = false;
		right = false;
	}
} PlayerDirection;

#endif // MISC_STRUCTS_H

