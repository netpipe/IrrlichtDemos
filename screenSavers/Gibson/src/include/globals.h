/*
    Creation date: unknown

    Copyright © 2010 John Serafino
    This file is part of ray3d.

    Ray3d is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Ray3d v0.01 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with ray3d If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef GLOBALS_H
#define GLOBALS_H

#include <irrlicht.h>


// renderer's
#define OGL  EDT_OPENGL
#define D3D  EDT_DIRECT3D9
#define SOFT EDT_SOFTWARE

// screen modes
#define FULLSCREEN true
#define WINDOWED   false

// light types
#define POINT         ELT_POINT
#define SPOT          ELT_SPOT
#define DIRECTIONAL   ELT_DIRECTIONAL

// use all irrlicht namespaces
using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

typedef SColor Color;

// pointers to irrlicht handlers
extern IrrlichtDevice *irrlicht;

extern IVideoDriver*    Video;
extern ISceneManager*   Scene;
extern IGUIEnvironment* Gui;

/*
  =========
    rotateNode -- rotate a scene node locally
  ==========
*/
void rotateNode(irr::scene::ISceneNode *node, irr::core::vector3df rot);

/*
  ==========
    rotateNode -- rotate a scene node locally
  ==========
*/
void translateNode(ISceneNode *node, vector3df vel, bool vertical = true);


/*
  ==========
    getMatrixRotation -- gets the real rotation of a matrix
  ==========
 */
vector3df getMatrixRotation(const matrix4& mx);



extern bool useHwSkinning;		// Should the engine use hardware skinning on meshes by default?
extern int  hwSkinSpeed;		// How often to update skinning shader

#endif
