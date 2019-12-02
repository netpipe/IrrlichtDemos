// Copyright (C) 2012 Asst.Prof. Dr. Arsa Tangchitsomkit
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

// PSK Mesh and PSA Animation loader
// File format designed by Unreal Tourament (c) EpicGames

#include "IrrCompileConfig.h"

#ifndef __C_UNREAL_MESH_LOADER_H_INCLUDED__
#define __C_UNREAL_MESH_LOADER_H_INCLUDED__

#include "IMeshLoader.h"
#include "ISceneManager.h"
#include "ISkinnedMesh.h"
#include "IReadFile.h"

namespace irr
{

namespace scene
{

//! Simple implementation of the ISkinnedMesh interface.
struct SSkinnedMesh : public ISkinnedMesh
{
public:

	SSkinnedMesh(ISkinnedMesh* mesh) : ISkinnedMesh(), Mesh(mesh) { }
	~SSkinnedMesh()
	{
		if( Mesh )
		{
			Mesh->drop( );
			Mesh = 0;
		}
	}

	//! Get the amount of mesh buffers.
	/** \return Amount of mesh buffers (IMeshBuffer) in this mesh. */
	virtual u32 getMeshBufferCount() const { return Mesh->getMeshBufferCount(); }

	//! Get pointer to a mesh buffer.
	/** \param nr: Zero based index of the mesh buffer. The maximum value is
	getMeshBufferCount() - 1;
	\return Pointer to the mesh buffer or 0 if there is no such
	mesh buffer. */
	virtual IMeshBuffer* getMeshBuffer(u32 nr) const { return Mesh->getMeshBuffer(nr); }

	//! Get pointer to a mesh buffer which fits a material
	/** \param material: material to search for
	\return Pointer to the mesh buffer or 0 if there is no such
	mesh buffer. */
	virtual IMeshBuffer* getMeshBuffer( const video::SMaterial &material) const { return Mesh->getMeshBuffer(material); }

	//! Get an axis aligned bounding box of the mesh.
	/** \return Bounding box of this mesh. */
	virtual const core::aabbox3d<f32>& getBoundingBox() const { return Mesh->getBoundingBox(); }

	//! Set user-defined axis aligned bounding box
	/** \param box New bounding box to use for the mesh. */
	virtual void setBoundingBox( const core::aabbox3df& box) { Mesh->setBoundingBox(box); }

	//! Sets a flag of all contained materials to a new value.
	/** \param flag: Flag to set in all materials.
	\param newvalue: New value to set in all materials. */
	virtual void setMaterialFlag(video::E_MATERIAL_FLAG flag, bool newvalue) { Mesh->setMaterialFlag(flag,newvalue); }

	//! Set the hardware mapping hint
	/** This methods allows to define optimization hints for the
	hardware. This enables, e.g., the use of hardware buffers on
	pltforms that support this feature. This can lead to noticeable
	performance gains. */
	virtual void setHardwareMappingHint(E_HARDWARE_MAPPING newMappingHint, E_BUFFER_TYPE buffer=EBT_VERTEX_AND_INDEX) { Mesh->setHardwareMappingHint(newMappingHint,buffer); }

	//! Flag the meshbuffer as changed, reloads hardware buffers
	/** This method has to be called every time the vertices or
	indices have changed. Otherwise, changes won't be updated
	on the GPU in the next render cycle. */
	virtual void setDirty(E_BUFFER_TYPE buffer=EBT_VERTEX_AND_INDEX) { Mesh->setDirty(buffer); }

	//! Gets the frame count of the animated mesh.
	/** \return The amount of frames. If the amount is 1,
	it is a static, non animated mesh. */
	virtual u32 getFrameCount() const { return Mesh->getFrameCount(); }

	//! Gets the animation speed of the animated mesh.
	/** \return The number of frames per second to play the
	animation with by default. If the amount is 0,
	it is a static, non animated mesh. */
	virtual f32 getAnimationSpeed() const { return Mesh->getAnimationSpeed(); }

	//! Sets the animation speed of the animated mesh.
	/** \param fps Number of frames per second to play the
	animation with by default. If the amount is 0,
	it is not animated. The actual speed is set in the
	scene node the mesh is instantiated in.*/
	virtual void setAnimationSpeed(f32 fps) { Mesh->setAnimationSpeed(fps); }

	//! Returns the IMesh interface for a frame.
	/** \param frame: Frame number as zero based index. The maximum
	frame number is getFrameCount() - 1;
	\param detailLevel: Level of detail. 0 is the lowest, 255 the
	highest level of detail. Most meshes will ignore the detail level.
	\param startFrameLoop: Because some animated meshes (.MD2) are
	blended between 2 static frames, and maybe animated in a loop,
	the startFrameLoop and the endFrameLoop have to be defined, to
	prevent the animation to be blended between frames which are
	outside of this loop.
	If startFrameLoop and endFrameLoop are both -1, they are ignored.
	\param endFrameLoop: see startFrameLoop.
	\return Returns the animated mesh based on a detail level. */
	virtual IMesh* getMesh(s32 frame, s32 detailLevel=255, s32 startFrameLoop=-1, s32 endFrameLoop=-1) { return Mesh->getMesh(frame,detailLevel,startFrameLoop,endFrameLoop); }

	//! Gets joint count.
	/** \return Amount of joints in the skeletal animated mesh. */
	virtual u32 getJointCount() const { return Mesh->getJointCount(); }

	//! Gets the name of a joint.
	/** \param number: Zero based index of joint. The last joint
	has the number getJointCount()-1;
	\return Name of joint and null if an error happened. */
	virtual const c8* getJointName(u32 number) const { return Mesh->getJointName(number); }

	//! Gets a joint number from its name
	/** \param name: Name of the joint.
	\return Number of the joint or -1 if not found. */
	virtual s32 getJointNumber(const c8* name) const { return Mesh->getJointNumber(name); }

	//! Use animation from another mesh
	/** The animation is linked (not copied) based on joint names
	so make sure they are unique.
	\return True if all joints in this mesh were
	matched up (empty names will not be matched, and it's case
	sensitive). Unmatched joints will not be animated. */
	virtual bool useAnimationFrom(const ISkinnedMesh *mesh) { return Mesh->useAnimationFrom(mesh); }

	//! Update Normals when Animating
	/** \param on If false don't animate, which is faster.
	Else update normals, which allows for proper lighting of
	animated meshes. */
	virtual void updateNormalsWhenAnimating(bool on) { Mesh->updateNormalsWhenAnimating(on); }

	//! Sets Interpolation Mode
	virtual void setInterpolationMode(E_INTERPOLATION_MODE mode) { Mesh->setInterpolationMode(mode); }

	//! Animates this mesh's joints based on frame input
	virtual void animateMesh(f32 frame, f32 blend) { Mesh->animateMesh(frame,blend); }

	//! Preforms a software skin on this mesh based of joint positions
	virtual void skinMesh() { Mesh->skinMesh(); }

	//! converts the vertex type of all meshbuffers to tangents.
	/** E.g. used for bump mapping. */
	virtual void convertMeshToTangents() { Mesh->convertMeshToTangents(); }

	//! Allows to enable hardware skinning.
	/* This feature is not implementated in Irrlicht yet */
	virtual bool setHardwareSkinning(bool on) { return Mesh->setHardwareSkinning(on); }

	//! exposed for loaders: to add mesh buffers
	virtual core::array<SSkinMeshBuffer*>& getMeshBuffers() { return Mesh->getMeshBuffers(); }

	//! exposed for loaders: joints list
	virtual core::array<SJoint*>& getAllJoints() { return Mesh->getAllJoints(); }

	//! exposed for loaders: joints list
	virtual const core::array<SJoint*>& getAllJoints() const { return Mesh->getAllJoints(); }

	//! loaders should call this after populating the mesh
	virtual void finalize() { Mesh->finalize(); }

	//! Adds a new meshbuffer to the mesh, access it as last one
	virtual SSkinMeshBuffer* addMeshBuffer() { return Mesh->addMeshBuffer(); }

	//! Adds a new joint to the mesh, access it as last one
	virtual SJoint* addJoint(SJoint *parent=0) { return Mesh->addJoint(parent); }

	//! Adds a new weight to the mesh, access it as last one
	virtual SWeight* addWeight(SJoint *joint) { return Mesh->addWeight(joint); }

	//! Adds a new position key to the mesh, access it as last one
	virtual SPositionKey* addPositionKey(SJoint *joint) { return Mesh->addPositionKey(joint); }
	//! Adds a new scale key to the mesh, access it as last one
	virtual SScaleKey* addScaleKey(SJoint *joint) { return Mesh->addScaleKey(joint); }
	//! Adds a new rotation key to the mesh, access it as last one
	virtual SRotationKey* addRotationKey(SJoint *joint) { return Mesh->addRotationKey(joint); }

	//! Check if the mesh is non-animated
	virtual bool isStatic() { return Mesh->isStatic(); }

	struct AnimInfo
	{
		AnimInfo( const core::stringc& name, s32 begin = 0, s32 end = 0, f32 fps = 30.0f )
		{
			Name = name;
			Fps = fps;
			Begin = begin;
			End = end;
		}

		//! The == operator is provided so that CFileList can slowly search the list!
		bool operator ==(const struct AnimInfo& other) const
		{
			if ( Name == other.Name )
				return true;

			return false;
		}

		core::stringc  Name;
		f32   Fps;
		s32	  Begin;
		s32   End;
	};

	void addAnimation( const core::stringc animationName, s32 begin = 0, s32 end = 0, f32 fps = 30.0f )
	{
		AnimInfo anim = AnimInfo( animationName,begin,end,fps );
		s32 id = Anims.linear_search( anim );
		if( id == -1 )
			Anims.push_back( anim );
	}

	bool getFrameLoop( const core::stringc animationName, s32& begin, s32& end, f32& fps )
	{
		s32 id = Anims.linear_search( AnimInfo(animationName) );
		if( id == -1 )
			return false;

		AnimInfo anim = Anims[id];
		begin = anim.Begin;
		end = anim.End;
		fps = anim.Fps;

		return true;
	}

	u32 getAnimationCount() const { return Anims.size(); }
	core::array<AnimInfo>& getAnimations( ) { return Anims; }
	const core::array<AnimInfo>& getAnimations( ) const { return Anims; }

private:

	core::array<AnimInfo> Anims;
	scene::ISkinnedMesh* Mesh;
};

//! Meshloader for B3D format
class CPSKMeshFileLoader : public IMeshLoader
{
public:

	//! Constructor
	CPSKMeshFileLoader(scene::ISceneManager* smgr, ILogger* log = 0 );

	//! returns true if the file maybe is able to be loaded by this class
	//! based on the file extension (e.g. ".bsp")
	virtual bool isALoadableFileExtension(const io::path& filename) const;

	//! creates/loads an animated mesh from the file.
	//! \return Pointer to the created mesh. Returns 0 if loading failed.
	//! If you no longer need the mesh, you should call IAnimatedMesh::drop().
	//! See IReferenceCounted::drop() for more information.
	virtual IAnimatedMesh* createMesh(io::IReadFile* file);

private:

	ISceneManager*	SceneManager;
	SSkinnedMesh*	AnimatedMesh;
	ILogger*		Log;

};

//! Meshloader for B3D format
class CPSAMeshFileLoader : public IMeshLoader
{
public:

	//! Constructor
	CPSAMeshFileLoader(scene::ISceneManager* smgr, ILogger* log = 0 );

	//! returns true if the file maybe is able to be loaded by this class
	//! based on the file extension (e.g. ".bsp")
	virtual bool isALoadableFileExtension(const io::path& filename) const;

	//! creates/loads an animated mesh from the file.
	//! \return Pointer to the created mesh. Returns 0 if loading failed.
	//! If you no longer need the mesh, you should call IAnimatedMesh::drop().
	//! See IReferenceCounted::drop() for more information.
	virtual IAnimatedMesh* createMesh(io::IReadFile* file);

private:

	ISceneManager*	SceneManager;
	SSkinnedMesh*	AnimatedMesh;
	ILogger*		Log;

};

} // end namespace scene
} // end namespace irr

#endif // __C_UNREAL_MESH_LOADER_H_INCLUDED__

