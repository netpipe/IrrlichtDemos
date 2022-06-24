// Copyright (C) 2009 JoshM, Christian Stehno
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
// Josh made the initial loader implementation, I've fixed some things and
// ported it to use Irrlicht structures

#ifndef __C_FBX_MESH_FILE_LOADER_H_INCLUDED__
#define __C_FBX_MESH_FILE_LOADER_H_INCLUDED__

#include "IMeshLoader.h"
#include "IFileSystem.h"
#include "IVideoDriver.h"
#include "ISceneManager.h"

namespace irr
{
namespace scene
{

//! Meshloader capable of loading FBX files
class CFBXMeshFileLoader : public IMeshLoader
{
public:

	//! Constructor
	CFBXMeshFileLoader(scene::ISceneManager* smgr);

	//! returns true if the file maybe is able to be loaded by this class
	//! based on the file extension (e.g. ".fbx")
	virtual bool isALoadableFileExtension(const io::path& fileName) const;

	//! creates/loads an animated mesh from the file.
	//! \return Pointer to the created mesh. Returns 0 if loading failed.
	//! If you no longer need the mesh, you should call IAnimatedMesh::drop().
	//! See IReferenceCounted::drop() for more information.
	virtual IAnimatedMesh* createMesh(io::IReadFile* file);

private:
	scene::ISceneManager* SceneManager;
};

} // end namespace scene
} // end namespace irr

#endif // __C_FBX_MESH_LOADER_H_INCLUDED__
