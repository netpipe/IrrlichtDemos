/*
 * main.cpp
 *
 *  Created on: Jan 10, 2012
 *      Author: guillaume
 */
 
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_syswm.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
 
#include <iostream>
 
using namespace std;
 
int main(int argc, char **argv) {
	Display * display;
	Window window;
	SDL_Surface * screen;
 
	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0 ) {
		cout << "SDL_Init Failed " << endl;
		return false;
	}
 
	// Make SDL Window
	if( !(screen = SDL_SetVideoMode(300, 200, 32, SDL_NOFRAME | SDL_ANYFORMAT | SDL_HWSURFACE | SDL_DOUBLEBUF ) ) ) {
		cout << "SDL_SetVideoMode Failed " << endl;
		return false;
	}
 
	// Create Info structure
	SDL_SysWMinfo info;
	SDL_VERSION(&info.version);
 
	// Get info from SDL
	if(!SDL_GetWMInfo(&info))
		cout << "SDL cant get from SDL" << endl;
 
	// Check we are on X11
	if ( info.subsystem != SDL_SYSWM_X11 )
		cout << "SDL is not running on X11" << endl;
 
	// Store X11 informations
	display = info.info.x11.display;
	window = info.info.x11.window;
 
	cout	<< "Display : " << display << endl
			<< "Window : " << window << endl;
 
	// try to move SDL Window
	switch(XMoveWindow(display, window,200,200)) {
	case BadWindow :
		cout << "BadWindow" << endl;
		break;
	case BadValue :
		cout << "BadValue" << endl;
		break;
	case BadRequest :
		cout << "BadRequest" << endl;
		break;
	}
 
	sleep(5);
}
