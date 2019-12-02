#include"irrAn8Loader.h"

#include "IReadFile.h"

#include "liban8/An8Loader.h"

namespace irr
{
namespace scene
{

//! Constructor
irrAn8Loader::irrAn8Loader(scene::ISceneManager* smgr, io::IFileSystem* fs)
: SceneManager(smgr), FileSystem(fs)
{
	if (FileSystem)
		FileSystem->grab();
}

//! destructor
irrAn8Loader::~irrAn8Loader()
{
	if (FileSystem)
		FileSystem->drop();
}

bool irrAn8Loader::isALoadableFileExtension(const c8* fileName) const
{
	return strstr(fileName, ".an8")!=0;
}

IAnimatedMesh* irrAn8Loader::createMesh(io::IReadFile* file)
{
	const long filesize = file->getSize();
	if (!filesize)
		return NULL;

	const core::stringc fullName = file->getFileName();
	const core::stringc relPath = FileSystem->getFileDir(fullName)+"/";

	SAnimatedMesh* am = new SAnimatedMesh();
    am->Type = EAMT_UNKNOWN;

	An8File* an8File = new An8File();
	if( an8File->LoadFile(fullName.c_str()) == false)
	{
		delete an8File;
		return NULL;
	}

	an8File->Ungroup();				// Ungroup meshes (easier to manipulate)
	an8File->ConvertAllToMesh();	// Convert all cubes/spheres/cylinders to mesh (remove old cubes/spheres/cylinders )
	an8File->ComputeAllPositions(); // Compute mesh position in object space (avoid the need of translation)
	an8File->Triangulize();			// Triangulize faces
	an8File->ComputeAllMatrices();	// Compute matices of camera, objects, Lights, ... from quaternions
	an8File->ComputeTextureIDForMaterial(); // Computer integer id to joint texture and material (.an8 files use strings)

	if( an8File->NbrObject < 1 ) // File not supported by the viewer
	{
		delete an8File;
		return NULL;
	}

	SMesh* mesh = new SMesh();

	if( an8File->NbrScene == 0 )
	{
		for( int i = 0 ; i < (int)an8File->vObjects[0].NbrMesh ; ++i )
		{
			if( an8File->vObjects[0].vMeshes[i].NbrFace == 0 )
				continue; // Some meshes can be empty

			AN8XMATRIX matrix; // for future use (matrix of object in scene)
			AddMesh(mesh,an8File,i,&matrix);
		}
	}
	else
	{
		for( int o = 0 ; o < (int)an8File->vScenes[0].vObjectElements.size() ; ++o )
		{
			for( int m = 0 ;m < (int)an8File->vObjects[an8File->vScenes[0].vObjectElements[o].ObjectID].NbrMesh ; ++m )
			{
				if( an8File->vObjects[an8File->vScenes[0].vObjectElements[o].ObjectID].vMeshes[m].NbrFace == 0 )
					continue; // Some meshes can be empty

				AN8XMATRIX matrix; // for future use (matrix of object in scene)
				AddMesh(mesh,an8File,m,&an8File->vScenes[0].vObjectElements[o].Matrix);
			}
		}
	}

	// calculate bounding box
	for (u32 j=0; j<mesh->MeshBuffers.size(); ++j)
    {
        ((SMeshBuffer*)mesh->MeshBuffers[j])->recalculateBoundingBox();
    }
	mesh->recalculateBoundingBox();

	am->addMesh(mesh);
    mesh->drop();
    mesh = NULL;

	delete an8File;

    am->recalculateBoundingBox();
    return am;
}

void irrAn8Loader::AddMesh(SMesh* mesh,An8File* aniFile,int meshID,AN8XMATRIX* matrix)
{
	An8Mesh* aniMesh = &aniFile->vObjects[0].vMeshes[meshID];

	if( aniMesh->vFaces.size() < 1 )
		return;

	scene::SMeshBuffer* meshBuffer;
	meshBuffer = new scene::SMeshBuffer();

	// Material
	int matNumber = aniMesh->vMatNumber[aniMesh->vFaces[0].Material];
	An8Material* material = &aniFile->vObjects[0].vMaterials[matNumber];

	if( material->UseAlpha == true )
		meshBuffer->Material.MaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL;
	else
		meshBuffer->Material.MaterialType = video::EMT_SOLID;

	meshBuffer->Material.Wireframe = false;
	meshBuffer->Material.Lighting = true;
	meshBuffer->Material.BackfaceCulling = true;
	meshBuffer->Material.NormalizeNormals = false;
	meshBuffer->Material.EmissiveColor.set(material->Alpha, material->EmissiveRGB[0], material->EmissiveRGB[1], material->EmissiveRGB[2]);
	meshBuffer->Material.DiffuseColor.set(material->Alpha, material->DiffuseRGB[0], material->DiffuseRGB[1], material->DiffuseRGB[2]);
	meshBuffer->Material.AmbientColor.set(material->Alpha, material->AmbiantRGB[0], material->AmbiantRGB[1], material->AmbiantRGB[2]);
	meshBuffer->Material.SpecularColor.set(material->Alpha, material->SpecularRGB[0], material->SpecularRGB[1], material->SpecularRGB[2]);
	meshBuffer->Material.Shininess = material->Brilliance;

	if( material->DiffuseIsTexture == true )
	{
		std::string texname = aniFile->vTextures[material->DiffuseTextureID].FileName;
		AN8XFileNameFromPath( &texname, &texname);

		video::ITexture * texture = 0;
		if (FileSystem->existFile(texname.c_str()))
			texture = SceneManager->getVideoDriver()->getTexture(texname.c_str());

		if ( texture )
			meshBuffer->Material.setTexture(0, texture);
	}


	int nbrVertex = aniMesh->NbrFace * 3; // because we don't use ComputeSmoothNormals()
	int nbrIndex  = aniMesh->NbrFace * 3;

	if( aniMesh->NbrNormal == 0 )
		aniMesh->ComputeFaceNormal(); // Usefull if normal are not present in file

	for( int i = 0 ; i < aniMesh->NbrFace ; ++i )
	{
		video::S3DVertex vertices[3];

		AN8XVECTOR3 vec3;

		AN8XVec3TransformCoord(&vec3,&aniMesh->vPoints[aniMesh->vFaces[i].vIndices[0].Point],matrix);
		vertices[0].Pos = core::vector3df(
			-vec3.x,
			vec3.y,
			vec3.z);

		AN8XVec3TransformCoord(&vec3,&aniMesh->vPoints[aniMesh->vFaces[i].vIndices[1].Point],matrix);
		vertices[1].Pos = core::vector3df(
			-vec3.x,
			vec3.y,
			vec3.z);

		AN8XVec3TransformCoord(&vec3,&aniMesh->vPoints[aniMesh->vFaces[i].vIndices[2].Point],matrix);
		vertices[2].Pos = core::vector3df(
			-vec3.x,
			vec3.y,
			vec3.z);

		// Use "Output Normal" in debug menu of Anim8or to export vertex normal
		// in .an8 file
		if( aniMesh->NbrNormal > 0 )
		{
			vertices[0].Normal = core::vector3df(
				aniMesh->vNormals[aniMesh->vFaces[i].vIndices[0].Normal].x,
				aniMesh->vNormals[aniMesh->vFaces[i].vIndices[0].Normal].y,
				aniMesh->vNormals[aniMesh->vFaces[i].vIndices[0].Normal].z
			);
			vertices[1].Normal = core::vector3df(
				aniMesh->vNormals[aniMesh->vFaces[i].vIndices[1].Normal].x,
				aniMesh->vNormals[aniMesh->vFaces[i].vIndices[1].Normal].y,
				aniMesh->vNormals[aniMesh->vFaces[i].vIndices[1].Normal].z
			);
			vertices[2].Normal = core::vector3df(
				aniMesh->vNormals[aniMesh->vFaces[i].vIndices[2].Normal].x,
				aniMesh->vNormals[aniMesh->vFaces[i].vIndices[2].Normal].y,
				aniMesh->vNormals[aniMesh->vFaces[i].vIndices[2].Normal].z
			);
		}
		else // vertex normal are not exported in the .an8 file. Use face Normal
		{    //  computed by LibAn8

			vertices[0].Normal = core::vector3df(
				aniMesh->vFaces[i].Normal.x,
				aniMesh->vFaces[i].Normal.y,
				aniMesh->vFaces[i].Normal.z
			);
			vertices[1].Normal = vertices[0].Normal;
			vertices[2].Normal = vertices[0].Normal;
		}

		if( aniMesh->NbrUVCoord > 0 )
		{
			vertices[0].TCoords = core::vector2df(
				 aniMesh->vTexCoords[aniMesh->vFaces[i].vIndices[0].TexCoord].x,
				 aniMesh->vTexCoords[aniMesh->vFaces[i].vIndices[0].TexCoord].y
			);
			vertices[1].TCoords = core::vector2df(
				 aniMesh->vTexCoords[aniMesh->vFaces[i].vIndices[1].TexCoord].x,
				 aniMesh->vTexCoords[aniMesh->vFaces[i].vIndices[1].TexCoord].y
			);
			vertices[2].TCoords = core::vector2df(
				 aniMesh->vTexCoords[aniMesh->vFaces[i].vIndices[2].TexCoord].x,
				 aniMesh->vTexCoords[aniMesh->vFaces[i].vIndices[2].TexCoord].y
			);
		}
		vertices[0].Color = video::SColor(255,255,255,255);
		vertices[1].Color = video::SColor(255,255,255,255);
		vertices[2].Color = video::SColor(255,255,255,255);

		meshBuffer->Vertices.push_back(vertices[0]);
		meshBuffer->Vertices.push_back(vertices[1]);
		meshBuffer->Vertices.push_back(vertices[2]);

		meshBuffer->Indices.push_back(i*3);
		meshBuffer->Indices.push_back(i*3+1);
		meshBuffer->Indices.push_back(i*3+2);
	}
	mesh->addMeshBuffer(meshBuffer);
    meshBuffer->drop();
}

} // end namespace scene
} // end namespace irr

