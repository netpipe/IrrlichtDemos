#ifndef MESHLOADER_H
#define MESHLOADER_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "mesh.h"


class meshLoader
{
	private:
	mesh* g_Mesh;
std::vector<vertexData>g_VertexData;
std::vector<unsigned int>g_Index;
std::vector<textureData>g_TexData;
	public:
	meshLoader();
	~meshLoader();
	void draw(unsigned int programId);
	unsigned int loadTexture(const char* filename);
};


#endif
