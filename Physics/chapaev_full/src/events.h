// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#ifndef OSRPG_EVENTS_H
#define OSRPG_EVENTS_H

/// \ingroup CoreGame
///@{

/// \file events.h
/// \brief Events

enum Events {
	EV_QUIT = 101,
	EV_ABOUT,
	EV_ABOUT_OK,
	EV_OPTIONS,
	EV_OPTIONS_OK,
	EV_OPTIONS_CANCEL,
	EV_OPTIONS_APPLY,
	EV_OPTIONS_VD,
	EV_OPTIONS_RES,
	EV_OPTIONS_BPP,
	EV_OPTIONS_FS,
	EV_OPTIONS_STEP,
	EV_OPTIONS_ITERS,
	EV_OPTIONS_GRAVITY
};

///@}

#endif // OSRPG_EVENTS_H

