/*
    globals.cpp

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

#include "globals.h"

IrrlichtDevice *irrlicht;

IVideoDriver*    Video;
ISceneManager*   Scene;
IGUIEnvironment* Gui;

/*
  ==========
    rotateNode -- rotate a scene node locally
  ==========
*/
void rotateNode(irr::scene::ISceneNode *node, irr::core::vector3df rot)
{
    irr::core::matrix4 m;
    //m.setRotationDegrees(node->getRotation());
    //m.setRotationDegrees(getMatrixRotation(node->getAbsoluteTransformation()));

    vector3df fore = vector3df(0,0,1);
    m = node->getAbsoluteTransformation();
    //m.rotateVect(fore);
    //m.setRotationDegrees(fore.getHorizontalAngle());


    irr::core::matrix4 n;
    n.setRotationDegrees(rot);

    m *= n;

    //node->setRotation( getMatrixRotation(m) );
    node->setRotation(m.getRotationDegrees());
    node->updateAbsolutePosition();
}


/*
  ==========
    translateNode -- translate a scene node locally
  ==========
*/
void translateNode(ISceneNode *node, vector3df vel, bool vertical)
{
    irr::core::matrix4 m;

    vector3df rot = node->getRotation();
    if(vertical != true)
    {
      rot.X = 0;
    }
    m.setRotationDegrees(rot);

    m.transformVect(vel);
    node->setPosition(node->getPosition() + vel);
    node->updateAbsolutePosition();
}

/*
  ==========
    getMatrixRotation -- gets the real rotation of a matrix
  ==========
 */
vector3df getMatrixRotation(const matrix4& mx)
{
   core::vector3df r;
   r.X = asinf(-mx(2,1));
   const f32 t = cosf(r.X);
   if (.001f < t) {
      r.Z = atan2f(mx(0,1), mx(1,1));
      r.Y = atan2f(mx(2,0), mx(2,2));
   }
   else {
      r.Z = atan2f(-mx (1,0), mx (0,0));
      r.Y = 0.f;
   }
   return (r * (180/PI));
}



bool useHwSkinning;		// Should the engine use hardware skinning on meshes by default?
int  hwSkinSpeed;		// How often to update skinning shader
