/***************************************************************
*                Terrain Engine issue of					   *
*							Dreamotion3D engine				   *
*		UTILS FUNCTIONS										   *
**************************************************************** 
*File Created: 29.03.08                                        *
*Author:			                                           *
*Contens: utilities functions						           *
***************************************************************/
#include <irrlicht.h>

using namespace irr;
using namespace core;

//*******************************************************************************
//	Distance3
//*******************************************************************************
float Distance3(vector3df* p1, vector3df *p2)
{
	return sqrtf(powf(p2->X-p1->X,2)+powf(p2->Y-p1->Y,2)+powf(p2->Z-p1->Z,2));
}
//*******************************************************************************
//	Direction3
//*******************************************************************************
void Direction3(vector3df* p1, vector3df* p2, vector3df* rp)
{
	register vector3df p3;
	register vector3df dir;
	register float distance = Distance3(p1,p2);

	p3.X = p2->X; p3.Y = p1->Y; p3.Z = p2->Z;
	dir.Y = Distance3(p2,&p3)/distance;

	p3.X = p1->X; p3.Y = p2->Y;
	dir.X = Distance3(p2,&p3)/distance;

	p3.X = p2->X; p3.Z = p1->Z;
	dir.Z = Distance3(p2,&p3)/distance;

	if (p1->X>p2->X) dir.X=-dir.X;
	if (p1->Y>p2->Y) dir.Y=-dir.Y;
	if (p1->Z>p2->Z) dir.Z=-dir.Z;

	rp->X = dir.X;
	rp->Y = dir.Y;
	rp->Z = dir.Z;
}
//*******************************************************************************
//	xform
//*******************************************************************************
void xform(const core::matrix4& M, core::aabbox3d<f32>& out)
{
		// Convert to center/extent representation.
		core::vector3df c = out.getCenter();
		core::vector3df e = out.getExtent();

		// Transform center in usual way.
		M.transformVect(c,c);//		D3DXVec3TransformCoord(&c, &c, &M);

		// Transform extent.
		core::matrix4 absM;
		absM.makeIdentity();
		absM(0,0) = fabsf(M(0,0)); absM(0,1) = fabsf(M(0,1)); absM(0,2) = fabsf(M(0,2));
		absM(1,0) = fabsf(M(1,0)); absM(1,1) = fabsf(M(1,1)); absM(1,2) = fabsf(M(1,2));
		absM(2,0) = fabsf(M(2,0)); absM(2,1) = fabsf(M(2,1)); absM(2,2) = fabsf(M(2,2));
		absM.transformVect(e,e);

		// Convert back to AABB representation.
		out.MinEdge = c - e;
		out.MaxEdge = c + e;
}

