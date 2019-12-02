#version 130


in vec4 gvo_position;
in vec2 gvo_TextureCoord;
in vec3 gvo_normal;

out vec4 o_Color;

uniform int   g_LightType;
uniform vec3   g_LightPosition; 
uniform int g_HasDiffuseTexture;
uniform sampler2D g_DiffuseTexture;
uniform vec4 g_DiffuseColor;


void main (void)  
{     
	vec4 fragColor = g_DiffuseColor;

	if(g_HasDiffuseTexture == 1 )
	{
		vec4 color = texture2D(g_DiffuseTexture,gvo_TextureCoord.st);
		fragColor = fragColor * color;

	}

	float dotNormalLight = 0.0f;
	

	if( g_LightType == 1 ) // Directionnal
	{
		vec3 lightDir = g_LightPosition.xyz;
		dotNormalLight = max( dot( normalize( gvo_normal ), normalize( lightDir ) ) , 0.0 ); 
	}
	else if( g_LightType == 2 ) // Local
	{
		vec3 lightDir = gvo_position.xyz - g_LightPosition.xyz;
		dotNormalLight = max( dot( normalize( gvo_normal ), normalize( -lightDir ) ) , 0.0 ); 
	}
	/*else if(  g_LightType == 3 ) // Spot
	{
		vec3 lightDir = gvo_position - g_LightPosition.xyz;
		dotNormalLight = max( dot( normalize( gvo_normal ), normalize( -lightDir ) ) , 0.0 );

		float spotEffect = dot(normalize(gl_LightSource[0].spotDirection), normalize(lightDir));
		if ( spotEffect > gl_LightSource[0].spotCosCutoff) 
		{
					dotNormalLight = dotNormalLight; 
		}
		else
		{
			dotNormalLight = 0.0f;
		}
	}*/
	else // No light
	{
		dotNormalLight = 1.0f;
	}
	
	fragColor = fragColor * dotNormalLight;


	o_Color = fragColor;

}  


