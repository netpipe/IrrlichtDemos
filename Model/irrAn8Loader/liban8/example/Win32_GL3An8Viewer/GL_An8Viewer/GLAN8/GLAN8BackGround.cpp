#include"GLAN8BackGround.h"


GLAN8BackGround::GLAN8BackGround( void )
{
	NbrVertex = 4;
	Vertices      = NULL;
	TextureCoords = NULL;
	Normals       = NULL;

	NbrIndex = 6;
	Indices       = NULL;

	Vertices      = new float[12];
	TextureCoords = new float[8];
	Normals       = new float[12];
	Indices       = new int[6];

	Vertices[0] = 0.0f;Vertices[1] =    0.0f;Vertices[2] = 0.0f;
	Normals[0]  = 0.0f;Normals[1]  =  1.0f;Normals[2]  = 0.0f;
	TextureCoords[0] = 0.0f;TextureCoords[1] = 1.0f;

	Vertices[3] =  640.0f;Vertices[4] =    0.0f;Vertices[5] = 0.0f;
	Normals[3]  = 0.0f;Normals[4]  =  1.0f;Normals[5]  = 0.0f;
	TextureCoords[2] = 1.0f;TextureCoords[3] = 1.0f;

	Vertices[6] = 640.0f;Vertices[7] =    480.0f;Vertices[8] = 0.0f;
	Normals[6]  = 0.0f;Normals[7]  =  1.0f;Normals[8]  = 0.0f;
	TextureCoords[4] = 1.0f;TextureCoords[5] = 0.0f;

	Vertices[9]  = 0.0f;Vertices[10] =    480.0f;Vertices[11] =  0.0f;
	Normals[9]  = 0.0f;Normals[10]  =  1.0f;Normals[11]  = 0.0f;
	TextureCoords[6] = 0.0f;TextureCoords[7] = 0.0f;

	Indices[0] = 0;Indices[1] = 1;Indices[2] = 2;
	Indices[3] = 2;Indices[4] = 3;Indices[5] = 0;

	TextureFileNames[0] = "";
	TextureFileNames[1] = "";
	TextureFileNames[2] = "";
	TextureFileNames[3] = "";
	TextureFileNames[4] = "";
	TextureFileNames[5] = "";

	GLGXMatrixOrthoOffCenter(&MatrixProjection,0, 640, 480, 0, 0, 1 );	
}

GLAN8BackGround::~GLAN8BackGround( void )
{
	if( Vertices != NULL)
	{
		delete [] Vertices;
		Vertices = NULL;
	}
	if( TextureCoords != NULL)
	{
		delete [] TextureCoords;
		TextureCoords = NULL;
	}
	if( Normals != NULL)
	{
		delete [] Normals;
		Normals = NULL;
	}
	if( Indices != NULL)
	{
		delete [] Indices;
		Indices = NULL;
	}

	glDeleteBuffersARB(1, &VertexVBO);
	glDeleteBuffersARB(1, &NormalVBO);
	glDeleteBuffersARB(1, &TexCoordVBO);
	glDeleteBuffersARB(1, &IndexVBO);
}

bool GLAN8BackGround::Build( void )
{

		// Vertices
		glGenBuffersARB( 1, &VertexVBO );
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, VertexVBO );
		glBufferDataARB( GL_ARRAY_BUFFER_ARB, sizeof(float)*3*NbrVertex, Vertices, GL_STATIC_DRAW_ARB );
		int nParam_ArrayObjectSize = 0;
		glGetBufferParameterivARB( GL_ARRAY_BUFFER_ARB, GL_BUFFER_SIZE_ARB, &nParam_ArrayObjectSize );
		if( nParam_ArrayObjectSize <= 0 )
			return GL_FALSE;

		// TextureCoords
		glGenBuffersARB( 1, &TexCoordVBO );
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, TexCoordVBO );
		glBufferDataARB( GL_ARRAY_BUFFER_ARB, sizeof(float)*2*NbrVertex, TextureCoords, GL_STATIC_DRAW_ARB );
		nParam_ArrayObjectSize = 0;
		glGetBufferParameterivARB( GL_ARRAY_BUFFER_ARB, GL_BUFFER_SIZE_ARB, &nParam_ArrayObjectSize );
		if( nParam_ArrayObjectSize <= 0 )
			return GL_FALSE;

		// Normals
		glGenBuffersARB( 1, &NormalVBO );
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, NormalVBO );
		glBufferDataARB( GL_ARRAY_BUFFER_ARB, sizeof(float)*3*NbrVertex, Normals, GL_STATIC_DRAW_ARB/*GL_STREAM_DRAW_ARB*/ );
		nParam_ArrayObjectSize = 0;
		glGetBufferParameterivARB( GL_ARRAY_BUFFER_ARB, GL_BUFFER_SIZE_ARB, &nParam_ArrayObjectSize );
		if( nParam_ArrayObjectSize <= 0 )
			return GL_FALSE;


		nParam_ArrayObjectSize = 0;
		glGenBuffersARB( 1, &IndexVBO );
		glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, IndexVBO );
		glBufferDataARB( GL_ELEMENT_ARRAY_BUFFER_ARB, sizeof(int)*NbrIndex, Indices, GL_STATIC_DRAW_ARB );
		glGetBufferParameterivARB( GL_ELEMENT_ARRAY_BUFFER_ARB, GL_BUFFER_SIZE_ARB, &nParam_ArrayObjectSize );
		if( nParam_ArrayObjectSize <= 0 )
			return GL_FALSE;

		//glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0 );
		//glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, 0 );


	if ( GLGXCreateTextureFromFile((char*)TextureFileNames[0].c_str(),(GLuint*)&Texture) == GL_FALSE)
	{
		if( GLGXCreateEmptyTexture(&Texture,32,32,255,255,255,255) == GL_FALSE)
		{
				return GL_FALSE;
		}
	}
	// Default Settings
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	return GL_TRUE;
}




void GLAN8BackGround::Begin(void)
{
	_EnableBlend     = glIsEnabled(GL_BLEND);
	_EnableDepthTest = glIsEnabled(GL_DEPTH_TEST);
	//_EnableTexture2d = glIsEnabled(GL_TEXTURE_2D); // deprecated
	_EnableCullFace = glIsEnabled(GL_CULL_FACE);
 
	if( _EnableBlend )
		glDisable(GL_BLEND);	
	if( _EnableDepthTest )
		glDisable(GL_DEPTH_TEST);
	//if( !_EnableTexture2d ) // deprecated
	//	glEnable(GL_TEXTURE_2D); // deprecated
	if( _EnableCullFace )
		glDisable(GL_CULL_FACE);

	glDepthMask(GL_FALSE);
}

void GLAN8BackGround::End(void)
{
	if( _EnableDepthTest )
		glEnable(GL_DEPTH_TEST);
	//if( !_EnableTexture2d ) // deprecated
	//	glDisable(GL_TEXTURE_2D); // deprecated
	if( _EnableCullFace )
		glEnable(GL_CULL_FACE);

	glDepthMask(GL_TRUE);
}

void GLAN8BackGround::Render( GLuint* i_ShaderProgramObject )
{
	Begin();

	if( i_ShaderProgramObject == NULL)
		return;

	glUniform1i(glGetUniformLocation(*i_ShaderProgramObject, "g_LightType"), 0);

	GLGXCOLOR   color(1.0f,1.0f,1.0f,1.0f);
	glUniform4fv(glGetUniformLocation(*i_ShaderProgramObject, "g_DiffuseColor"), 1,color);

	//glEnable(GL_TEXTURE_2D); // deprecated
	glBindTexture(GL_TEXTURE_2D,Texture);

	glUniform1i(glGetUniformLocation(*i_ShaderProgramObject, "g_HasDiffuseTexture"), 1);
	glUniform1i(glGetUniformLocation(*i_ShaderProgramObject, "g_DiffuseTexture"), 0);

	glUniformMatrix4fv(glGetUniformLocation(*i_ShaderProgramObject, "g_MatWorld"),1,false,Matrix);
	glUniformMatrix4fv(glGetUniformLocation(*i_ShaderProgramObject, "g_MatView"),1,false,MatrixView);
	glUniformMatrix4fv(glGetUniformLocation(*i_ShaderProgramObject, "g_MatProj"),1,false,MatrixProjection);

	GLuint positionIndex	= glGetAttribLocation(*i_ShaderProgramObject, "g_vboPosition");
	GLuint textureIndex		= glGetAttribLocation(*i_ShaderProgramObject, "g_vboTexture");
	GLuint normalIndex		= glGetAttribLocation(*i_ShaderProgramObject, "g_vboNormal");

	// Vertices
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, VertexVBO);
	glVertexAttribPointer(positionIndex, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(positionIndex);

	// Textures
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, TexCoordVBO);
	glVertexAttribPointer(textureIndex, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(textureIndex);

	// Normals
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, NormalVBO);
	glVertexAttribPointer(normalIndex, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(normalIndex);

	// Indices
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, IndexVBO);

	// Draw
	glDrawElements(GL_TRIANGLES, NbrIndex, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(positionIndex);
	glDisableVertexAttribArray(textureIndex);
	glDisableVertexAttribArray(normalIndex);
	
	End();
}