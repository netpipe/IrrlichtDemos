// Copyright (C) 2002-2011 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#include "IrrCompileConfig.h"
//#ifdef _IRR_COMPILE_WITH_BSP_LOADER_

#include "CBSPMeshFileLoader.h"
#include "CQ3LevelMesh.h"
#include "CMOHAALevelMesh.h"

namespace irr
{
namespace scene
{


//IMesh* CBSPMeshFileLoader::getBrushEntityMesh(quake3::IEntity &ent) const{};



//! Constructor
CBSPMeshFileLoader::CBSPMeshFileLoader(scene::ISceneManager* smgr,
		io::IFileSystem* fs)
: FileSystem(fs), SceneManager(smgr)
{

	#ifdef _DEBUG
	setDebugName("CBSPMeshFileLoader");
	#endif

	if (FileSystem)
		FileSystem->grab();
}


//! destructor
CBSPMeshFileLoader::~CBSPMeshFileLoader()
{
	if (FileSystem)
		FileSystem->drop();
}


//! returns true if the file maybe is able to be loaded by this class
//! based on the file extension (e.g. ".bsp")
bool CBSPMeshFileLoader::isALoadableFileExtension(const io::path& filename) const
{
	return core::hasFileExtension ( filename, "bsp", "shader", "cfg" );
}


//! creates/loads an animated mesh from the file.
//! \return Pointer to the created mesh. Returns 0 if loading failed.
//! If you no longer need the mesh, you should call IAnimatedMesh::drop().
//! See IReferenceCounted::drop() for more information.
IAnimatedMesh* CBSPMeshFileLoader::createMesh(io::IReadFile* file)
{
	s32 BSPid;
	s32 type = core::isFileExtension ( file->getFileName(), "bsp", "shader", "cfg" );
	file->read( &BSPid, sizeof(BSPid) );
	file->seek( 0, false );
	CQ3LevelMesh* q = 0;
	CMOHAALevelMesh* m = 0;

	switch ( type )
	{
		case 1:
			if ( BSPid == 0x50534249 || BSPid == 0x50534252 ) { // IBSP (Q3), RBSP
				q = new CQ3LevelMesh(FileSystem, SceneManager, LoadParam);
				// determine real shaders in LoadParam
				if ( 0 == LoadParam.loadAllShaders )
				{
					q->getShader("scripts/common.shader");
					q->getShader("scripts/sfx.shader");
					q->getShader("scripts/gfx.shader");
					q->getShader("scripts/liquid.shader");
					q->getShader("scripts/models.shader");
					q->getShader("scripts/walls.shader");
					//q->getShader("scripts/sky.shader");
				}

				if ( q->loadFile(file) )
					return q;

				q->drop();
			}
			else if ( BSPid == 0x35313032 ) { // 2015 (MOHAA)
				m = new CMOHAALevelMesh(FileSystem, SceneManager, LoadParam);

				if ( m->loadFile(file) )
					return m;

				m->drop();
			}
			else {// other format ?
				return 0;
//				os::Printer::log("Could not load .bsp file, unknown header.", file->getFileName(), ELL_ERROR);
			}
			break;

		case 2:
			q = new CQ3LevelMesh(FileSystem, SceneManager,LoadParam);
			q->getShader( file );
			return q;
			break;

		case 3:
			// load quake 3 loading parameter
			if ( file->getFileName() == "levelparameter.cfg" )
			{
				file->read ( &LoadParam, sizeof ( LoadParam ) );
			}
			else
			{
				q = new CQ3LevelMesh(FileSystem, SceneManager,LoadParam);
				q->getConfiguration( file );
				return q;
			}
			break;
	}

	return 0;
}

} // end namespace scene
} // end namespace irr

//#endif // _IRR_COMPILE_WITH_BSP_LOADER_

