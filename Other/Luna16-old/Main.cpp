#include "Luna.h"
#include <cstdio>

#ifdef __APPLE__
	#import <Cocoa/cocoa.h>
#endif

#ifdef WIN32
	#include <winsock2.h>
#endif

int main ( int argc, char** argv )
{
	#ifdef __OSX__
		NSApplicationLoad();
	#endif
		Luna game ( argc,argv );
		game.Run();
		system("PAUSE");
	return 0;
}

