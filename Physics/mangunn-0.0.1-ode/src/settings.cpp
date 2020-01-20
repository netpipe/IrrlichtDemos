// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#include <fstream>
#include "settings.h"
#include "misc/help.h"
using namespace std;
using namespace irr;

Settings::Settings (const char *fn) {
	filename = new char[strlen (fn) + 1];
	strcpy (filename, fn);
	// Set defaults
	SetDataDir ("data/");
	SetFullScreen (false);
	SetScreenRes (0);
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

core::stringc Settings::GetMapFile () const {
	return map_file;
}

unsigned short Settings::GetScreenHeight () const {
	return sh;
}

unsigned short Settings::GetScreenRes () const {
	switch (sw) {
		case 640:
			return 0;
			break;
		case 800:
			return 1;
			break;
		case 1024:
			return 2;
			break;
		default:
			return 0;
			break;
	}
}

unsigned short Settings::GetScreenWidth () const {
	return sw;
}

bool Settings::IsFullScreen () const {
	return fs;
}

Settings& Settings::SetDataDir (irr::core::stringc data_dir) {
	Settings::data_dir = data_dir;
	return *this;
}

Settings& Settings::SetFullScreen (const bool state) {
	fs = state;
	return *this;
}

Settings& Settings::SetMapFile (irr::core::stringc mf) {
	map_file = mf;
	return *this;
}

Settings& Settings::SetScreenRes (const unsigned short i) {
	switch (i) {
		case 0:
			sw = 640;
			sh = 480;
			break;
		case 1:
			sw = 800;
			sh = 600;
			break;
		case 2:
			sw = 1024;
			sh = 768;
			break;
		default:
			sw = 640;
			sh = 480;
			break;
	}
	return *this;
}

void Settings::Load () {
	ifstream in (filename);
	if (in.is_open ()) {
	string s;
	// Data dir
	getline (in, s);
	if (strstr (s.c_str (), "DATA_DIR=")) {
		string ss;
		for (unsigned short i = 9; i < s.length (); i++)
			ss += s[i];
		SetDataDir (core::stringc (ss.c_str ()));
	}
	else
		Report ("DATA_DIR was not found. Using defaults", "Settings::Load");
	const int BS = 64;
	char cmp[BS];
	// Fullscreen
	getline (in, s);
	sscanf (s.c_str (), "FULLSCREEN=%s", cmp);
	if (strcmp ("TRUE", cmp) == 0)
		SetFullScreen (true);
	else
		if (strcmp ("FALSE", cmp) == 0)
			SetFullScreen (false);
		else
			Report ("FullScreen boolean wasn't recongized. Using defaults.",
					"Settings::Load");
	// Screen resolution
	getline (in, s);
	sscanf (s.c_str (), "SCREEN_RESOLUTION=%s", cmp);
	if (strcmp ("640x480", cmp) == 0)
		SetScreenRes (0);
	else
		if (strcmp ("800x600", cmp) == 0)
			SetScreenRes (1);
		else
			if (strcmp ("1024x768", cmp) == 0)
				SetScreenRes (2);
			else
				Report ("Screen resolution wasn't recognized. "
						"Using defaults.", "Settings::Load");
	// Map file
	getline (in, s);
	if (strstr (s.c_str (), "MAP_FILE=")) {
		string ss;
		for (unsigned short i = 9; i < s.length (); i++)
			ss += s[i];
		SetMapFile (core::stringc (ss.c_str ()));
	}
	else
		Report ("ERROR: MAP_FILE was not found", "Settings::Load");


	}
}

void Settings::Save () {
	ofstream out (filename);
	// Data directory
	out << "DATA_DIR=" << data_dir.c_str () << endl;
	// FullScreen
	if (fs)
		out << "FULLSCREEN=TRUE\n";
	else
		out << "FULLSCREEN=FALSE\n";
	// Screen resolution
	switch (GetScreenRes ()) {
		case 0:
			out << "SCREEN_RESOLUTION=640x480\n";
			break;
		case 1:
			out << "SCREEN_RESOLUTION=800x600\n";
			break;
		case 2:
			out << "SCREEN_RESOLUTION=1024x768\n";
			break;
	}
	// Map file
	out << "MAP_FILE=" << map_file.c_str () << endl;
}

