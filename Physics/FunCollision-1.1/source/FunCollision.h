/* -------------------------------------------------------------------------------
    FunCollision -- Collision detection and response system

    Created by Peter Szollosi, 2009-2010.

    This software is provided 'as-is', without any express or implied
    warranty. In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
       claim that you wrote the original software. If you use this software
       in a product, an acknowledgement in the product documentation would be
       appreciated but is not required.
    2. Altered source versions must be clearly marked as such, and must not be
       misrepresented as being the original software.
    3. This notice may not be removed or altered from any source distribution.
-------------------------------------------------------------------------------- */

#ifndef FUNCOLLISION_H_INCLUDED
#define FUNCOLLISION_H_INCLUDED

//! versions
#define FUNCOLLISION_VERSION_MAJOR 1
#define FUNCOLLISION_VERSION_MINOR 1
#define FUNCOLLISION_VERSION_BUILD 100121

//! interfaces
#include "CCollCommon.h"
#include "ICollManager.h"
#include "ICollSelector.h"
#include "ICollSampler.h"
#include "ICollObject.h"
#include "ICollEntity.h"

//! the main namespace
namespace COLL
{
    //! Creates a collision manager and returns a pointer to the interface for manipulation.
    /// When you don't need it anymore, call "manager->drop();" instead of deleting it.
    /// All the other collision related classes should be created via the manager, use the
    /// "add" and/or "create" functions. Classes created with the "add" functions should
    /// not be dropped, while those are created by "create" functions should be dropped.
    /// \param Device: pointer to the Irrlicht device
    /// \param GravityStrength: sets the gravity strength
    /// \param BaseFriction: sets the friction for the whole system
    ICollManager* createCollisionManager(IrrlichtDevice* Device, float GravityStrength = 0.0f, float BaseFriction = 0.92f);
}

#endif // FUNCOLLISION_H_INCLUDED
