uniform sampler2D oceanNormalMap;
uniform sampler2D oceanReflection;
//uniform sampler2D oceanFoam;

uniform float waterTime;

uniform vec4 AmbientLight;

varying float posY;
uniform int terrainScale;

void main()
{
	float scale = float(terrainScale) * 0.145;
	vec2 texCoord = vec2(gl_TexCoord[0]);
	
	vec4 tex0    = texture2D( oceanNormalMap, vec2(texCoord.x*5.0 + waterTime,texCoord.y*5.0));
	vec4 tex1    = texture2D( oceanReflection, texCoord.xy*2.0 + tex0.r*0.5 );
	//vec4 tex2    = texture2D( oceanFoam, vec2(texCoord.x*5.0 + waterTime*0.3, -texCoord.y*5.0)); 
	
	//tex1.a = (-posY/scale)-(0.1)*3.0; // Old formula for blending was not that good.
	tex1.a = ((-posY-40.0)/(scale+20.0)); // This control the transparency and the blend with the water foam
	tex1.a = min(tex1.a,1.0); // Don`t allow to go more than 1.0 as value
	
 	vec4 finalColor = tex1 * AmbientLight;
	//finalColor = mix(vec4(tex2.rgb,1.0),finalColor,finalColor.a);
	
	//if (tex1.a<0.01)
	//	finalColor.a=0.0; //Hide the borders
	
	
	float fog = (gl_Fog.end - gl_FogFragCoord) * gl_Fog.scale;
	finalColor = mix(gl_Fog.color,finalColor, fog);
	
	gl_FragColor = finalColor;

	
}


