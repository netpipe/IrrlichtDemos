//main entry point for the application. not much to do here
#include "App.h"

CApp* g_pApp;
int main()
{
	CApp theapp;
	g_pApp = &theapp;
	theapp.Run();
}
