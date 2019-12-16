[[FX]]

<Sampler id="depthBuf">
	<StageConfig addressMode="CLAMP" />
</Sampler>

<Sampler id="buf0">
	<StageConfig addressMode="CLAMP" />
</Sampler>

<Sampler id="buf1">
	<StageConfig addressMode="CLAMP" />
</Sampler>

<Sampler id="buf2">
	<StageConfig addressMode="CLAMP" />
</Sampler>

<Sampler id="ambientMap">
	<StageConfig addressMode="CLAMP" filtering="BILINEAR" maxAnisotropy="1" />
</Sampler>


<Context id="AMBIENT">
	<Shaders vertex="VS_QUAD" fragment="FS_AMBIENT" />
	<RenderConfig writeDepth="false" blendMode="REPLACE" />
</Context>

<Context id="LIGHTING">
	<Shaders vertex="VS_QUAD" fragment="FS_LIGHTING" />
	<RenderConfig writeDepth="false" blendMode="ADD" />
</Context>

<Context id="COPY_DEPTH">
	<Shaders vertex="VS_QUAD" fragment="FS_COPY_DEPTH" />
</Context>


[[VS_QUAD]]

varying vec2 texCoords;

void main( void )
{
	texCoords = gl_MultiTexCoord0.st;
	gl_Position = gl_ProjectionMatrix * gl_Vertex;
}


[[FS_AMBIENT]]

#include "shaders/utilityLib/fragDeferredRead.glsl"

uniform samplerCube ambientMap;
varying vec2 texCoords;

void main( void )
{
	if( getMatID( texCoords ) == 0.0 )	// Background
	{
		gl_FragColor.rgb = vec3( 0, 0, 0 );
	}
	else if( getMatID( texCoords ) == 2.0 )	// Sky
	{
		gl_FragColor.rgb = getAlbedo( texCoords );
	}
	else
	{
		gl_FragColor.rgb = getAlbedo( texCoords ) * textureCube( ambientMap, getNormal( texCoords ) ).rgb;
	}
}


[[FS_LIGHTING]]

#include "shaders/utilityLib/fragLighting.glsl"
#include "shaders/utilityLib/fragDeferredRead.glsl"

varying vec2 texCoords;

void main( void )
{
	if( getMatID( texCoords ) == 1.0 )	// Standard phong material
	{
		float vsPos = (gl_ModelViewMatrix * vec4( getPos( texCoords ), 1.0 )).z;
		
		gl_FragColor.rgb =
			calcPhongSpotLight( getPos( texCoords ), getNormal( texCoords ),
								getAlbedo( texCoords ), getSpecMask( texCoords ), 16.0, -vsPos, 0.3 );
	}
	else discard;
}


[[FS_COPY_DEPTH]]

uniform sampler2D depthBuf;
varying vec2 texCoords;

void main( void )
{
	gl_FragDepth = texture2D( depthBuf, texCoords ).r;
}
