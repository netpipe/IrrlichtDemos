#include"TextureManager.h"



Texture::Texture(GLvoid)
{
	IsCubeMap = false;
	GLID = 0;
}
Texture::~Texture(GLvoid)
{
	if( GLID != 0 ) // 0 is never used by glGenTextures
	{
		glDeleteTextures(1, &GLID);
	}
}


GLuint TextureManager::AddTexture(Texture* i_Texture)
{
	bool alreadyPrensent = false;
	for (std::vector<Texture*>::iterator i = m_vTextures.begin(); i != m_vTextures.end(); ++i) 
	{
		if( (*i)->FileName.compare(i_Texture->FileName) == 0 )
		{
			alreadyPrensent = true;
			return (*i)->GLID;
			break;
		}
	}

	m_vTextures.push_back(i_Texture);
	Build(i_Texture);

	return i_Texture->GLID;
}

GLuint TextureManager::GetGLID(std::string* i_TextureName)
{
	for (std::vector<Texture*>::iterator i = m_vTextures.begin(); i != m_vTextures.end(); ++i) 
	{
		if( (*i)->Name.compare(*i_TextureName) == 0 )
		{
			return (*i)->GLID;
		}
	}
	return 0;
}


bool TextureManager::Build(Texture* i_Texture)
{
	if( i_Texture->IsCubeMap == false )
	{
		if ( GLGXCreateTextureFromFile((char*)i_Texture->FileName.c_str(),(GLuint*)&i_Texture->GLID) == GL_FALSE)
		{
			if( GLGXCreateEmptyTexture(&i_Texture->GLID,32,32,255,255,255,255) == GL_FALSE)
			{
				return GL_FALSE;
			}
		}
		// Default Settings
		//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
		//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		// ToDo
		if ( GLGXCreateCubeTextureFromFile(
			(char*)i_Texture->CubeMap[0].c_str(), // Right
			(char*)i_Texture->CubeMap[1].c_str(), // Left
			(char*)i_Texture->CubeMap[2].c_str(), // Up
			(char*)i_Texture->CubeMap[3].c_str(), // Down
			(char*)i_Texture->CubeMap[4].c_str(), // Front
			(char*)i_Texture->CubeMap[5].c_str(), // Back
			(GLuint*)&i_Texture->GLID
		) == GL_FALSE)
		{
			return GL_FALSE;
		}
	}

	return GL_TRUE;
}

void TextureManager::Release(Texture* i_Texture)
{
	if( i_Texture->GLID != 0 ) // 0 is never used by glGenTextures
	{
		glDeleteTextures(1, &i_Texture->GLID);
	}
}

void TextureManager::Release(void)
{
	for (std::vector<Texture*>::iterator i = m_vTextures.begin(); i != m_vTextures.end(); ++i) 
	{
		delete (*i);
		(*i) = NULL;
	}
	m_vTextures.clear();
}
