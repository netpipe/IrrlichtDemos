// Copyright (C) 2002-2006 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __S_VIEW_FRUSTRUM_H_INCLUDED__
#define __S_VIEW_FRUSTRUM_H_INCLUDED__

#include "plane3d.h"
#include "vector3d.h"
#include "aabbox3d.h"
#include "matrix4.h"

namespace irr
{
namespace scene
{

	//! Defines the view frustrum. Thats the space viewed by the camera.
	/** The view frustrum is enclosed by 6 planes. These six planes share
	four points. A bounding box around these four points is also stored in
	this structure.
	*/
	struct SViewFrustrum
	{
		enum VFPLANES
		{
			//! Far plane of the frustrum. Thats the plane farest away from the eye.
			VF_FAR_PLANE = 0,
			//! Near plane of the frustrum. Thats the plane nearest to the eye.
			VF_NEAR_PLANE,
			//! Left plane of the frustrum.
			VF_LEFT_PLANE,
			//! Right plane of the frustrum.
			VF_RIGHT_PLANE,
			//! Bottom plane of the frustrum.
			VF_BOTTOM_PLANE,
			//! Top plane of the frustrum.
			VF_TOP_PLANE,

			//! Amount of planes enclosing the view frustum. Should be 6.
			VF_PLANE_COUNT
		};

		//! Default Constructor
		SViewFrustrum() {};

		//! This constructor creates a view frustrum based on a projection and/or
		//! view matrix.
		SViewFrustrum(const core::matrix4& mat);

		//! the position of the camera
		core::vector3df cameraPosition;

		//! all planes enclosing the view frustrum.
		core::plane3d<f32> planes[VF_PLANE_COUNT];

		//! bouding box around the view frustrum
		core::aabbox3d<f32> boundingBox;

		//! transforms the frustrum by the matrix
		//! \param mat: Matrix by which the view frustrum is transformed.
		void transform(const core::matrix4 &mat);

		//! returns the point which is on the far left upper corner inside the
		//! the view frustrum.
		core::vector3df getFarLeftUp() const;

		//! returns the point which is on the far left bottom corner inside the
		//! the view frustrum.
		core::vector3df getFarLeftDown() const;

		//! returns the point which is on the far right top corner inside the
		//! the view frustrum.
		core::vector3df getFarRightUp() const;

		//! returns the point which is on the far right bottom corner inside the
		//! the view frustrum.
		core::vector3df getFarRightDown() const;

		//! returns a bounding box enclosing the whole view frustrum
		core::aabbox3d<f32> getBoundingBox() const;

		//! recalculates the bounding box member based on the planes
		inline void recalculateBoundingBox();
	};


	//! transforms the furstum by the matrix
	//! \param Matrix by which the view frustrum is transformed.
	inline void SViewFrustrum::transform(const core::matrix4 &mat)
	{
		for (int i=0; i<VF_PLANE_COUNT; ++i)
			mat.transformPlane(planes[i]);

		mat.transformVect(cameraPosition);

		recalculateBoundingBox();
	}


	//! returns the point which is on the far left upper corner inside the
	//! the view frustrum.
	inline core::vector3df SViewFrustrum::getFarLeftUp() const
	{
		core::vector3df p;
		planes[scene::SViewFrustrum::VF_FAR_PLANE].getIntersectionWithPlanes(
			planes[scene::SViewFrustrum::VF_TOP_PLANE],
			planes[scene::SViewFrustrum::VF_LEFT_PLANE], p);

		return p;
	}

	//! returns the point which is on the far left bottom corner inside the
	//! the view frustrum.
	inline core::vector3df SViewFrustrum::getFarLeftDown() const
	{
		core::vector3df p;
		planes[scene::SViewFrustrum::VF_FAR_PLANE].getIntersectionWithPlanes(
			planes[scene::SViewFrustrum::VF_BOTTOM_PLANE],
			planes[scene::SViewFrustrum::VF_LEFT_PLANE], p);

		return p;
	}

	//! returns the point which is on the far right top corner inside the
	//! the view frustrum.
	inline core::vector3df SViewFrustrum::getFarRightUp() const
	{
		core::vector3df p;
		planes[scene::SViewFrustrum::VF_FAR_PLANE].getIntersectionWithPlanes(
			planes[scene::SViewFrustrum::VF_TOP_PLANE],
			planes[scene::SViewFrustrum::VF_RIGHT_PLANE], p);

		return p;
	}

	//! returns the point which is on the far right bottom corner inside the
	//! the view frustrum.
	inline core::vector3df SViewFrustrum::getFarRightDown() const
	{
		core::vector3df p;
		planes[scene::SViewFrustrum::VF_FAR_PLANE].getIntersectionWithPlanes(
			planes[scene::SViewFrustrum::VF_BOTTOM_PLANE],
			planes[scene::SViewFrustrum::VF_RIGHT_PLANE], p);

		return p;
	}


	//! returns a bounding box encosing the whole view frustrum
	inline core::aabbox3d<f32> SViewFrustrum::getBoundingBox() const
	{
		return boundingBox;
	}


	//! recalculates the bounding box member based on the planes
	inline void SViewFrustrum::recalculateBoundingBox()
	{
		core::aabbox3d<f32> box(cameraPosition);

		box.addInternalPoint(getFarLeftUp());
		box.addInternalPoint(getFarRightUp());
		box.addInternalPoint(getFarLeftDown());
		box.addInternalPoint(getFarRightDown());

		boundingBox = box;
	}


	//! This constructor creates a view frustrum based on a projection and/or
	//! view matrix.
	inline SViewFrustrum::SViewFrustrum(const core::matrix4& mat)
	{
		#define sw(a,b)		(mat((b),(a)))

		// left clipping plane
		planes[SViewFrustrum::VF_LEFT_PLANE].Normal.X = -(sw(0,3) + sw(0,0));
		planes[SViewFrustrum::VF_LEFT_PLANE].Normal.Y = -(sw(1,3) + sw(1,0));
		planes[SViewFrustrum::VF_LEFT_PLANE].Normal.Z = -(sw(2,3) + sw(2,0));
		planes[SViewFrustrum::VF_LEFT_PLANE].D =		  -(sw(3,3) + sw(3,0));

		// right clipping plane
		planes[SViewFrustrum::VF_RIGHT_PLANE].Normal.X = -(sw(0,3) - sw(0,0));
		planes[SViewFrustrum::VF_RIGHT_PLANE].Normal.Y = -(sw(1,3) - sw(1,0));
		planes[SViewFrustrum::VF_RIGHT_PLANE].Normal.Z = -(sw(2,3) - sw(2,0));
		planes[SViewFrustrum::VF_RIGHT_PLANE].D =        -(sw(3,3) - sw(3,0));

		// top clipping plane
		planes[SViewFrustrum::VF_TOP_PLANE].Normal.X = -(sw(0,3) - sw(0,1));
		planes[SViewFrustrum::VF_TOP_PLANE].Normal.Y = -(sw(1,3) - sw(1,1));
		planes[SViewFrustrum::VF_TOP_PLANE].Normal.Z = -(sw(2,3) - sw(2,1));
		planes[SViewFrustrum::VF_TOP_PLANE].D =        -(sw(3,3) - sw(3,1));

		// bottom clipping plane
		planes[SViewFrustrum::VF_BOTTOM_PLANE].Normal.X = -(sw(0,3) + sw(0,1));
		planes[SViewFrustrum::VF_BOTTOM_PLANE].Normal.Y = -(sw(1,3) + sw(1,1));
		planes[SViewFrustrum::VF_BOTTOM_PLANE].Normal.Z = -(sw(2,3) + sw(2,1));
		planes[SViewFrustrum::VF_BOTTOM_PLANE].D =        -(sw(3,3) + sw(3,1));

		// near clipping plane
		planes[SViewFrustrum::VF_NEAR_PLANE].Normal.X = -sw(0,2);
		planes[SViewFrustrum::VF_NEAR_PLANE].Normal.Y = -sw(1,2);
		planes[SViewFrustrum::VF_NEAR_PLANE].Normal.Z = -sw(2,2);
		planes[SViewFrustrum::VF_NEAR_PLANE].D =        -sw(3,2);

		// far clipping plane
		planes[SViewFrustrum::VF_FAR_PLANE].Normal.X = -(sw(0,3) - sw(0,2));
		planes[SViewFrustrum::VF_FAR_PLANE].Normal.Y = -(sw(1,3) - sw(1,2));
		planes[SViewFrustrum::VF_FAR_PLANE].Normal.Z = -(sw(2,3) - sw(2,2));
		planes[SViewFrustrum::VF_FAR_PLANE].D =        -(sw(3,3) - sw(3,2));

		// normalize normals

		for (s32 i=0; i<6; ++i)
		{
			f32 len = (f32)(1.0f / planes[i].Normal.getLength());
			planes[i].Normal *= len;
			planes[i].D *= len;
		}

		// make bounding box

		recalculateBoundingBox();
	}



} // end namespace scene
} // end namespace irr

#endif

