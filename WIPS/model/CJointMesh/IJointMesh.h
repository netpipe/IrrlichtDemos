// Copyright (C) 2002-2010 Nikolaus Gebhardt; 2011 Jaromir Veber

#ifndef __I_JOINT_MESH_H_INCLUDED__
#define __I_JOINT_MESH_H_INCLUDED__

#include "IAnimatedMesh.h"
#include "WSSkinMeshBuffer.h"


namespace irr
{
namespace scene
{
	//! Interface for using some special functions of Skinned meshes
	class IJointMesh : public IAnimatedMesh
	{
	public:

		/** Joints - storage of infromation about actual and possible future world transformation of joint.
		 * Joints have identifier "Name" so every joint need unique name. \see getName
		 */
		struct SJoint
		{
			//! The name of this joint used internally in skinning algo but is free to be used at public
			const core::stringc& getName() const
#if (IRRLICHT_VERSION_MAJOR > 1 || IRRLICHT_VERSION_MINOR > 7) //1.8+
			{ return *Name; }
#else
			{ return Name; }
#endif
			//! getGlobal model position after animation - maybe used for making "read-only" bones.
			const core::matrix4& getGlobalAnimatedMatrix() const { return GlobalAnimatedMatrix; }

			//! get Local matrix of this joint - this is much for loaders to see what was filled by loader_addJoint
			const core::matrix4& getLocalMatrix() const { return LocalMatrix; }

		private:
			//!set GlobalAnimation for all attached Meshes
			void updateMeshTransformation()
			{
				// buffers use animation matrix for position except with skinned buffers!
				// (skinning algorithms use global model skining so skinned buffers use identity matrix)
				for (u32 j = 0; j < AttachedMeshes.size(); ++j)
					if (! (AttachedMeshes[j]->weight_array.size()))
						AttachedMeshes[j]->Transformation = GlobalAnimatedMatrix;
			}

			//! Internal members used by CSkinnedMesh
			friend class CJointMesh;

			//! List of child joints
			core::array<SJoint*> Children;

			//! Global model position after animation
			core::matrix4 GlobalAnimatedMatrix;

#if (IRRLICHT_VERSION_MAJOR > 1 || IRRLICHT_VERSION_MINOR > 7) //1.8+
			//! pointer to name of joint
			const core::stringc* Name;
#else
			//! name of joint
			core::stringc Name;
#endif
			//! Local matrix of this joint
			core::matrix4 LocalMatrix;

			//! List of attached meshes
			core::array<WSSkinMeshBuffer*> AttachedMeshes;

			//! Animation keys causing translation change
			core::array<f32> 				PositionFrame;
			//! position changes
			core::array<core::vector3df> 	PositionPosition;

			//! Animation keys causing scale change
			core::array<f32> 				ScaleFrame;
			//! scale changes
			core::array<core::vector3df>	ScalePosition;

			//! Animation keys causing rotation change
			core::array<f32> 				RotationFrame;
			//! rotation changes
			core::array<core::quaternion> 	RotationRotation;

			//! Inverse of Global binding Matrix of this joint (used for skinning)
			core::matrix4 GlobalInversedMatrix;
		};

	public:
		//! Add animation from another mesh to frame offset
		/** The animation is linked (not copied) based on joint
		 * names so make sure they are unique.
		 * \note mesh must be finalized before calling this method use #loader_finalize
		 *
		 * \param mesh Source mesh containing animations (must be type JointMesh)
		 * \param FrameOffset where to add animation offset - mostly its getFrameCount()
		 * if you don't wana remove any animaition from current mesh if you want replace
		 * all animations use 0 etc..
		 * \return True if some joints in this mesh were
		 * matched up (empty names will not be matched, and it's case sensitive).
		 * Unmatched joints will not be animated.
		 *
		 * Reference to \param mesh is grabbed so do not delete it it is necesary
		 * to use <b>drop</b> method on it!
		 * <p>
		 * Animation scheme is supported the way that one JointMesh can inherit
		 * animation from other but this one can be still used for inherence so
		 * there may be pretty big inherence trees.
		 */
		virtual bool addAnimationFrom(IJointMesh *mesh, f32 FrameOffset) = 0;

		//! Update Normals when Animating
		/** \param on If false don't animate, which is faster.
			Else update normals, which allows for proper lighting of
			animated meshes. */
		virtual void updateNormalsWhenAnimating(bool on) = 0;

		//! converts the vertex type of all meshbuffers to tangents.
		/** E.g. used for bump mapping. */
		virtual void convertMeshToTangents() = 0;

		//! Allows to enable hardware skinning.
		/* This feature is not implementated in Irrlicht yet */
		virtual bool setHardwareSkinning(bool on) = 0;

		/**
		 * Support for transitions - get state of all joints (transformation matrices)
		 * This method should be used to store state before transition and after transition
		 */
		virtual bool getState(core::array< core::matrix4 > &currState) const = 0;

		/**
		 * Support for transitions - set state of all joints (transformation matrices)
		 * This method should be used to set interpolated state
		 */
		virtual bool setState(const core::array< core::matrix4 > &newState) = 0;

		/**
		 * Preforms a software or hardware skin on this mesh based of joint positions
		 * (Change of vertex positions in buffers)
		 * \note Weights are used here
		 * \see loader_addWeight
		 */
		virtual void skinMesh() = 0;

		/**
		 * Get mesh position for splecified frame
		 * \note Position, Rotation and Scale keys are used here
		 * \see loader_addPositionKey loader_addScaleKey loader_addRotationKey
		 */
		virtual void animateMesh(f32 frame) = 0;

		/** create Static Mesh (IMesh type) from current position of JointMesh
		 *  It may be used to save memory if mesh is static #isStatic or if
		 *  many non animated clones (in differnet positions) are necessary.
		 */
		virtual IMesh* toSMesh() = 0;

		//! Gets the pointer to joint
		/** \param JointName: string with name of joint
		 * \return Pointer to Joint or null if joint with this name is not present in mesh */
		virtual SJoint* getJoint(const core::stringc JointName) const = 0;

		//! similar to #getJoint but just checking a presence
		bool hasJoint(const core::stringc JointName) { return (getJoint(JointName) != NULL); }

		//Interface for the mesh loaders (finalize should lock these functions, and they should have some prefix like loader_

		/** Allow modification of Mesh again after it's locked by
		 *  #loader_finalize -> Mesh needs to be finalized again.
		 */
		virtual void loader_unlock() = 0;

		/** loaders should call this after populating the mesh -
		 *	finalize is LOCKing all other loader_ prefix methods so they'll return
		 *  all false or NULL!!! (unless they unlocked by loader_unlock)
		 * \note It is called automatilcally before any animation method i called eg. #getFrameCount, #getMesh, #addAnimationFrom
		 * \see loader_unlock loader_addJoint loader_addMeshBuffer loader_addPositionKey loader_addRotationKey loader_addScaleKey loader_addWeight loader_removeMeshBuffers
		 */
		virtual void loader_finalize() = 0;

		/** Adds a new joint to the mesh
		 * \note It is recomended to make initial scale for all joints (buffers) to 1!!
		 *
		 * \param JointName -  identifier for all consequtive operations with Joint (alias part name)
		 * \param ParentName - identifier of its parent animation of parent affects animation of its descendants
		 * (may be "" to specify it's root of mesh so joint is not affected by any other joints)
		 * \param LocalMatrix - binding matrix of this Joint alias distance and orientation of this joint (part) to it's parent should have scale = (1,1,1)
		 * \return true if everything successful, false may be caused by - parent Joint not found, mesh locked by #loader_finalize
		 * or memory allocation error.
		 */
		virtual bool loader_addJoint(const core::stringc& JointName,const core::stringc& ParentName,const core::matrix4& LocalMatrix) = 0;

		/** Adds a new meshbuffer to the mesh, access it as last one
		 *  \note It is recomended to make initial scale for all joints (buffers) to 1!!
		 *  \param JointName - name of joint to attach buffer to
		 *  \return newly allocated buffer that loader may fill with
		 *  Vertex, Indices data and Material data. May return NULL if JointName
		 *  not present or if mesh is finalized or some allocation error happens.
		 */
		virtual WSSkinMeshBuffer* loader_addMeshBuffer(const core::stringc& JointName) = 0;

		/** Remove all mesh buffers attached to joint by
		 *  loader_addMeshBuffer.
		 */
		virtual bool loader_removeMeshBuffers(const core::stringc& JointName) = 0;

		/** Adds a new position key to the mesh
		 * \note All frames must be consequtive so newly added frame has higher number than last one it's loader
		 * work to sort them by frame!
		 *
		 * \param JointName joint identifier to add position key to.
		 * \param frame frame (time) of current joint position
		 * \param position desired 3D position of joint
		 * \return true if everything successful, false may be caused by - Joint not found, mesh locked by #loader_finalize
		 * or frame not in order or memory allocation error.
		 */
		virtual bool loader_addPositionKey(const core::stringc& JointName, f32 frame,const  core::vector3df &position ) = 0;

		/** Adds a new rotation key to the mesh
		 * \note All frames must be consequtive so newly added frame has higher number than last one it's loader
		 * work to sort them by frame!
		 * \param JointName joint identifier to add position key to.
		 * \param frame frame (time) of current joint orientation
		 * \param rotation desired 3D rotation of joint
		 * \return true if everything successful, false may be caused by - Joint not found, mesh locked by #loader_finalize
		 * or frame not in order or memory allocation error.
		 */
		virtual bool loader_addRotationKey(const core::stringc& JointName, f32 frame,const core::quaternion &rotation ) = 0;

		/** Adds a new scale key to the mesh
		 * \note All frames must be consequtive so newly added frame has higher number than last one it's loader
		 * work to sort them by frame!
		 * \param JointName joint identifier to add position key to.
		 * \param frame frame (time) of current joint orientation
		 * \param scale desired 3D scale of joint
		 * \return true if everything successful, false may be caused by - Joint not found, mesh locked by #loader_finalize
		 * or frame not in order or memory allocation error.
		 */
		virtual bool loader_addScaleKey(const core::stringc& JointName, f32 frame,const core::vector3df &scale ) = 0;

		/** Adds a new weight key to the mesh
		 * \param JointName joint identifier to add position key to.
		 * \param buffer buffer identifier - returned by #loader_addMeshBuffer
		 * \param vertexID id of vertex in buffer that this weight should be applyed
		 * \param weight strength should be number 0-1 however weights are normalised later so it does not matter
		 * (should not be negative - that whould prevent normalisation to work well...)
		 * \return true if everything successful, false may be caused by - Joint not found, mesh locked by #loader_finalize
		 * or weight negative or memory allocation error.
		 */
		virtual bool loader_addWeight(const core::stringc& JointName, WSSkinMeshBuffer * buffer, u16 vertexID, f32 weight) = 0;
	};

} // end namespace scene
} // end namespace irr
#endif //#ifndef __I_JOINT_MESH_H_INCLUDED__