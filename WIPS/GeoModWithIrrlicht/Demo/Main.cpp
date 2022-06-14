/*
===============================================================================
	File:	Main.cpp
	Desc:
===============================================================================
*/

#include "stdafx.h"
#pragma hdrstop
#include <Include/IrrMix.h>

namespace {
	::mix::mxWorld	g_World;
}//end of anonymous namespace

int main( int argc, int argv[] )
{
	//return MainWithPortals();

	{
		using namespace mix;

		GWorld = & g_World;

        GWorld->Spawn();
		GWorld->Run();
		GWorld->Shutdown();
	}

	return 0;
}



