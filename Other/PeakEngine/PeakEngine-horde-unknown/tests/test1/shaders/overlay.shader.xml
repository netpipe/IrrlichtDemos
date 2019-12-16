[[FX]]

<Sampler id="albedoMap" />

<Context id="OVERLAY">
	<Shaders vertex="VS_OVERLAY" fragment="FS_OVERLAY" />
	<RenderConfig writeDepth="false" blendMode="BLEND" />
</Context>

[[VS_OVERLAY]]

varying vec2 texCoords;

void main( void )
{
	texCoords = vec2( gl_MultiTexCoord0.s, -gl_MultiTexCoord0.t ); 
	gl_Position = gl_ProjectionMatrix * gl_Vertex;
}


[[FS_OVERLAY]]

uniform vec4 olayColor;
uniform sampler2D albedoMap;
varying vec2 texCoords;

void main( void )
{
	vec4 albedo = texture2D( albedoMap, texCoords );
	
	gl_FragColor = albedo * olayColor;
}