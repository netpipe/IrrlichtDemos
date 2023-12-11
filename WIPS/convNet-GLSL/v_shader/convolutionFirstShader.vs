#version 130

attribute vec3 vertex;
attribute vec3 normal;
attribute vec3 tangent;
attribute vec3 color;
attribute vec2 UV;

varying float leftTex, rightTex, topTex, bottomTex, centerXTex, centerYTex;


uniform mat4 modelViewProjectionMatrix;
uniform float TEX_WIDTH;
uniform float TEX_HEIGHT;
uniform float SCREEN_WIDTH;
uniform float SCREEN_HEIGHT;

void main()
{
	gl_Position=modelViewProjectionMatrix*vec4(vertex,1.0);
	
	leftTex = (SCREEN_WIDTH *  UV.x - 1.0) / TEX_WIDTH;
	bottomTex = (SCREEN_HEIGHT *  UV.y - 1.0) / TEX_HEIGHT;
	rightTex = (SCREEN_WIDTH *  UV.x + 1.0) / TEX_WIDTH;
	topTex = (SCREEN_HEIGHT *  UV.y + 1.0) / TEX_HEIGHT;
	centerXTex = (SCREEN_WIDTH *  UV.x) / TEX_WIDTH;
	centerYTex = (SCREEN_HEIGHT *  UV.y) / TEX_HEIGHT;
}
