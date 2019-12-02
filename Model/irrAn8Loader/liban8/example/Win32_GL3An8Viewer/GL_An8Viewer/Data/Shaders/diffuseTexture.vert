#version 130

uniform mat4 g_MatWorld;
uniform mat4 g_MatView;
uniform mat4 g_MatProj;

uniform int g_HasDiffuseTexture;
uniform int g_LightType;
uniform sampler2D g_DiffuseTexture;
uniform vec4 g_DiffuseColor;

// VBO
in vec3 g_vboPosition; 
in vec2 g_vboTexture; 
in vec3 g_vboNormal; 

out vec4 gvo_position;
out vec2 gvo_TextureCoord;
out vec3 gvo_normal;


void main(void)
{
	mat4 matWorldViewProj =  g_MatProj  * g_MatView * g_MatWorld ;
	
	gvo_position	 = matWorldViewProj * vec4(g_vboPosition,1);
	gl_Position 	 = gvo_position;

	gvo_TextureCoord = g_vboTexture;
	gvo_normal 		 = g_vboNormal;
}    


