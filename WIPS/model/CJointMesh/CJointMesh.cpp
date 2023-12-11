// Copyright (C) 2002-2010 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

// Jaromir Veber 2011
// Rewrite of most methods changed interface etc..
// (I forked original SkinMesh and created new fork JointMesh to be slowly adapted by irrlicht)
// coz i don't like concept and interface of SkinMesh

/* Jaromir Veber 2011
 * The Original skin algorithm used in 1.7.2 was somehow strange
 * There is used much improved version (faster, memory efficent, hwskinning compatible).
 */


#include "CJointMesh.h"
#include "CMeshManipulator.h"

namespace irr
{
namespace scene
{


//! constructor
CJointMesh::CJointMesh()
  : AnimationFrames(0.f), //it's not animated yet
	LastAnimatedFrame(-1.f), //no frame animated yet so reset cache
	HasSkin(false), //has ofc no skin
	AnimateNormals(true), HardwareSkinning(false), //normals are initialy animated and is not hw skinned
	IsFinalized(false) //ofc is not yet finalized (LOCKED)
{
	#ifdef _DEBUG
	setDebugName("CJointMesh");
	#endif
}


//! destructor
CJointMesh::~CJointMesh()
{
	//free memory used by joints
	for (core::map< core::stringc, SJoint* >::Iterator i = JointNameMap.getIterator(); !i.atEnd(); i++)
	{
		SJoint *joint = i->getValue();
		delete joint;
	}

	//free memory used by buffers
	for (u32 j=0; j<LocalBuffers.size(); ++j)
	{
		if (LocalBuffers[j])
			LocalBuffers[j]->drop();
	}

	//and release sources used for animation mapping
	for (u32 j=0; j<AnimTimeMap.size(); ++j)
	{
		AnimTimeMap[j].Source->drop();
	}
}

//// basic implementation for IMesh

//! returns amount of mesh buffers.
u32 CJointMesh::getMeshBufferCount() const
{
	return LocalBuffers.size();
}

//! returns pointer to a mesh buffer
IMeshBuffer* CJointMesh::getMeshBuffer(u32 nr) const
{
	if (nr < LocalBuffers.size())
		return LocalBuffers[nr];
	else
		return 0;
}

//! Returns pointer to a mesh buffer which fits a material
IMeshBuffer* CJointMesh::getMeshBuffer(const video::SMaterial &material) const
{
	for (u32 i=0; i<LocalBuffers.size(); ++i)
	{
		if (LocalBuffers[i]->getMaterial() == material)
			return LocalBuffers[i];
	}
	return 0;
}

//! returns an axis aligned bounding box
const core::aabbox3d<f32>& CJointMesh::getBoundingBox() const
{
	return BoundingBox;
}

//! set user axis aligned bounding box
void CJointMesh::setBoundingBox( const core::aabbox3df& box)
{
	BoundingBox = box;
}

//! sets a flag of all contained materials to a new value
void CJointMesh::setMaterialFlag(video::E_MATERIAL_FLAG flag, bool newvalue)
{
	for (u32 i=0; i<LocalBuffers.size(); ++i)
		LocalBuffers[i]->Material.setFlag(flag,newvalue);
}

//! set the hardware mapping hint, for driver
void CJointMesh::setHardwareMappingHint(E_HARDWARE_MAPPING newMappingHint,
		E_BUFFER_TYPE buffer)
{
	for (u32 i=0; i<LocalBuffers.size(); ++i)
		LocalBuffers[i]->setHardwareMappingHint(newMappingHint, buffer);
}


//! flags the meshbuffer as changed, reloads hardware buffers
void CJointMesh::setDirty(E_BUFFER_TYPE buffer)
{
	for (u32 i=0; i<LocalBuffers.size(); ++i)
		LocalBuffers[i]->setDirty(buffer);
}

//// basic implementation for IAnimatedMesh

u32 CJointMesh::getFrameCount() const
{
	return core::floor32(AnimationFrames);
}

IMesh* CJointMesh::getMesh(s32 frame, s32 detailLevel, s32 startFrameLoop, s32 endFrameLoop)
{
	if (frame >= 0)
	{
	  animateMesh(frame + (detailLevel / 1000.0f ) ); // CAnimatedMesh.. //s32 frameBlend = (s32) (core::fract ( getFrameNr() ) * 1000.f);
	  skinMesh();
	}
	return this;
}

E_ANIMATED_MESH_TYPE CJointMesh::getMeshType() const
{
	return EAMT_JOINT;
}

// basic implamantation for IJointMesh

//Update Normals when Animating (False= Don't animate them, faster) (True= Update normals (default))
void CJointMesh::updateNormalsWhenAnimating(bool on)
{
	AnimateNormals = on;
}

// (This feature is not implementated yet)
bool CJointMesh::setHardwareSkinning(bool on)
{
	//expecting vertex shader to do this work ...
	return false;
}

// lookup name to get joint
IJointMesh::SJoint* CJointMesh::getJoint(const core::stringc JointName) const
{
	core::map < core::stringc, SJoint* >::Node *n = JointNameMap.find(JointName);
	return n ? n->getValue() : NULL;
}

bool CJointMesh::getState(core::array< core::matrix4 > &currState) const
{
	for (core::map< core::stringc, SJoint* >::Iterator i = JointNameMap.getIterator(); !i.atEnd(); i++)
	{
		SJoint *joint = i->getValue();
		currState.push_back(joint->GlobalAnimatedMatrix);
	}
	return true;
}

bool CJointMesh::setState(const core::array< core::matrix4 > &newState)
{
	int j = 0;
	for (core::map< core::stringc, SJoint* >::Iterator i = JointNameMap.getIterator(); !i.atEnd(); i++, ++j)
	{
		SJoint *joint = i->getValue();
		joint->GlobalAnimatedMatrix = newState[j];
		joint->updateMeshTransformation();
	}
	return true;
}

// Basic mapping is done through names so the first possible solution would be
// map <name,joint> in every supermodel.. that would be memory effective solution
// and mapping of joint would be done as map lookup.. I'm not sure it
// will be CPU effective bit it looks working fast enough on my laptop :)
//
// Second possible sloution is not memory effective and would mean to add joint
// mapping for every supermodel to it's base model (this mesh)
// there would be array of Joints with size of AllJoints.size() * u16 as number to
// mesh->getAllJoints(). This is CPU effective.
bool CJointMesh::addAnimationFrom(IJointMesh *SuperMesh, f32 FrameOffset)
{
	// Safechecks
	if (SuperMesh == NULL) // null for addition?
		return false;

	if (! SuperMesh->getFrameCount() ) //no animation so nothing to add
		return false;

	if ( SuperMesh->getMeshType() != getMeshType() ) //not JointMesh cant inherit it..
		return false;

	if (!IsFinalized) //our mesh not finalized?
		loader_finalize();

	if (FrameOffset > AnimationFrames) //no empty frames plz.. so fix it
		FrameOffset = AnimationFrames;

	// Well the valid supremodel has at least one animated joint
	// with same name as base model.
	bool Valid = false;
	for (core::map< core::stringc, SJoint* >::Iterator i = JointNameMap.getIterator(); !i.atEnd(); i++)
	{
		SJoint *joint = i->getValue();
		SJoint *DistantJoint = SuperMesh->getJoint(joint->getName());
		if (DistantJoint && (DistantJoint->PositionFrame.size() || DistantJoint->RotationFrame.size() || DistantJoint->ScaleFrame.size()))
		{
			Valid = true;
			break;
		}
	}

	//if none Joint if this model whould be animated
	//don't add anyting.
	if (!Valid)
		return false;

	//Make a mapping
	AnimMap A;
	A.BeginFrame = FrameOffset;
	A.Source = SuperMesh;
	SuperMesh->grab();
	AnimTimeMap.push_back(A); //and store it to animation map

	//need to copy information from animation map of supermesh for a case it has got added animations..
	CJointMesh* JMesh = reinterpret_cast<CJointMesh*>(SuperMesh); //there's a check that it's same type up there co I can do it!

	for (u32 i = 0; i < JMesh->AnimTimeMap.size(); i++)
	{
		A = JMesh->AnimTimeMap[i];
		A.BeginFrame = A.BeginFrame + FrameOffset;
		AnimTimeMap.push_back(A);
	}
	//alter length of animation for this mesh
	AnimationFrames = FrameOffset + SuperMesh->getFrameCount();
	return true;
}

IMesh* CJointMesh::toSMesh()
{
	//new Static Mesh to return
	SMesh *StaticMesh = new SMesh();

	//we need transformator of vertexes to global space
	IMeshManipulator* MeshManipulator = new scene::CMeshManipulator();

	for (core::map< core::stringc, SJoint* >::Iterator i = JointNameMap.getIterator(); !i.atEnd(); i++)
	{
		SJoint *joint = i->getValue();
		for (u32 j = 0; j < joint->AttachedMeshes.size(); j++)
		{
			scene::IMeshBuffer* mbuffer = NULL;
			switch (joint->AttachedMeshes[j]->getVertexType())
			{
				case video::EVT_STANDARD:
				{
					scene::SMeshBuffer* cbuffer = new SMeshBuffer();
					//copy material
					cbuffer->Material = joint->AttachedMeshes[j]->getMaterial();
					mbuffer = cbuffer;
					break;
				}
				case video::EVT_2TCOORDS:
				{
					SMeshBufferLightMap* cbuffer = new SMeshBufferLightMap();
					//copy material
					cbuffer->Material = joint->AttachedMeshes[j]->getMaterial();
					mbuffer = cbuffer;
					break;
				}
				case video::EVT_TANGENTS:
				{
					SMeshBufferTangents* cbuffer = new SMeshBufferTangents();
					//copy material
					cbuffer->Material = joint->AttachedMeshes[j]->getMaterial();
					mbuffer = cbuffer;
					break;
				}
			}
			//copy vertices and indices
			mbuffer->append(joint->AttachedMeshes[j]);
			//transform meshes to global model space
			MeshManipulator->transform(mbuffer, joint->GlobalAnimatedMatrix);
			//add it to Static model
			StaticMesh->addMeshBuffer(mbuffer);
			mbuffer->drop();
		}
	}
	delete MeshManipulator;
	return StaticMesh;
}

void CJointMesh::convertMeshToTangents()
{
	// now calculate tangents
	for (u32 b=0; b < LocalBuffers.size(); ++b)
	{
		LocalBuffers[b]->convertToTangents();

		const s32 idxCnt = LocalBuffers[b]->getIndexCount();

		u16* idx = LocalBuffers[b]->getIndices();
		video::S3DVertexTangents* v =
			(video::S3DVertexTangents*)LocalBuffers[b]->getVertices();

		for (s32 i=0; i<idxCnt; i+=3)
		{
			calculateTangents(
				v[idx[i+0]].Normal,
				v[idx[i+0]].Tangent,
				v[idx[i+0]].Binormal,
				v[idx[i+0]].Pos,
				v[idx[i+1]].Pos,
				v[idx[i+2]].Pos,
				v[idx[i+0]].TCoords,
				v[idx[i+1]].TCoords,
				v[idx[i+2]].TCoords);

			calculateTangents(
				v[idx[i+1]].Normal,
				v[idx[i+1]].Tangent,
				v[idx[i+1]].Binormal,
				v[idx[i+1]].Pos,
				v[idx[i+2]].Pos,
				v[idx[i+0]].Pos,
				v[idx[i+1]].TCoords,
				v[idx[i+2]].TCoords,
				v[idx[i+0]].TCoords);

			calculateTangents(
				v[idx[i+2]].Normal,
				v[idx[i+2]].Tangent,
				v[idx[i+2]].Binormal,
				v[idx[i+2]].Pos,
				v[idx[i+0]].Pos,
				v[idx[i+1]].Pos,
				v[idx[i+2]].TCoords,
				v[idx[i+0]].TCoords,
				v[idx[i+1]].TCoords);
		}
	}
}

//--------------------------------------------------------------------------
//			Keyframe Animation
//--------------------------------------------------------------------------

//This makes mapping for joint animation
void CJointMesh::animateMesh(f32 frame)
{
	if ( frame > AnimationFrames || LastAnimatedFrame==frame)
		return;

	if (!IsFinalized)
		loader_finalize();

	LastAnimatedFrame = frame;

	IJointMesh *Source = this; //actual source of joints for animation
	f32 FrameOffset = 0;	     //offset of frames to use

	//decode actual animation joint offset to use - may be optimized
	for (u32 j = 0; j < AnimTimeMap.size(); ++j)
	{
		if ( (frame > AnimTimeMap[j].BeginFrame && j+1 >= AnimTimeMap.size()) ||
			(frame > AnimTimeMap[j].BeginFrame && frame < AnimTimeMap[j+1].BeginFrame)
		)
		{
			FrameOffset = AnimTimeMap[j].BeginFrame;
			Source = AnimTimeMap[j].Source;
		}
	}

	//fill GlobalAnimatedMatrix of all joints using animation
	for (u32 i=0; i<RootJoints.size(); ++i)
	  recursiveAnimateJoints(RootJoints[i], core::IdentityMatrix, Source, frame - FrameOffset);

	if (!HasSkin) //skinMesh() gonna update aabbBox
		updateBoundingBox();
}

//this compute GlobalAnimatedMatrices and fills buffers matrices
void CJointMesh::recursiveAnimateJoints(SJoint *joint,const core::matrix4& parentMatrix, IJointMesh *Source,f32 frame)
{
    core::matrix4 LocalAnimatedMatrix(joint->LocalMatrix); //use local matrix

    SJoint *DistantJoint = joint;
    if (Source != this)
       DistantJoint = Source->getJoint(joint->getName());

    if (DistantJoint)
		getMatrixForFrame(DistantJoint, LocalAnimatedMatrix, frame);

    // Find global matrix...
	joint->GlobalAnimatedMatrix = parentMatrix * LocalAnimatedMatrix;

	//update transformation matrix of all attached meshbuffers
	joint->updateMeshTransformation();

	// recursion
    for (u32 j=0; j<joint->Children.size(); ++j)
      recursiveAnimateJoints(joint->Children[j], joint->GlobalAnimatedMatrix, Source, frame);
}

//This actually do animation for Joint!
void CJointMesh::getMatrixForFrame(SJoint *joint , core::matrix4 &LocalMatrix, f32 frame)
{
	//Shotcuts to avoid tousand pointers!
	const core::array<f32> &RotationFrame = joint->RotationFrame;
	const core::array<core::quaternion> &RotationRotation = joint->RotationRotation;
	const core::array<f32> &PositionFrame = joint->PositionFrame;
	const core::array<core::vector3df> &PositionPosition = joint->PositionPosition;
	const core::array<f32> &ScaleFrame = joint->ScaleFrame;
	const core::array<core::vector3df> &ScalePosition = joint->ScalePosition;

	// parse rotation Keys
	if (RotationFrame.size())
	{
		//fast binary search for index of close low boundary in array (logaritmic)
		u32 LowPosIdx = BinaryLowerSearch(RotationFrame, frame, 0, RotationFrame.size()-1);

		//in this case we have index that should affect this frame
		if (RotationFrame[LowPosIdx] <= frame)
		{
			core::quaternion newRot;
			// check if we do interpolation or we got exact keyframe
			if ( RotationFrame.size() > LowPosIdx + 1 && RotationFrame[LowPosIdx] != frame )
			{
				u32 HighPosIdx = LowPosIdx + 1;
				const f32 fd1 = frame - RotationFrame[LowPosIdx];
				const f32 fd2 = RotationFrame[HighPosIdx] - frame;
				//spherical interpolation
				newRot.slerp( RotationRotation[LowPosIdx],RotationRotation[HighPosIdx], fd1 / (fd1 + fd2));
			} else
				newRot = RotationRotation[LowPosIdx];

			//construction of new matrix from quaternion unfortunately sets
			//translation to 0,0,0 so it must be set later to some value.
			const core::vector3df oldPos(LocalMatrix.getTranslation()); //store position
			LocalMatrix = newRot.getMatrix();							//change rotation
			LocalMatrix.setTranslation(oldPos);							//restore position
		}
	}

	// parse position keys
	if ( PositionFrame.size() )
	{
		//new array method that allow fast binary search for index of close low boundary in array
		u32 LowPosIdx = BinaryLowerSearch(PositionFrame, frame, 0, PositionFrame.size()-1);

		//in this case we have index that should not affect this frame
		if (PositionFrame[LowPosIdx] <= frame)
		{
			core::vector3df newPos;
			// check if we do interpolation or we got exact keyframe
			if ( PositionFrame.size() > LowPosIdx + 1 && PositionFrame[LowPosIdx] != frame )
			{
				u32 HighPosIdx = LowPosIdx + 1;
				const f32 fd1 = frame - PositionFrame[LowPosIdx];
				const f32 fd2 = PositionFrame[HighPosIdx] - frame;
				//linear interpolation
				newPos = core::lerp(PositionPosition[LowPosIdx], PositionPosition[HighPosIdx], fd1 / (fd1 + fd2) );
			} else
				newPos = PositionPosition[LowPosIdx];

			LocalMatrix.setTranslation(newPos);
		}
	}

	// parse scale keys (hold this after rotation coz of matrix multiplication)
	if ( ScaleFrame.size() )
	{
		//new array method that allow fast binary search for index of close low boundary in array
		u32 LowPosIdx = BinaryLowerSearch(ScaleFrame, frame, 0, ScaleFrame.size()-1);

		//in this case we have index that should not affect this frame yet
		if (ScaleFrame[LowPosIdx] <= frame)
		{
			core::vector3df newScale;
			if ( ScaleFrame.size() > LowPosIdx + 1 && ScaleFrame[LowPosIdx] != frame )
			{
				u32 HighPosIdx = LowPosIdx + 1;
				const f32 fd1 = frame - ScaleFrame[LowPosIdx];
				const f32 fd2 = ScaleFrame[HighPosIdx] - frame;
				//linear interpolation
				newScale = core::lerp(ScalePosition[LowPosIdx], ScalePosition[HighPosIdx], fd1 / (fd1 + fd2) );
			} else
				newScale = ScalePosition[LowPosIdx];

			core::matrix4 ScaleMatrix;
			ScaleMatrix.setScale(newScale);
			LocalMatrix = LocalMatrix * ScaleMatrix;
		}
	}
}

//--------------------------------------------------------------------------
//				Software Skinning
//--------------------------------------------------------------------------

//! Preforms a software skin on this mesh based of joint positions
void CJointMesh::skinMesh()
{
	if ( !HasSkin )
		return;

	if (!IsFinalized)
		loader_finalize();

	if (!HardwareSkinning)
	{
		//Software skining....
		for ( u32 buffer_id=0; buffer_id< LocalBuffers.size(); ++buffer_id )
		{
			//rather get pointer to Weight array to avoid 3 indexes -> tousand pointers -> messy code...
			core::array< core::array<WSSkinMeshBuffer::SWeight> > &Warray = LocalBuffers[buffer_id]->weight_array;
			if (!Warray.size()) //no weights -> no skin -> go ahead
				continue;

			for ( u32 VertIdx = 0; VertIdx < Warray.size(); VertIdx++ )
			{
				video::S3DVertex *v = LocalBuffers[buffer_id]->getVertex(VertIdx);
				core::matrix4 M; //may use here 3x4 matrices with SIMD optimations bur for now...

				for ( u32 WeightID = 0; WeightID < Warray[VertIdx].size(); WeightID++ )
				{
					//I was not able to set SWeight member to IJointMesh::SJoint so I used void* (even if its type IJointMesh::SJoint*).
					SJoint * joint = reinterpret_cast<IJointMesh::SJoint*>(Warray[VertIdx][WeightID].Joint); //so this reinterpret_cast is safe.
					// M = W(1)*I(1)*w(1) +  W(2)*I(2)*w(2) + W(3)*I(3)*w(3) +....
					if ( M == core::IdentityMatrix )
						M = (joint->GlobalAnimatedMatrix * joint->GlobalInversedMatrix) * Warray[VertIdx][WeightID].strength;
					else
						M += (joint->GlobalAnimatedMatrix * joint->GlobalInversedMatrix) * Warray[VertIdx][WeightID].strength;
				}

				v->Pos = LocalBuffers[buffer_id]->InitialPositions[VertIdx]; //restore original position
				M.transformVect(v->Pos);									 //transform using all weights

				if (AnimateNormals)
				{
					v->Normal = LocalBuffers[buffer_id]->InitialNormals[VertIdx];
					M.rotateVect(v->Normal);
					//since matrix may be nonuniform we need to normalize
					f32 length = v->Normal.getLength();
					if ( !core::equals(length,1.0f) ) //may set here better tolerance it it becomes bottleneck
						v->Normal *=  core::reciprocal_squareroot(length);
				}

			}
			LocalBuffers[buffer_id]->setDirty(EBT_VERTEX);
			LocalBuffers[buffer_id]->boundingBoxNeedsRecalculated();
		}
	}

	updateBoundingBox();
}

//It needs to be here because it works recursively. It's part of finalize method.
void CJointMesh::CalculateGlobalMatrices(SJoint *joint,SJoint *parentJoint)
{
	if (!parentJoint)
		joint->GlobalAnimatedMatrix = joint->LocalMatrix;
	else
		joint->GlobalAnimatedMatrix = parentJoint->GlobalAnimatedMatrix * joint->LocalMatrix;

	// inversed matrix necessary for skinning
	// (well we might skip joints that do not affect any skin)
	// but it would take some CPU to find out which joints do and which do not so...
	joint->GlobalInversedMatrix = joint->GlobalAnimatedMatrix;
	joint->GlobalInversedMatrix.makeInverse();

	for (u32 j=0; j<joint->AttachedMeshes.size(); ++j)
		joint->AttachedMeshes[j]->Transformation = joint->GlobalAnimatedMatrix;

	for (u32 j=0; j<joint->Children.size(); ++j)
		CalculateGlobalMatrices(joint->Children[j],joint);
}

//animation finalize
void CJointMesh::ProcessAnimation()
{
	//Check for animation...
	AnimationFrames=0;

	for (core::map< core::stringc, SJoint* >::Iterator i = JointNameMap.getIterator(); !i.atEnd(); i++)
	{
		SJoint *joint = i->getValue();

		//--- Find the length of the animation ---
		f32 PoskeyMaxFrame = 0;
		if (joint->PositionFrame.size())
			if (joint->PositionFrame.getLast() > AnimationFrames)
				PoskeyMaxFrame=joint->PositionFrame.getLast();

		f32 ScaleKeyMaxFrame = 0;
		if (joint->ScaleFrame.size())
			if (joint->ScaleFrame.getLast() > AnimationFrames)
				ScaleKeyMaxFrame=joint->ScaleFrame.getLast();

		f32 RotKeyMaxFrame = 0;
		if (joint->RotationFrame.size())
			if (joint->RotationFrame.getLast() > AnimationFrames)
				RotKeyMaxFrame=joint->RotationFrame.getLast();

		AnimationFrames = core::max_(AnimationFrames, core::f32_max3(PoskeyMaxFrame, ScaleKeyMaxFrame, RotKeyMaxFrame));

		//Animation code expects scale (1,1,1) - this is coz of simplicity of computation of LocalAnimatedMatrix it that case.
		//Well loader should do this part of scaling however I don't wana be hard on writers of loaders. So I'll trigger
		//only warning and do scaling.
		if (joint->LocalMatrix.getScale() != core::vector3df(1, 1, 1))
		{
			//os::Printer::log("LocalMatrix scale not 1! Scaling attached buffers",ELL_WARNING);
			IMeshManipulator* MeshManipulator = new scene::CMeshManipulator();

			core::vector3df oldScale = joint->LocalMatrix.getScale();
			joint->LocalMatrix.setScale(core::vector3df(1,1,1));

			//rescale all attached meshes to (1,1,1)
			for(u32 mesh_id = 0; mesh_id < joint->AttachedMeshes.size(); ++mesh_id)
				MeshManipulator->scale(joint->AttachedMeshes[mesh_id], oldScale);

			delete MeshManipulator;
		}
	}

	//check if some buffer is skinned
	for(u32 i=0; i < LocalBuffers.size();++i)
	{
		if(LocalBuffers[i]->weight_array.size())
		{
			HasSkin = true;
			break;
		}
	}

	// normalize weights - (for skinning)
	if (HasSkin)
		normalizeWeights();
}


//! called by loader after populating with mesh and bone data
void CJointMesh::loader_finalize()
{
	if (IsFinalized)
		return;

	LastAnimatedFrame=-1;

	// check animation presence, length and some checks around keys
	// and weights
	ProcessAnimation();

	//Needed for animation and skinning... if model is static global matrices store info
	for (u32 i=0; i<RootJoints.size(); ++i)
		CalculateGlobalMatrices(RootJoints[i],0);

	//skining preparation
	IMeshManipulator* MeshManipulator = new scene::CMeshManipulator();

	for(u32 buffer_id = 0; buffer_id < LocalBuffers.size(); buffer_id++ )
	{
		core::array< core::array<WSSkinMeshBuffer::SWeight> > &Warray = LocalBuffers[buffer_id]->weight_array;
		if (Warray.size())
		{
			//transform mesh relative to model base -> base of IdentityMatrix.
			if (LocalBuffers[buffer_id]->Transformation != core::IdentityMatrix)
			{
				MeshManipulator->transform(LocalBuffers[buffer_id],LocalBuffers[buffer_id]->Transformation);
				LocalBuffers[buffer_id]->Transformation = core::IdentityMatrix;
			}
			//store initial positions & normals
			for (u32 vertex_id = 0; vertex_id < LocalBuffers[buffer_id]->getVertexCount(); vertex_id++)
			{
				video::S3DVertex *v = LocalBuffers[buffer_id]->getVertex(vertex_id);
				LocalBuffers[buffer_id]->InitialPositions.push_back( v->Pos );
				LocalBuffers[buffer_id]->InitialNormals.push_back( v->Normal );
			}
		}

	}

	delete MeshManipulator;

	IsFinalized = true;

	updateBoundingBox();
}


void CJointMesh::updateBoundingBox(void)
{
	BoundingBox.reset(0,0,0);

	if (!LocalBuffers.empty())
	{
		for (u32 j=0; j<LocalBuffers.size(); ++j)
		{
			LocalBuffers[j]->recalculateBoundingBox();
			core::aabbox3df bb = LocalBuffers[j]->BoundingBox;
			LocalBuffers[j]->Transformation.transformBoxEx(bb);
			BoundingBox.addInternalBox(bb);
		}
	}
}

void CJointMesh::loader_unlock()
{
	IsFinalized = false;
	return;
}

bool CJointMesh::loader_removeMeshBuffers(const core::stringc& JointName)
{
	if (IsFinalized)
		return false;

	IJointMesh::SJoint *joint = getJoint(JointName);
	if (!joint)
		return false;

	for (u32 j = 0; j < joint->AttachedMeshes.size(); ++j)
	{
		for(u32 i = 0; i < LocalBuffers.size(); ++i)
		{
			if (joint->AttachedMeshes[j] == LocalBuffers[i])
			{
				LocalBuffers[i]->drop();
				LocalBuffers.erase(i);
				break;
			}
		}
	}

	joint->AttachedMeshes.clear();
	return true;
}

scene::WSSkinMeshBuffer *CJointMesh::loader_addMeshBuffer(const core::stringc& JointName)
{
	if (IsFinalized)
		return NULL;

	IJointMesh::SJoint *joint = getJoint(JointName);
	if (!joint)
		return NULL;

	scene::WSSkinMeshBuffer *buffer=new scene::WSSkinMeshBuffer();
	u32 buffer_id = LocalBuffers.size();
	LocalBuffers.push_back(buffer);
	joint->AttachedMeshes.push_back(buffer);
	return buffer;
}

//
bool CJointMesh::loader_addJoint(const core::stringc& JointName, const core::stringc& ParentName, const core::matrix4& LocalMatrix )
{
	if (IsFinalized)
		return false;

	core::array<SJoint*> *ParentArray;
	if (ParentName == "")
	{
		ParentArray = &RootJoints;
	} else
	{
		core::map < core::stringc, SJoint* >::Node *n = JointNameMap.find(ParentName);
		if (!n)
			return false; //Error Parent not found!!
		ParentArray = &(n->getValue()->Children);
	}

	SJoint *joint= new SJoint;
	joint->LocalMatrix = LocalMatrix; //store position and rotation
	//store to map

	JointNameMap.insert(JointName, joint);
	core::map < core::stringc, SJoint* >::Node *n = JointNameMap.find(JointName);

#if (IRRLICHT_VERSION_MAJOR > 1 || IRRLICHT_VERSION_MINOR > 7)
	//well this is name connection to key from Joint it
	//does not look well but it save space and is valid for
	//irrlicht core::map s 1.8+
	joint->Name = &n->getKey();
#else
	joint.Name = JointName;
#endif
	// store to array
	ParentArray->push_back(joint);

	return true;
}

// Add position key to joint (should it be by joint_id?)
// Keys must be sorted by frame!
bool CJointMesh::loader_addPositionKey(const core::stringc &JointName, f32 frame,const core::vector3df &position)
{
	if (IsFinalized)
		return false;

	SJoint *joint = getJoint(JointName);
	if (!joint)
	{
		return false;
	}

	//safechecksv to elminate unwanted keys
	u32 nPositionKeys = joint->PositionFrame.size();
	if (nPositionKeys)
	{
		f32 LastFrame = joint->PositionFrame.getLast();

		//Same frame as last? Silently ignore this key...
		if (LastFrame == frame)
			return true;

		//Keys not in order -> warning - Loader must! sort them!
		if ( LastFrame > frame )
			return false;

		//if it's not necessary to store coz there's no new information about position
		if (nPositionKeys > 1 && joint->PositionPosition[nPositionKeys-1] == position && joint->PositionPosition[nPositionKeys-2] == position)
		{
			joint->PositionFrame[nPositionKeys-1] = frame;
			return true;
		}
	}
	//Store position key to array!
	joint->PositionFrame.push_back(frame);
	joint->PositionPosition.push_back(position);
	return true;
}

// Add scale key to joint (should it be by joint_id?)
// Keys must be sorted by frame!
// This method and method above share most of code so it is
// possible to merge parts of it
bool CJointMesh::loader_addScaleKey(const core::stringc &JointName, f32 frame,const core::vector3df &scale)
{
	if (IsFinalized)
		return false;

	SJoint *joint = getJoint(JointName);
	if (!joint)
		return false;

	//safechecksv to elminate unwanted keys
	u32 nScaleKeys = joint->ScaleFrame.size();
	if (nScaleKeys)
	{
		f32 LastFrame = joint->ScaleFrame.getLast();

		//Same frame as last? Silently ignore this key...
		if (LastFrame == frame)
			return true;

		//Keys not in order -> warning - Loader must! sort them!
		if ( LastFrame > frame )
			return false;


		//if it's not necessary to store coz there's no new information about position
		if (nScaleKeys > 1 && joint->ScalePosition[nScaleKeys-1] == scale && joint->ScalePosition[nScaleKeys-2] == scale)
		{
			joint->ScaleFrame[nScaleKeys-1] = frame;
			return true;
		}
	}

	joint->ScaleFrame.push_back(frame);
	joint->ScalePosition.push_back(scale);
	return true;
}

// Add rotation key to joint (should it be by joint_id?)
// Keys must be sorted by frame!
// This method and method above share most of code so it is
// possible to merge parts of it
bool CJointMesh::loader_addRotationKey(const core::stringc &JointName, f32 frame,const core::quaternion &rotation)
{
	if (IsFinalized)
		return false;

	SJoint *joint = getJoint(JointName);
	if (!joint)
		return false;

	//safechecksv to elminate unwanted keys
	u32 nRotationKeys = joint->RotationFrame.size();
	if (nRotationKeys)
	{
		f32 LastFrame = joint->RotationFrame.getLast();

		//Same frame as last? Silently ignore this key...
		if (LastFrame == frame)
			return true;

		//Keys not in order -> Loader must! sort them!
		if ( LastFrame > frame )
			return false;

		//if it's not necessary to store coz there's no new information about position
		if (nRotationKeys > 1 && joint->RotationRotation[nRotationKeys-1] == rotation && joint->RotationRotation[nRotationKeys-2] == rotation)
		{
			joint->RotationFrame[nRotationKeys-1] = frame;
			return true;
		}
	}
	joint->RotationFrame.push_back(frame);
	joint->RotationRotation.push_back(rotation);
	return true;
}


// Add weight of joint to vertex identified by vertexID from buffer bufferID
// Ok there is the method that add Weight...
// It must work this way and I hope this is the interface that should be used in future
// so the programmer chan only add weights not change anyting in them!!!
// actually programmenr even does not know where are weights stored (Mesh not Joint!)
// Weights must be stored at mesh because in optimized way we need to compute vertex
// position at once!! It is necessary for vertex shaders etc.so it must be stored in
// connection to Mesh. However this interface allow storing at Joint (bone) as before
// too.
bool CJointMesh::loader_addWeight(const core::stringc &JointName, WSSkinMeshBuffer *buffer, u16 vertex_id, f32 weight)
{
	//checks
	if (IsFinalized)
		return false;

	if (!buffer)
		return false;

	if (vertex_id >= buffer->getVertexCount())
		return false;

	SJoint* Joint = getJoint(JointName);
	if (!Joint)
		return false;

	core::array <core::array <WSSkinMeshBuffer::SWeight> > &Warray = buffer->weight_array;

	//Resize array if it's too small!!!
	//(cant use set_used it's bugging a bit for two dimensional arrays)
	if ( Warray.size() <= vertex_id )
	{
		while(Warray.size() <= vertex_id)
		{
			core::array <WSSkinMeshBuffer::SWeight> array;
			Warray.push_back(array);
		}
	}

	//Add weight to position
	WSSkinMeshBuffer::SWeight Weight;
	Weight.strength = weight;

	//actually I was not able to define it as ISkinnedMesh::SJoint so not so ugly as it seems
	Weight.Joint = reinterpret_cast<void*>(Joint);
	Warray[vertex_id].push_back(Weight);
	return true;
}

void CJointMesh::normalizeWeights()
{
	//New alg normalize
 	for (u32 i=0; i<LocalBuffers.size(); ++i)
	{
		core::array< core::array<WSSkinMeshBuffer::SWeight> > Warray = LocalBuffers[i]->weight_array;
		for (u32 VertIdx = 0; VertIdx < Warray.size(); VertIdx++)
		{
			f32 TotalStrength = 0;
			for (u32 WeightID = 0; WeightID < Warray[VertIdx].size(); WeightID++)
			{
				TotalStrength += Warray[VertIdx][WeightID].strength;
			}

			if (TotalStrength != 1.0 && TotalStrength != 0.0)
			{
				for (u32 WeightID = 0; WeightID < Warray[VertIdx].size(); WeightID++)
				{
					Warray[VertIdx][WeightID].strength /= TotalStrength;
				}
			}
		}
	}
}

void CJointMesh::calculateTangents(
	core::vector3df& normal,
	core::vector3df& tangent,
	core::vector3df& binormal,
	core::vector3df& vt1, core::vector3df& vt2, core::vector3df& vt3, // vertices
	core::vector2df& tc1, core::vector2df& tc2, core::vector2df& tc3) // texture coords
{
	core::vector3df v1 = vt1 - vt2;
	core::vector3df v2 = vt3 - vt1;
	normal = v2.crossProduct(v1);
	normal.normalize();

	// binormal

	f32 deltaX1 = tc1.X - tc2.X;
	f32 deltaX2 = tc3.X - tc1.X;
	binormal = (v1 * deltaX2) - (v2 * deltaX1);
	binormal.normalize();

	// tangent

	f32 deltaY1 = tc1.Y - tc2.Y;
	f32 deltaY2 = tc3.Y - tc1.Y;
	tangent = (v1 * deltaY2) - (v2 * deltaY1);
	tangent.normalize();

	// adjust

	core::vector3df txb = tangent.crossProduct(binormal);
	if (txb.dotProduct(normal) < 0.0f)
	{
		tangent *= -1.0f;
		binormal *= -1.0f;
	}
}

int CJointMesh::BinaryLowerSearch(core::array<f32> data,const f32 element, s32 left, s32 right)
{
	if (data.size() <= 0)
			return -1;


	while ( right - 1 > left )
	{
		s32 pivot = (left + right) >> 1;

		if (data[pivot] < element)
			left = pivot;
		else
			right = pivot;
	}

	return data[right] > element ? left : right;
}

} // end namespace scene
} // end namespace irr

