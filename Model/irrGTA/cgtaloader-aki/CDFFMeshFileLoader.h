/*
  CDFFMeshFileLoader: DFF mesh format loader for Irrlicht

  Copyright (C) 2007-2008 David Kment

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.


  v0.2, first release, developed and tested with Irrlicht 1.4.2

  Notes:
    - Parses DFF file versions from GTA3/GTAVC/GTASA/ManHunt
    - Frames/animations are currently unsupported
    - Set the path where textures (tga, jpg or png) are found like this:
      device->getSceneManager()->getParameters()->setAttribute("DFF_TEXTURE_PATH", "textures/");
    - Note that the GTA games have different texture subpaths for each mesh,
      so updating the above variable is needed before loading each mesh.
      The respective subpaths are defined in the IPL files.
    - Texture filter flags are currently ignored, so no transparency supported

  Known Bugs:
    - DFF files from GTAVC/GTASA/ManHunt are correctly parsed
      but mesh data is not correctly interpreted, resulting
      in corrupt looking meshes
    - Most ManHunt files seem to store multiple models in one
      DFF file, probably seperating them using frames. Since
      frames are not supported currently, these won't work.
*/

#ifndef _CDFFMESHFILELOADER_H_
#define _CDFFMESHFILELOADER_H_

#include "irrlicht.h"
#include "IrrlichtDevice.h"
#include "IMeshLoader.h"
#include "IFileSystem.h"
#include "SMesh.h"
#include "IFileSystem.h"
#include "IVideoDriver.h"

#include <vector>

// DFF file versions
#define DFF_VERSION_GTA3_1 0
#define DFF_VERSION_GTA3_2 2048
#define DFF_VERSION_GTA3_3 65400
#define DFF_VERSION_GTAVC_1 3074
#define DFF_VERSION_GTAVC_2 4099
#define DFF_VERSION_GTASA 6147

// DFF section IDs
#define DFF_RW_DATA 1
#define DFF_RW_STRING 2
#define DFF_RW_EXTENSION 3
#define DFF_RW_TEXTURE 6
#define DFF_RW_MATERIAL 7
#define DFF_RW_MATERIALLIST 8
#define DFF_RW_FRAMELIST 14
#define DFF_RW_GEOMETRY 15
#define DFF_RW_CLUMP 16
#define DFF_RW_ATOMIC 20
#define DFF_RW_GEOMETRYLIST 26
#define DFF_RW_MATERIALSPLIT 1294
#define DFF_RW_FRAME 39056126

using namespace irr;
using namespace core;
using namespace scene;

class CDFFMeshFileLoader : public IMeshLoader
{

public:

	CDFFMeshFileLoader(IrrlichtDevice* device);
	virtual ~CDFFMeshFileLoader();
	virtual bool isALoadableFileExtensiona(const c8* fileName) const;
	virtual bool isALoadableFileExtension(const io::path& fileName) const;
	virtual IAnimatedMesh* createMesh(io::IReadFile* file);

   // bool isALoadableFileExtension(const io::path& filename);

private:

	bool Failed;
    unsigned short FileVersion;
    bool SubsAreTriStrips;

	void parseDFFSection(int size, int depthlevel, int previoustype);
	void processDFFData(int size, int type, int version);
	void processDFFString(int size, int type, int version);
	void compileMesh();

    io::IReadFile* File;

	SMesh* Mesh;

    array<unsigned int> SubFaceCounts;
    array<unsigned int> SubMaterialIndexes;
    array<float> TexCoords;
    array<float> Normals;
    array<unsigned short> Faces;
    array<float> Vertices;
    array<stringc> Textures;


	io::IAttributes* Parameters;
	video::IVideoDriver* Driver;
	io::IFileSystem* FileSystem;
	ILogger* Logger;
};

#endif /* _CDFFMESHFILELOADER_H_ */
