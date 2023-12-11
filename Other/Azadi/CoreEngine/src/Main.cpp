#include "Azadi.h"
#include "GUI/Console.h"
#include <cstdio>

#ifdef __APPLE__
	#import <Cocoa/cocoa.h>
#endif

#ifdef WIN32
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    char **tempo;
    tempo = new char*[3];
    *tempo = new char[15];
    strcpy(tempo[0], "-fullscreen");
    #ifndef NDEBUG
    strcpy(tempo[0], "Azadi");
    #endif
    int ret = main(1, tempo);
    delete [] *tempo;
    delete [] tempo;
    return ret;
}
#endif

#ifdef __VISCPP__
int _tmain (int argc, char* argv[])
#else
int main ( int argc, char** argv )
#endif
{
	#ifdef WIN32
	WSADATA wsaData;

	if(WSAStartup(MAKEWORD(1, 1), &wsaData) != 0)
	{
		fprintf(stderr, "WSAStartup failed.\n");
		return 1;
	}

	#endif

    #ifdef __OSX__
	NSApplicationLoad();
    #endif

    Azadi game(argc,argv);
    game.Run();

	#ifdef WIN32
	WSACleanup();
	#endif

    return 0;
}




