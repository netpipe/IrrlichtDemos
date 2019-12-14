// main.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "main.h"


enum PASSES
{
	m_printDefine,
	m_printTypeDefine,
	m_printDataStructures,
	m_printFuntionCallbacks,
	m_printClassMethodNewton,
	m_printClassMethodJoints,
};

PASSES pass;
FILE* iNewtonHeader;
FILE* iNewtonSource;

void PrintClass ();
void PrintFuntionBody (const char* name);
void PrintPrototype (FILE* file, const char* name);

char* license = "/* Copyright (c) <2003-2011> <Julio Jerez, Newton Game Dynamics>\n"
				"* \n"
				"* This software is provided 'as-is', without any express or implied\n"
				"* warranty. In no event will the authors be held liable for any damages\n"
				"* arising from the use of this software.\n"
				"* \n"
				"* Permission is granted to anyone to use this software for any purpose,\n"
				"* including commercial applications, and to alter it and redistribute it\n"
				"* freely, subject to the following restrictions:\n"
				"* \n"
				"* 1. The origin of this software must not be misrepresented; you must not\n"
				"* claim that you wrote the original software. If you use this software\n"
				"* in a product, an acknowledgment in the product documentation would be\n"
				"* appreciated but is not required.\n"
				"* \n"
				"* 2. Altered source versions must be plainly marked as such, and must not be\n"
				"* misrepresented as being the original software.\n"
				"* \n"
				"* 3. This notice may not be removed or altered from any source distribution.\n"
				"*/\n\n";

char *newtonHeaderPath = "./newton.h";
char *jointsHeaderPath = "./jointLibrary.h";


int main(int argc, char* argv[])
{
	char path[1024];
	
	strcpy (path, newtonHeaderPath);
	strrchr (path, '/')[0] = 0;
	strcat (path, "/iNewton.h");
	iNewtonHeader = fopen (path, "w");

	strcpy (path, newtonHeaderPath);
	strrchr (path, '/')[0] = 0;
	strcat (path, "/iNewton.mm");
	iNewtonSource = fopen (path, "w");


	fprintf (iNewtonSource, "%s", license);
	fprintf (iNewtonHeader, "%s", license);
	fprintf (iNewtonHeader, "#import <Foundation/Foundation.h>\n\n\n");

	
	//  *****************	
	pass = m_printDefine; 
	Newtonin = fopen (newtonHeaderPath, "rb");
	Newtonparse ();
	fclose (Newtonin);

	Newtonin = fopen (jointsHeaderPath, "rb");
	Newtonparse ();
	fclose (Newtonin);
	fprintf (iNewtonHeader, "\n\n");

	//  *****************
	pass = m_printTypeDefine; 
	Newtonin = fopen (newtonHeaderPath, "rb");
	Newtonparse ();
	fclose (Newtonin);

	Newtonin = fopen (jointsHeaderPath, "rb");
	Newtonparse ();
	fclose (Newtonin);
	fprintf (iNewtonHeader, "\n\n");

	//  *****************
	pass = m_printDataStructures; 
	Newtonin = fopen (newtonHeaderPath, "rb");
	Newtonparse ();
	fclose (Newtonin);

	Newtonin = fopen (jointsHeaderPath, "rb");
	Newtonparse ();
	fclose (Newtonin);
	fprintf (iNewtonHeader, "\n\n");


	//  *****************
	pass = m_printFuntionCallbacks; 
	Newtonin = fopen (newtonHeaderPath, "rb");
	Newtonparse ();
	fclose (Newtonin);

	Newtonin = fopen (jointsHeaderPath, "rb");
	Newtonparse ();
	fclose (Newtonin);
	fprintf (iNewtonHeader, "\n\n");

	// *******************
	PrintClass ();

	// *******************
	pass = m_printClassMethodNewton; 
	Newtonin = fopen (newtonHeaderPath, "rb");
	Newtonparse ();
	fclose (Newtonin);

	pass = m_printClassMethodJoints; 
	Newtonin = fopen (jointsHeaderPath, "rb");
	Newtonparse ();
	fclose (Newtonin);
	fprintf (iNewtonHeader, "\n\n");

	fprintf (iNewtonHeader, "@end\n");
	fprintf (iNewtonSource, "@end\n");

	fclose (iNewtonSource);
	fclose (iNewtonHeader);
	return 0;
}


static int callbackCount;
static char callbackPool[1024][128];

void SaveCallback (const char* name)
{
	sprintf (&callbackPool[callbackCount][0], "%s", name);
	callbackCount ++;
}

const char* IsCallback (const char* name)
{
	for (int i = 0; i < callbackCount; i ++) {
		if (strstr (name, &callbackPool[i][0])) {
			return &callbackPool[i][0];
		}
	}
	return NULL;
}



void PrintClass ()
{
	fprintf (iNewtonHeader, "\n\n"
			"@interface iNewton : NSObject\n" 
			"{\n"
			"\tvoid* m_world;\n"
			"}\n\n"
			"-(id) init;\n"
			"-(id) initWithAllocator: (iNewtonAllocMemory) allocator: (iNewtonFreeMemory) deallocator;\n"
			"-(void) dealloc;\n");

	fprintf (iNewtonSource, 
			"//\n"
			"//  iNewton.mm\n"
			"//  newton_iPhone\n"
			"//\n"
			"//  Created by Julio Jerez on 7/27/09.\n"
			"//  Copyright 2009 Newton Game Dynamics. All rights reserved.\n"
			"//\n"
			"\n"
			"#import \"iNewton.h\"\n"
			"#include \"Newton.h\"\n"
			"#include \"JointLibrary.h\"\n"
			"\n\n"
			"@implementation iNewton\n"
			"\n\n"
			"-(id) init\n"
			"{\n"
			"\t[super init];\n"
			"\tm_world = NewtonCreate();\n"
			"\tNewtonSetPlatformArchitecture((const NewtonWorld*) m_world, 0);\n"
			"\t// set the linear solver model for faster speed \n"
			"\tNewtonSetSolverModel ((const NewtonWorld*) m_world, 0);\n"
			"\t\n"	
			"\tNewtonSetThreadsCount((const NewtonWorld*) m_world, 1);\n"
			"\t\n"
			"\t// set the adaptive friction model for faster speed\n" 
			"\tNewtonSetFrictionModel ((const NewtonWorld*) m_world, 0);\n"	
			"\t\n"	
			"\treturn self;\n"
			"}\n\n"
			"-(id) initWithAllocator: (iNewtonAllocMemory) allocator: (iNewtonFreeMemory) deallocator;\n"
			"{\n"
			"\t[super init];\n"
			"\t// Set the global memory allocator\n"
			"\tNewtonSetMemorySystem (allocator, deallocator);\n\n"
			"\tm_world = NewtonCreate();\n"
			"\tNewtonSetPlatformArchitecture((const NewtonWorld*) m_world, 0);\n"
			"\t// set the linear solver model for faster speed \n"
			"\tNewtonSetSolverModel ((const NewtonWorld*) m_world, 0);\n"
			"\t\n"	
			"\tNewtonSetThreadsCount((const NewtonWorld*) m_world, 1);\n"
			"\t\n"
			"\t// set the adaptive friction model for faster speed\n" 
			"\tNewtonSetFrictionModel ((const NewtonWorld*) m_world, 0);\n"	
			"\t\n"	
			"\treturn self;\n"
			"}\n\n"
			"-(void) dealloc\n"
			"{\n"
			"\tNewtonDestroy((const NewtonWorld*) m_world);\n"
			"\t[super dealloc];\n"
			"}\n\n");
}



void PrintDefine (const char* name)
{
	if (pass == m_printDefine) {
		fprintf (iNewtonHeader, "%s", name);
	}
}

void PrintStrutureDefine (const char* name)
{
	if (pass == m_printTypeDefine) {
		fprintf (iNewtonHeader, "%s", name);
	}
}


void PrintDataStrutures (const char* name)
{
	if (pass == m_printDataStructures) {
		if (!strstr (name, "NewtonHingeSliderUpdateDesc")){
			fprintf (iNewtonHeader, "%s", name);
		}
	}
}

void PrintFunctionCallbacks (const char* name)
{
	if (pass == m_printFuntionCallbacks) {
		if (!(strstr (name, "NewtonBallCallBack") ||
			strstr (name, "NewtonHingeCallBack") ||
			strstr (name, "NewtonUniversalCallBack") ||
			strstr (name, "NewtonCorkscrewCallBack") ||
			strstr (name, "NewtonSliderCallBack"))){
			fprintf (iNewtonHeader, "%s", name);
		}
	}
}



void PrintClassMethod (const char* name)
{

	if (pass == m_printClassMethodNewton) {
		if (!(strstr (name, "-(struct iNewtonWorld*) Create:") || 
			strstr (name, "-(void) Destroy:") ||
			strstr (name, "Ball") ||
			strstr (name, "Hinge") ||
			strstr (name, "Slider") ||
			strstr (name, "Corkscrew") ||
			strstr (name, "Universal"))){

			PrintPrototype (iNewtonHeader, name);
			fprintf (iNewtonHeader, ";\n");
			PrintFuntionBody (name);
		}
	} else if (pass == m_printClassMethodJoints) {
		PrintPrototype (iNewtonHeader, name);
		fprintf (iNewtonHeader, ";\n");
		PrintFuntionBody (name);
	}
}


void PrintPrototype (FILE* file, const char* name)
{
	char *ptr;
	char proto[1024];	
	strcpy (proto, name);

	ptr = strtok (proto, ":");
	if (strchr(ptr, ';')) {
		*strchr(ptr, ';') = 0;
	}
	fprintf (file, "%s", ptr);

	ptr = strtok (NULL, ":");
	if (ptr) {
		if (!strstr (ptr, "iNewtonWorld")) {
			if (strchr(ptr, ';')) {
				*strchr(ptr, ';') = 0;
			}
			fprintf (file, ":%s", ptr);
		}

		for (ptr = strtok (NULL, ":"); ptr; ptr = strtok (NULL, ":")) {
			if (strchr(ptr, ';')) {
				*strchr(ptr, ';') = 0;
			}
			fprintf (file, ":%s", ptr);
		}
	}
}


void PrintFuntionBody (const char* name)
{
	char *ptr;
	char proto[1024];	

//-(int) WorldConvexCast: (const float*) matrix: (const float*) target: (const struct iNewtonCollision*) shape 
//: (float*) hitParam: (void*) userData: (iNewtonWorldRayPrefilterCallback) prefilter
//: (struct iNewtonWorldConvexCastReturnInfo*) info: (int) maxContactsCount: (int) threadIndex
//if (strstr (name, "WorldGetDestructorCallBack")){
//	ptr = NULL;
//}


	PrintPrototype (iNewtonSource, name);
	fprintf (iNewtonSource, "\n");
	fprintf (iNewtonSource, "{\n");

	strcpy (proto, name);

	ptr = strtok (proto, ":");
	if (strchr(ptr, ';')) {
		*strchr(ptr, ';') = 0;
	}
	
	if (!strncmp (ptr, "-(void)", 7)) {
		if (pass == m_printClassMethodNewton) {
			fprintf (iNewtonSource, "\tNewton%s (", ptr + 8);
		} else {
			fprintf (iNewtonSource, "\t%s (", ptr + 8);
		}
	} else {
		char* function;
//		char tmp [256];

		function = strrchr (ptr, ' ') + 1;
		function[-1] = 0;
//		strcpy (tmp, ptr);
		*strrchr (ptr, ')') = 0;
//		if (IsCallback(tmp + 2)) {
//			if (pass == m_printClassMethodNewton) {
//				fprintf (iNewtonSource, "\treturn (%s) Newton%s(", tmp + 3, function);
//			} else {
//				fprintf (iNewtonSource, "\treturn (%s) %s(", tmp + 3, function);
//			}
//		} else {
			if (pass == m_printClassMethodNewton) {
//				fprintf (iNewtonSource, "\treturn (%s) Newton%s(", tmp + 2, function);
				fprintf (iNewtonSource, "\treturn (%s) Newton%s(", ptr + 2, function);
			} else {
				fprintf (iNewtonSource, "\treturn (%s) %s(", ptr + 2, function);
			}
//		}

	}

	int count = 0;
	for (ptr = strtok (NULL, ":"); ptr; ptr = strtok (NULL, ":")) {
		if (count) {
			fprintf (iNewtonSource, ", ");
		}

		if (!strstr (ptr, "iNewtonWorld*")) {
			char* ptr1;
			char tmp[256];

			ptr1 = strstr(ptr, "struct i");
			if (ptr1) {
				ptr1[7] = ' ';
			}

			if (strchr(ptr, ';')) {
				*strchr(ptr, ';') = 0;
			}


            strcpy (tmp, ptr);
			*strchr (tmp, ')') = 0;
			if (IsCallback(strchr (tmp, '(') + 2)) {
				ptr[2] = ' ';
			}

			fprintf (iNewtonSource, "%s", ptr + 1);
			
		} else {
			fprintf (iNewtonSource, "(const NewtonWorld*) m_world");
		}
		count ++;
	}


	fprintf (iNewtonSource, ");\n");
	fprintf (iNewtonSource, "}\n\n");
}


void XXXX ()
{

}

void CopyLable (char* const dst, const char* const lable)	
{
	strcpy (dst, lable);
}
