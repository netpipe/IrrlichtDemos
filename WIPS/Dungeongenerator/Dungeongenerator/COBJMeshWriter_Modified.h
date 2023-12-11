// Copyright (C) 2008-2010 Christian Stehno
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
//
// modified by Maximilian Honig

// zur Verhinderung doppelter Definition
#ifndef OBJMESHWRITER_MODIFED_H
#define OBJMESHWRITER_MODIFED_H

#include <irrlicht.h>

namespace irr
{
namespace io
{
	class IFileSystem;
} // end namespace io
namespace scene
{
	class IMeshBuffer;
	class ISceneManager;

	//! class to write meshes, implementing a OBJ writer
	class COBJMeshWriter_Modified : public IMeshWriter
	{
	public:

		COBJMeshWriter_Modified(scene::ISceneManager* smgr, io::IFileSystem* fs);
		virtual ~COBJMeshWriter_Modified();

		//! Returns the type of the mesh writer
		virtual EMESH_WRITER_TYPE getType() const;

		//! writes a mesh
		virtual bool writeMesh(io::IWriteFile* file, scene::IMesh* mesh, s32 flags=EMWF_NONE);

	protected:
		// create vector output with line end into string
		void getVectorAsStringLine(const core::vector3df& v,
				core::stringc& s) const;

		// create vector output with line end into string
		void getVectorAsStringLine(const core::vector2df& v,
				core::stringc& s) const;

		// create color output with line end into string
		void getColorAsStringLine(const video::SColor& color,
				const c8* const prefix, core::stringc& s) const;

		scene::ISceneManager* SceneManager;
		io::IFileSystem* FileSystem;
	};

} // end namespace
} // end namespace

#endif

