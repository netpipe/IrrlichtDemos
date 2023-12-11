// Copyright (C) 2007-2011 David Kment
// For license see license.txt

#include "CDFFMeshFileLoader.h"

#include <SAnimatedMesh.h>
#include <SMeshBuffer.h>
#include <irrString.h>
#include <IAttributes.h>
#include <IrrlichtDevice.h>

CDFFMeshFileLoader::CDFFMeshFileLoader(IrrlichtDevice* device)
{
    Mesh = NULL;

	FileSystem = device->getFileSystem();
	FileSystem->grab();

	Driver = device->getVideoDriver();
	Driver->grab();

	Parameters = device->getSceneManager()->getParameters();

	Logger = device->getLogger();
}

CDFFMeshFileLoader::~CDFFMeshFileLoader()
{
	if (FileSystem) {FileSystem->drop();}

	if (Driver) {Driver->drop();}

	if (Mesh) {Mesh->drop();}

	// if (Logger) {Logger->drop();}
}

bool CDFFMeshFileLoader::isALoadableFileExtension(const io::path &filename) const
{
	return core::hasFileExtension(filename, "dff");
}

IAnimatedMesh* CDFFMeshFileLoader::createMesh(io::IReadFile* infile)
{
    Failed = false;
    SubsAreTriStrips = false;

	Mesh = new SMesh();

    File = infile;

    if(File->getSize() == 0) {return NULL;}

    unsigned int sectionType, sectionSize;
    unsigned short unknown, version;

    File->read(&sectionType, sizeof(int));
    File->read(&sectionSize, sizeof(int));
    File->read(&unknown, sizeof(short));
    File->read(&version, sizeof(short));

    if(version == DFF_VERSION_GTA3_1 || version == DFF_VERSION_GTA3_2 || version == DFF_VERSION_GTA3_3)
    {
        // GTA3 DFF file
        // note: DFF_VERSION_GTA3_2 file version is also used in GTAVC
    }
    else if(version == DFF_VERSION_GTAVC_1 || version == DFF_VERSION_GTAVC_2)
    {
        // GTAVC DFF file
    }
    else if(version == DFF_VERSION_GTASA)
    {
        // GTASA or ManHunt DFF file
    }
    else
    {
        // unsupported DFF file
        stringc tmp = "";
        tmp += version;
        Logger->log("unsupported DFF file", infile->getFileName().c_str(), ELL_WARNING);
        Logger->log("version", tmp.c_str());
        return NULL;
    }

    FileVersion = version;

    parseRWSection(sectionSize, 0, 1);

    compileMesh();

    // return NULL if DFF parsing failed
    if(Failed)
    {
        Logger->log("Error while parsing DFF file", infile->getFileName().c_str(), ELL_WARNING);
        return NULL;
    }

    // calculate bounding box
	for (u32 j=0; j<Mesh->MeshBuffers.size(); ++j)
    {
        ((SMeshBuffer*)Mesh->MeshBuffers[j])->recalculateBoundingBox();
    }
	Mesh->recalculateBoundingBox();

    SAnimatedMesh* am = new SAnimatedMesh();
    am->Type = EAMT_UNKNOWN;
    am->addMesh(Mesh);
    am->recalculateBoundingBox();
    Mesh->drop();
    Mesh = 0;
    return am;
}

void CDFFMeshFileLoader::parseRWSection(int size, int depthlevel, int parentType)
{
    if(size == 0) {return;}

    if(parentType == DFF_RW_MATERIAL)
    {
		SGeometry *geometry = Geometries.getLast();

        SMaterial *material = new SMaterial();
        material->hasTexture = false;

        geometry->Materials.push_back(material);
    }

    unsigned int sectionType, sectionSize;
    unsigned short sectionunknown = 0, sectionVersion = 0;

    int endoffset = File->getPos() + size;

    while(File->getPos() < endoffset)
    {
        File->read(&sectionType, sizeof(int));
        File->read(&sectionSize, sizeof(int));
        File->read(&sectionunknown, sizeof(short));
        File->read(&sectionVersion, sizeof(short));

        // uncomment to output the CLUMP structure in stdout
        // for(int i=0; i < depthlevel; i++) {fprintf(stdout, "  ");}
        // fprintf(stdout, "sectionType: %d, sectionSize: %d\n", sectionType, sectionSize);

        switch(sectionType)
        {
            case DFF_RW_DATA:
                processRWData(sectionSize, parentType, sectionVersion);
                break;

            case DFF_RW_STRING:
                processRWString(sectionSize, parentType, sectionVersion);
                break;

            case DFF_RW_FRAME:
                // not yet implemented, skip sectionSize bytes
                File->seek(sectionSize, true);
                break;

            case DFF_RW_MATERIALSPLIT:
                processRWData(sectionSize, DFF_RW_MATERIALSPLIT, sectionVersion);
                break;

            // all below can contain more sections
            case DFF_RW_EXTENSION:
                parseRWSection(sectionSize, depthlevel + 1, sectionType);
                break;

            case DFF_RW_TEXTURE:
                parseRWSection(sectionSize, depthlevel + 1, sectionType);
                break;

            case DFF_RW_MATERIAL:
                parseRWSection(sectionSize, depthlevel + 1, sectionType);
                break;

            case DFF_RW_MATERIALLIST:
                parseRWSection(sectionSize, depthlevel + 1, sectionType);
                break;

            case DFF_RW_FRAMELIST:
                parseRWSection(sectionSize, depthlevel + 1, sectionType);
                break;

            case DFF_RW_GEOMETRY:
                parseRWSection(sectionSize, depthlevel + 1, sectionType);
                break;

            case DFF_RW_CLUMP:
                parseRWSection(sectionSize, depthlevel + 1, sectionType);
                break;

            case DFF_RW_ATOMIC:
                parseRWSection(sectionSize, depthlevel + 1, sectionType);
                break;

            case DFF_RW_GEOMETRYLIST:
                parseRWSection(sectionSize, depthlevel + 1, sectionType);
                break;

            default:
                // unknown section type, skip sectionSize bytes
                File->seek(sectionSize, true);
                break;
        }
    }
}

void CDFFMeshFileLoader::processRWData(int size, int type, int version)
{
    unsigned int i, j;

    if(type == DFF_RW_MATERIALSPLIT)
    {
		SGeometry *geometry = Geometries.getLast();

        unsigned int triangleSplit, splitCount, faceCount;

        File->read(&triangleSplit, sizeof(int));
        File->read(&splitCount, sizeof(int));
        File->read(&faceCount, sizeof(int));

        if(triangleSplit != 0)
        {
            SubsAreTriStrips = true;
        }

        unsigned int faceIndex, materialIndex, vertex;

		geometry->SubFaceCounts.reallocate(faceCount);
		geometry->SubMaterialIndexes.reallocate(splitCount);

		// this overwrites our faces
		geometry->Faces.clear();
		geometry->Faces.reallocate(faceCount);

        for(i=0; i < splitCount; i++)
        {
            File->read(&faceIndex, sizeof(int));
            File->read(&materialIndex, sizeof(int));

            geometry->SubFaceCounts.push_back(faceIndex);
            geometry->SubMaterialIndexes.push_back(materialIndex);

            for(j = 0; j < faceIndex; j++)
            {
                File->read(&vertex, sizeof(int));
                geometry->Faces.push_back(vertex);
            }
        }
    }

    else if(type == DFF_RW_GEOMETRY)
    {
		SGeometry *geometry = new SGeometry();

        unsigned short flags, unknown;
        unsigned int triangleCount, vertexCount, morphTargetCount;

        File->read(&flags, sizeof(short));
        File->read(&unknown, sizeof(short));
        File->read(&triangleCount, sizeof(int));
        File->read(&vertexCount, sizeof(int));
        File->read(&morphTargetCount, sizeof(int)); // aka frame count

        if (version < DFF_VERSION_GTAVC_2)
        {
            // geometry has lighting
            // ignore lighting for now, skip 12 bytes
            File->seek(12, true);
        }

        if (flags & 8)
        {
            // geometry has color info
            // ignore colors for now, skip vertexCount*4 bytes
            File->seek(vertexCount*4, true);
        }

        if (flags & 4)
        {
            // geometry has texture coordinates

            geometry->TexCoords.reallocate(vertexCount * 2);

            float coord_x, coord_y;

            for (i=0; i < vertexCount; i++)
            {
                File->read(&coord_x, sizeof(float));
                File->read(&coord_y, sizeof(float));

                geometry->TexCoords.push_back(coord_x);
                geometry->TexCoords.push_back(coord_y);
            }
        }

        // get geometry faces

        geometry->Faces.reallocate(triangleCount * 3);

        unsigned short indexflags = 0, index1 = 0, index2 = 0, index3 = 0;

        for (i=0; i < triangleCount; i++)
        {
            File->read(&index2, sizeof(short));
            File->read(&index1, sizeof(short));
            File->read(&indexflags, sizeof(short));
            File->read(&index3, sizeof(short));

            geometry->Faces.push_back(index1);
            geometry->Faces.push_back(index2);
            geometry->Faces.push_back(index3);
        }

        // skip 24 bytes of extra data
        File->seek(24, true);

        // get geometry vertices

        geometry->Vertices.reallocate(vertexCount * 3);

        float vertex_x, vertex_y, vertex_z;

        for (i=0; i < vertexCount; i++)
        {
            File->read(&vertex_x, sizeof(float));
            File->read(&vertex_y, sizeof(float));
            File->read(&vertex_z, sizeof(float));

            geometry->Vertices.push_back(-vertex_x);
            geometry->Vertices.push_back(vertex_y);
            geometry->Vertices.push_back(vertex_z);
        }

        if (flags & 16)
        {
            // geometry has normals info

            geometry->Normals.reallocate(vertexCount * 3);

            float normal_x, normal_y, normal_z;

            for (i=0; i < vertexCount; i++)
            {
                File->read(&normal_x, sizeof(float));
                File->read(&normal_y, sizeof(float));
                File->read(&normal_z, sizeof(float));

                geometry->Normals.push_back(normal_x);
                geometry->Normals.push_back(normal_y);
                geometry->Normals.push_back(normal_z);
            }
        }

		Geometries.push_back(geometry);
    }
    else
    {
        // unknown data section type, skip size bytes
        File->seek(size, true);
    }
}

void CDFFMeshFileLoader::processRWString(int size, int type, int version)
{
    char *temp = new char[size + 1];

    File->read(temp, size);

    temp[size] = 0;

    stringc str = temp;

    delete [] temp;

    if(type == DFF_RW_TEXTURE)
    {
        if(str.size() > 0)
        {
			SMaterial *material = Geometries.getLast()->Materials.getLast();

            material->hasTexture = true;
            material->textureName = str;
        }
    }
}

void CDFFMeshFileLoader::compileMesh()
{
	// hack: only use last geometry
	SGeometry *geometry = Geometries.getLast();

    scene::SMeshBuffer* meshBuffer;

    unsigned int subFaceOffset = 0, vertexOffset = 0, facesOffset = 0, materialOffset = 0;

	unsigned int materialIndex;

    for(subFaceOffset=0; subFaceOffset<geometry->SubFaceCounts.size(); subFaceOffset++)
    {
        // some bounds checking
        if(materialOffset >= geometry->SubMaterialIndexes.size())
        {
            Logger->log("materialOffset >= SubMaterialIndexes.size()", ELL_INFORMATION);
            Failed = true;
            break;
        }

        if(geometry->SubMaterialIndexes[materialOffset] >= geometry->Materials.size())
        {
            Logger->log("SubMaterialIndexes[materialOffset] >= Materials.size()", ELL_INFORMATION);
            Failed = true;
            break;
        }

		materialIndex = geometry->SubMaterialIndexes[materialOffset];

        meshBuffer = new scene::SMeshBuffer();

        // add texture if material has texture
        if(geometry->Materials[materialIndex]->hasTexture)
        {
            stringc texturePath = Parameters->getAttributeAsString("DFF_TEXTURE_PATH");

            texturePath += geometry->Materials[materialIndex]->textureName;

            meshBuffer->Material.MaterialType = video::EMT_SOLID;
            meshBuffer->Material.Wireframe = false;
            meshBuffer->Material.Lighting = true;
            meshBuffer->Material.BackfaceCulling = false;
            meshBuffer->Material.NormalizeNormals = false;
            meshBuffer->Material.EmissiveColor.set(255, 255, 255, 255);
            // meshBuffer->Material.BilinearFilter = true;

            video::ITexture *tmpTexture = NULL;

            stringc fileName;

            fileName = texturePath + "." + Parameters->getAttributeAsString("GTA_TEXTURES_EXTENSION");

            if(FileSystem->existFile(fileName.c_str()))
            {
                tmpTexture = Driver->getTexture(fileName.c_str());
                meshBuffer->getMaterial().setTexture(0, tmpTexture);

                if(tmpTexture)
                {
                    if(tmpTexture->hasAlpha())
                    {
                        meshBuffer->Material.MaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF;
                    }
                }
            }
            else
            {
                Logger->log("Texture does not exist", fileName.c_str(), ELL_WARNING);
            }
        }

        materialOffset++;

        // add vertexes & faces

        video::S3DVertex currentvertex;

        unsigned int face;

        meshBuffer->Indices.reallocate(geometry->SubFaceCounts[subFaceOffset]);
        meshBuffer->Vertices.reallocate(geometry->SubFaceCounts[subFaceOffset]);

        for(vertexOffset = 0; vertexOffset < geometry->SubFaceCounts[subFaceOffset]; vertexOffset++)
        {
            face = geometry->Faces[facesOffset++];

            // bounds check
            if(geometry->Vertices.size() < ((face * 3) + 2))
            {
                Logger->log("Vertices.size() < ((face * 3) + 2)", ELL_INFORMATION);
                Failed = true;

                break;
            }

            currentvertex.Pos.X = geometry->Vertices[face * 3];
            currentvertex.Pos.Y = geometry->Vertices[(face * 3) + 1];
            currentvertex.Pos.Z = geometry->Vertices[(face * 3) + 2];

            // check wether we have normals
            if(geometry->Normals.size() > 0)
            {
                // bounds check
                if(geometry->Normals.size() < ((face * 3) + 2))
                {
                    Logger->log("Normals.size() < ((face * 3) + 2)", ELL_INFORMATION);
                    Failed = true;
                    break;
                }

                currentvertex.Normal.X = geometry->Normals[face * 3];
                currentvertex.Normal.Y = geometry->Normals[(face * 3) + 1];
                currentvertex.Normal.Z = geometry->Normals[(face * 3) + 2];
            }

            // bounds check
            if(geometry->TexCoords.size() < ((face * 2) + 1))
            {
                Logger->log("TexCoords.size() < ((face * 2) + 1)", ELL_INFORMATION);
                Failed = true;
                break;
            }

            currentvertex.TCoords.X = geometry->TexCoords[face * 2];
            currentvertex.TCoords.Y = geometry->TexCoords[(face * 2) + 1];

            if(SubsAreTriStrips)
            {
                if(vertexOffset < 3)
                {
                    meshBuffer->Indices.push_back(vertexOffset);
                }
                else
                {
                    meshBuffer->Indices.push_back(vertexOffset - 2);
                    meshBuffer->Indices.push_back(vertexOffset - 1);
                    meshBuffer->Indices.push_back(vertexOffset);
                }
            }
            else
            {
                meshBuffer->Indices.push_back(vertexOffset);
            }

            meshBuffer->Vertices.push_back(currentvertex);
        }

        Mesh->addMeshBuffer(meshBuffer);
        meshBuffer->drop();
    }

    geometry->SubFaceCounts.clear();
    geometry->SubMaterialIndexes.clear();
    geometry->TexCoords.clear();
    geometry->Normals.clear();
    geometry->Faces.clear();
    geometry->Vertices.clear();
    geometry->Materials.clear();
}
