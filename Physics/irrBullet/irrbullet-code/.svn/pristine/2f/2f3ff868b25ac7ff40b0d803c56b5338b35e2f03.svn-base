/*
irrBullet Bullet physics wrapper for the Irrlicht rendering engine.
Copyright (C) 2009-2011 Josiah Hartzell (Skyreign Software)

This software is provided 'as-is', without any express or implied
warranty.  In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
    claim that you wrote the original software. If you use this software
    in a product, an acknowledgment in the product documentation would be
    appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.

Josiah Hartzell fighterstw@hotmail.com or josiah.hartzell@gmail.com
Irrlicht Engine - http://www.irrlicht.sf.net
Bullet Physics Engine - http://www.bulletphysics.com/

The above copyright notice and its accompanying information may not be removed.

The same applies for all other copyright notices at the top of files included with this library.
*/

#ifndef __IRR_BULLET_H_INCLUDED__
#define __IRR_BULLET_H_INCLUDED__

#include "irrbullet_compile_config.h"
#include "Bullet/btBulletDynamicsCommon.h"
#include "Bullet/btBulletCollisionCommon.h"
#include "Bullet/BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h"
#include "irrbulletcommon.h"
#include "bulletworld.h"
#include "boxshape.h"
#include "sphereshape.h"
#include "gimpactmeshshape.h"
#include "bvhtrianglemeshshape.h"
#include "convexhullshape.h"
#include "motionstate.h"
#include "raycastvehicle.h"
#include "collisionobjectaffector.h"
#include "collisionobjectaffectordelete.h"
#include "collisionobjectaffectorattract.h"
//#include "collisionobjectaffectorbuoyancy.h"
#include "collisioncallbackinformation.h"
#include "liquidbody.h"
#include "kinematiccharactercontroller.h"


/*!
    @mainpage irrBullet Wrapper Documentation


    @section intro Introduction

    Welcome to the irrBullet API documenation.

    If you have any questions or suggestions, please email them to me at fighterstw@hotmail.com.


    @section irrbulletexample irrBullet Example

    Using irrBullet with Irrlicht is very straight-forward and simple.


    Example source code and projects can be found in (<irrBulletDir> / examples),
    and compiled examples can be found in (<irrBulletDir> / bin / win32_gcc).


    @section linkerandsearchdirs Linker and Search Directories Settings

    Before you can begin integrating irrBullet in your application, there are a few things
    to add to your project first.

    Link against these static libraries (included in (<irrBulletDir> / lib), pre-compiled):

        <b>libirrBullet.a/.lib
        libbulletdynamics.a/.lib
        libbulletsoftbody.a/.lib
        libGIMPACTUtils.a/.lib
        liblinearmath.a/.lib
        libbulletcollision.a/.lib</b>

    This is for most features and with GImpact included. Other libs must be linked against for some features,
    such as libconvexdecomposition.a for decomposing objects.

    <i>It is important that they are linked against in this order to avoid linker errors.</i>


    Next, add these directories to your Search Directories list:

        <b>
        <irrBulletDirectory> / source
        <irrBulletDirectory> / source / bheaders
        <irrBulletDirectory> / source / bheaders / bullet
        </b>

    Then just include irrBullet.h into the top of your files using irrBullet.


    You are now ready to begin the integration process!

*/

irrBulletWorld *createIrrBulletWorld(irr::IrrlichtDevice* const device, bool useGImpact = false, bool useDebugDrawer = false);

#endif // __I_BULLET_WORLD_H_INCLUDED__



