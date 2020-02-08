// Copyright (C) 2002-2005 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __C_OBJ_MESH_FILE_LOADER_H_INCLUDED__
#define __C_OBJ_MESH_FILE_LOADER_H_INCLUDED__

#include "compile_config.h"
#include <IMeshLoader.h>
#include <IFileSystem.h>
#include <ITimer.h>
#include "ISceneManager.h"
#include <irrString.h>
#include <SMesh.h>
#include <SMeshBuffer.h>
#include <SMeshBufferLightMap.h>

namespace irr
{
namespace scene
{

//! Meshloader capable of loading 3ds meshes.
class CLMOMeshFileLoader : public IMeshLoader
{
public:

	//! Constructor
	CLMOMeshFileLoader(ISceneManager* smgr, io::IFileSystem* fs, ITimer* timer);

	//! destructor
	virtual ~CLMOMeshFileLoader();

	//! returns true if the file maybe is able to be loaded by this class
	//! based on the file extension (e.g. ".lmo")
	bool isALoadableFileExtension(const io::path& filename) const;

	//! creates/loads an animated mesh from the file.
	//! \return Pointer to the created mesh. Returns 0 if loading failed.
	//! If you no longer need the mesh, you should call IAnimatedMesh::drop().
	//! See IReferenceCounted::drop() for more information.
	virtual IAnimatedMesh* createMesh(irr::io::IReadFile* file);

private:

	struct SObjMtl
	{
		SObjMtl() : illumination(0) {}
		SObjMtl(const SObjMtl& o) : Material(o.Material), name(o.name), illumination(o.illumination) {}

		~SObjMtl()  { }

		video::SMaterial Material;
		core::stringc name;
		c8 illumination;
	};

	struct SObjGroup
	{
		SObjGroup() : Meshbuffer(0), MeshbufferLM(0) {};
		SObjGroup(SObjGroup& o) : Meshbuffer(o.Meshbuffer), MeshbufferLM(o.MeshbufferLM)
		{
		    if ( o.Meshbuffer )
                o.Meshbuffer->grab();
            if ( o.MeshbufferLM )
                o.MeshbufferLM->grab();
		};
		~SObjGroup() { 	};

		scene::SMeshBuffer * Meshbuffer;
		scene::SMeshBufferLightMap * MeshbufferLM;
		core::stringc name;
	};

	// returns a pointer to the first printable character available in the buffer
	const c8* goFirstWord(const c8* buf, const c8* pBufEnd);
	// returns a pointer to the first printable character after the first non-printable
	const c8* goNextWord(const c8* buf, const c8* pBufEnd);
	// returns a pointer to the next printable character after the first line break
	const c8* goNextLine(const c8* buf, const c8* pBufEnd);
	// copies the current word from the inBuf to the outBuf
	void copyWord(c8* outBuf, const c8* inBuf, s32 outBufLength, const c8* pBufEnd);
	// copies the current line from the inBuf to the outBuf
	void copyLine(c8* outBuf, const c8* inBuf, s32 outBufLength, const c8* pBufEnd);
	// combination of goNextWord followed by copyWord
	const c8* goAndCopyNextWord(c8* outBuf, const c8* inBuf, s32 outBufLength, const c8* pBufEnd);

	const c8* readTextures(const c8* bufPtr, const c8* const bufEnd, SObjMtl* currMaterial, const io::path& relPath);
	//! Read the material from the given file
	void readMTL(const c8* pFileName, const io::path& relPath);
	//! Find and return the material with the given name
	SObjMtl * findMtl(c8* pMtlName);

	//! Read RGB color
	const c8* readColor(const c8* bufPtr, video::SColor& color, const c8* const pBufEnd);
	//! Read 3d vector of floats
	const c8* readVec3(const c8* bufPtr, core::vector3df& vec, const c8* const pBufEnd);
	//! Read 2d vector of floats
	const c8* readUV(const c8* bufPtr, core::vector2df& vec, const c8* const pBufEnd);
	//! Read boolean value represented as 'on' or 'off'
	const c8* readBool(const c8* bufPtr, bool& tf, const c8* const bufEnd);

	SObjGroup * addGroup(c8* pGroupName);
	SObjGroup * findGroup(c8* pGroupName);
	SObjGroup * findOrAddGroup(c8* pGroupName);

	// reads and convert to integer the vertex indices in a line of obj file's face statement
	// -1 for the index if it doesn't exist
	// indices are changed to 0-based index instead of 1-based from the obj file
	bool retrieveVertexIndices(c8* pVertexData, s32* pIdx, const c8* pBufEnd);

	void cleanUp();

	video::ITexture * getTexture(const c8* textureName, const io::path& relPath, bool checkAlternatives);

	scene::ISceneManager* SceneManager;
	io::IFileSystem* FileSystem;
	ITimer* Timer;

	core::array<SObjMtl> materials;
	core::array<SObjGroup*> groups;
	SMesh* Mesh;

	bool mIsLightmap;
	core::stringc mLightmapTexture;
};

} // end namespace scene
} // end namespace irr

#endif

