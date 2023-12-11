// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#include "help.h"
#include <iostream>
using namespace std;
using namespace irr;

float DegreeToRadian (float degree) {
	return M_PI * degree / 180;
}

void PrintVector (const core::vector3df vec) {
	cout << "vec[0] = " << vec.X << endl;
	cout << "vec[1] = " << vec.Y << endl;
	cout << "vec[2] = " << vec.Z << endl;
	cout << "_________\n";
}

float RadianToDegree (float radian) {
	return 180 * radian / M_PI;
}

void Report (const char *msg, const char *sender) {
	cout << "\n========================================\n";
	cout << "Message from " << sender;
	cout << "\n/--------------------------------------\\\n";
	cout << msg << endl;
	cout << "\\--------------------------------------/\n";
}

