// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#include <fstream>
#include "settings.h"
#include "help.h"
using namespace std;
using namespace irr;
using namespace video;

Settings::Settings (const char *fn) {
	filename = new char[strlen (fn) + 1];
	strcpy (filename, fn);
	// Set defaults
	SetDriverType (EDT_OPENGL);
	SetGravity (-10);
	SetBPP (32);
	SetFullScreen (false);
	SetIters (2);
	SetScreenRes (0);
	SetStep (0.01);
	// Try to load settings
	Load ();
}

Settings::~Settings () {
	Save ();
	delete filename;
}

unsigned short Settings::GetBPP () const {
	return bpp;
}

unsigned short Settings::GetBPPIndex () const {
	switch (bpp) {
		case 16:
			return 0;
			break;
		case 32:
			return 1;
			break;
		default:
			return 0;
			break;
	}
}

unsigned short Settings::GetDriverIndex () const {
	switch (dt) {
		case EDT_OPENGL:
			return 0;
			break;
	//	case EDT_SOFTWARE2:
	//		return 1;
	//		break;
		default:
			return 0;
	}
}

E_DRIVER_TYPE Settings::GetDriverType () const {
	return dt;
}

float Settings::GetGravity () const {
	return gr1;
}

unsigned short Settings::GetIters () const {
	return iters1;
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

float Settings::GetStep () const {
	return step1;
}

bool Settings::IsFullScreen () const {
	return fs;
}

Settings& Settings::SetBPP (const unsigned short bpp) {
	Settings::bpp = bpp;
	return *this;
}

Settings& Settings::SetBPPIndex (const unsigned short i) {
	switch (i) {
		case 0:
			bpp = 16;
			break;
		case 1:
			bpp = 32;
			break;
		default:
			bpp = 16;
			break;
	}
	return *this;
}

Settings& Settings::SetDriverIndex (const unsigned short i) {
	switch (i) {
		case 0:
			dt = EDT_OPENGL;
			break;
	//	case 1:
	//		dt = EDT_SOFTWARE2;
	//		break;
		default:
			dt = EDT_OPENGL;
			break;
	}
	return *this;
}

Settings& Settings::SetDriverType (const E_DRIVER_TYPE dt) {
	Settings::dt = dt;
	return *this;
}

Settings& Settings::SetGravity (const float g) {
	gr2 = g;
	return *this;
}

Settings& Settings::SetFullScreen (const bool st) {
	fs = st;
	return *this;
}

Settings& Settings::SetIters (const unsigned short i) {
	iters2 = i;
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

Settings& Settings::SetStep (const float s) {
	step2 = s;
	return *this;
}

void Settings::Load () {
	const int BS = 128;
	// cmp stands for char tmp
	char buf[BS],
		 cmp[BS];
	FILE *f;
	if ((f = fopen (filename, "r")) == NULL) {
		Report ("Settings file doesn't exist. New one will be created "
				"upon application exit. Using defaults.", "Settings::Load");
		return;
	}
	// Driver type
	fgets (buf, BS, f);
	sscanf (buf, "VIDEO_DRIVER=%s", &cmp);
	if (strcmp ("OPENGL", cmp) == 0)
		SetDriverType (EDT_OPENGL);
	else
		//if (strcmp ("SOFTWARE", cmp) == 0)
		//	SetDriverType (EDT_SOFTWARE2);
		//else
		//	Report ("Video driver wasn't recognized. Using defaults.",
		//			"Settings::Load");
	// Screen resolution
	fgets (buf, BS, f);
	sscanf (buf, "SCREEN_RESOLUTION=%s", &cmp);
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
						"Using defaults.",
						"Settings::Load");
	int imp;
	// Bits per pixel
	if (fgets (buf, BS, f) && sscanf (buf, "BPP=%i", &imp))
		SetBPP (imp);
	else
		Report ("Bits per pixel weren't recognized. Using defaults.",
				"Settings::Load");
	// FullScreen
	fgets (buf, BS, f);
	sscanf (buf, "FULLSCREEN=%s", &cmp);
	if (strcmp ("TRUE", cmp) == 0)
		SetFullScreen (true);
	else
		if (strcmp ("FALSE", cmp) == 0)
			SetFullScreen (false);
		else
			Report ("FullScreen boolean wasn't recongized. Using defaults.",
					"Settings::Load");
	float fmp;
	// Gravity
	if (fgets (buf, BS, f) && sscanf (buf, "GRAVITY=%f", &fmp))
		SetGravity (fmp);
	else
		Report ("Gravity wasn't recognized. Using defaults.",
				"Settings::Load");
	// Step
	if (fgets (buf, BS, f) && sscanf (buf, "STEP=%f", &fmp))
		SetStep (fmp);
	else
		Report ("Step wasn't recognized. Using defaults.",
				"Settings::Load");
	// Iters
	if (fgets (buf, BS, f) && sscanf (buf, "ITERS=%i", &imp))
		SetIters (imp);
	else
		Report ("Iters weren't recognized. Using defaults.",
				"Settings::Load");

	gr1 = gr2;
	iters1 = iters2;
	step1 = step2;
}

void Settings::Save () {
	ofstream out (filename);
	// Video driver
	if (dt == EDT_OPENGL)
		out << "VIDEO_DRIVER=OPENGL\n";
	else
		out << "VIDEO_DRIVER=SOFTWARE\n";
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
	// Bits per pixel
	out << "BPP=" << bpp << endl;
	// FullScreen
	if (fs)
		out << "FULLSCREEN=TRUE\n";
	else
		out << "FULLSCREEN=FALSE\n";
	// Gravity
	out << "GRAVITY=" << gr2 << endl;
	// Step
	out << "STEP=" << step2 << endl;
	// Iters
	out << "ITERS=" << iters2 << endl;
}

