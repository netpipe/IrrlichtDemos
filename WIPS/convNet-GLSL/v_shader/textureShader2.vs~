#version 130
attribute vec3 vertex;
attribute vec3 normal;
attribute vec3 tangent;
attribute vec3 color;
attribute vec2 UV;

varying vec2 outUV;
varying vec2 topTextureCoordinate; 
varying vec2 bottomTextureCoordinate;

varying vec2 secTopTextureCoordinate; 
varying vec2 secBottomTextureCoordinate;

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

	topTextureCoordinate = vec2(UV.x, UV.y + firstOffset.y);
	bottomTextureCoordinate = vec2(UV.x, UV.y - firstOffset.y);

	secTopTextureCoordinate = vec2(UV.x, UV.y + secondOffset.y);
	secBottomTextureCoordinate = vec2(UV.x, UV.y - secondOffset.y);
}
