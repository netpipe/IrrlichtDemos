uniform sampler2D terrainLayer0;
uniform sampler2D terrainLayer1;
uniform sampler2D terrainLayer2;
uniform sampler2D terrainLayer3;
uniform sampler2D terrainLayer4;

uniform float plateau;
uniform int terrainTextureScale;
uniform int terrainScale;

uniform int editingTerrain;

uniform vec4 AmbientLight;
uniform vec3 mLightPos;
uniform vec3 mCamPos;

varying vec3 normal;
varying vec3 position;
varying vec4 worldCoord;

void main() 
{
    float scale = float(terrainScale) / 4.0;
	if (terrainScale==1)
		scale = 1.0;
		
	vec2 texCoord = vec2(gl_TexCoord[0]);

	vec4 tex0    = texture2D( terrainLayer0, texCoord.xy*float(terrainTextureScale*2.6) );
	vec4 tex1    = texture2D( terrainLayer1, texCoord.xy*float(terrainTextureScale) );
	vec4 tex2    = texture2D( terrainLayer2, texCoord.xy*float(terrainTextureScale) );
	vec4 tex3    = texture2D( terrainLayer3, texCoord.xy*float(terrainTextureScale) );
	vec4 tex4    = texture2D( terrainLayer4, texCoord.xy*float(terrainTextureScale) );

	tex2 = mix( tex2, tex1, min(1.0-normal.y,2.0) );
	tex3 = mix( tex2, tex3, ((position.y-50.0)/float(scale)));
	
	vec4 tex10;
	
	float pos2 = (position.y/scale)*10;
	
	if(position.y >= 0.0)
	{
		//tex10 = tex2;
	  tex10 = mix( tex2, tex4, (position.y/float(scale)));
	  tex10 = mix( tex10, tex3, (min(1.0-normal.y-0.2,0.5)) );
	}
	else
	{
	  tex10 = mix( tex2, tex3, min(1.0-normal.y-0.5,1.0));
	  
	  // Sand layer
	  tex10 = mix( tex10, tex1, min(1.0,-((position.y)/float(scale))*5.0));
	  
	  
	  if(position.y < -50.0)
	  {
	    //Rock layer
	    tex10 = mix( tex10, tex0, min(1.0,-((position.y+50.0)/float(scale))*1.2));
		// Theses are underwater (water shadow)
		tex10 = mix( tex10, vec4(0.0,0.2,0.05,1.0), min(1.0,-((position.y+50.0)/float(scale))*3.0));
	  }
	}
	
	if(position.y>(plateau-2.5) && position.y<(plateau+2.5) && editingTerrain==1) tex10*=vec4(1.0,0.6,0.4,1.0);
	float fog = (gl_Fog.end - gl_FogFragCoord) * gl_Fog.scale;
	gl_FragColor = mix(gl_Fog.color,(tex10*AmbientLight), fog);
}


