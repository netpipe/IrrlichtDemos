#ifndef SQUIRREL_INTERFACE_H
#define SQUIRREL_INTERFACE_H

// Wrapper class for Squirrel
// Coded by Evo
// Version 0.1 Date: 18-september-2006
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the author be held liable for any damages
// arising from the use of this software.
//
// This code is placed in the public domain

#include "definitions.h"
#include "Scene_Interface.h"

using namespace irr;
using namespace SqPlus;

class Squirrel_interface
{
public:
	 Squirrel_interface(IrrlichtDevice * dev, Scene_Interface	*si);
	~Squirrel_interface(void);

	bool	loadScript       (const c8* FileName, const c8* ShortName = 0, const s32 ID = -1);
	bool	loadGlobalScript (const c8* FileName);
	bool	runScript        (const c8* ShortName);
	bool	runFunction      (const c8* FunctionName);

private:
	s32		findScript(core::stringc ShortName);

	struct ScriptLibrary
	{
		core::stringc				ShortName;	// Short name of script, may be used for calling scripts by name
		core::stringc				FileName;	// Filename of script
		SquirrelObject				SqOb;		// Handle to SquirrelObject used for loading/compling and running scripts
		s32							node;		// Optional value for node handled by current script
	};

	IrrlichtDevice				   *device;
	core::array<ScriptLibrary>		so;			// Array of names of all loaded scripts, may be used for calling scripts by name
};
#endif	// SQUIRREL_INTERFACE_H
