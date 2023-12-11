/*
irrBullet Bullet physics wrapper for the Irrlicht rendering engine.
Copyright (C) 2009-2010 Josiah H. (Skyreign Software)

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

Josiah H. fighterstw@hotmail.com / josiah@skyreignsoftware.net16.net
Irrlicht Engine - http://www.irrlicht.sf.net
Bullet Physics Engine - http://www.bulletphysics.com/

*/

#ifndef __IRR_BULLET_H_INCLUDED__
#define __IRR_BULLET_H_INCLUDED__

#include "common.h"
#include "bulletworld.h"
#include "boxshape.h"
#include "sphereshape.h"
#include "gimpactmeshshape.h"
#include "bvhtrianglemeshshape.h"
#include "motionstate.h"

irrBulletWorld *createIrrBulletWorld(irr::IrrlichtDevice* const device)
{
    irrBulletWorld *world = new irrBulletWorld(device);

    return world;
}

#endif // __I_BULLET_WORLD_H_INCLUDED__



