#include "GLAN8FileLoader.h"

#include"../Engine/TextureManager.h"
#include"../Engine/CameraManager.h"

#include<math.h>

extern TextureManager g_TextureManager;
extern CameraManager g_CameraManager;


GLAN8FileLoader::GLAN8FileLoader(void)
{
}

GLAN8FileLoader::~GLAN8FileLoader(void)
{
}

bool GLAN8FileLoader::LoadObjectFromFile(std::string* o_Error, char* i_FileName, GLAN8Object* i_pObject)
{
	An8File* an8File = new An8File();

	if( an8File->LoadFile(i_FileName) == false)
	{
		*o_Error = "Can't Load File\n";
		return false;
	}

	an8File->Ungroup(); // Ungroup meshes (easier to manipulate) 
	//an8File->ConvertAllToMesh(); // Convert all cubes/spheres/cylinders to mesh (remove old cubes/spheres/cylinders )
	an8File->ComputeAllPositions(); // Compute mesh position in object space (avoid the need of translation)
	
	an8File->Triangulize(); // Triangulize faces
	an8File->ComputeAllMatrices(); // Compute matices of camera, objects, Lights, ... from quaternions
	an8File->ComputeTextureIDForMaterial(); // Computer integer id to joint texture and material (.an8 files use strings)

	if( an8File->NbrObject < 1 ) // File not supported by the viewer
	{
		delete an8File;
		*o_Error = "No Object in file\n";
		return false;
	}

	for( int i = 0 ; i < an8File->NbrTexture ; ++i )
	{
		Texture* newTexture = new Texture();

		if( an8File->vTextures[i].IsCubeMap == false )
		{
			newTexture->IsCubeMap = false;
			newTexture->Name = an8File->vTextures[i].Name;
			AN8XFileNameFromPath(&newTexture->FileName,&an8File->vTextures[i].FileName);
		}
		else
		{
			newTexture->IsCubeMap = true;
			newTexture->Name = an8File->vTextures[i].Name;
			AN8XFileNameFromPath(&newTexture->CubeMap[0],&an8File->vTextures[i].CubeMap[0]);
			AN8XFileNameFromPath(&newTexture->CubeMap[1],&an8File->vTextures[i].CubeMap[1]);
			AN8XFileNameFromPath(&newTexture->CubeMap[2],&an8File->vTextures[i].CubeMap[2]);
			AN8XFileNameFromPath(&newTexture->CubeMap[3],&an8File->vTextures[i].CubeMap[3]);
			AN8XFileNameFromPath(&newTexture->CubeMap[4],&an8File->vTextures[i].CubeMap[4]);
			AN8XFileNameFromPath(&newTexture->CubeMap[5],&an8File->vTextures[i].CubeMap[5]);
		}
		g_TextureManager.AddTexture(newTexture);
	}

	// Objects
	LoadObject(i_pObject,&an8File->vObjects[0]);

	if( GLAN8FileLoader::LoadDefaultShader(o_Error) == false )
	{
		//*o_Error = "Can't load shader\n";
		return false;
	}
	i_pObject->SetShaderProgramObject(&GLAN8FileLoader::DefaultShaderProgramObject); // Give the default shader pointer

	if( i_pObject->Build() == false )
	{
		*o_Error = "No Object in file\n";
		return false;
	}




	return true;
}

//--------------------------------------------------------------------------------------
// Function to load .an8 data to the scene graph
//--------------------------------------------------------------------------------------
bool GLAN8FileLoader::LoadSceneFromFile(std::string* o_Error,char* i_FileName, GLAN8File* i_pLevel)
{
	An8File* an8File = new An8File();

	if( an8File->LoadFile(i_FileName) == false)
	{
		*o_Error = "Can't Load File\n";
		return false;
	}
	/*
	// To load .an8 file stored in memory (char*)
	char* mystring = NULL;
	mystring = GLGXTextLoadFromFile(i_FileName);

	AN8XSTRINGPARSER parser;
	parser.SetString(mystring);
	an8File->SetParser(&parser);
	if( an8File->Load() == false )
		return -1;
	delete [] mystring;
	*/


	if( an8File->NbrScene < 1 ) // File not supported by the viewer
	{
		delete an8File; 
		*o_Error = "No scene in file\n";
		return false;
	}
	if( an8File->NbrObject < 1 ) // File not supported by the viewer
	{
		delete an8File;
		*o_Error = "No Object in file\n";
		return false;
	}

	

	int numScene = 0; // We will just get the 1st scene of the file
	i_pLevel->NbrScene = an8File->NbrScene;
	i_pLevel->Scenes = new GLAN8Scene[i_pLevel->NbrScene];
	
	an8File->Ungroup(); // Ungroup meshes (easier to manipulate) 
	//an8File->ConvertAllToMesh(); // Convert all cubes/spheres/cylinders to mesh (remove old cubes/spheres/cylinders )

	an8File->ComputeAllPositions(); // Compute mesh position in object space (avoid the need of translation)
	
	an8File->Triangulize(); // Triangulize faces
	an8File->ComputeAllMatrices(); // Compute matices of camera, objects, Lights, ... from quaternions
	an8File->ComputeTextureIDForMaterial(); // Computer integer id to joint texture and material (.an8 files use strings)

	
	
	/*if( an8File->vScenes[numScene].NbrCamera < 1 ) // File not supported by the viewer
	{
		delete an8File;
		*o_Error = "No camera in file\n";
		return false;
	}
	*/
	
	for( int i = 0 ; i < an8File->NbrTexture ; ++i )
	{
		Texture* newTexture = new Texture();

		if( an8File->vTextures[i].IsCubeMap == false )
		{
			newTexture->IsCubeMap = false;
			newTexture->Name = an8File->vTextures[i].Name;
			AN8XFileNameFromPath(&newTexture->FileName,&an8File->vTextures[i].FileName);
		}
		else
		{
			newTexture->IsCubeMap = true;
			newTexture->Name = an8File->vTextures[i].Name;
			AN8XFileNameFromPath(&newTexture->CubeMap[0],&an8File->vTextures[i].CubeMap[0]);
			AN8XFileNameFromPath(&newTexture->CubeMap[1],&an8File->vTextures[i].CubeMap[1]);
			AN8XFileNameFromPath(&newTexture->CubeMap[2],&an8File->vTextures[i].CubeMap[2]);
			AN8XFileNameFromPath(&newTexture->CubeMap[3],&an8File->vTextures[i].CubeMap[3]);
			AN8XFileNameFromPath(&newTexture->CubeMap[4],&an8File->vTextures[i].CubeMap[4]);
			AN8XFileNameFromPath(&newTexture->CubeMap[5],&an8File->vTextures[i].CubeMap[5]);
		}
		g_TextureManager.AddTexture(newTexture);
	}

	// Objects
	i_pLevel->NbrObject = an8File->NbrObject;
	i_pLevel->Objects = new GLAN8Object[i_pLevel->NbrObject];
	for( int i = 0 ; i < i_pLevel->NbrObject ; ++i )
	{
		LoadObject(&i_pLevel->Objects[i],&an8File->vObjects[i]);
	}

	for( int i = 0 ; i < an8File->NbrScene ; ++i )
	{
		if( LoadScene(i_pLevel,&i_pLevel->Scenes[i],&an8File->vScenes[i]) == 3 )
		{
			delete an8File; 
			*o_Error = "Can't load scene\n";
			return false;
		}
	}
	
	delete an8File;

	if( GLAN8FileLoader::LoadDefaultShader(o_Error) == false )
	{
		return false;
	}
	i_pLevel->SetShaderProgramObject(&GLAN8FileLoader::DefaultShaderProgramObject); // Give the default shader pointer

	if( i_pLevel->Build() == false )
	{
		*o_Error = "Can't build scene.\n";
		return false;
	}

	


	return true; // OK
}

int GLAN8FileLoader::LoadScene( GLAN8File* i_pLevel, GLAN8Scene* i_pScene, An8Scene* i_pAn8Scene)
{
	if( i_pAn8Scene->NbrCamera < 1 ) // File not supported by the viewer
	{
		return 3;
	}


	// We will display static scene. So we can use absolute position computed by LibAn8 using ComputeAllMatrices()
	// We get pointers of all elements
	std::vector<An8ObjectElement*> vObjects = i_pAn8Scene->GetObjectElements();
	i_pScene->NbrObjectElement = (int)vObjects.size();
	if( i_pScene->NbrObjectElement > 0 )
	{
		i_pScene->ObjectElements = new GLAN8ObjectElement[i_pScene->NbrObjectElement];
		for( int i = 0 ; i < i_pScene->NbrObjectElement ; ++i )
		{
			LoadObjectElement(i_pLevel,&i_pScene->ObjectElements[i],vObjects[i]);
		}
	}

	std::vector<An8Camera*> vCameras = i_pAn8Scene->GetCameras();
	i_pScene->NbrCamera = (int)vCameras.size();
	if( i_pScene->NbrCamera > 0 )
	{
		i_pScene->Cameras = new int[i_pScene->NbrCamera];
		for( int i = 0 ; i < i_pScene->NbrCamera ; ++i )
		{
			LoadCamera(i_pScene,vCameras[i]);
		}
	}

	std::vector<An8Light*> vLights = i_pAn8Scene->GetLights();
	i_pScene->NbrLight = (int)vLights.size();
	if( i_pScene->NbrLight > 0 )
	{
		i_pScene->Lights = new GLAN8Light[i_pScene->NbrLight];
		for( int i = 0 ; i < i_pScene->NbrLight ; ++i )
		{
			LoadLight(i,&i_pScene->Lights[i],vLights[i]);
		}
	}

	i_pScene->BackGroundType = i_pAn8Scene->BackGroundType;
	if( i_pScene->BackGroundType == 1 )
	{
		i_pScene->BackGroundImage = new GLAN8BackGround();
		//i_pScene->BackGroundImage->TextureFileNames[0] = i_pAn8Scene->BackGroundImage;
		AN8XFileNameFromPath(&i_pScene->BackGroundImage->TextureFileNames[0],&i_pAn8Scene->BackGroundImage);
	}

	// Environement
	i_pScene->BackGround[0] = i_pAn8Scene->BackGround[0]/255.0f;
	i_pScene->BackGround[1] = i_pAn8Scene->BackGround[1]/255.0f;
	i_pScene->BackGround[2] = i_pAn8Scene->BackGround[2]/255.0f;


	i_pScene->HasGroundGrid = i_pAn8Scene->GroundGrid;
	
	if( i_pAn8Scene->HasFog == true )
	{
		i_pScene->HasFog = true;

		i_pScene->FogColor.x = i_pAn8Scene->FogColor[0]/255.0f;
		i_pScene->FogColor.y = i_pAn8Scene->FogColor[1]/255.0f;
		i_pScene->FogColor.z = i_pAn8Scene->FogColor[2]/255.0f;
		i_pScene->FogColor.w = 1.0f;

		i_pScene->FogPercent = ((float)i_pAn8Scene->FogPercent)/100.0f;
		i_pScene->FogStart   = i_pAn8Scene->FogStart;
		i_pScene->FogEnd     = i_pAn8Scene->FogEnd;

	}
	return 0; // OK
}

//--------------------------------------------------------------------------------------
// Load object
//--------------------------------------------------------------------------------------
void GLAN8FileLoader::LoadObject( GLAN8Object* i_pObject, An8Object* i_pAn8Object)
{
	i_pObject->NbrMaterial = i_pAn8Object->NbrMaterial;
	i_pObject->Materials = new GLAN8Material[i_pObject->NbrMaterial];
	for( int i = 0 ; i < i_pObject->NbrMaterial ; ++i )
	{
		LoadMaterial(&i_pObject->Materials[i],&i_pAn8Object->vMaterials[i]);
	}

	// Cube, sphere, cylinder will be converted to meshes
	i_pObject->NbrMesh = i_pAn8Object->NbrMesh + i_pAn8Object->NbrCube + i_pAn8Object->NbrSphere + i_pAn8Object->NbrCylinder;
	if( i_pObject->NbrMesh > 0 )
		i_pObject->Meshes = new GLAN8Mesh[i_pObject->NbrMesh];

	// Meshes
	for( int i = 0 ; i < i_pAn8Object->NbrMesh ; ++i )
	{
		LoadMesh(&i_pObject->Meshes[i],&i_pAn8Object->vMeshes[i]);
		int matNumber = i_pAn8Object->vMeshes[i].vMatNumber[i_pAn8Object->vMeshes[i].vFaces[0].Material];
		i_pObject->Meshes[i].Material = &i_pObject->Materials[matNumber];
	}

	// If you don't want to use ConvertAllToMesh():
	// Cubes
	
	for( int i = 0 ; i < i_pAn8Object->NbrCube ; ++i )
	{
		An8Mesh* cubeMesh = NULL;
		cubeMesh = AN8XMeshFromCube( cubeMesh, &i_pAn8Object->vCubes[i], AN8_NORMAL );
		if( cubeMesh != NULL )
		{
			cubeMesh->Triangulize();
			cubeMesh->ComputePosition(); // Transform vertices from local space to object space
			LoadMesh(&i_pObject->Meshes[ i_pAn8Object->NbrMesh + i ],cubeMesh);
			int matNumber = cubeMesh->vMatNumber[cubeMesh->vFaces[0].Material];
			i_pObject->Meshes[ i_pAn8Object->NbrMesh + i ].Material = &i_pObject->Materials[matNumber];
			delete cubeMesh;
		}
	}

	// Spheres
	for( int i = 0 ; i < i_pAn8Object->NbrSphere ; ++i )
	{
		An8Mesh* sphereMesh = NULL;
		sphereMesh = AN8XMeshFromSphere( sphereMesh, &i_pAn8Object->vSpheres[i], AN8_NORMAL );
		if( sphereMesh != NULL )
		{
			sphereMesh->Triangulize();
			sphereMesh->ComputePosition(); // Transform vertices from local space to object space
			LoadMesh(&i_pObject->Meshes[ i_pAn8Object->NbrMesh + i_pAn8Object->NbrCube + i ],sphereMesh);
			int matNumber = sphereMesh->vMatNumber[sphereMesh->vFaces[0].Material];
			i_pObject->Meshes[ i_pAn8Object->NbrMesh + i_pAn8Object->NbrCube + i ].Material = &i_pObject->Materials[matNumber];
			delete sphereMesh;
		}
	}

	// Cylinder
	for( int i = 0 ; i < i_pAn8Object->NbrCylinder ; ++i )
	{
		An8Mesh* cylinderMesh = NULL;
		cylinderMesh = AN8XMeshFromCylinder( cylinderMesh, &i_pAn8Object->vCylinders[i], AN8_NORMAL );
		if( cylinderMesh != NULL )
		{
			cylinderMesh->Triangulize();
			cylinderMesh->ComputePosition(); // Transform vertices from local space to object space
			LoadMesh(&i_pObject->Meshes[ i_pAn8Object->NbrMesh + i_pAn8Object->NbrCube + i_pAn8Object->NbrSphere + i ],cylinderMesh);
			int matNumber = cylinderMesh->vMatNumber[cylinderMesh->vFaces[0].Material];
			i_pObject->Meshes[ i_pAn8Object->NbrMesh + i_pAn8Object->NbrCube + i_pAn8Object->NbrSphere + i ].Material = &i_pObject->Materials[matNumber];
			delete cylinderMesh;
		}
	}
	
}

//--------------------------------------------------------------------------------------
// Load material
//--------------------------------------------------------------------------------------
void GLAN8FileLoader::LoadMaterial( GLAN8Material* i_pMaterial, An8Material* i_pAn8Material)
{
	// We ignore back face material in this viewer
	i_pMaterial->Name = i_pAn8Material->Name;
	i_pMaterial->DiffuseRGB.r = ((float)i_pAn8Material->DiffuseRGB[0])/255.0f;
	i_pMaterial->DiffuseRGB.g = ((float)i_pAn8Material->DiffuseRGB[1])/255.0f;
	i_pMaterial->DiffuseRGB.b = ((float)i_pAn8Material->DiffuseRGB[2])/255.0f;
	i_pMaterial->DiffuseFactor = i_pAn8Material->DiffuseFactor;
	i_pMaterial->DiffuseIsTexture = i_pAn8Material->DiffuseIsTexture;
	i_pMaterial->DiffuseAlphaMode = i_pAn8Material->DiffuseAlphaMode;
	i_pMaterial->DiffuseBlendMode = i_pAn8Material->DiffuseBlendMode;
	i_pMaterial->DiffuseTextureID = i_pAn8Material->DiffuseTextureID;
	i_pMaterial->DiffuseTextureName = i_pAn8Material->DiffuseTexName;

	i_pMaterial->SpecularRGB.r = ((float)i_pAn8Material->SpecularRGB[0])/255.0f;
	i_pMaterial->SpecularRGB.g = ((float)i_pAn8Material->SpecularRGB[1])/255.0f;
	i_pMaterial->SpecularRGB.b = ((float)i_pAn8Material->SpecularRGB[2])/255.0f;
	i_pMaterial->SpecularFactor = i_pAn8Material->SpecularFactor;
	i_pMaterial->SpecularIsTexture = i_pAn8Material->SpecularIsTexture;
	i_pMaterial->SpecularAlphaMode = i_pAn8Material->SpecularAlphaMode;
	i_pMaterial->SpecularBlendMode = i_pAn8Material->SpecularBlendMode;
	i_pMaterial->SpecularTextureID = i_pAn8Material->SpecularTextureID;

	i_pMaterial->HasBumpMap = i_pAn8Material->HasBumpMap;
	i_pMaterial->BumpMapAlphaMode = i_pAn8Material->BumpMapAlphaMode;
	i_pMaterial->BumpMapBlendMode = i_pAn8Material->BumpMapBlendMode;
	i_pMaterial->BumpMapPercent   = i_pAn8Material->BumpMapPercent;
	i_pMaterial->BumpTextureID = i_pAn8Material->BumpMapTextureID;

	i_pMaterial->HasEnvironment       = i_pAn8Material->HasEnvironment;
	i_pMaterial->EnvironmentAlphaMode = i_pAn8Material->EnvironmentAlphaMode;
	i_pMaterial->EnvironmentBlendMode = i_pAn8Material->EnvironmentBlendMode;
	i_pMaterial->EnvironmentPercent   = i_pAn8Material->EnvironmentPercent;
	i_pMaterial->EnvironmentTextureID = i_pAn8Material->EnvironmentTextureID;

	i_pMaterial->UseAlpha = i_pAn8Material->UseAlpha;
	i_pMaterial->Alpha = i_pAn8Material->Alpha;


}

//--------------------------------------------------------------------------------------
// Build Vertices array / Indices array
// Warning: This function would be very bad for a game because all vertices are 
//          duplicated for each faces in order to have one vertex normal for each faces.
//          Index buffer are not very usefull with this method.
//          It would be better to not duplicate vertices for each faces and to compute 
//          one normal per vertex. And to duplicate vertices only when the normal for
//          different faces are different.
//          ComputeSmoothNormals() compute 1 normal per vertex and avoid duplication of 
//          the same vertex for each face. But the current version of ComputeSmoothNormals()
//          do no create vertex if the angles between faces are great. The result is "too" smooth (wrong)
//--------------------------------------------------------------------------------------
void GLAN8FileLoader::LoadMesh( GLAN8Mesh* i_pMesh, An8Mesh* i_pAn8Mesh)
{
	i_pAn8Mesh->ComputeBoundingElements();

	i_pMesh->BoundingSphereCenter.x = i_pAn8Mesh->BoundingSphereCenter.x;
	i_pMesh->BoundingSphereCenter.y = i_pAn8Mesh->BoundingSphereCenter.y;
	i_pMesh->BoundingSphereCenter.z = i_pAn8Mesh->BoundingSphereCenter.z;
	i_pMesh->BoundingSphereRadius = i_pAn8Mesh->BoundingSphereRadius;

	i_pMesh->NbrFace	= i_pAn8Mesh->NbrFace;
	i_pMesh->NbrVertex = i_pAn8Mesh->NbrFace * 3; // because we don't use ComputeSmoothNormals()
	i_pMesh->NbrIndex  = i_pAn8Mesh->NbrFace * 3;

	i_pMesh->Vertices      = new float[i_pMesh->NbrVertex*3];
	i_pMesh->Normals       = new float[i_pMesh->NbrVertex*3];
	i_pMesh->TextureCoords = new float[i_pMesh->NbrVertex*2];
	i_pMesh->Indices = new int[i_pMesh->NbrIndex];

	
	if( i_pAn8Mesh->NbrNormal == 0 ) // The file don't have normal exported
	{
		// Face normal are not great but fast to compute
		i_pAn8Mesh->ComputeFaceNormal();
	}
	
	/* If you want to use just one normal per vertex, you can use ComputeSmoothNormals()
	Which must be called after ComputeFaceNormal()
	i_pAn8Mesh->ComputeFaceNormal();
	i_pAn8Mesh->ComputeSmoothNormals();
	*/

	int i9 = 0;
	for( int i = 0 ; i < i_pAn8Mesh->NbrFace ; ++i )
	{
		i9 = i*9;
		i_pMesh->Vertices[i9]   = i_pAn8Mesh->vPoints[i_pAn8Mesh->vFaces[i].vIndices[0].Point].x;
		i_pMesh->Vertices[i9+1] = i_pAn8Mesh->vPoints[i_pAn8Mesh->vFaces[i].vIndices[0].Point].y;
		i_pMesh->Vertices[i9+2] = i_pAn8Mesh->vPoints[i_pAn8Mesh->vFaces[i].vIndices[0].Point].z;

		i_pMesh->Vertices[i9+3] = i_pAn8Mesh->vPoints[i_pAn8Mesh->vFaces[i].vIndices[1].Point].x;
		i_pMesh->Vertices[i9+4] = i_pAn8Mesh->vPoints[i_pAn8Mesh->vFaces[i].vIndices[1].Point].y;
		i_pMesh->Vertices[i9+5] = i_pAn8Mesh->vPoints[i_pAn8Mesh->vFaces[i].vIndices[1].Point].z;

		i_pMesh->Vertices[i9+6] = i_pAn8Mesh->vPoints[i_pAn8Mesh->vFaces[i].vIndices[2].Point].x;
		i_pMesh->Vertices[i9+7] = i_pAn8Mesh->vPoints[i_pAn8Mesh->vFaces[i].vIndices[2].Point].y;
		i_pMesh->Vertices[i9+8] = i_pAn8Mesh->vPoints[i_pAn8Mesh->vFaces[i].vIndices[2].Point].z;

		if( i_pAn8Mesh->NbrNormal > 0 )
		{
			i_pMesh->Normals[i9]   = i_pAn8Mesh->vNormals[i_pAn8Mesh->vFaces[i].vIndices[0].Normal].x;
			i_pMesh->Normals[i9+1] = i_pAn8Mesh->vNormals[i_pAn8Mesh->vFaces[i].vIndices[0].Normal].y;
			i_pMesh->Normals[i9+2] = i_pAn8Mesh->vNormals[i_pAn8Mesh->vFaces[i].vIndices[0].Normal].z;

			i_pMesh->Normals[i9+3] = i_pAn8Mesh->vNormals[i_pAn8Mesh->vFaces[i].vIndices[1].Normal].x;
			i_pMesh->Normals[i9+4] = i_pAn8Mesh->vNormals[i_pAn8Mesh->vFaces[i].vIndices[1].Normal].y;
			i_pMesh->Normals[i9+5] = i_pAn8Mesh->vNormals[i_pAn8Mesh->vFaces[i].vIndices[1].Normal].z;

			i_pMesh->Normals[i9+6] = i_pAn8Mesh->vNormals[i_pAn8Mesh->vFaces[i].vIndices[2].Normal].x;
			i_pMesh->Normals[i9+7] = i_pAn8Mesh->vNormals[i_pAn8Mesh->vFaces[i].vIndices[2].Normal].y;
			i_pMesh->Normals[i9+8] = i_pAn8Mesh->vNormals[i_pAn8Mesh->vFaces[i].vIndices[2].Normal].z;
		}
		else // Normal computed by ComputeFaceNormal() are stored in faces
		{
			i_pMesh->Normals[i9]   = i_pAn8Mesh->vFaces[i].Normal.x;
			i_pMesh->Normals[i9+1] = i_pAn8Mesh->vFaces[i].Normal.y;
			i_pMesh->Normals[i9+2] = i_pAn8Mesh->vFaces[i].Normal.z;

			i_pMesh->Normals[i9+3] = i_pAn8Mesh->vFaces[i].Normal.x;
			i_pMesh->Normals[i9+4] = i_pAn8Mesh->vFaces[i].Normal.y;
			i_pMesh->Normals[i9+5] = i_pAn8Mesh->vFaces[i].Normal.z;

			i_pMesh->Normals[i9+6] = i_pAn8Mesh->vFaces[i].Normal.x;
			i_pMesh->Normals[i9+7] = i_pAn8Mesh->vFaces[i].Normal.y;
			i_pMesh->Normals[i9+8] = i_pAn8Mesh->vFaces[i].Normal.z;
		}
		
		/* If you use ComputeSmoothNormals(), vSmoothNormals and vPoints use the same indices
		{
			i_pMesh->Normals[i*9]   = i_pAn8Mesh->vSmoothNormals[i_pAn8Mesh->vFaces[i].vIndices[0].Point].x;
			i_pMesh->Normals[i*9+1] = i_pAn8Mesh->vSmoothNormals[i_pAn8Mesh->vFaces[i].vIndices[0].Point].y;
			i_pMesh->Normals[i*9+2] = i_pAn8Mesh->vSmoothNormals[i_pAn8Mesh->vFaces[i].vIndices[0].Point].z;

			i_pMesh->Normals[i*9+3] = i_pAn8Mesh->vSmoothNormals[i_pAn8Mesh->vFaces[i].vIndices[1].Point].x;
			i_pMesh->Normals[i*9+4] = i_pAn8Mesh->vSmoothNormals[i_pAn8Mesh->vFaces[i].vIndices[1].Point].y;
			i_pMesh->Normals[i*9+5] = i_pAn8Mesh->vSmoothNormals[i_pAn8Mesh->vFaces[i].vIndices[1].Point].z;

			i_pMesh->Normals[i*9+6] = i_pAn8Mesh->vSmoothNormals[i_pAn8Mesh->vFaces[i].vIndices[2].Point].x;
			i_pMesh->Normals[i*9+7] = i_pAn8Mesh->vSmoothNormals[i_pAn8Mesh->vFaces[i].vIndices[2].Point].y;
			i_pMesh->Normals[i*9+8] = i_pAn8Mesh->vSmoothNormals[i_pAn8Mesh->vFaces[i].vIndices[2].Point].z;
		}
		*/

		if( i_pAn8Mesh->NbrUVCoord > 0 )
		{
			i_pMesh->TextureCoords[i*6]   = i_pAn8Mesh->vTexCoords[i_pAn8Mesh->vFaces[i].vIndices[0].TexCoord].x;
			i_pMesh->TextureCoords[i*6+1] = i_pAn8Mesh->vTexCoords[i_pAn8Mesh->vFaces[i].vIndices[0].TexCoord].y;
			
			i_pMesh->TextureCoords[i*6+2] = i_pAn8Mesh->vTexCoords[i_pAn8Mesh->vFaces[i].vIndices[1].TexCoord].x;
			i_pMesh->TextureCoords[i*6+3] = i_pAn8Mesh->vTexCoords[i_pAn8Mesh->vFaces[i].vIndices[1].TexCoord].y;
			
			i_pMesh->TextureCoords[i*6+4] = i_pAn8Mesh->vTexCoords[i_pAn8Mesh->vFaces[i].vIndices[2].TexCoord].x;
			i_pMesh->TextureCoords[i*6+5] = i_pAn8Mesh->vTexCoords[i_pAn8Mesh->vFaces[i].vIndices[2].TexCoord].y;
		}

		i_pMesh->Indices[i*3]   = i*3;
		i_pMesh->Indices[i*3+1] = i*3+1;
		i_pMesh->Indices[i*3+2] = i*3+2;
	}



}

//--------------------------------------------------------------------------------------
// Load ObjectElement : Instance of Object in the scene
//--------------------------------------------------------------------------------------
void GLAN8FileLoader::LoadObjectElement( GLAN8File* i_pLevel, GLAN8ObjectElement* i_pObjectElement, An8ObjectElement* i_pAn8ObjectElement)
{
	float* dest = &i_pObjectElement->Matrix._11;
	float* src  = &i_pAn8ObjectElement->Matrix._11;
	for( int i = 0; i < 16 ; ++i )
	{
		dest[i] = src[i];
	}

	i_pObjectElement->ObjectID = i_pAn8ObjectElement->ObjectID; // Id of object used in the list of objects
	i_pObjectElement->Object = &i_pLevel->Objects[i_pObjectElement->ObjectID]; // get pointer
}

//--------------------------------------------------------------------------------------
// Load camera
//--------------------------------------------------------------------------------------
void GLAN8FileLoader::LoadCamera( GLAN8Scene* i_pScene, An8Camera* i_pAn8Camera)
{
	Camera* newCamera = new Camera();
	newCamera->Loc.x = i_pAn8Camera->LookAtLoc.x;
	newCamera->Loc.y = i_pAn8Camera->LookAtLoc.y;
	newCamera->Loc.z = i_pAn8Camera->LookAtLoc.z;

	newCamera->Eye.x = i_pAn8Camera->LookAtEye.x;
	newCamera->Eye.y = i_pAn8Camera->LookAtEye.y;
	newCamera->Eye.z = i_pAn8Camera->LookAtEye.z;

	newCamera->Up.x  = i_pAn8Camera->LookAtUp.x;
	newCamera->Up.y  = i_pAn8Camera->LookAtUp.y;
	newCamera->Up.z  = i_pAn8Camera->LookAtUp.z;

	newCamera->FOV = i_pAn8Camera->Fov;
	i_pScene->Cameras[0] = g_CameraManager.Add(newCamera);
}

//--------------------------------------------------------------------------------------
// Load lights
//--------------------------------------------------------------------------------------
void GLAN8FileLoader::LoadLight( int i_ID, GLAN8Light* i_pLight, An8Light* i_pAn8Light)
{
	i_pLight->Loc.x = i_pAn8Light->LookAtLoc.x;
	i_pLight->Loc.y = i_pAn8Light->LookAtLoc.y;
	i_pLight->Loc.z = i_pAn8Light->LookAtLoc.z;
	i_pLight->Loc.w = 1.0f;

	i_pLight->Eye.x = i_pAn8Light->LookAtEye.x;
	i_pLight->Eye.y = i_pAn8Light->LookAtEye.y;
	i_pLight->Eye.z = i_pAn8Light->LookAtEye.z;
	i_pLight->Eye.w = 0.0f;

	i_pLight->Kind = i_pAn8Light->Kind; // Local, Infinit, spot ?
	i_pLight->ID = GL_LIGHT0 + i_ID;
	i_pLight->InAngle  = cosf(GLGXToRadian( i_pAn8Light->InAngle  ) ); // Spot
	i_pLight->OutAngle = cosf(GLGXToRadian( i_pAn8Light->OutAngle ) );

}
