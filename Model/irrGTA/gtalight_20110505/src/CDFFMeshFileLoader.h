// Copyright (C) 2007-2011 David Kment
// For license see license.txt

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

struct SMaterial
{
    bool hasTexture;
    stringc textureName;
};

struct SGeometry
{
    array<unsigned int> SubFaceCounts;
    array<unsigned int> SubMaterialIndexes;
    array<float> TexCoords;
    array<float> Normals;
    array<unsigned short> Faces;
    array<float> Vertices;
    array<SMaterial*> Materials;
};

class CDFFMeshFileLoader : public IMeshLoader
{

public:

	CDFFMeshFileLoader(IrrlichtDevice* device);
	virtual ~CDFFMeshFileLoader();
    virtual bool isALoadableFileExtension(const io::path &filename) const;
	virtual IAnimatedMesh* createMesh(io::IReadFile* file);

private:

	bool Failed;
    unsigned short FileVersion;
    bool SubsAreTriStrips;

	void parseRWSection(int size, int depthlevel, int previoustype);
	void processRWData(int size, int type, int version);
	void processRWString(int size, int type, int version);
	void compileMesh();

    io::IReadFile* File;

	SMesh* Mesh;

    array<SGeometry*> Geometries;

    io::IAttributes* Parameters;
    video::IVideoDriver* Driver;
    io::IFileSystem* FileSystem;
    ILogger* Logger;
};

#endif /* _CDFFMESHFILELOADER_H_ */
