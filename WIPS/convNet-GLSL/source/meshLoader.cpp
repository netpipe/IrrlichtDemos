#include "../header/meshLoader.h"

extern SDL_Surface* gScreenSurface;
meshLoader::meshLoader()
{

g_VertexData.reserve(4);
vertexData tempVData;
textureData tempTexData;

tempVData.position.change(-1.0,1.0,-1);
tempVData.U = 0;
tempVData.V = 0;
g_VertexData.push_back(tempVData);

tempVData.position.change(-1.0,-1.0,-1);
tempVData.U = 0;
tempVData.V = 1;
g_VertexData.push_back(tempVData);

tempVData.position.change(1.0,-1.0,-1);
tempVData.U = 1;
tempVData.V = 1;
g_VertexData.push_back(tempVData);

tempVData.position.change(1.0,1.0,-1);
tempVData.U = 1;
tempVData.V = 0;
g_VertexData.push_back(tempVData);

//for a triangle ID = 3

g_Index.reserve(4);

g_Index.push_back(0);
g_Index.push_back(1);
g_Index.push_back(2);

g_Index.push_back(0);
g_Index.push_back(2);
g_Index.push_back(3);

tempTexData.id = loadTexture("imL.png");
tempTexData.type = 0;
g_TexData.push_back(tempTexData);
g_Mesh = new mesh(&g_VertexData, &g_Index, &g_TexData);
}


unsigned int meshLoader::loadTexture(const char* filename)
{
	unsigned int num;
	glGenTextures(1,&num);
	std::string file_name = "../models/textures/";
	file_name += filename;
	SDL_Surface* imgs=IMG_Load(file_name.c_str());
	if(imgs==NULL)
	{
		//std::cout << "img was not loaded" << std::endl;
		return -1;
	}
	//SDL_PixelFormat form={NULL,32,4,0,0,0,0,0,0,0,0,0xff000000,0x00ff0000,0x0000ff00,0x000000ff,0,255};
	SDL_Surface* img2=SDL_ConvertSurface( imgs, gScreenSurface->format,0 );
	if(img2==NULL)
	{
		//std::cout << "img2 was not loaded" << std::endl;
		return -1;		
	}
	glBindTexture(GL_TEXTURE_2D,num);		
	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); 
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); 
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,img2->w,img2->h,0,GL_BGRA,GL_UNSIGNED_BYTE,img2->pixels);		
	//glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,img->w,img->h,0,GL_BGRA,GL_UNSIGNED_BYTE,img->pixels);		
	SDL_FreeSurface(imgs);
	SDL_FreeSurface(img2);
	return num;	
}

meshLoader::~meshLoader()
{
	delete g_Mesh;
}

void meshLoader::draw(unsigned int programId)
{
	g_Mesh->draw(programId);
}
