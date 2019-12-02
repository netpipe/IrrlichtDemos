// Copyright (C) 2002-2010 Nikolaus Gebhardt; 2011 Jaromir Veber

//New Joint mesh


#ifndef __C__JOINT_MESH__H_INCLUDED__
#define __C__JOINT_MESH__H_INCLUDED__


#include "SMeshBuffer.h"
#include "S3DVertex.h"
#include "SMesh.h"
#include "irrString.h"
#include "irrMap.h"
#include "matrix4.h"
#include "quaternion.h"
#include "IJointMesh.h"

namespace irr
{
namespace scene
{
	class CJointMesh: public IJointMesh
	{
	public:
		// CJointMesh specific

		//! constructor
		CJointMesh();

		//! destructor
		virtual ~CJointMesh();

		/**
		 * Support for transitions - get state of all joints (transformation matrices)
		 * This method should be used to store state before transition and after transition
		 */
		virtual bool getState(core::array< core::matrix4 > &currState) const;

		/**
		 * Support for transitions - set state of all joints (transformation matrices)
		 * This method should be used to set interpolated state
		 */
		virtual bool setState(const core::array< core::matrix4 > &newState);

				/**
		 * Preforms a software or hardware skin on this mesh based of joint positions
		 * (Change of vertex positions in buffers)
		 * \note Weights are used here
		 * \see loader_addWeight
		 */
		virtual void skinMesh();    		 // Preforms a software or hardware skin on this mesh based of joint positions

		/**
		 * Get mesh position for splecified frame
		 * \note Position, Rotation and Scale keys are used here
		 * \see loader_addPositionKey loader_addScaleKey loader_addRotationKey
		 */
		virtual void animateMesh(f32 frame); //animation for frame

		/**
		 *  Fix bounding box size
		 */
		void updateBoundingBox(void);

		// IMesh
		virtual void setBoundingBox( const core::aabbox3df& box);

		virtual const core::aabbox3d<f32>& getBoundingBox() const;

		virtual u32 getMeshBufferCount() const;

		virtual IMeshBuffer* getMeshBuffer(u32 nr) const;

		virtual IMeshBuffer* getMeshBuffer( const video::SMaterial &material) const;

		virtual void setMaterialFlag(video::E_MATERIAL_FLAG flag, bool newvalue);

		virtual void setHardwareMappingHint(E_HARDWARE_MAPPING newMappingHint, E_BUFFER_TYPE buffer=EBT_VERTEX_AND_INDEX);

		virtual void setDirty(E_BUFFER_TYPE buffer=EBT_VERTEX_AND_INDEX);

		// IAnimatedMesh
		virtual u32 getFrameCount() const;

		virtual IMesh* getMesh(s32 frame, s32 detailLevel=255, s32 startFrameLoop=-1, s32 endFrameLoop=-1);

		virtual E_ANIMATED_MESH_TYPE getMeshType() const;

		//IJointMesh
		virtual bool addAnimationFrom(IJointMesh *mesh, f32 FrameOffset);

		virtual void updateNormalsWhenAnimating(bool on);

		virtual void convertMeshToTangents();

		virtual bool setHardwareSkinning(bool on);

		virtual IMesh* toSMesh();

		virtual SJoint* getJoint(const core::stringc JointName) const;

		//Interface for the mesh loaders (finalize should lock these functions, and they should have some prefix like loader_

		virtual void loader_unlock();

		virtual void loader_finalize();

		virtual bool loader_addJoint(const core::stringc& JointName,const core::stringc& ParentName,const core::matrix4& LocalMatrix);

		virtual bool loader_removeMeshBuffers(const core::stringc& JointName);

		virtual WSSkinMeshBuffer* loader_addMeshBuffer(const core::stringc& JointName);

		virtual bool loader_addPositionKey(const core::stringc& JointName, f32 frame,const  core::vector3df &position );

		virtual bool loader_addRotationKey(const core::stringc& JointName, f32 frame,const core::quaternion &rotation );

		virtual bool loader_addScaleKey(const core::stringc& JointName, f32 frame,const core::vector3df &scale );

		virtual bool loader_addWeight(const core::stringc& JointName, WSSkinMeshBuffer * buffer, u16 vertexID, f32 weight);

private:
		int BinaryLowerSearch(core::array<f32> data, const f32 element, s32 left, s32 right);

		void ProcessAnimation();			// helper method for finalize

		void normalizeWeights();	// helper method for finalize

		void getMatrixForFrame(SJoint *joint , core::matrix4 &LocalMatrix, f32 frame);	// calculate LocalAnimatedMatrix for (Joint,Frame)

		void recursiveAnimateJoints(SJoint *joint,const core::matrix4& parentMatrix, IJointMesh *Source,f32 frame); //recursive calculation including animation

		void CalculateGlobalMatrices(SJoint *Joint,SJoint *ParentJoint);		//recursive global matrix calculation

		void calculateTangents(core::vector3df& normal,							//well old method from skinned mesh hope it works :)
			core::vector3df& tangent, core::vector3df& binormal,
			core::vector3df& vt1, core::vector3df& vt2, core::vector3df& vt3,
			core::vector2df& tc1, core::vector2df& tc2, core::vector2df& tc3);

		core::array<WSSkinMeshBuffer*> LocalBuffers; //array of that current mesh use buffers

		core::map < core::stringc, SJoint* > JointNameMap; //mapping for name, joint
		core::array<SJoint*> RootJoints;	//joints that have no parent

		struct AnimMap
		{
			f32 BeginFrame;
			IJointMesh *Source;
		};

		core::array<AnimMap> AnimTimeMap; //maping for added animations fomr different models
		core::aabbox3d<f32> BoundingBox;  //common bounding box for IMesh


		f32 AnimationFrames;	//number of frames that can be animated!
		f32 LastAnimatedFrame;  //cache for last frame to allow faster response for same frame

		//guess names are enuff to understand what theese mean

		bool IsFinalized;
		bool HasSkin;
		bool AnimateNormals;
		bool HardwareSkinning;
	};

} // end namespace scene
} // end namespace irr

#endif // #ifndef __C__JOINT_MESH__H_INCLUDED__