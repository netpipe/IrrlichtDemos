#include"GLAN8FileLoader.h"

GLuint GLAN8FileLoader::DefaultShaderProgramObject = 0; 

char glAn8DefaultVertexShader[] = "\n\
#version 130 \n\
 \n\
uniform mat4 g_MatWorld; \n\
uniform mat4 g_MatView; \n\
uniform mat4 g_MatProj; \n\
 \n\
uniform int g_HasDiffuseTexture; \n\
uniform int g_LightType; \n\
uniform vec4   g_LightPosition;  \n\
uniform vec4   g_LightSpotDirection; \n\
uniform float   g_LightSpotCutOff;  \n\
uniform sampler2D g_DiffuseTexture; \n\
uniform vec4 g_DiffuseColor; \n\
 \n\
in vec3 g_vboPosition;  \n\
in vec2 g_vboTexture;  \n\
in vec3 g_vboNormal;  \n\
 \n\
out vec4 gvo_position; \n\
out vec2 gvo_TextureCoord; \n\
out vec3 gvo_normal; \n\
 \n\
void main(void) \n\
{ \n\
	mat4 matWorldViewProj =  g_MatProj  * g_MatView * g_MatWorld ; \n\
 \n\
	gvo_position	 = matWorldViewProj * vec4(g_vboPosition,1); \n\
	gl_Position 	 = gvo_position; \n\
 \n\
	gvo_TextureCoord = g_vboTexture; \n\
	gvo_normal 		 = g_vboNormal; \n\
}     \n\
\n";

char glAn8DefaultPixelShader[] = "\n\
#version 130 \n\
 \n\
in vec4 gvo_position; \n\
in vec2 gvo_TextureCoord; \n\
in vec3 gvo_normal; \n\
 \n\
out vec4 o_Color; \n\
 \n\
uniform int   g_LightType; \n\
uniform vec4   g_LightPosition;  \n\
uniform vec4   g_LightSpotDirection; \n\
uniform float   g_LightSpotCutOff;  \n\
uniform int g_HasDiffuseTexture; \n\
uniform sampler2D g_DiffuseTexture; \n\
uniform vec4 g_DiffuseColor; \n\
 \n\
void main (void)   \n\
{      \n\
	vec4 fragColor = g_DiffuseColor; \n\
 \n\
	if(g_HasDiffuseTexture == 1 ) \n\
	{ \n\
		vec4 color = texture2D(g_DiffuseTexture,gvo_TextureCoord.st); \n\
		fragColor = fragColor * color; \n\
 \n\
	} \n\
	float dotNormalLight = 0.0f; \n\
 \n\
 \n\
	if( g_LightType == 1 ) \n\
	{ \n\
		vec3 lightDir = g_LightPosition.xyz; \n\
		dotNormalLight = max( dot( normalize( gvo_normal ), normalize( -lightDir ) ) , 0.0 );  \n\
	} \n\
	else if( g_LightType == 2 )  \n\
	{ \n\
		vec3 lightDir = gvo_position.xyz - g_LightPosition.xyz; \n\
		dotNormalLight = max( dot( normalize( gvo_normal ), normalize( -lightDir ) ) , 0.0 );  \n\
	} \n\
	else if(  g_LightType == 3 ) // Spot \n\
	{ \n\
		vec3 tempLightDirection = vec3(0.0f,-1.0f,0.0f); \n\
		vec3 tempLightPosition = vec3(48.92f,223.6f,5.9517f); \n\
		//vec3 lightDir = gvo_position.xyz - g_LightPosition.xyz; \n\
		vec3 lightDir = gvo_position.xyz - tempLightPosition; \n\
\n\
		dotNormalLight   = max( dot( normalize( gvo_normal )         , normalize( -lightDir ) ) , 0.0 ); \n\
	//	float spotEffect =      dot( normalize( g_LightSpotDirection.xyz), normalize( lightDir ) ); \n\
\n\
		float spotEffect =      dot( normalize( tempLightDirection), normalize( lightDir ) ); \n\
		if ( spotEffect > 0.95f)//g_LightSpotCutOff)  \n\
		{ \n\
					dotNormalLight = dotNormalLight;  \n\
		} \n\
		else \n\
		{ \n\
			dotNormalLight = 0.0f; \n\
		} \n\
	} \n\
	else  \n\
	{ \n\
		dotNormalLight = 1.0f; \n\
	} \n\
	fragColor = fragColor * dotNormalLight; \n\
	o_Color = fragColor; \n\
}   \n\
\n";


bool GLAN8FileLoader::LoadDefaultShader(std::string* o_Error)
{
	if( DefaultShaderProgramObject == 0 )
	{
		//std::string error;
		if( GLGXCreateEffectFromStrings(&DefaultShaderProgramObject,o_Error,glAn8DefaultVertexShader,glAn8DefaultPixelShader) == false ) // Load shaders
		{
			//fctDisplayMessage("GLSL Shader ERROR",(char*)error.c_str());
			DefaultShaderProgramObject = 0;
			return false;
		}
	}
	return true;
}

void GLAN8FileLoader::Release(void)
{
	if( DefaultShaderProgramObject != 0 )
	{
		glDeleteObjectARB(DefaultShaderProgramObject);
	}
}

