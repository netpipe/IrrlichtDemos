
// Author: Greg Santucci, 2008 and 2009
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#include "ML_Disc.h"
#include "ML_Triangle.h"
#include "ML_Line.h"

#include "GX_RenderingLine.h"

mlDisc::mlDisc(const mlVector3D & i_vPointA, const mlVector3D & i_vPointB, const mlVector3D & i_vPointC)
{
	// This wasn't implemented for some reason...

	mlVector3D vNormal = mlTriangle(i_vPointA, i_vPointB, i_vPointC).Normal();
	vNormal.Normalise();

	mlLine line1 = mlPerpendicularBisector(mlTriangle(i_vPointA, i_vPointB, i_vPointC));
	mlLine line2 = mlPerpendicularBisector(mlTriangle(i_vPointB, i_vPointC, i_vPointA));

	mlLine intersectionline = line1.ShortestLineToLine(line2);

	m_vNormal = vNormal;
	m_vCenter = intersectionline.a;
	m_fRadius = (m_vCenter - i_vPointA).Magnitude();
}

mlDisc MakeTouchingDisc1(
	mlTransform & i_tVehicleTransform,
	mlDisc & i_oTouchDisc)
{
	mlVector3D vPosition = i_tVehicleTransform.TransformPoint(mlVector3DZero);
	mlVector3D vForward = i_tVehicleTransform.TransformPoint(mlVector3D(1.0f, 0.0f, 0.0f)) - vPosition;
	mlVector3D vRight = i_tVehicleTransform.TransformPoint(mlVector3D(0.0f, 1.0f, 0.0f)) - vPosition;

	mlFloat r2 = i_oTouchDisc.m_fRadius;
	mlFloat rx = vRight.x;
	mlFloat ry = vRight.y;
	mlFloat px = vPosition.x;
	mlFloat py = vPosition.y;
	mlFloat c2x = i_oTouchDisc.m_vCenter.x;
	mlFloat c2y = i_oTouchDisc.m_vCenter.y;

	mlFloat kx = px - c2x;
	mlFloat ky = py - c2y;

	mlFloat kx2 = kx * kx;
	mlFloat ky2 = ky * ky;
	mlFloat r22 = r2 * r2;

	mlFloat r1 = (r22 - kx2 - ky2) / (2.0f * (kx * rx + ky * ry - r2));

	mlVector3D vCenter = vPosition + vRight * r1;

	return mlDisc(mlFabs(r1), vCenter, mlVector3D(0.0f, 0.0f, 1.0f));
}

mlDisc MakeTouchingDisc2(
	mlTransform & i_tVehicleTransform,
	mlDisc & i_oTouchDisc)
{
	mlVector3D vPosition = i_tVehicleTransform.TransformPoint(mlVector3DZero);
	mlVector3D vForward = i_tVehicleTransform.TransformPoint(mlVector3D(1.0f, 0.0f, 0.0f)) - vPosition;
	mlVector3D vRight = i_tVehicleTransform.TransformPoint(mlVector3D(0.0f, 1.0f, 0.0f)) - vPosition;

	mlFloat r2 = i_oTouchDisc.m_fRadius;
	mlFloat rx = vRight.x;
	mlFloat ry = vRight.y;
	mlFloat px = vPosition.x;
	mlFloat py = vPosition.y;
	mlFloat c2x = i_oTouchDisc.m_vCenter.x;
	mlFloat c2y = i_oTouchDisc.m_vCenter.y;

	mlFloat kx = px - c2x;
	mlFloat ky = py - c2y;

	mlFloat kx2 = kx * kx;
	mlFloat ky2 = ky * ky;
	mlFloat r22 = r2 * r2;

	mlFloat r1 = (r22 - kx2 - ky2) / (2.0f * (kx * rx + ky * ry + r2));

	mlVector3D vCenter = vPosition + vRight * r1;

	return mlDisc(mlFabs(r1), vCenter, mlVector3D(0.0f, 0.0f, 1.0f));
}

void mlDisc::Render(const gxColor & color, bool bRenderSpokes)
{
	mlVector3D p1 = m_vCenter;
	mlVector3D p2 = m_vCenter + m_vNormal;
	
	mlVector3D plane[3];
	mlVector3D p,r,s,p1p2;

	p1p2 = p1 - p2;
	p = m_vCenter;
	r = mlVectorCross(p1p2, p);
	s = mlVectorCross(p1p2, r);
	r.Normalise();
	r = r * m_fRadius;
	s.Normalise();
	s = s * m_fRadius;
	mlFloat fTwoPi = 6.283185307179586476925287f;
	mlFloat dtheta = fTwoPi / 72.0f;
	for (mlFloat theta=0; theta < fTwoPi; theta += dtheta)
	{
		plane[0].x = p1.x + r.x * cos(theta) + s.x * sin(theta);
		plane[0].y = p1.y + r.y * cos(theta) + s.y * sin(theta);
		plane[0].z = p1.z + r.z * cos(theta) + s.z * sin(theta);
		plane[1]   = p1;
		plane[2].x = p1.x + r.x * cos(theta+dtheta) + s.x * sin(theta+dtheta);
		plane[2].y = p1.y + r.y * cos(theta+dtheta) + s.y * sin(theta+dtheta);
		plane[2].z = p1.z + r.z * cos(theta+dtheta) + s.z * sin(theta+dtheta);
		
		if(bRenderSpokes)
		{
			for (int iLine = 0; iLine < 3; iLine++)
			{
				gxRenderingLine renderLine(color, plane[iLine], plane[(iLine + 1) % 3]);
				renderLine.Render();
			}
		}
		else
		{
			gxRenderingLine renderLine(color, plane[0], plane[2]);
			renderLine.Render();
		}
	}
}
