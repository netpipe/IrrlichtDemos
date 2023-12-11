uniform sampler2D oceanNormalMap;
uniform sampler2D oceanReflection;

uniform float waterTime;

uniform vec4 AmbientLight;

varying float posY;
uniform int terrainScale;

void main()
{
	float scale = float(terrainScale) * 0.145;
	vec2 texCoord = vec2(gl_TexCoord[0]);
	
	vec4 tex0    = texture2D( oceanNormalMap, vec2(texCoord.x*5.0 + waterTime,texCoord.y*3.0));
	vec4 tex1    = texture2D( oceanReflection, texCoord.xy*2.0 + tex0.r*0.5 );
	
	//tex1.a = (-posY/scale)-(0.1)*3.0;
	tex1.a = 0.65;
	
 	vec4 finalColor = tex1 * AmbientLight;
	float pAlpha = finalColor.a;

	float fog = (gl_Fog.end - gl_FogFragCoord) * gl_Fog.scale;
	finalColor = mix(gl_Fog.color,finalColor, fog);

	gl_FragColor = vec4(finalColor.r,finalColor.g,finalColor.b,pAlpha);
}


