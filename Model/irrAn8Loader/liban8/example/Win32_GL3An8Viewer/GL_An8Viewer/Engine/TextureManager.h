#ifndef _AbyTextureManager_H
#define _AbyTextureManager_H

#include"../GLAN8/GLGX/GLGX.h"
#include<string>
#include<vector>

class Texture
{
public:

	std::string Name;

	bool IsCubeMap;

	std::string FileName;
	std::string CubeMap[6];

	GLuint GLID;

	Texture(GLvoid);
	~Texture(GLvoid);
};

class TextureManager
{
private:
	std::vector<Texture*> m_vTextures;
	bool Build(Texture* i_Texture);
	void Release(Texture* i_Texture);
public:
	GLuint AddTexture(Texture* i_Texture);
	GLuint GetGLID(std::string* i_TextureName);
	void Release(void);


};

#endif	