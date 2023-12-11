
// Author: Greg Santucci, 2008 and 2009
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#include "3DWorld.h"

#include "GX_RenderingLine.h"
#include "GX_RenderingTriangle.h"

#include "GX_Model.h"

#include "GX_ModelLibrary.h"
#include "GX_ModelUtil.h"

#include "GX_Intersection.h"

World3D::World3D() : World()
{
	m_nID = 1;
}

World3D::~World3D()
{
}
