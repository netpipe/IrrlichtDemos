#version 130
attribute vec3 vertex;
attribute vec2 UV;

varying vec2 outUV;

uniform mat4 modelViewProjectionMatrix;

void main()
{
	gl_Position=modelViewProjectionMatrix*vec4(vertex,1.0);
	outUV = UV;	
}
