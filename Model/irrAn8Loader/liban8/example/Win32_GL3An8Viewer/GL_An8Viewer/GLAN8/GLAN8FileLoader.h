#ifndef _GLAN8FileLoader_H
#define _GLAN8FileLoader_H

#include "Liban8/An8Loader.h"

#include "GLAN8Scene.h"
#include "GLAN8Light.h"
#include "GLAN8Mesh.h"
#include "GLAN8Object.h"
#include "GLAN8ObjectElement.h"
#include "GLAN8Texture.h"
#include "GLAN8File.h"
#include "GLAN8BackGround.h"




class GLAN8FileLoader
{
public:
	static bool  LoadSceneFromFile(std::string* o_Error,char* i_FileName, GLAN8File* i_pLevel);
	static bool  LoadObjectFromFile(std::string* o_Error,char* i_FileName, GLAN8Object* i_pObject); // Load object 0 of the file
	//int  BuildLevel(GLAN8File* i_pLevel);

	static GLuint DefaultShaderProgramObject; 
	static bool LoadDefaultShader(std::string* o_Error);
	static void Release(void);

private:
	static int LoadScene( GLAN8File* i_pLevel, GLAN8Scene* i_pScene, An8Scene* i_pAn8Scene);
	static void LoadObject( GLAN8Object* i_pObject, An8Object* i_pAn8Object);
	static void LoadMesh( GLAN8Mesh* i_pMesh, An8Mesh* i_pAn8Mesh);
	static void LoadObjectElement( GLAN8File* i_pLevel, GLAN8ObjectElement* i_pObjectElement, An8ObjectElement* i_pAn8ObjectElement);
	//void LoadCamera( GLAN8Camera* i_pCamera, An8Camera* i_pAn8Camera);
	static void LoadCamera( GLAN8Scene* i_pScene, An8Camera* i_pAn8Camera);
	static void LoadLight( int i_ID, GLAN8Light* i_pLight, An8Light* i_pAn8Light);
	static void LoadMaterial( GLAN8Material* i_pMaterial, An8Material* i_pAn8Material);

public:
	GLAN8FileLoader(void);
	~GLAN8FileLoader(void);
};

#endif	
