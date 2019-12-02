#ifndef _ENGINE_H
#define _ENGINE_H

#include"GLAN8/GLGX/GLGX.h"

#include"GLAN8/GLAN8Scene.h"
#include"GLAN8/GLAN8Texture.h"
#include"GLAN8/GLAN8File.h"


class Engine
{
public:
	GLint WindowWidth;
	GLint WindowHeight;

	GLAN8File* Levels; // Store one .an8 file
	GLuint ShaderProgramObject; 

	GLAN8Object* PlayerModel;


	GLGXMATRIX MatrixLookAt; 
	GLGXFONT Font;

	std::string CurrentFileName;

	bool IsToonRendering;
	void SetToonRendering(bool state);
	bool GetToonRenderingState(void){return IsToonRendering;};

	GLboolean LoadFile( char* fileName );
	void OnFrameRender( double i_fTime, float i_fElapsedTime );
	void OnFrameResize( GLint width , GLint height );


	
	void (*fctDisplayMessage)(char*, char*); // Used to display messages


public:
	Engine( void (*fctDisplayMessage)(char*, char*) );
	~Engine( void );
};

#endif	// _ENGINE_H
