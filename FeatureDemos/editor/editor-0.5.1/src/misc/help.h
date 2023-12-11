// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#ifndef OSRPG_EDITOR_MISC_HELP_H
#define OSRPG_EDITOR_MISC_HELP_H

/// \ingroup Miscelleneous
///@{
/// \file help.h
/// \brief Some helpful functions

#include <irrlicht.h>

/// Convert degree to radian

/// \param Degree
/// \return Radian
float DegreeToRadian (float degree);
/// Print vector to console

/// \param vec Vector
///
void PrintVector (const irr::core::vector3df vec);
/// Conver radian to degree

/// \param radian Radian
/// \return Degree
float RadianToDegree (float radian);
/// Report message to console

/// \param msg Message
/// \param sender Sender
void Report (const char *msg, const char *sender);

///@}

#endif // OSRPG_EDITOR_MISC_HELP_H

