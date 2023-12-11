
// Author: Greg Santucci, 2008 and 2009
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#ifndef GX_TEXTUREARCHIVE_H
#define GX_TEXTUREARCHIVE_H

#pragma warning(disable:4786)

#include <string>
#include <map>

#include <GL/glut.h>

class gxTextureArchive
{
public:

	static void Startup();
	static void Shutdown();

	static gxTextureArchive & GetInstance();

	void LoadTexture(const std::string & filename, const std::string & textureName);

	void MakeBlankTexture(int nSize, const std::string & textureName, bool bPixellated = false);

	int GetTextureID(const std::string textureName);

private:

	gxTextureArchive();
	virtual ~gxTextureArchive();

	static gxTextureArchive * s_instance;

	typedef std::map<std::string, GLuint> TextureNameIDMap;

	TextureNameIDMap m_archive;
};

#endif // GX_TEXTUREARCHIVE_H
