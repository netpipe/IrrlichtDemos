/*
Bullet Continuous Collision Detection and Physics Library
Copyright (c) 2003-2009 Erwin Coumans  http://bulletphysics.com

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.

Experimental Buoyancy fluid demo written by John McCutchan
*/
//This is an altered source version based on the HeightFieldFluidDemo included with Bullet Physics 2.80(bullet-2.80-rev2531).

#ifndef HFFLUID_GL_SHAPE_DRAWER_H
#define HFFLUID_GL_SHAPE_DRAWER_H

#include "GL_ShapeDrawer.h"


class FluidHfDemo_GL_ShapeDrawer : public GL_ShapeDrawer
{
public:
	bool m_drawHfFluidWithTriangles;
	bool m_drawHfGroundWithTriangles;

	bool m_drawHfFluidAsColumns;
	bool m_drawHfGroundAsColumns;

	FluidHfDemo_GL_ShapeDrawer()
	{
		m_drawHfFluidWithTriangles = m_drawHfGroundWithTriangles = true;
		m_drawHfFluidAsColumns = m_drawHfGroundAsColumns = false;
	}
	
	///drawOpenGL might allocate temporary memory, stores pointer in shape userpointer
	virtual void drawOpenGL(btScalar* m, const btCollisionShape* shape, const btVector3& color,
							int	debugMode, const btVector3& worldBoundsMin, const btVector3& worldBoundsMax);
};

#endif //HFFLUID_GL_SHAPE_DRAWER_H
