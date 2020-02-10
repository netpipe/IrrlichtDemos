// Copyright (C) 2002-2006 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __I_ANIMATED_MESH_B3D_H_INCLUDED__
#define __I_ANIMATED_MESH_B3D_H_INCLUDED__

#include "IAnimatedMesh.h"

namespace irr
{
namespace scene
{
	//! Interface for using some special functions of B3d meshes
	class IAnimatedMeshB3d : public IAnimatedMesh
	{
	public:

		//! Returns a pointer to a transformation matrix of a part of the
		//! mesh based on a frame time. This is used for being able to attach
		//! objects to parts of animated meshes. For example a weapon to an animated
		//! hand.
		//! \param jointNumber: Zero based index of joint. The last joint has the number
		//! IAnimatedMeshMS3D::getJointCount()-1;
		//! \param frame: Frame of the animation.
		//! \return Returns a pointer to the matrix of the mesh part or
		//! null if an error occured.
		virtual core::matrix4* getMatrixOfJoint(s32 jointNumber, s32 frame) = 0;

		//! Returns a pointer to a matrix of a part of the mesh unanimated
		virtual core::matrix4* getMatrixOfJointUnanimated(s32 jointNumber) = 0;

		//! Set a Matrix to be multiplied by the animated joint's matrix each frame
		//! For example a turning a meshes head
		virtual void AddMatrixToJoint(s32 jointNumber, core::matrix4* matrix) = 0;

		//! Gets joint count.
		//! \return Returns amount of joints in the skeletal animated mesh.
		virtual s32 getJointCount() const = 0;

		//! Gets the name of a joint.
		//! \param number: Zero based index of joint. The last joint has the number
		//! IAnimatedMeshMS3D::getJointCount()-1;
		//! \return Returns name of joint and null if an error happened.
		virtual const c8* getJointName(s32 number) const = 0;

		//! Gets a joint number from its name
		//! \param name: Name of the joint.
		//! \return Returns the number of the joint or -1 if not found.
		virtual s32 getJointNumber(const c8* name) const = 0;
	};

} // end namespace scene
} // end namespace irr

#endif

