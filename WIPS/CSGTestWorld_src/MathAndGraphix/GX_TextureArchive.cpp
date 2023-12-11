
// Author: Greg Santucci, 2008 and 2009
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#include "GX_TextureArchive.h"

#include "GX_TextureLoader.h"

gxTextureArchive * gxTextureArchive::s_instance = NULL;

void gxTextureArchive::Startup()
{
	//ASSERT(s_instance == NULL);

	if(s_instance == NULL)
		s_instance = new gxTextureArchive;
}

void gxTextureArchive::Shutdown()
{
	//ASSERT(s_instance != NULL);

	delete s_instance;
}

gxTextureArchive & gxTextureArchive::GetInstance()
{
	//ASSERT(s_instance != NULL);

	return *s_instance;
}

gxTextureArchive::gxTextureArchive()
{
}

gxTextureArchive::~gxTextureArchive()
{
	// iterate through the map, freeing each texture.

	for(TextureNameIDMap::iterator i = m_archive.begin(); i != m_archive.end(); i++)
	{
		GLuint nID = (*i).second;

		glDeleteTextures(1, &nID);
	}

	// Its not really necessary to clear the map...
	//m_archive.clear();
}

void gxTextureArchive::LoadTexture(const std::string & filename, const std::string & textureName)
{
	GLuint nTextureID = gxTextureLoader::MakeTexture(filename.c_str());

	if(gxTextureLoader::bErrorOccured == true)
	{
		return;
	}

	GLclampf fPriority = 1.0f;
	glPrioritizeTextures(1, &nTextureID, &fPriority);

	m_archive[textureName] = nTextureID;
}

// We need "MakeBlankTexture"
// ...and here it is

void gxTextureArchive::MakeBlankTexture(int nSize, const std::string & textureName, bool bPixellated)
{
	GLuint nTextureID = gxTextureLoader::MakeBlankTexture(nSize, bPixellated);

	GLclampf fPriority = 1.0f;
	glPrioritizeTextures(1, &nTextureID, &fPriority);

	m_archive[textureName] = nTextureID;
}

int gxTextureArchive::GetTextureID(const std::string textureName)
{
	if(m_archive.find(textureName) != m_archive.end())
	{
		return m_archive[textureName];
	}
	else
	{
		return 0;
	}
}
