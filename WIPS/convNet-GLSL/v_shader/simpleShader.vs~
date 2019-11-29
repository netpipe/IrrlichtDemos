attribute vec3 vertex;
attribute vec3 normal;
attribute vec3 tangent;
attribute vec3 color;
attribute vec2 UV;

varying vec2 outUV;
varying vec2 leftTextureCoordinate;
varying vec2 rightTextureCoordinate;
varying vec2 topTextureCoordinate; 
varying vec2 bottomTextureCoordinate;

float texelWidthOffset = 0.0;
float texelHeightOffset = 0.0; 
uniform mat4 modelViewProjectionMatrix;

void main()
{
	texelWidthOffset = 1.0/600.0;			//SCREEN_WIDTH
	texelHeightOffset = 1.0/600.0;			//SCREEN_HEIGHT
	gl_Position=modelViewProjectionMatrix*vec4(vertex,1.0);
	outUV = UV;	
	vec2 firstOffset = vec2(1.3846153846 * texelWidthOffset, 1.3846153846 * texelHeightOffset);	


	leftTextureCoordinate = vec2(UV.x - firstOffset.x, UV.y);
	rightTextureCoordinate = vec2(UV.x + firstOffset.x, UV.y);
	topTextureCoordinate = vec2(UV.x, UV.y + firstOffset.y);
	bottomTextureCoordinate = vec2(UV.x, UV.y - firstOffset.y);
}
