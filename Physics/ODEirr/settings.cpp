// The code is distributed under the terms of the GNU General Public License
// Part of the OSRPG project (http://osrpgcreation.sf.net)
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#include "settings.h"

//=====================================
// Global settings
//=====================================

GameSettings *settings;

GameSettings::GameSettings (string filename) {
	// Set default values, which will be used if Load () fails to read
	// some settings
	settings_file = filename;
	SetFullscreen (false);
	SetDriverType (EDT_OPENGL);
	SetScreenResolution (0);
	SetBoxesAmount (30);
	SetBoxMass (5);
	SetBoxSize (5);
	SetIters (10);
	SetStep (0.05);
	SetGravity (-2.5);
}

bool GameSettings::Load () {
	// We use C functions here, because... um... give us a better C++ code?
	char buf[256];
	char tmp[256];
	FILE *f;
	if ((f = fopen (settings_file.c_str (), "r")) == NULL) {
		cout << "Settings file doesn't exist. "
			"New one will be created at the end of the program, "
			"now default values will be used.";
		// Defaults were already set, so quit the function
		return true;
	}
	// Get driver type
	fgets (buf, 256, f);
	sscanf (buf, "VIDEO_DRIVER=%s", &tmp);

		SetDriverType (EDT_OPENGL);

	// Get screen resolution
	fgets (buf, 256, f);
	sscanf (buf, "SCREEN_RESOLUTION=%s", &tmp);
	if (strcmp ("640x480", tmp) == 0) {
		SetScreenResolution (0);
	}
	else
		if (strcmp ("800x600", tmp) == 0) {
			SetScreenResolution (1);
		}
		else
			if (strcmp ("1024x768", tmp) == 0) {
				SetScreenResolution (2);
			}
			else {
				cout << "Screen resolution wasn't recognized, "
					"default values will be used instead.\n";
			}
	// Fullscreen?
	fgets (buf, 256, f);
	sscanf (buf, "FULLSCREEN=%s", &tmp);
	if (strcmp ("TRUE", tmp) == 0) {
		SetFullscreen (true);
	}
	else
		if (strcmp ("FALSE", tmp) == 0) {
			SetFullscreen (false);
		}
		else {
			cout << "Fullscreen boolean wasn't recognized, default "
				"value will be used instead.\n";
		}
	// WARNING: No error check
	// Gravity
	float fmp;
	fgets (buf, 256, f);
	// May be try {try - catch} block?
	sscanf (buf, "GRAVITY=%f", &fmp);
	SetGravity (fmp);
	// Step
	fgets (buf, 256, f);
	sscanf (buf, "STEP=%f", &fmp);
	SetStep (fmp);
	// Iters
	int imp;
	fgets (buf, 256, f);
	sscanf (buf, "ITERS=%i", &imp);
	SetIters (imp);
	// Boxes amount
	fgets (buf, 256, f);
	sscanf (buf, "BOXES_AMOUNT=%i", &imp);
	SetBoxesAmount (imp);
	// Box size
	fgets (buf, 256, f);
	sscanf (buf, "BOX_SIZE=%f", &fmp);
	SetBoxSize (fmp);
	// Box mass
	fgets (buf, 256, f);
	sscanf (buf, "BOX_MASS=%f", &fmp);
	SetBoxMass (fmp);
	// Capsule mass
	fgets (buf, 256, f);
	sscanf (buf, "CAPSULE_MASS=%f", &fmp);
	SetCapsuleMass (fmp);

	return true;
}

bool GameSettings::Save () {
	ofstream out (settings_file.c_str ());
	// Video driver
	if (GetDriverType () == EDT_OPENGL)
		out << "VIDEO_DRIVER=OPENGL" << endl;
	else
		out << "VIDEO_DRIVER=SOFTWARE" << endl;
	// Screen resolution
	switch (GetScreenResolution ()) {
		case 0:
			out << "SCREEN_RESOLUTION=640x480" << endl;
			break;
		case 1:
			out << "SCREEN_RESOLUTION=800x600" << endl;
			break;
		case 2:
			out << "SCREEN_RESOLUTION=1024x768" << endl;
			break;
	}
	// Fullscreen?
	if (IsFullscreen ())
		out << "FULLSCREEN=TRUE" << endl;
	else
		out << "FULLSCREEN=FALSE" << endl;
	// Gravity
	out << "GRAVITY=" << gravity << endl;
	// Step
	out << "STEP=" << step << endl;
	// Iters
	out << "ITERS=" << iters << endl;
	// Boxes amount
	out << "BOXES_AMOUNT=" << boxes_amount << endl;
	// Box size
	out << "BOX_SIZE=" << box_size << endl;
	// Box mass
	out << "BOX_MASS=" << box_mass << endl;
	// Capsule mass
	out << "CAPSULE_MASS=" << capsule_mass << endl;
	return true;
}

bool GameSettings::IsFullscreen () {
	return fullscreen;
}

void GameSettings::SetFullscreen (bool fs) {
	fullscreen = fs;
}

unsigned short GameSettings::GetScreenWidth () {
	return screen_width;
}

unsigned short GameSettings::GetScreenHeight () {
	return screen_height;
}

void GameSettings::SetScreenResolution (unsigned short index) {
	// We use only standard resolutions, because... um...
	// Don't modify it to handle non-standard resolutions, since you'll break
	// GetScreenResolution func then!
	switch (index) {
		case 0: screen_width = 640;
				screen_height = 480;
				break;
		case 1: screen_width = 800;
				screen_height = 600;
				break;
		case 2: screen_width = 1024;
				screen_height = 768;
				break;
	}
}

unsigned short GameSettings::GetScreenResolution () {
	switch (screen_width) {
		case 640:
			return 0;
			break;
		case 800:
			return 1;
			break;
		case 1024:
			return 2;
			break;
	}
}

E_DRIVER_TYPE GameSettings::GetDriverType () {
	return driver_type;
}

unsigned short GameSettings::GetDriverTypeIdx () {
	switch (driver_type) {
		case EDT_OPENGL:
			return 0;
			break;
//		case EDT_SOFTWARE2:
	//		return 1;
	//		break;
	}
}

void GameSettings::SetDriverType (E_DRIVER_TYPE dt) {
	driver_type = dt;
}

void GameSettings::SetDriverType (unsigned short index) {
	switch (index) {
		case 0:
			driver_type = EDT_OPENGL;
			break;
		//case 1:
		//	driver_type = EDT_SOFTWARE2;
		//	break;
	}
}

unsigned short GameSettings::GetBoxesAmount () {
	return boxes_amount;
}

void GameSettings::SetBoxesAmount (unsigned short am) {
	boxes_amount = am;
}

float GameSettings::GetBoxMass () {
	return box_mass;
}

void GameSettings::SetBoxMass (float m) {
	box_mass = m;
}

float GameSettings::GetBoxSize () {
	return box_size;
}

void GameSettings::SetBoxSize (float s) {
	box_size = s;
}

unsigned short GameSettings::GetIters () {
	return iters;
}

void GameSettings::SetIters (unsigned short i) {
	iters = i;
}

float GameSettings::GetStep () {
	return step;
}

void GameSettings::SetStep (float st) {
	step = st;
}

float GameSettings::GetGravity () {
	return gravity;
}

void GameSettings::SetGravity (float gr) {
	gravity = gr;
}

float GameSettings::GetCapsuleMass () {
	return capsule_mass;
}

void GameSettings::SetCapsuleMass (float m) {
	capsule_mass = m;
}

