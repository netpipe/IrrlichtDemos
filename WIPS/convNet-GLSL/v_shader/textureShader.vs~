#version 130
attribute vec3 vertex;
attribute vec3 normal;
attribute vec3 tangent;
attribute vec3 color;
attribute vec2 UV;

varying vec2 outUV;
varying vec2 leftTextureCoordinate;
varying vec2 rightTextureCoordinate;

varying vec2 secLeftTextureCoordinate;
varying vec2 secRightTextureCoordinate;

float texelWidthOffset = 0.0;
float texelHeightOffset = 0.0; 
uniform mat4 modelViewProjectionMatrix;

void main()
{
	texelWidthOffset = 1.0/600.0;			//SCREEN_WIDTH
	texelHeightOffset = 1.0/600.0;			//SCREEN_HEIGHT

	vec2 firstOffset = vec2(1.3846153846 * texelWidthOffset, 1.3846153846 * texelHeightOffset);	
    	vec2 secondOffset = vec2(3.2307692308 * texelWidthOffset, 3.2307692308 * texelHeightOffset);

	gl_Position = modelViewProjectionMatrix*vec4(vertex,1.0);
	outUV=UV;

	leftTextureCoordinate = vec2(UV.x - firstOffset.x, UV.y);
	rightTextureCoordinate = vec2(UV.x + firstOffset.x, UV.y);

	secLeftTextureCoordinate = vec2(UV.x - secondOffset.x, UV.y);
	secRightTextureCoordinate = vec2(UV.x + secondOffset.x, UV.y);
}
