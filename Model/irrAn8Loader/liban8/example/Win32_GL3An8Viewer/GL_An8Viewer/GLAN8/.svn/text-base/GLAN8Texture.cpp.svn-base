#include "GLAN8Texture.h"
//#include "GLGXTexture.h"

/*

GLAN8Texture::GLAN8Texture(void)
{
	Texture = 0; // 0 is never used by glGenTextures
	IsCubeMap = false;
}

GLAN8Texture::~GLAN8Texture(void)
{
	Release();
}

GLboolean GLAN8Texture::Build(void)
{

	if( this->IsCubeMap == false )
	{
		if ( GLGXCreateTextureFromFile((char*)FileName.c_str(),(GLuint*)&Texture) == GL_FALSE)
		{
			if( GLGXCreateEmptyTexture(&Texture,32,32,255,255,255,255) == GL_FALSE)
			{
				return GL_FALSE;
			}
		}
		// Default Settings
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		// ToDo
		if ( GLGXCreateCubeTextureFromFile(
			(char*)CubeMap[0].c_str(), // Right
			(char*)CubeMap[1].c_str(), // Left
			(char*)CubeMap[2].c_str(), // Up
			(char*)CubeMap[3].c_str(), // Down
			(char*)CubeMap[4].c_str(), // Front
			(char*)CubeMap[5].c_str(), // Back
			(GLuint*)&Texture
		) == GL_FALSE)
		{
			return GL_FALSE;
		}
	}

	return GL_TRUE;
}

void GLAN8Texture::Release(void)
{
	if( Texture != 0 ) // 0 is never used by glGenTextures
	{
		glDeleteTextures(1, &Texture);
	}
}

*/