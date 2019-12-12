/* Copyright (c) <2009> <Newton Game Dynamics>
* 
* This software is provided 'as-is', without any express or implied
* warranty. In no event will the authors be held liable for any damages
* arising from the use of this software.
* 
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely
*/

#include "dStdAfxMath.h"
#include "dMathDefines.h"


#ifdef _DEBUG
#include <stdio.h>

#if (_MSC_VER >= 1400)
#	pragma warning (disable: 4996) // for 2005 users declared deprecated
#endif

void dExpandTraceMessage (const char *fmt, ...)
{
	va_list v_args;
	char text[1024];

	text[0] = 0;
	va_start (v_args, fmt);     
	vsprintf(text, fmt, v_args);
	va_end (v_args);            

	OutputDebugStringA (text);
}
#endif

