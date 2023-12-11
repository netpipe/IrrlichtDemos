1. Installation

	This demo application uses the Irrlicht engine for rendering, ODE for physics and irrKlang for sound.
	There are precompiled libs in the lib directory.

	To build the demo with gcc, go to the source directory and type

	linux: 'make'
	windows: 'mingw32-make all_win32' (not tested yet)

	this will build the demo with sound. if you want to don't want to use irrKlang as sound library, type

	'make NOSOUND=1'
	'mingw32-make NOSOUND=1 all_win32'

2. Usage

	The binary is located in the demo directory.

	Movement:

		W - move forward
		A - roll left
		S - move backward
		D - roll right

		UP - pitch down
		DOWN - pitch up	
		LEFT - turn left
		RIGHT - turn right

	Controls:

		C - lock upVector of camera
		F12 - make screenshot

		Mouse button left - shoot
		Mouse button right - show forces (gravity as blue line, spring forces as yellow lines)

                SPACE - shoot

