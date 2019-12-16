// *************************************************************************************************
//
// Horde3D
//   Next-Generation Graphics Engine
// --------------------------------------
// Copyright (C) 2006-2008 Nicolas Schulz
//
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
//
// *************************************************************************************************

#ifndef _daeMain_H_
#define _daeMain_H_

#include "utils.h"
#include "daeLibImages.h"
#include "daeLibEffects.h"
#include "daeLibMaterials.h"
#include "daeLibGeometries.h"
#include "daeLibVisualScenes.h"
#include "daeLibControllers.h"
#include "daeLibAnimations.h"


class ColladaDocument
{
public:

	bool				y_up;
	
	DaeLibImages		libImages;
	DaeLibEffects		libEffects;
	DaeLibMaterials		libMaterials;
	DaeLibGeometries	libGeometries;
	DaeLibVisScenes		libVisScenes;
	DaeLibControllers	libControllers;
	DaeLibAnimations	libAnimations;

	DaeVisualScene		*scene;


	ColladaDocument()
	{
		scene = 0x0;
		y_up = true;
	}
	
	
	bool parseFile( const string &fileName );
};

#endif // _daeMain_H_
