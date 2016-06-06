// The code is distributed under the terms of the GNU General Public License
// Part of the OSRPG project (http://osrpgcreation.sf.net)
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#ifndef MISC_PLAYER_DIRECTION
#define MISC_PLAYER_DIRECTION

class MiscPlayerDirection {
	public:
		MiscPlayerDirection ();
		void Reset ();

		bool forward,
			 backward,
			 left,
			 right;
	private:
};

#endif // MISC_PLAYER_DIRECTION

