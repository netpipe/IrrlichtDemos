// The code is distributed under the terms of the GNU General Public License
// Part of the OSRPG project (http://osrpgcreation.sf.net)
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

/// \file evs.h
/// \brief Events enum

#ifndef MISC_ENUMS_H
#define MISC_ENUMS_H

/// Events enum
enum Evs {
	EvQuit = 101,
	EvTab1,
	EvTab2,
	EvApply,
	EvOk,
	EvCancel,
	EvRes,
	EvFullScr,
	EvOptions,
	EvVD,
	EvBoxesAmount,
	EvBoxSize,
	EvBoxMass,
	EvStep,
	EvGravity,
	EvIters,
	EvCapsule
};

/// We'll use this in main () to decide which way to go
enum RunMsg {
	RM_QUIT_APPLICATION = 0,
	RM_RESTART_RENDERER
};

#endif // MISC_ENUMS_H

