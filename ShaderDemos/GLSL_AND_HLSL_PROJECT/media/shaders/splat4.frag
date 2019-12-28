uniform sampler2D terrainLayer0;
uniform sampler2D terrainLayer1;
uniform sampler2D terrainLayer2;
uniform sampler2D terrainLayer3;

uniform float plateau;
uniform int terrainTextureScale;
uniform int terrainScale;

uniform int editingTerrain;

uniform vec4 AmbientLight;

varying vec3 normal;
varying vec3 position;
varying vec4 worldCoord;

void main() 
{
    float scale = float(terrainScale);
		
	vec2 texCoord = vec2(gl_TexCoord[0]);
	
	vec4 tex0    = texture2D( terrainLayer0, texCoord.xy*float(terrainTextureScale));
	vec4 tex1    = texture2D( terrainLayer1, texCoord.xy*float(terrainTextureScale));
	vec4 tex2    = texture2D( terrainLayer2, texCoord.xy*float(terrainTextureScale));
	vec4 tex3    = texture2D( terrainLayer3, texCoord.xy*float(terrainTextureScale));

	//tex1 = mix( tex0, tex1, min(1.0-normal.y,1.0) );
	
	// First layer of composition soil & grass blending
	float position2=position.y+160.0;
	if (position2>128.0 && position2<178.0)
		tex1 = mix( tex1, tex0, 1.0-(position2-128.0)/50.0);
	
	if (position2<=128.0)	
		tex1=tex0;
	
	// Second composition of first block and rock blending
	if (position2>178.0 && position2<512.0)
	    tex2 = mix( tex1, tex2, ((position2-256.0)/256.0));

	if (position2<=178.0)
		tex2 = tex1;
		
	// Third composition of first block and snow blending
	if (position2>620.0 && position2<768.0)
		tex3 = mix( tex2, tex3, ((position2-620.0)/148.0));
		
	if (position2<=620.0)
		tex3 = tex2;

	vec4 tex10 = tex3;
	
	//Plateau band
	if(position.y>(plateau-1.5) && position.y<(plateau+1.5) && editingTerrain==1) 
		tex10=vec4(0.0,0.5,0.0,0.5);
		
	// Directional light no attenuation (Sun)
	vec3 norm = normalize(normal);
	vec3 sunVector = normalize(vec3(2500.0,50000.0,2500.0) - worldCoord.xyz);
	float sunDir = max(0.0, dot(norm, sunVector));
	
	vec4 diffuse;
	diffuse = (gl_LightSource[0].diffuse * sunDir); //1.45 brighten the whole texture
	
	// Rendering with 1 directional light source 
	vec4 finalColor = vec4(diffuse.rgb * tex10.rgb, 1.0);  //*AmbientLight; //0.45 diffuse the shadowing
	
	// Rendering with no light source
	//vec4 finalColor = (vec4(tex10.rgb, 1.0))*AmbientLight;
	
	//Fog blending
	float fog = (gl_Fog.end - gl_FogFragCoord) * gl_Fog.scale;
	finalColor = mix(gl_Fog.color,finalColor, fog);
	gl_FragColor = vec4(finalColor.rgb,1.0);
}


