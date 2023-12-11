// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#include <fstream>
#include "settings.h"
using namespace std;
using namespace irr;
using namespace video;
using namespace std;

Settings::Settings (const char *fn) {
	filename = new char[strlen (fn) + 1];
	strcpy (filename, fn);
	// Set defaults
	SetDataDir ("data/");
	// Try to load settings
	Load ();
}

Settings::~Settings () {
	Save ();
	delete filename;
}

core::stringc Settings::GetDataDir () const {
	return data_dir;
}

Settings& Settings::SetDataDir (irr::core::stringc data_dir) {
	Settings::data_dir = data_dir;
	return *this;
}

void Settings::Load () {
	ifstream in (filename);
	string s;
	getline (in, s);
	if (strstr (s.c_str (), "DATA_DIR=")) {
		string ss;
		for (unsigned short i = 9; i < s.length (); i++)
			ss += s[i];
		SetDataDir (core::stringc (ss.c_str ()));
	}
}

void Settings::Save () {
	ofstream out (filename);
	out << "DATA_DIR=" << data_dir.c_str () << endl;
}

